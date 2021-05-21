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
 * File:   PGM_Export.h
 *
 * Created on 20. Mai 2016, 14:53
 */

#include <exception>

#ifndef PGM_EXPORT_H
#define	PGM_EXPORT_H

class AutoBinException : public std::exception {
    public:
        AutoBinException(std::string what_);
        virtual ~AutoBinException();
        virtual const char* what() const throw();
    private:
        std::string whatstr;
};

void PGM_Export_from_uint32buf(std::string fullpath, const uint32_t* buf, int w, int h, uint32_t whiteval);
uint32_t Maximum_Value_in_uint32buf(const uint32_t* buf, int w, int h);
void PGM_Export_from_uint32buf_autoBC(std::string fullpath, const uint32_t* buf, int w, int h);
void PGM_Export_from_uint32buf_Plot_autoMax(std::string fullpath, const uint32_t* buf, long w, int pgm_w, int pgm_h);
void PGM_Export_from_uint32buf_Plot(std::string fullpath, const uint32_t* buf, long w, int pgm_w, int pgm_h, uint32_t maxval);
uint32_t* AutoBin_uint32Spectrum_To_New_Size(const uint32_t* buf, int size, int newsize);
#endif	/* PGM_EXPORT_H */

