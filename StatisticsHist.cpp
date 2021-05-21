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


#include "StatisticsHist.h"
#include <iostream>

StatisticsHist::StatisticsHist() {
    StatisticsHist(100);
}


StatisticsHist::StatisticsHist(int nrbins_) {
    nrbins = nrbins_;
    //std::cout << "StatisticsHist(" << nrbins_ << ") called" << std::endl;
    data.reserve(nrbins);
    cdf.reserve(nrbins);
    for (int i=0; i<nrbins; i++) {
        data.push_back(0);
        cdf.push_back(0);
    }
    _binsize = 1;
    //std::cout << "StatisticsHist(" << nrbins_ << ") finished" << std::endl;
}

StatisticsHist::~StatisticsHist() {

}

void StatisticsHist::Reset() {
    //std::cout << "StatisticsHist::Reset() got called" << std::endl;
    //std::cout << "StatisticsHist::Reset() nrbins: " << nrbins << std::endl;
    for (int i = 0; i<nrbins; i++)
        data[i] = 0;
    
}

void StatisticsHist::Update(const uint32_t* buf, long len, uint32_t max) {
    // Assume minimum is 0
    if (nrbins<1) return;
    Reset();
    _binsize = max/nrbins;
    if (max%nrbins>0)
        _binsize += 1;
    int bin;
    if (_binsize==0) _binsize = 1;
    for (long i = 0; i<len; i++) {
        bin = buf[i]/_binsize;
        if (bin>=nrbins) bin = nrbins-1;
        data[bin]++;
    }
    _cdf_update_needed = true;
}


void StatisticsHist::Update(const uint32_t* buf, long len) {
    _update_min_max(buf, len);
    Update(buf, len, _max);
}

void StatisticsHist::_update_min_max(const uint32_t* buf, long len) {
    _min = 4294967295U;
    _max = 0;
    uint32_t v;
    for (long i = 0; i<len; i++) {
        v = buf[i];
        _min = v<_min ? v : _min;
        _max = v>_max ? v : _max;
    }
}

uint32_t StatisticsHist::GetQuantile(double p) {
    if (nrbins<1) return 0;
    if (_cdf_update_needed)
        _update_cdf();
    uint32_t v = (uint32_t) (p*(double)cdf[nrbins-1]);
    //std::cout << "v=" << v << std::endl;
    for (int i=0; i<nrbins; i++)
        if (v<cdf[i]) 
            return i*_binsize;
    return _max;
}

void StatisticsHist::_update_cdf() {
    if (nrbins<1) return;
    uint32_t cumulative = 0;
    for (int i=0; i<nrbins; i++) {
        cumulative += data[i];
        cdf[i] = cumulative;
    }
    _cdf_update_needed = false;
    //std::cout << "H binsize : " << _binsize << std::endl;
    //for (int i = 0; i<nrbins; i++) {
    //    std::cout << "cdf[" << i << "] = " << cdf[i] << std::endl;
    //}
}

uint32_t StatisticsHist::GetBinSize() {
    return _binsize;
}

uint32_t StatisticsHist::GetMax() {
    return _max;
}

uint32_t StatisticsHist::GetMin() {
    return _min;
}

int StatisticsHist::GetNrBins() {
    return nrbins;
}
