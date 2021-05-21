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
    void SurfaceConceptTDC::AddImageAttributes() {
        // Hist_Live_XY ############################################################################################
        hist_live_xy_attr = new CustomImageAttr("Hist_Live_XY", Tango::DEV_LONG, Tango::READ, hist_images_max_xsize, hist_images_max_ysize);
        hist_live_xy_attr->SetReadCallback(this, &SurfaceConceptTDC::Hist_Live_Accu_Image_ReadCallback);
        //hist_live_xy_attr->set_change_event(true, false); // we will push change events ( so no polling is required )
        m_hist_map.at("Hist_Live_XY")->SetTangoOutAttribute(hist_live_xy_attr);
        Tango::UserDefaultAttrProp ap1;
        ap1.set_description("XY image in live mode");
        hist_live_xy_attr->set_default_properties(ap1);
        add_attribute(hist_live_xy_attr);
        // Hist_Live_XT ############################################################################################
        hist_live_xt_attr = new CustomImageAttr("Hist_Live_XT", Tango::DEV_LONG, Tango::READ, hist_images_max_xsize, hist_images_max_tsize);
        hist_live_xt_attr->SetReadCallback(this, &SurfaceConceptTDC::Hist_Live_Accu_Image_ReadCallback);
        //hist_live_xt_attr->set_change_event(true, false); // we will push change events ( so no polling is required )
        m_hist_map.at("Hist_Live_XT")->SetTangoOutAttribute(hist_live_xt_attr);
        Tango::UserDefaultAttrProp ap2;
        ap2.set_description("XT image in live mode");
        hist_live_xt_attr->set_default_properties(ap2);
        add_attribute(hist_live_xt_attr);
        // Hist_Live_YT ############################################################################################
        hist_live_yt_attr = new CustomImageAttr("Hist_Live_YT", Tango::DEV_LONG, Tango::READ, hist_images_max_ysize, hist_images_max_tsize);
        hist_live_yt_attr->SetReadCallback(this, &SurfaceConceptTDC::Hist_Live_Accu_Image_ReadCallback);
        //hist_live_yt_attr->set_change_event(true, false); // we will push change events ( so no polling is required )
        m_hist_map.at("Hist_Live_YT")->SetTangoOutAttribute(hist_live_yt_attr);
        Tango::UserDefaultAttrProp ap3;
        ap3.set_description("YT image in live mode");
        hist_live_yt_attr->set_default_properties(ap3);
        add_attribute(hist_live_yt_attr);
        // Hist_Accu_XY ############################################################################################
        hist_accu_xy_attr = new CustomImageAttr("Hist_Accu_XY", Tango::DEV_LONG, Tango::READ, hist_images_max_xsize, hist_images_max_ysize);
        hist_accu_xy_attr->SetReadCallback(this, &SurfaceConceptTDC::Hist_Live_Accu_Image_ReadCallback);
        //hist_accu_xy_attr->set_change_event(true, false); // we will push change events ( so no polling is required )
        m_hist_map.at("Hist_Accu_XY")->SetTangoOutAttribute(hist_accu_xy_attr);
        Tango::UserDefaultAttrProp ap4;
        ap4.set_description("XY image in accumulation mode");
        hist_accu_xy_attr->set_default_properties(ap4);
        add_attribute(hist_accu_xy_attr);
        // Hist_Accu_XT ############################################################################################
        hist_accu_xt_attr = new CustomImageAttr("Hist_Accu_XT", Tango::DEV_LONG, Tango::READ, hist_images_max_xsize, hist_images_max_tsize);
        hist_accu_xt_attr->SetReadCallback(this, &SurfaceConceptTDC::Hist_Live_Accu_Image_ReadCallback);
        //hist_accu_xt_attr->set_change_event(true, false); // we will push change events ( so no polling is required )
        m_hist_map.at("Hist_Accu_XT")->SetTangoOutAttribute(hist_accu_xt_attr);
        Tango::UserDefaultAttrProp ap5;
        ap5.set_description("XT image in accumulation mode");
        hist_accu_xt_attr->set_default_properties(ap5);
        add_attribute(hist_accu_xt_attr);
        // Hist_Accu_YT ############################################################################################
        hist_accu_yt_attr = new CustomImageAttr("Hist_Accu_YT", Tango::DEV_LONG, Tango::READ, hist_images_max_ysize, hist_images_max_tsize);
        hist_accu_yt_attr->SetReadCallback(this, &SurfaceConceptTDC::Hist_Live_Accu_Image_ReadCallback);
        //hist_accu_yt_attr->set_change_event(true, false); // we will push change events ( so no polling is required )
        m_hist_map.at("Hist_Accu_YT")->SetTangoOutAttribute(hist_accu_yt_attr);
        Tango::UserDefaultAttrProp ap6;
        ap6.set_description("YT image in accumulation mode");
        hist_accu_yt_attr->set_default_properties(ap6);
        add_attribute(hist_accu_yt_attr);
    }
    
    void SurfaceConceptTDC::Hist_Live_Accu_Image_ReadCallback(Tango::DeviceImpl*, Tango::Attribute& attr) {
        std::string name = attr.get_name();
        GeneralHistogram* h = NULL;
        h = m_hist_map.at(name);
        Tango::DevLong* tbuf = h->GetTangoBuffer();
        if (tbuf!=NULL) {
            attr.set_value(tbuf, h->GetTangoBufferDataWidth(), h->GetTangoBufferDataHeight());
        }
        else {
            //std::cout << "sending dummy buffer for attribute " << name << std::endl;
            Tango::DevLong dummybuf[4] = {0,0,0,0};
            attr.set_value(dummybuf, 2, 2);
        }
    }

}