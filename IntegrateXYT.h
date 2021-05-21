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
 * File:   IntegrateXYT.h
 *
 * Created on 18. Mai 2016, 09:30
 */

#ifndef INTEGRATEXYT_H
#define	INTEGRATEXYT_H

#include <string>
#include "GeneralHistogram.h"

namespace SurfaceConceptTDC_ns {

    int IntegrateXYT_X(GeneralHistogram& xyt, GeneralHistogram& target, long x1, long x2);
    int IntegrateXYT_Y(GeneralHistogram& xyt, GeneralHistogram& target, long y1, long y2);

    /**
    * Integrate the t axis of a 3d data set, yielding an xy image
    * @param xyt      the source 3D data set
    * @param target   the image to be written to
    * @param t1       the lower integration limit for the t axis, included in the integration
    * @param t2       the upper integration limit for the t axis, not included in the integration
    * @return         0 if succesful, otherwise an error code to be interpreted by ..._ErrMsg(...)
    */
    int IntegrateXYT_T(GeneralHistogram& xyt, GeneralHistogram& target, long t1, long t2);
    
    /**
     * Integrate the x and y axes of a 3d data set, yielding a counts-vs-time (1dim) dataset
     * @param xyt      the source 3D data set
     * @param target   the 1d dataset to be written to
     * @param x1       the left integration limit for the x axis, included in the integration
     * @param x2       the right integration limit for the x axis, excluded from the integration
     * @param y1       the upper integration limit for the y axis, included in the integration
     * @param y2       the lower integration limit for the y axis, excluded from the integration
     * @return         0 if succesful, otherwise an error code to be interpreted by ..._ErrMsg(...)
     */
    int IntegrateXYT_XY(GeneralHistogram& xyt, GeneralHistogram& target, long x1, long x2, long y1, long y2);

    /**
     * Yield an error message to a given error code
     * @param errcode
     * @return 
     */
    std::string IntegrateXYT_ErrMsg(int errcode);
    
}


#endif	/* INTEGRATEXYT_H */

