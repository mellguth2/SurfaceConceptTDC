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


/* 
 * File:   ImageHist.h
 *
 * Created on 4. Oktober 2016, 11:25
 */

#ifndef IMAGEHIST_H
#define IMAGEHIST_H

#include <vector>
#include <stdint.h>

class StatisticsHist {
public:
    StatisticsHist();
    StatisticsHist(int nrbins_);
    ~StatisticsHist();
    
    void Update(const uint32_t* buf, long len);
    void Update(const uint32_t* buf, long len, uint32_t max);
    void Reset();
    uint32_t GetQuantile(double p);
    uint32_t GetMin();
    uint32_t GetMax();
    uint32_t GetBinSize();
    int GetNrBins();
    
    
private:
    std::vector<uint32_t> data;
    std::vector<uint32_t> cdf;
    bool _cdf_update_needed;
    int nrbins;
    uint32_t _min, _max, _binsize;
    
    void _update_min_max(const uint32_t* buf, long len);
    void _update_cdf();
};

#endif /* IMAGEHIST_H */

