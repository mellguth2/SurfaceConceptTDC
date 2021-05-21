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
 * File:   SaveXYTtoTiff.h
 *
 * Created on 18. Mai 2016, 18:38
 */

#ifndef SAVEXYTTOTIFF_H
#define	SAVEXYTTOTIFF_H

#include "GeneralHistogram.h"

namespace SurfaceConceptTDC_ns {
    bool SaveXYTtoTiff(GeneralHistogram& h, const std::string& path, const std::string& filename);
    //bool SaveXYTtoTiff(GeneralHistogram& h, const char* path, const char* filename);
    bool SaveXYtoTiff(GeneralHistogram& h, const std::string& path, const std::string& filename);
    //bool SaveXYtoTiff(GeneralHistogram& h, const char* path, const char* filename);
}

#endif	/* SAVEXYTTOTIFF_H */

