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

#include "IntegrateXYT.h"

namespace SurfaceConceptTDC_ns {
    
    std::string IntegrateXYT_ErrMsg(int errcode) {
        if (errcode==0) return std::string("No error.");
        else return std::string("Unknown error.");
    }
    
    int IntegrateXYT_T(GeneralHistogram& xyt, GeneralHistogram& target, long t1, long t2) {
        long w = xyt.GetWidth();
        long h = xyt.GetHeight();
        long zs = xyt.GetZSize();
        if (t1>t2)
            std::swap(t1, t2);
        if (t1>=zs) t1 = zs-1;
        if (t2>zs) t2 = zs;
        if (t1<0) t1 = 0;
        if (t2<0) t2 = 0;
        
        // copy axis boundaries
        target.SetAbscissaOffset(xyt.GetAbscissaOffset());
        target.SetWidth(xyt.GetWidth());
        target.SetOrdinateOffset(xyt.GetOrdinateOffset());
        target.SetHeight(xyt.GetHeight());
        //
        target.AccomodateDatabufSize(true); // true = also clears databuffer
        long x,y,t, srcoff, targoff;
        uint32_t* pxyt = (uint32_t*) xyt.GetDatabufPointer();
        uint32_t* pxy = (uint32_t*) target.GetDatabufPointer();
        
        for (t = t1; t<t2; t++) {
            for (y = 0; y<h; y++) {
                srcoff = t*w*h+y*w;
                targoff = y*w;
                for (x = 0; x<w; x++) {
                    *(pxy+targoff+x) += *(pxyt+srcoff+x);
                }
            }
        }
        return 0;
    }
    
    int IntegrateXYT_Y(GeneralHistogram& xyt, GeneralHistogram& target, long y1, long y2) {
        long w = xyt.GetWidth();
        long h = xyt.GetHeight();
        long zs = xyt.GetZSize();
        if (y1>y2)
            std::swap(y1, y2);
        if (y1>=h) y1 = h-1;
        if (y2>h) y2 = h;
        if (y1<0) y1 = 0;
        if (y2<0) y2 = 0;
        // copy axis boundaries
        target.SetAbscissaOffset(xyt.GetAbscissaOffset());
        target.SetWidth(xyt.GetWidth());
        target.SetOrdinateOffset(xyt.GetZOffset());
        target.SetHeight(xyt.GetZSize());
        //
        target.AccomodateDatabufSize(true); // true = also clears databuffer
        long x,y,t, srcoff, targoff;
        target.ClearBuffer();
        uint32_t* pxyt = (uint32_t*) xyt.GetDatabufPointer();
        uint32_t* pxt = (uint32_t*) target.GetDatabufPointer();
        
        for (t = 0; t<zs; t++) {
            for (y = y1; y<y2; y++) {
                srcoff = t*w*h+y*w;
                targoff = t*w;
                for (x = 0; x<w; x++) {
                    uint32_t pixelval = *(pxyt+srcoff+x);
                    *(pxt+targoff+x) += pixelval;
                }
            }
        }
        return 0;
    }

    int IntegrateXYT_X(GeneralHistogram& xyt, GeneralHistogram& target, long x1, long x2) {
        long w = xyt.GetWidth();
        long h = xyt.GetHeight();
        long zs = xyt.GetZSize();
        if (x1>x2)
            std::swap(x1, x2);
        if (x1>=w) x1 = w-1;
        if (x2>w) x2 = w;
        if (x1<0) x1 = 0;
        if (x2<0) x2 = 0;
        // copy axis boundaries
        target.SetAbscissaOffset(xyt.GetOrdinateOffset());
        target.SetWidth(xyt.GetHeight());
        target.SetOrdinateOffset(xyt.GetZOffset());
        target.SetHeight(xyt.GetZSize());
        //
        target.AccomodateDatabufSize(true); // true = also clears databuffer
        long x,y,t, srcoff, targoff;
        target.ClearBuffer();
        uint32_t* pxyt = (uint32_t*) xyt.GetDatabufPointer();
        uint32_t* pyt = (uint32_t*) target.GetDatabufPointer();
        
        for (t = 0; t<zs; t++) {
            for (y = 0; y<h; y++) {
                srcoff = t*w*h+y*w;
                targoff = t*h+y;  // in target image, h is the width and zs is the height! y is on the x axis!
                for (x = x1; x<x2; x++) {
                    *(pyt+targoff) += *(pxyt+srcoff+x);
                }
            }
        }
        return 0;
    }

    int IntegrateXYT_XY(GeneralHistogram& xyt, GeneralHistogram& target, long x1, long x2, long y1, long y2) {
        long w = xyt.GetWidth();
        long h = xyt.GetHeight();
        long zs = xyt.GetZSize();
        if (x1>x2)
            std::swap(x1, x2);
        if (y1>y2)
            std::swap(y1, y2);
        if (x1>=w) x1 = w-1;
        if (x2>w) x2 = w;
        if (x1<0) x1 = 0;
        if (x2<0) x2 = 0;
        if (y1>=h) y1 = h-1;
        if (y2>h) y2 = h;
        if (y1<0) y1 = 0;
        if (y2<0) y2 = 0;
        
        //std::cout << "IntegrateXYT_XY: (x1, x2, y1, y2)=" << x1 << x2 << y1 << y2 << std::endl;
        target.SetAbscissaOffset(xyt.GetZOffset());
        target.SetWidth(xyt.GetZSize());
        target.AccomodateDatabufSize(true); // true = also clears databuffer
        long x,y,t, srcoff, targoff;
        target.ClearBuffer();
        uint32_t* pxyt = (uint32_t*) xyt.GetDatabufPointer();
        uint32_t* pt = (uint32_t*) target.GetDatabufPointer();
        for (t = 0; t<zs; t++) {
            targoff = t;
            for (y = y1; y<y2; y++) {
                srcoff = t*w*h+y*w;
                for (x = x1; x<x2; x++) {
                    *(pt+targoff) += *(pxyt+srcoff+x);
                }
            }
        }
        return 0;
    }
}
