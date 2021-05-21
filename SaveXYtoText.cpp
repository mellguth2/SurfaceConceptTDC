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
 * File:   SaveXYTtoText.cpp
 *
 * Created on 27. Februar 2018, 16:13
 */

#include "SaveXYtoText.h"
#include "Helper.h"
#include <fstream>
#include <iomanip>

namespace SurfaceConceptTDC_ns {

bool SaveXYtoText(GeneralHistogram& hist, const std::string& path, const std::string& filename, long accumulated_time_val) {
        uint32_t *pxy = (uint32_t*) hist.GetDatabufPointer();
        if (pxy==nullptr) return false;
        if (hist.depth!=32) {
            std::cout << "SaveXYtoText: bit depths other than 32 not implemented yet!" << std::endl;
            std::cout << " Cancelling save to text." << std::endl;
            return false;
        }
        std::string fullpath = Helper::join_pathnames(path, filename);
        if (Helper::test_file_exists(fullpath)) {
            std::cout << "SaveXYtoText: file already exists!" << std::endl;
            std::cout << " " << fullpath;
            std::cout << " Cancelling save to text." << std::endl;
            return false;
        }
        if (!Helper::test_file_writable(fullpath)) {
            std::cout << "SaveXYtoText: cannot write to file" << std::endl;
            std::cout << " " << fullpath;
            return false;
        }
        long w = hist.GetWidth();
        long h = hist.GetHeight();
        
        std::ofstream ofs(fullpath.c_str(), std::ofstream::out);
        // ImageJ is not able to parse header lines with the built in File->Import->Text Image command
//        ofs << "# XY accumulated image as text matrix saved by TDC Tango Server" << std::endl;
//        ofs << "# width            " << w << std::endl;
//        ofs << "# height           " << h << std::endl;
//        ofs << "# date             " << Helper::get_date_string("-") << std::endl;
//        ofs << "# time             " << Helper::get_time_string(":") << std::endl;
//        ofs << "# accumulated time " << accumulated_time_val << " ms" << std::endl;
//        ofs << "# " << std::endl;
//        ofs << "# START OF DATA" << std::endl;
        
        for (long y=0; y<h; y++) {
            for (long x=0; x<w; x++) {
                ofs << std::setw(11) << pxy[x+y*w];
            }
            ofs << std::endl;
        }
        ofs.close();
        return true;
    
}

}
