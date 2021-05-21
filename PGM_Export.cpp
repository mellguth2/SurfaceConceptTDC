/*
 * The MIT License
 *
 * Copyright 2016-2018 Surface Concept GmbH
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 * THE SOFTWARE.
 */

#include <fstream>
#include <iostream>
#include <math.h>
#include <exception>

#include <fcntl.h>
#include <sys/file.h>
#include <unistd.h>

#include "PGM_Export.h"

AutoBinException::AutoBinException(std::string what_) {
    whatstr = what_;
}

const char* AutoBinException::what() const throw() {
    return whatstr.c_str();
}

AutoBinException::~AutoBinException() {
}


void PGM_Export_from_uint32buf(std::string fullpath, const uint32_t* buf, int w, int h, uint32_t whiteval) {
    std::ofstream f;
    long long v; // value variable
    int v2;
    char v3;
    char *bufout = new char[w*h];
    for (int y = 0; y<h; y++)
        for (int x = 0; x<w; x++) {
            v = ((long long) buf[y*w+x])*255;
            v = v / whiteval;
            v2 = (int) v;
            if (v2>255)
                bufout[y*w+x] = 255;
            else
                bufout[y*w+x] = (char) v2;
        }
    f.open(fullpath, std::ios::out | std::ios::trunc);
    f << "P5" << " " << w << " " << h << " " << " 255 "; // header
    f.close();
    f.open(fullpath, std::ios::out | std::ios::app | std::ios::binary);
    f.write(bufout, w*h);
    f.close();   
    delete[] bufout;
}

void PGM_Export_from_uint32buf_autoBC(std::string fullpath, const uint32_t* buf, int w, int h) {
    //std::cout << "writing PGM " << fullpath << std::endl;
    uint32_t whiteval = Maximum_Value_in_uint32buf(buf, w, h);
    if (whiteval==0) whiteval=1;
    PGM_Export_from_uint32buf(fullpath, buf, w, h, whiteval);
}

void PGM_Export_from_uint32buf_Plot_autoMax(std::string fullpath, const uint32_t* buf, long w, int pgm_w, int pgm_h) {
    uint32_t maxval = Maximum_Value_in_uint32buf(buf, w, 1);
    if (maxval==0) maxval=1;
    PGM_Export_from_uint32buf_Plot(fullpath, buf, w, pgm_w, pgm_h, maxval);
}

void PGM_Export_from_uint32buf_Plot(std::string fullpath, const uint32_t* buf, long w, int pgm_w, int pgm_h, uint32_t maxval) {
    std::ofstream f;
    char offval = 250;
    char onval = 0;
    char borderval = 127;
    if (maxval==0) maxval=1;
    double logmax = log((double) maxval);
    double xf = ((double) pgm_w)/((double) w);
    double yf = ((double) pgm_h)/((double) logmax);
    int pgm_x, pgm_y;
    char *bufout = new char[pgm_w*pgm_h];
    for (int y=0; y<pgm_h; y++) {
        for (int x = 0; x<pgm_w; x++) 
            if (x==0 || x==pgm_w-1 || y==0 || y==pgm_h-1)
                bufout[pgm_w*y+x] = borderval;
            else
                bufout[pgm_w*y+x] = offval;
    }
    double v;
    int step = 1;
    if (w/pgm_w > 10)
        step = (w/pgm_w)/10;
    for (long x = 0; x<w; x+=step) {
        v = (double) (buf[x]<1?1:buf[x]);
        v = log(v);
        pgm_y = ((int) round(v*yf));
        pgm_x = ((int) round(x*xf));
        if (pgm_x>=pgm_w-2)
            pgm_x = pgm_w-3;
        if (pgm_y>=pgm_h-2)
            pgm_y = pgm_h-3;
        bufout[pgm_x+1+(pgm_h-pgm_y-2)*pgm_w] = onval; // center
        bufout[pgm_x+(pgm_h-pgm_y-2)*pgm_w] = onval;   // left
        bufout[pgm_x+2+(pgm_h-pgm_y-2)*pgm_w] = onval; // right
        bufout[pgm_x+1+(pgm_h-pgm_y-1)*pgm_w] = onval; // up
        bufout[pgm_x+1+(pgm_h-pgm_y-3)*pgm_w] = onval; // down
    }
    int fd = open("/var/lock/sc_tdc_pgm_export", O_RDWR | O_CREAT, 0666); // open or create lockfile
    int rc = flock(fd, LOCK_EX | LOCK_NB); // demand lock, but don't wait for it
    f.open(fullpath, std::ios::out | std::ios::trunc);
    
    f << "P5" << " " << pgm_w << " " << pgm_h << " " << " 255 "; // header
    f.close();
    f.open(fullpath, std::ios::out | std::ios::app | std::ios::binary);
    f.write(bufout, pgm_w*pgm_h);
    f.close();
    close(fd);
    delete[] bufout;
}

uint32_t Maximum_Value_in_uint32buf(const uint32_t* buf, int w, int h) {
    uint32_t max = 0;
    for (int y = 0; y<h; y++)
        for (int x = 0; x<w; x++)
            if (buf[y*w+x]>max)
                max = buf[y*w+x];
    return max;
}

uint32_t* AutoBin_uint32Spectrum_To_New_Size(const uint32_t* buf, int size, int newsize) {
    int newbufsize = newsize; // the caller expects a buffer as large as newsize
    if (newsize>size)
        newsize = size;
    // IMPROVE: don't use exceptions, this function is called in a non-main thread, exception is not well propagated and may remain unrecognized
    if (newsize<1) throw new AutoBinException("AutoBin_uint32Spectrum_To_New_Size(...): illegal value of newsize \n");
    if (size<0) throw new AutoBinException("AutoBin_uint32Spectrum_To_New_Size(...): illegal value of size \n");
    int bin = size/newsize;
    double step = (double)size/(double)newsize;
    if (size%newsize>0)
        bin++;
    uint32_t* newbuf = new uint32_t[newbufsize];
    int off;
    uint32_t binned_val;
    for (int i=0; i<newsize; i++) {
        off = (int)(step*i+0.5);
        if (off+bin>=size)
            off = size-bin;
        binned_val = 0;
        for (int j=off; j<off+bin; j++)
            binned_val += buf[j];
        newbuf[i] = binned_val;
    }
    return newbuf;
}