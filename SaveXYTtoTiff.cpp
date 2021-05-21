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

#include "SaveXYTtoTiff.h"
#include "Helper.h"
#include <tiffio.h>

namespace SurfaceConceptTDC_ns {
    bool _SaveXYTtoTiff_impl(GeneralHistogram& hist, const std::string& path, const std::string& filename, long zsize_limit) {
        uint32_t *pxyt = (uint32_t*) hist.GetDatabufPointer();
        if (pxyt==NULL) return false;
        if (hist.depth!=32) {
            std::cout << "SaveXYTtoTiff: bit depths other than 32 not implemented yet!" << std::endl;
            std::cout << " Cancelling save to Tiff." << std::endl;
            return false;
        }
        std::string fullpath = Helper::join_pathnames(path, filename);
        if (Helper::test_file_exists(fullpath)) {
            std::cout << "SaveXYTtoTiff: file already exists!" << std::endl;
            std::cout << " " << fullpath;
            std::cout << " Cancelling save to Tiff." << std::endl;
            return false;
        }
        if (!Helper::test_file_writable(fullpath)) {
            std::cout << "SaveXYTtoTiff: cannot write to file" << std::endl;
            std::cout << " " << fullpath;
            return false;
        }
        long w = hist.GetWidth();
        long h = hist.GetHeight();
        long zs = hist.GetZSize();
        if (zsize_limit > 0 && zs > zsize_limit)
            zs = zsize_limit;
        TIFF* tif;
        int32 rowsperstrip = 1;
	if ((tif = TIFFOpen(fullpath.c_str(), "w"))==NULL) {
            std::cout << "SaveXYTtoTiff: Unknown problem calling TIFFOpen in write mode" << std::endl;
            return false;
        }
        TIFFSetField(tif, TIFFTAG_SUBFILETYPE, 0 );
	TIFFSetField(tif, TIFFTAG_IMAGEWIDTH, w);
	TIFFSetField(tif, TIFFTAG_IMAGELENGTH, h);
	TIFFSetField(tif, TIFFTAG_BITSPERSAMPLE, sizeof(float)*8);
	TIFFSetField(tif, TIFFTAG_SAMPLESPERPIXEL, 1);
	rowsperstrip = TIFFDefaultStripSize(tif,h);
	uint16 strips = h/rowsperstrip;
        
        float *stripbuf = new float[w*rowsperstrip];
        int strip_bytesize = w*rowsperstrip*sizeof(float);
        long x, srcoff, stripbufoff;
        long y = 0;
        long z = 0;
	if (h%rowsperstrip>0)
		strips++;
        for (long z=0; z<zs; z++) {
            y = 0;
            TIFFSetField(tif, TIFFTAG_SUBFILETYPE, 3);
            TIFFSetField(tif, TIFFTAG_IMAGEWIDTH, w);
            TIFFSetField(tif, TIFFTAG_IMAGELENGTH, h);
            TIFFSetField(tif, TIFFTAG_BITSPERSAMPLE, sizeof(float)*8);
            TIFFSetField(tif, TIFFTAG_SAMPLESPERPIXEL, 1);
            TIFFSetField(tif, TIFFTAG_ROWSPERSTRIP, rowsperstrip);
            TIFFSetField(tif, TIFFTAG_SAMPLEFORMAT, SAMPLEFORMAT_IEEEFP);
            TIFFSetField(tif, TIFFTAG_COMPRESSION, COMPRESSION_NONE);
            TIFFSetField(tif, TIFFTAG_PHOTOMETRIC, PHOTOMETRIC_MINISBLACK);            
            for (int strip=0; strip<strips; strip++) {
                for (int row=0; row<rowsperstrip && y<h; row++) {
                    srcoff = y*w+z*w*h;
                    stripbufoff = row*w;
                    for (x=0; x<w; x++) {
                        stripbuf[x+stripbufoff] = (float) pxyt[srcoff+x];
                    }
                    y++;
                }
                TIFFWriteRawStrip(tif, strip, stripbuf, strip_bytesize);
            }
            if ( (z+1) < zs)
                TIFFWriteDirectory(tif); // do not write directory for the last slice (TIFFClose does), avoids blank extra slice
        }
        delete[] stripbuf;
	TIFFClose(tif);
        return true;
    }
    
    bool SaveXYTtoTiff(GeneralHistogram& hist, const std::string& path, const std::string& filename) {
        return _SaveXYTtoTiff_impl(hist, path, filename, -1);
    }
    
    bool SaveXYtoTiff(GeneralHistogram& hist, const std::string& path, const std::string& filename) {
        return _SaveXYTtoTiff_impl(hist, path, filename, 1);
    }
}
