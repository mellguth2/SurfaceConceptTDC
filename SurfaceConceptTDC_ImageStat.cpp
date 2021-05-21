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


#include "SurfaceConceptTDC.h"
#include "CustomAttr.h"

namespace SurfaceConceptTDC_ns {

    void SurfaceConceptTDC::AddImageStatAttributes() {
        Tango::UserDefaultAttrProp apmax, ap998;
        apmax.set_description("Maximum intensity value in the image");
        ap998.set_description("99.8% Quartile of the intensity values in the image (Maximum after discarding 0.2% of the intensity values that are highest)");
        for (std::string s3 : {"_Max", "_Q998"})
            for (std::string s2 : {"_XY", "_XT", "_YT"})
                for (std::string s1 : {"_Live", "_Accu"}) {
                    std::string name = "Hist"+s1+s2+s3;
                    imagestat_attrs[name] = new CustomAttr(name.c_str(), Tango::DEV_LONG, Tango::READ, Tango::AssocWritNotSpec);
                    imagestat_vals[name] = 0;
                    if (s3.compare("_Max")==0)
                        imagestat_attrs[name]->set_default_properties(apmax);
                    else
                        imagestat_attrs[name]->set_default_properties(ap998);
                    imagestat_attrs[name]->SetReadCallback(this, &SurfaceConceptTDC::ImageStat_ReadCallback);
                    this->add_attribute(imagestat_attrs[name]);
                }
    }
    
    void SurfaceConceptTDC::ImageStat_ReadCallback(Tango::DeviceImpl* dev, Tango::Attribute& att) {
        att.set_value(&(imagestat_vals[att.get_name()]));
    }





}