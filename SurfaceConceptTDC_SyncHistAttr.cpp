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

// Creation and read/write handling of histogram attributes
// which allow for setting the 3 live 2D planar cross-sections (XY,XT,YT) and 
// the 3 accumulated 2D planar cross-sections (XY,XT,YT), as well as the
// XYT cube and the time histogram by a reduced set of 22 parameters

#include <string>

#include "SurfaceConceptTDC.h"
#include "CustomAttr.h"


namespace SurfaceConceptTDC_ns {

    void SurfaceConceptTDC::AddSyncHistAttributes() {
        sync_hist_bin_x_attr = new CustomAttr("Sync_Hist_BIN_X", Tango::DEV_LONG64, Tango::READ_WRITE, Tango::AssocWritNotSpec);
        Tango::UserDefaultAttrProp ap1;
        ap1.set_description("Binning of the x axis (performed before ROI selection)");
        sync_hist_bin_x_attr->set_default_properties(ap1);
        sync_hist_bin_x_attr->set_memorized();
        sync_hist_bin_x_attr->SetReadCallback(this, &SurfaceConceptTDC::Sync_Hist_ReadCallback);
        sync_hist_bin_x_attr->SetWriteCallback(this, &SurfaceConceptTDC::Sync_Hist_WriteCallback);

        sync_hist_bin_y_attr = new CustomAttr("Sync_Hist_BIN_Y", Tango::DEV_LONG64, Tango::READ_WRITE, Tango::AssocWritNotSpec);
        Tango::UserDefaultAttrProp ap2;
        ap2.set_description("Binning of the y axis (performed before ROI selection)");
        sync_hist_bin_y_attr->set_default_properties(ap2);
        sync_hist_bin_y_attr->set_memorized();
        sync_hist_bin_y_attr->SetReadCallback(this, &SurfaceConceptTDC::Sync_Hist_ReadCallback);
        sync_hist_bin_y_attr->SetWriteCallback(this, &SurfaceConceptTDC::Sync_Hist_WriteCallback);

        sync_hist_bin_t_attr = new CustomAttr("Sync_Hist_BIN_T", Tango::DEV_LONG64, Tango::READ_WRITE, Tango::AssocWritNotSpec);
        Tango::UserDefaultAttrProp ap3;
        ap3.set_description("Binning of the time axis (performed before ROI selection)");
        ap3.set_format("%10d");
        sync_hist_bin_t_attr->set_default_properties(ap3);
        sync_hist_bin_t_attr->set_memorized();
        sync_hist_bin_t_attr->SetReadCallback(this, &SurfaceConceptTDC::Sync_Hist_ReadCallback);
        sync_hist_bin_t_attr->SetWriteCallback(this, &SurfaceConceptTDC::Sync_Hist_WriteCallback);

        sync_hist_roi_x1_attr = new CustomAttr("Sync_Hist_ROI_X1", Tango::DEV_LONG64, Tango::READ_WRITE, Tango::AssocWritNotSpec);
        Tango::UserDefaultAttrProp ap4;
        ap4.set_description("Region of interest, left x (in binned pixels)");
        sync_hist_roi_x1_attr->set_default_properties(ap4);
        sync_hist_roi_x1_attr->set_memorized();
        sync_hist_roi_x1_attr->SetReadCallback(this, &SurfaceConceptTDC::Sync_Hist_ReadCallback);
        sync_hist_roi_x1_attr->SetWriteCallback(this, &SurfaceConceptTDC::Sync_Hist_WriteCallback);
        

        sync_hist_roi_x2_attr = new CustomAttr("Sync_Hist_ROI_X2", Tango::DEV_LONG64, Tango::READ_WRITE, Tango::AssocWritNotSpec);
        Tango::UserDefaultAttrProp ap5;
        ap5.set_description("Region of interest, right x (in binned pixels)");
        sync_hist_roi_x2_attr->set_default_properties(ap5);
        sync_hist_roi_x2_attr->set_memorized();
        sync_hist_roi_x2_attr->SetReadCallback(this, &SurfaceConceptTDC::Sync_Hist_ReadCallback);
        sync_hist_roi_x2_attr->SetWriteCallback(this, &SurfaceConceptTDC::Sync_Hist_WriteCallback);

        sync_hist_roi_y1_attr = new CustomAttr("Sync_Hist_ROI_Y1", Tango::DEV_LONG64, Tango::READ_WRITE, Tango::AssocWritNotSpec);
        Tango::UserDefaultAttrProp ap8;
        ap8.set_description("Region of interest, upper y (in binned pixels)");
        sync_hist_roi_y1_attr->set_default_properties(ap8);
        sync_hist_roi_y1_attr->set_memorized();
        sync_hist_roi_y1_attr->SetReadCallback(this, &SurfaceConceptTDC::Sync_Hist_ReadCallback);
        sync_hist_roi_y1_attr->SetWriteCallback(this, &SurfaceConceptTDC::Sync_Hist_WriteCallback);

        sync_hist_roi_y2_attr = new CustomAttr("Sync_Hist_ROI_Y2", Tango::DEV_LONG64, Tango::READ_WRITE, Tango::AssocWritNotSpec);
        Tango::UserDefaultAttrProp ap9;
        ap9.set_description("Region of interest, lower y (in binned pixels)");
        sync_hist_roi_y2_attr->set_default_properties(ap9);
        sync_hist_roi_y2_attr->set_memorized();
        sync_hist_roi_y2_attr->SetReadCallback(this, &SurfaceConceptTDC::Sync_Hist_ReadCallback);
        sync_hist_roi_y2_attr->SetWriteCallback(this, &SurfaceConceptTDC::Sync_Hist_WriteCallback);

        sync_hist_roi_t1_attr = new CustomAttr("Sync_Hist_ROI_T1", Tango::DEV_LONG64, Tango::READ_WRITE, Tango::AssocWritNotSpec);
        Tango::UserDefaultAttrProp ap12;
        ap12.set_description("Region of interest, first time (in binned pixels)");
        sync_hist_roi_t1_attr->set_default_properties(ap12);
        sync_hist_roi_t1_attr->set_memorized();
        sync_hist_roi_t1_attr->SetReadCallback(this, &SurfaceConceptTDC::Sync_Hist_ReadCallback);
        sync_hist_roi_t1_attr->SetWriteCallback(this, &SurfaceConceptTDC::Sync_Hist_WriteCallback);

        sync_hist_roi_t2_attr = new CustomAttr("Sync_Hist_ROI_T2", Tango::DEV_LONG64, Tango::READ_WRITE, Tango::AssocWritNotSpec);
        Tango::UserDefaultAttrProp ap13;
        ap13.set_description("Region of interest, second time (in binned pixels)");
        sync_hist_roi_t2_attr->set_default_properties(ap13);
        sync_hist_roi_t2_attr->set_memorized();
        sync_hist_roi_t2_attr->SetReadCallback(this, &SurfaceConceptTDC::Sync_Hist_ReadCallback);
        sync_hist_roi_t2_attr->SetWriteCallback(this, &SurfaceConceptTDC::Sync_Hist_WriteCallback);

        sync_hist_roi_toff_attr = new CustomAttr("Sync_Hist_ROI_TOFF", Tango::DEV_LONG64, Tango::READ_WRITE, Tango::AssocWritNotSpec);
        Tango::UserDefaultAttrProp ap14;
        ap14.set_description("Region of interest, time offset (in binned pixels), shifts t1 and t2, simultaneously");
        sync_hist_roi_toff_attr->set_default_properties(ap14);
        sync_hist_roi_toff_attr->set_memorized();
        sync_hist_roi_toff_attr->SetReadCallback(this, &SurfaceConceptTDC::Sync_Hist_ReadCallback);
        sync_hist_roi_toff_attr->SetWriteCallback(this, &SurfaceConceptTDC::Sync_Hist_WriteCallback);

        sync_hist_roi_tsize_attr = new CustomAttr("Sync_Hist_ROI_TSIZE", Tango::DEV_LONG64, Tango::READ_WRITE, Tango::AssocWritNotSpec);
        Tango::UserDefaultAttrProp ap15;
        ap15.set_description("Region of interest, time size (in binned pixels), sets t2=t1+tsize");
        sync_hist_roi_tsize_attr->set_default_properties(ap15);
        sync_hist_roi_tsize_attr->set_memorized();
        sync_hist_roi_tsize_attr->SetReadCallback(this, &SurfaceConceptTDC::Sync_Hist_ReadCallback);
        sync_hist_roi_tsize_attr->SetWriteCallback(this, &SurfaceConceptTDC::Sync_Hist_WriteCallback);

        sync_hist_int_x1_attr = new CustomAttr("Sync_Hist_INT_X1", Tango::DEV_LONG64, Tango::READ_WRITE, Tango::AssocWritNotSpec);
        Tango::UserDefaultAttrProp ap16;
        ap16.set_description("Integration range, left x (in binned pixels)");
        sync_hist_int_x1_attr->set_default_properties(ap16);
        sync_hist_int_x1_attr->set_memorized();
        sync_hist_int_x1_attr->SetReadCallback(this, &SurfaceConceptTDC::Sync_Hist_ReadCallback);
        sync_hist_int_x1_attr->SetWriteCallback(this, &SurfaceConceptTDC::Sync_Hist_WriteCallback);

        sync_hist_int_x2_attr = new CustomAttr("Sync_Hist_INT_X2", Tango::DEV_LONG64, Tango::READ_WRITE, Tango::AssocWritNotSpec);
        Tango::UserDefaultAttrProp ap17;
        ap17.set_description("Integration range, right x (in binned pixels)");
        sync_hist_int_x2_attr->set_default_properties(ap17);
        sync_hist_int_x2_attr->set_memorized();
        sync_hist_int_x2_attr->SetReadCallback(this, &SurfaceConceptTDC::Sync_Hist_ReadCallback);
        sync_hist_int_x2_attr->SetWriteCallback(this, &SurfaceConceptTDC::Sync_Hist_WriteCallback);

        sync_hist_int_y1_attr = new CustomAttr("Sync_Hist_INT_Y1", Tango::DEV_LONG64, Tango::READ_WRITE, Tango::AssocWritNotSpec);
        Tango::UserDefaultAttrProp ap18;
        ap18.set_description("Integration range, upper y (in binned pixels)");
        sync_hist_int_y1_attr->set_default_properties(ap18);
        sync_hist_int_y1_attr->set_memorized();
        sync_hist_int_y1_attr->SetReadCallback(this, &SurfaceConceptTDC::Sync_Hist_ReadCallback);
        sync_hist_int_y1_attr->SetWriteCallback(this, &SurfaceConceptTDC::Sync_Hist_WriteCallback);

        sync_hist_int_y2_attr = new CustomAttr("Sync_Hist_INT_Y2", Tango::DEV_LONG64, Tango::READ_WRITE, Tango::AssocWritNotSpec);
        Tango::UserDefaultAttrProp ap19;
        ap19.set_description("Integration range, lower y (in binned pixels)");
        sync_hist_int_y2_attr->set_default_properties(ap19);
        sync_hist_int_y2_attr->set_memorized();
        sync_hist_int_y2_attr->SetReadCallback(this, &SurfaceConceptTDC::Sync_Hist_ReadCallback);
        sync_hist_int_y2_attr->SetWriteCallback(this, &SurfaceConceptTDC::Sync_Hist_WriteCallback);

        sync_hist_int_t1_attr = new CustomAttr("Sync_Hist_INT_T1", Tango::DEV_LONG64, Tango::READ_WRITE, Tango::AssocWritNotSpec);
        Tango::UserDefaultAttrProp ap20;
        ap20.set_description("Integration range, first time (in binned pixels)");
        sync_hist_int_t1_attr->set_default_properties(ap20);
        sync_hist_int_t1_attr->set_memorized();
        sync_hist_int_t1_attr->SetReadCallback(this, &SurfaceConceptTDC::Sync_Hist_ReadCallback);
        sync_hist_int_t1_attr->SetWriteCallback(this, &SurfaceConceptTDC::Sync_Hist_WriteCallback);

        sync_hist_int_t2_attr = new CustomAttr("Sync_Hist_INT_T2", Tango::DEV_LONG64, Tango::READ_WRITE, Tango::AssocWritNotSpec);
        Tango::UserDefaultAttrProp ap21;
        ap21.set_description("Integration range, second time (in binned pixels)");
        sync_hist_int_t2_attr->set_default_properties(ap21);
        sync_hist_int_t2_attr->set_memorized();
        sync_hist_int_t2_attr->SetReadCallback(this, &SurfaceConceptTDC::Sync_Hist_ReadCallback);
        sync_hist_int_t2_attr->SetWriteCallback(this, &SurfaceConceptTDC::Sync_Hist_WriteCallback);
        
        sync_hist_modulo_attr = new CustomAttr("Sync_Hist_MODULO", Tango::DEV_LONG64, Tango::READ_WRITE, Tango::AssocWritNotSpec);
        Tango::UserDefaultAttrProp ap22;
        ap22.set_description("Time axis modulo, applied after binning");
        sync_hist_modulo_attr->set_default_properties(ap22);
        sync_hist_modulo_attr->set_memorized();
        sync_hist_modulo_attr->SetReadCallback(this, &SurfaceConceptTDC::Sync_Hist_ReadCallback);
        sync_hist_modulo_attr->SetWriteCallback(this, &SurfaceConceptTDC::Sync_Hist_WriteCallback);
        
        sync_hist_raw_toff_attr = new CustomAttr("Sync_Hist_Raw_TOFF", Tango::DEV_LONG64, Tango::READ_WRITE, Tango::AssocWritNotSpec);
        Tango::UserDefaultAttrProp ap23;
        ap23.set_description("Time axis offset in unbinned units");
        sync_hist_raw_toff_attr->set_default_properties(ap23);
        sync_hist_raw_toff_attr->set_memorized();
        sync_hist_raw_toff_attr->SetReadCallback(this, &SurfaceConceptTDC::Sync_Hist_ReadCallback);
        sync_hist_raw_toff_attr->SetWriteCallback(this, &SurfaceConceptTDC::Sync_Hist_WriteCallback);
        
        sync_hist_raw_toff_fixed_attr = new CustomAttr("Sync_Hist_Raw_TOFF_Fixed", Tango::DEV_BOOLEAN, Tango::READ_WRITE, Tango::AssocWritNotSpec);
        Tango::UserDefaultAttrProp ap24;
        ap24.set_description("If set to true, auto-adjust Sync_Hist_ROI_TOFF when Sync_Hist_BIN_T is changed, so that Sync_Hist_RAW_TOFF is unchanged.");
        sync_hist_raw_toff_fixed_attr->set_default_properties(ap24);
        sync_hist_raw_toff_fixed_attr->set_memorized();
        sync_hist_raw_toff_fixed_attr->set_memorized_init(true);
        sync_hist_raw_toff_fixed_attr->SetReadCallback(this, &SurfaceConceptTDC::Sync_Hist_ReadCallback);
        sync_hist_raw_toff_fixed_attr->SetWriteCallback(this, &SurfaceConceptTDC::Sync_Hist_WriteCallback);

        this->add_attribute(sync_hist_bin_x_attr);
        this->add_attribute(sync_hist_bin_y_attr);
        this->add_attribute(sync_hist_bin_t_attr);
        this->add_attribute(sync_hist_roi_x1_attr);
        this->add_attribute(sync_hist_roi_x2_attr);
        this->add_attribute(sync_hist_roi_y1_attr);
        this->add_attribute(sync_hist_roi_y2_attr);
        this->add_attribute(sync_hist_roi_t1_attr);
        this->add_attribute(sync_hist_roi_t2_attr);
        this->add_attribute(sync_hist_roi_toff_attr);
        this->add_attribute(sync_hist_roi_tsize_attr);
        this->add_attribute(sync_hist_int_x1_attr);
        this->add_attribute(sync_hist_int_x2_attr);
        this->add_attribute(sync_hist_int_y1_attr);
        this->add_attribute(sync_hist_int_y2_attr);
        this->add_attribute(sync_hist_int_t1_attr);
        this->add_attribute(sync_hist_int_t2_attr);
        this->add_attribute(sync_hist_modulo_attr);
        this->add_attribute(sync_hist_raw_toff_attr);
        this->add_attribute(sync_hist_raw_toff_fixed_attr);
    }

    void SurfaceConceptTDC::Sync_Hist_ReadCallback(Tango::DeviceImpl* dev, Tango::Attribute& att) {
        std::string attrname = att.get_name();
        if (attrname.find("Sync_Hist_")!=0)
            return;
        attrname = attrname.substr(10); // discard Sync_Hist_
        if (attrname.substr(0,3).compare("BIN")==0) {
            // BINNING
            attrname = attrname.substr(4); // discard BIN_
            if (attrname.compare("X")==0)
                att.set_value(&sync_hist_bin_x_val);
            else if (attrname.compare("Y")==0)
                att.set_value(&sync_hist_bin_y_val);
            else if (attrname.compare("T")==0)
                att.set_value(&sync_hist_bin_t_val);
        }
        else if (attrname.substr(0,3).compare("ROI")==0) {
            // REGION OF INTEREST   
            attrname = attrname.substr(4); // discard ROI_
            if (attrname.compare("X1")==0)
                att.set_value(&sync_hist_roi_x1_val);
            else if (attrname.compare("X2")==0)
                att.set_value(&sync_hist_roi_x2_val);
            else if (attrname.compare("Y1")==0)
                att.set_value(&sync_hist_roi_y1_val);
            else if (attrname.compare("Y2")==0)
                att.set_value(&sync_hist_roi_y2_val);
            else if (attrname.compare("T1")==0)
                att.set_value(&sync_hist_roi_t1_val);
            else if (attrname.compare("T2")==0)
                att.set_value(&sync_hist_roi_t2_val);
            else if (attrname.compare("TOFF")==0)
                att.set_value(&sync_hist_roi_toff_val);
            else if (attrname.compare("TSIZE")==0)
                att.set_value(&sync_hist_roi_tsize_val);
        }
        else if (attrname.substr(0,3).compare("INT")==0) {
            // INTEGRATION RANGE
            attrname = attrname.substr(4); // discard INT_
            if (attrname.compare("X1")==0)
                att.set_value(&sync_hist_int_x1_val);
            else if (attrname.compare("X2")==0)
                att.set_value(&sync_hist_int_x2_val);
            else if (attrname.compare("Y1")==0)
                att.set_value(&sync_hist_int_y1_val);
            else if (attrname.compare("Y2")==0)
                att.set_value(&sync_hist_int_y2_val);
            else if (attrname.compare("T1")==0)
                att.set_value(&sync_hist_int_t1_val);
            else if (attrname.compare("T2")==0)
                att.set_value(&sync_hist_int_t2_val);
        }
        else if (attrname.compare("MODULO")==0) {
            att.set_value(&sync_hist_modulo_val);
        }
        else if (attrname.compare("Raw_TOFF")==0) {
            att.set_value(&sync_hist_raw_toff_val);
        }
        else if (attrname.compare("Raw_TOFF_Fixed")==0) {
            att.set_value(&sync_hist_raw_toff_fixed_val);
        }
    }
    
    void SurfaceConceptTDC::Sync_Hist_WriteCallback(Tango::DeviceImpl*, Tango::WAttribute& attr) {
        std::string attrname = attr.get_name();
        Tango::DevLong64 w_val;
        attr.get_write_value(w_val);
        if (attrname.find("Sync_Hist_")!=0)
            return;
        
        attrname = attrname.substr(10); // discard Sync_Hist_
        if (attrname.substr(0,3).compare("BIN")==0) {
            // BINNING
            attrname = attrname.substr(4); // discard BIN_
            if (attrname.compare("X")==0) {// handler for Sync_Hist_BIN_X
                for (std::string targetattr : {"Hist_Live_XY_BIN_X",
                        "Hist_Live_XT_BIN_X","Hist_Live_T_BIN_X",
                        "Hist_Live_YT_BIN_X","Hist_Accu_XYT_BIN_X", "Hist_User_T_BIN_X"})
                            Write_Own_DevLong64_Attribute(targetattr, w_val);
                sync_hist_bin_x_val = w_val;
            }
            else if (attrname.compare("Y")==0) { // handler for Sync_Hist_BIN_Y
                for (std::string targetattr : {"Hist_Live_XY_BIN_Y",
                        "Hist_Live_YT_BIN_Y","Hist_Live_XT_BIN_Y",
                        "Hist_Live_T_BIN_Y","Hist_Accu_XYT_BIN_Y", "Hist_User_T_BIN_Y"})
                            Write_Own_DevLong64_Attribute(targetattr, w_val);
                sync_hist_bin_y_val = w_val;
            }
            else if (attrname.compare("T")==0) { // handler for Sync_Hist_BIN_T
                for (std::string targetattr : {"Hist_Live_XT_BIN_T",
                        "Hist_Live_YT_BIN_T","Hist_Live_XY_BIN_T",
                        "Hist_Live_T_BIN_T","Hist_Accu_XYT_BIN_T"})
                            Write_Own_DevLong64_Attribute(targetattr, w_val);
                sync_hist_bin_t_val = w_val;
                if (sync_hist_raw_toff_fixed_val)
                    Write_Own_DevLong64_Attribute("Sync_Hist_Raw_TOFF", sync_hist_raw_toff_val); // rewrite it
            }
        }
        else if (attrname.substr(0,3).compare("ROI")==0) {
            // REGION OF INTEREST   
            attrname = attrname.substr(4); // discard ROI_
            if (attrname.compare("X1")==0 || attrname.compare("X2")==0) {
                for (std::string targetattr : {"Hist_Live_XY_ROI_",
                        "Hist_Live_XT_ROI_","Hist_Accu_XYT_ROI_"})
                            Write_Own_DevLong64_Attribute(targetattr+attrname, w_val);
                if (attrname.compare("X1")==0)
                    sync_hist_roi_x1_val = w_val;
                else 
                    sync_hist_roi_x2_val = w_val;
            }
            else if (attrname.compare("Y1")==0 || attrname.compare("Y2")==0) {
                for (std::string targetattr : {"Hist_Live_XY_ROI_",
                        "Hist_Live_YT_ROI_","Hist_Accu_XYT_ROI_"})
                            Write_Own_DevLong64_Attribute(targetattr+attrname, w_val);                    
                if (attrname.compare("Y1")==0)
                    sync_hist_roi_y1_val = w_val;
                else 
                    sync_hist_roi_y2_val = w_val;
                
            }
            else if (attrname.compare("T1")==0 || attrname.compare("T2")==0 ||
                    attrname.compare("TOFF")==0 || attrname.compare("TSIZE")==0) {
                for (std::string targetattr : {"Hist_Live_XT_ROI_",
                        "Hist_Live_YT_ROI_","Hist_Live_T_ROI_","Hist_Accu_XYT_ROI_"})
                            Write_Own_DevLong64_Attribute(targetattr+attrname, w_val);
                if (attrname.compare("T1")==0) {
                    sync_hist_roi_t1_val = w_val;
                    sync_hist_roi_toff_val = w_val;
                    sync_hist_roi_tsize_val = sync_hist_roi_t2_val-sync_hist_roi_t1_val+1;
                    sync_hist_raw_toff_val =  w_val * sync_hist_bin_t_val;
                }
                else if (attrname.compare("T2")==0) {
                    sync_hist_roi_t2_val = w_val;
                    sync_hist_roi_tsize_val = sync_hist_roi_t2_val-sync_hist_roi_t1_val+1;
                }
                else if (attrname.compare("TOFF")==0) {
                    sync_hist_roi_toff_val = w_val;
                    sync_hist_roi_t1_val = w_val;
                    sync_hist_roi_t2_val = w_val+sync_hist_roi_tsize_val-1;
                    sync_hist_raw_toff_val = w_val * sync_hist_bin_t_val;
                }
                else { // "TSIZE"
                    sync_hist_roi_tsize_val = w_val;
                    sync_hist_roi_t2_val = sync_hist_roi_t1_val+sync_hist_roi_tsize_val-1;
                }

            }
        }
        else if (attrname.substr(0,3).compare("INT")==0) {
            // INTEGRATION RANGE
            attrname = attrname.substr(4); // discard INT_
            if (attrname.compare("X1")==0 || attrname.compare("X2")==0 ) {
                for (std::string targetattr : {"Hist_Live_YT_ROI_",
                        "Hist_Live_T_ROI_","Hist_Accu_YT_ROI_","Hist_Accu_T_ROI_"})
                            Write_Own_DevLong64_Attribute(targetattr+attrname, w_val);
                if (attrname.compare("X1")==0)
                    sync_hist_int_x1_val = w_val;
                else
                    sync_hist_int_x2_val = w_val;
            }
            else if (attrname.compare("Y1")==0 || attrname.compare("Y2")==0) {
                for (std::string targetattr : {"Hist_Live_XT_ROI_",
                        "Hist_Live_T_ROI_","Hist_Accu_XT_ROI_","Hist_Accu_T_ROI_"})
                            Write_Own_DevLong64_Attribute(targetattr+attrname, w_val);
                if (attrname.compare("Y1")==0)
                    sync_hist_int_y1_val = w_val;
                else
                    sync_hist_int_y2_val = w_val;
            }
            else if (attrname.compare("T1")==0 || attrname.compare("T2")==0) {
                for (std::string targetattr : {"Hist_Live_XY_ROI_", "Hist_Accu_XY_ROI_"}) 
                    Write_Own_DevLong64_Attribute(targetattr+attrname, w_val);
                if (attrname.compare("T1")==0)
                    sync_hist_int_t1_val = w_val;
                else
                    sync_hist_int_t2_val = w_val;
                
            }
        }
        else if (attrname.compare("MODULO")==0) {
            for (std::string targetattr : {"Hist_Live_XY_", "Hist_Live_XT_",
                    "Hist_Live_YT_", "Hist_Live_T_",  "Hist_Accu_XYT_"})
                Write_Own_DevLong64_Attribute(targetattr+"MODULO", w_val);
            sync_hist_modulo_val = w_val;        
        }
        else if (attrname.compare("Raw_TOFF")==0) {
            Write_Own_DevLong64_Attribute("Sync_Hist_ROI_TOFF", w_val/sync_hist_bin_t_val);
            sync_hist_raw_toff_val = w_val;
        }
        else if (attrname.compare("Raw_TOFF_Fixed")==0) {
            Tango::DevBoolean boolw_val; // boolean is not well-casted to Tango::DevLong
            attr.get_write_value(boolw_val);
            sync_hist_raw_toff_fixed_val = boolw_val;
        }
    }
    
} // namespace