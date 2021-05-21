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

#include <time.h>
#include <string>
#include <vector>
#include <string.h>
#include <thread>
#include <iomanip>
#include <scTDC_types.h>
#include <scTDC_deprecated.h>
//#include <attrdesc.h>

#include "SurfaceConceptTDC.h"
#include "CustomAttr.h"
#include "GeneralHistogram.h"
#include "Helper.h"
#include "IntegrateXYT.h"
#include "SaveAfterAccumModes.h"

namespace SurfaceConceptTDC_ns {

void SurfaceConceptTDC::AddHistogramAttributes() {
    m_hist_map["Hist_Live_XY"] = new GeneralHistogram(::sc_pipe_type_t::DLD_IMAGE_XY);
    m_hist_map["Hist_Accu_XY"] = new GeneralHistogram(::sc_pipe_type_t::DLD_IMAGE_XY);
    m_hist_map["Hist_Live_XT"] = new GeneralHistogram(::sc_pipe_type_t::DLD_IMAGE_XT);
    m_hist_map["Hist_Accu_XT"] = new GeneralHistogram(::sc_pipe_type_t::DLD_IMAGE_XT);
    m_hist_map["Hist_Live_YT"] = new GeneralHistogram(::sc_pipe_type_t::DLD_IMAGE_YT);
    m_hist_map["Hist_Accu_YT"] = new GeneralHistogram(::sc_pipe_type_t::DLD_IMAGE_YT);
    m_hist_map["Hist_Live_T"] = new GeneralHistogram(::sc_pipe_type_t::DLD_SUM_HISTO);
    m_hist_map["Hist_Accu_T"] = new GeneralHistogram(::sc_pipe_type_t::DLD_SUM_HISTO);
    m_hist_map["Hist_Accu_XYT"] = new GeneralHistogram(::sc_pipe_type_t::DLD_IMAGE_3D);
    m_hist_map["Hist_Full_XY"] = new GeneralHistogram(::sc_pipe_type_t::DLD_IMAGE_XY); // live and full range
    m_hist_map["Hist_Full_T"] = new GeneralHistogram(::sc_pipe_type_t::DLD_SUM_HISTO); // live and full range
    m_hist_map["Hist_User_T"] = new GeneralHistogram(::sc_pipe_type_t::DLD_SUM_HISTO); // independent modulo, binning, and ROI
    // remove a few attributes (which will be exposed as Tango attributes) : those that are linked
    m_hist_map["Hist_Accu_XY"]->Attributes.clear();
    m_hist_map["Hist_Accu_XY"]->Attributes.push_back("ROI_T1");
    m_hist_map["Hist_Accu_XY"]->Attributes.push_back("ROI_T2");
    m_hist_map["Hist_Accu_XY"]->Attributes.push_back("ROI_TOFF");
    m_hist_map["Hist_Accu_XY"]->Attributes.push_back("ROI_TSIZE");
    m_hist_map["Hist_Accu_XT"]->Attributes.clear();
    m_hist_map["Hist_Accu_XT"]->Attributes.push_back("ROI_Y1");
    m_hist_map["Hist_Accu_XT"]->Attributes.push_back("ROI_Y2");
    m_hist_map["Hist_Accu_YT"]->Attributes.clear();
    m_hist_map["Hist_Accu_YT"]->Attributes.push_back("ROI_X1");
    m_hist_map["Hist_Accu_YT"]->Attributes.push_back("ROI_X2");
    m_hist_map["Hist_Accu_T"]->Attributes.clear();
    m_hist_map["Hist_Accu_T"]->Attributes.push_back("ROI_X1");
    m_hist_map["Hist_Accu_T"]->Attributes.push_back("ROI_X2");
    m_hist_map["Hist_Accu_T"]->Attributes.push_back("ROI_Y1");
    m_hist_map["Hist_Accu_T"]->Attributes.push_back("ROI_Y2");
    // allow certain histograms to be changed during acquisition (those that don't need a pipe)
    pipeless_histograms.push_back("Hist_Accu_XY");
    pipeless_histograms.push_back("Hist_Accu_XT");
    pipeless_histograms.push_back("Hist_Accu_YT");
    pipeless_histograms.push_back("Hist_Accu_T");
    
    
    m_hist_map["Hist_Full_XY"]->Attributes.clear(); // only 4 Tango attributes
    m_hist_map["Hist_Full_XY"]->Attributes.push_back("ROI_X2");
    m_hist_map["Hist_Full_XY"]->Attributes.push_back("ROI_Y2");
    m_hist_map["Hist_Full_XY"]->Attributes.push_back("BIN_X");
    m_hist_map["Hist_Full_XY"]->Attributes.push_back("BIN_Y");
    
    m_hist_map["Hist_Full_T"]->Attributes.clear(); // no Tango attributes at all
    m_hist_map["Hist_Full_Counts"] = new GeneralHistogram(::sc_pipe_type_t::DLD_SUM_HISTO); // live and full range
    m_hist_map["Hist_Full_Counts"]->Attributes.clear(); // no Tango attributes at all

    if (livePreviewModeFileActive) {
        // set file targets
        std::string fpath;
        fpath = Helper::join_pathnames(this->livePreviewModeFilePath, "live", "xy");
        m_hist_map.at("Hist_Live_XY")->SetFilePath(fpath);
        m_hist_map.at("Hist_Live_XY")->SetFileOutputActive(true);
        fpath = Helper::join_pathnames(this->livePreviewModeFilePath, "live", "xt");
        m_hist_map.at("Hist_Live_XT")->SetFilePath(fpath);
        m_hist_map.at("Hist_Live_XT")->SetFileOutputActive(true);
        fpath = Helper::join_pathnames(this->livePreviewModeFilePath, "live", "yt");
        m_hist_map.at("Hist_Live_YT")->SetFilePath(fpath);
        m_hist_map.at("Hist_Live_YT")->SetFileOutputActive(true);
        fpath = Helper::join_pathnames(this->livePreviewModeFilePath, "live", "t");
        m_hist_map.at("Hist_Live_T")->SetFilePath(fpath);
        m_hist_map.at("Hist_Live_T")->SetFileOutputActive(true);
        m_hist_map.at("Hist_Live_T")->SetPGMPath(fpath); // will append .pgm by itself
        m_hist_map.at("Hist_Live_T")->SetPGMOutputActive(true);
        fpath = Helper::join_pathnames(this->livePreviewModeFilePath, "live", "xyfull");
        m_hist_map.at("Hist_Full_XY")->SetPGMPath(fpath);
        m_hist_map.at("Hist_Full_XY")->SetPGMOutputActive(true);
        fpath = Helper::join_pathnames(this->livePreviewModeFilePath, "live", "tfull");
        m_hist_map.at("Hist_Full_T")->SetPGMPath(fpath);
        m_hist_map.at("Hist_Full_T")->SetPGMOutputActive(true);
        m_hist_map.at("Hist_Full_T")->SetPGMSize(fullHistTPGMPreviewWidth,128);
        fpath = Helper::join_pathnames(this->livePreviewModeFilePath, "accu", "xy");
        m_hist_map.at("Hist_Accu_XY")->SetFilePath(fpath);
        m_hist_map.at("Hist_Accu_XY")->SetFileOutputActive(true);
        fpath = Helper::join_pathnames(this->livePreviewModeFilePath, "accu", "xt");
        m_hist_map.at("Hist_Accu_XT")->SetFilePath(fpath);
        m_hist_map.at("Hist_Accu_XT")->SetFileOutputActive(true);
        fpath = Helper::join_pathnames(this->livePreviewModeFilePath, "accu", "yt");
        m_hist_map.at("Hist_Accu_YT")->SetFilePath(fpath);
        m_hist_map.at("Hist_Accu_YT")->SetFileOutputActive(true);
        fpath = Helper::join_pathnames(this->livePreviewModeFilePath, "accu", "t");
        m_hist_map.at("Hist_Accu_T")->SetFilePath(fpath);
        m_hist_map.at("Hist_Accu_T")->SetFileOutputActive(true);
    }
    
    if (livePreviewModeTangoActive) {
        // Hist_Full_T ############################################################################################
        hist_full_t_attr = new CustomSpectrumAttr("Hist_Full_T", Tango::DEV_LONG, Tango::READ, fullHistTSize);
        hist_full_t_attr->SetReadCallback(this, &SurfaceConceptTDC::Hist_Full_T_ReadCallback);
        hist_full_t_attr->set_change_event(true, false); // we will push change events ( so no polling is required )
        m_hist_map.at("Hist_Full_T")->SetTangoOutAttribute(hist_full_t_attr);
        Tango::UserDefaultAttrProp ap1;
        ap1.set_description("full time axis spectrum (size controlled by device properties) in live mode");
        hist_full_t_attr->set_default_properties(ap1);
        add_attribute(hist_full_t_attr);
        // Hist_Accu_Full_T ############################################################################################
        hist_accu_full_t_attr = new CustomSpectrumAttr("Hist_Full_Accu_T", Tango::DEV_LONG, Tango::READ, fullHistTSize);
        hist_accu_full_t_attr->SetReadCallback(this, &SurfaceConceptTDC::Hist_Full_Accu_T_ReadCallback);
        hist_accu_full_t_attr->set_change_event(true, false); // we will push change events ( so no polling is required )
        m_hist_map.at("Hist_Full_T")->SetTangoAccuBufferActive(true);
        Tango::UserDefaultAttrProp ap2;
        ap2.set_description("full time axis spectrum (size controlled by device properties) in accumulation mode");
        hist_accu_full_t_attr->set_default_properties(ap2);
        add_attribute(hist_accu_full_t_attr);
        // Hist_Live_T ############################################################################################
        hist_live_t_attr = new CustomSpectrumAttr("Hist_Live_T", Tango::DEV_LONG, Tango::READ, hist_accu_t_max_size);
        hist_live_t_attr->SetReadCallback(this, &SurfaceConceptTDC::Hist_Live_T_ReadCallback);
        hist_live_t_attr->set_change_event(true, false);
        Tango::UserDefaultAttrProp ap3a;
        ap3a.set_description("time spectrum defined by user ROI according to the dimensions of the Accu_XYT set");
        hist_live_t_attr->set_default_properties(ap3a);
        m_hist_map.at("Hist_Live_T")->SetTangoOutAttribute(hist_live_t_attr);
        add_attribute(hist_live_t_attr);
        // Hist_Accu_T ############################################################################################
        hist_accu_t_attr = new CustomSpectrumAttr("Hist_Accu_T", Tango::DEV_LONG, Tango::READ, hist_accu_t_max_size);
        hist_accu_t_attr->SetReadCallback(this, &SurfaceConceptTDC::Hist_Accu_T_ReadCallback);
        hist_accu_t_attr->set_change_event(true, false);
        Tango::UserDefaultAttrProp ap3;
        ap3.set_description("time spectrum representing the integration over x and y of the accumulated xyz data set");
        hist_accu_t_attr->set_default_properties(ap3);
        m_hist_map.at("Hist_Accu_T")->SetTangoOutAttribute(hist_accu_t_attr);
        add_attribute(hist_accu_t_attr);
        // Hist_Live_User_T ############################################################################################
        hist_live_user_t_attr = new CustomSpectrumAttr("Hist_Live_User_T", Tango::DEV_LONG, Tango::READ, hist_accu_t_max_size);
        hist_live_user_t_attr->SetReadCallback(this, &SurfaceConceptTDC::Hist_Live_User_T_ReadCallback);
        hist_live_user_t_attr->set_change_event(true, false);
        Tango::UserDefaultAttrProp ap3b;
        ap3b.set_description("time spectrum with MODULO, x,y,t-ROI, t-BINNING set by Hist_User_T_... attributes.");
        hist_live_user_t_attr->set_default_properties(ap3b);
        m_hist_map.at("Hist_User_T")->SetTangoOutAttribute(hist_live_user_t_attr); // Hist_Live_User_T and Hist_Accu_User_T both use GeneralHistogram Hist_User_T
        add_attribute(hist_live_user_t_attr);
        // Hist_Accu_User_T ############################################################################################
        hist_accu_user_t_attr = new CustomSpectrumAttr("Hist_Accu_User_T", Tango::DEV_LONG, Tango::READ, hist_accu_t_max_size);
        hist_accu_user_t_attr->SetReadCallback(this, &SurfaceConceptTDC::Hist_Accu_User_T_ReadCallback);
        hist_accu_user_t_attr->set_change_event(true, false);
        Tango::UserDefaultAttrProp ap3c;
        ap3c.set_description("accumulated time spectrum with MODULO, x,y,t-ROI, t-BINNING set by Hist_User_T_... attributes.");
        hist_accu_user_t_attr->set_default_properties(ap3c);
        m_hist_map.at("Hist_User_T")->SetTangoAccuBufferActive(true); // Hist_Live_User_T and Hist_Accu_User_T both use GeneralHistogram Hist_User_T
        add_attribute(hist_accu_user_t_attr);
        // Hist_Live_TAxis #########################################################################################
        hist_live_taxis_attr = new CustomSpectrumAttr("Hist_Live_TAxis", Tango::DEV_DOUBLE, Tango::READ, hist_accu_t_max_size);
        hist_live_taxis_attr->SetReadCallback(this, &SurfaceConceptTDC::Hist_Live_TAxis_ReadCallback);
        Tango::UserDefaultAttrProp ap4a;
        ap4a.set_description("Time axis for Hist_Live_T attribute in pixels or nanoseconds");
        hist_live_taxis_attr->set_default_properties(ap4a);
        add_attribute(hist_live_taxis_attr);
        // Hist_Accu_TAxis #########################################################################################
        hist_accu_taxis_attr = new CustomSpectrumAttr("Hist_Accu_TAxis", Tango::DEV_DOUBLE, Tango::READ, hist_accu_t_max_size);
        hist_accu_taxis_attr->SetReadCallback(this, &SurfaceConceptTDC::Hist_Accu_TAxis_ReadCallback);
        Tango::UserDefaultAttrProp ap4;
        ap4.set_description("Time axis for Hist_Accu_T attribute in pixels or nanoseconds");
        hist_accu_taxis_attr->set_default_properties(ap4);
        add_attribute(hist_accu_taxis_attr);
        // Hist_Full_TAxis #########################################################################################
        hist_full_taxis_attr = new CustomSpectrumAttr("Hist_Full_TAxis", Tango::DEV_DOUBLE, Tango::READ, fullHistTSize);
        hist_full_taxis_attr->SetReadCallback(this, &SurfaceConceptTDC::Hist_Full_TAxis_ReadCallback);
        Tango::UserDefaultAttrProp ap5;
        ap5.set_description("Time axis for Hist_Full_T and Hist_Full_Accu_T attributes in the unit set in Hist_TAxis_Unit");
        hist_full_taxis_attr->set_default_properties(ap5);
        add_attribute(hist_full_taxis_attr);
        // Hist_User_TAxis ############################################################################################
        hist_user_taxis_attr = new CustomSpectrumAttr("Hist_User_TAxis", Tango::DEV_DOUBLE, Tango::READ, hist_accu_t_max_size);
        hist_user_taxis_attr->SetReadCallback(this, &SurfaceConceptTDC::Hist_User_Taxis_ReadCallback);
        Tango::UserDefaultAttrProp ap5b;
        ap5b.set_description("Time axis for Hist_Live_User_T and Hist_Accu_User_T.");
        hist_user_taxis_attr->set_default_properties(ap5b);
        add_attribute(hist_user_taxis_attr);        
        // Hist_TAxis_In_Ns #######################################################################################
        hist_taxis_unit_attr = new CustomAttr("Hist_TAxis_Unit", Tango::DEV_STRING, Tango::READ_WRITE, Tango::AssocWritNotSpec);
        hist_taxis_unit_attr->SetReadCallback(this, &SurfaceConceptTDC::MiscTDCReadCallback);
        hist_taxis_unit_attr->SetWriteCallback(this, &SurfaceConceptTDC::MiscTDCWriteCallback);
        hist_taxis_unit_attr->set_memorized();
        hist_taxis_unit_attr->set_memorized_init(true);
        Tango::UserDefaultAttrProp ap6;
        ap6.set_description("The unit of Hist_..._TAxis attributes. Allowed values: pixels, unbinned pixels, ps, ns, us, ms, s");
        ap6.set_unit("");
        hist_taxis_unit_attr->set_default_properties(ap6);
        add_attribute(hist_taxis_unit_attr);
        
    }
    
    std::string name;
    std::string attrib;
    
    Tango::UserDefaultAttrProp	attrprop;
    for (auto &hist : m_hist_map) 
        for (std::string attrib : hist.second->Attributes) {
            name = hist.first + "_" + attrib;
            m_dyn_attr_names.push_back(std::string(name.c_str())); // force a deep copy
            CustomAttr *newattr = new CustomAttr(m_dyn_attr_names.back().c_str(), Tango::DEV_LONG64, Tango::READ_WRITE, Tango::AssocWritNotSpec);
            // set maximum value property
            {   // scope for local, short-lived oss variable
                std::ostringstream oss; // sigh, the c++ way to format numbers...
                long maxval = hist.second->GetAttributeMax(attrib);
                if (maxval!=-1L) {
                    oss << maxval;
                    attrprop.max_value = oss.str();
                }
                else attrprop.max_value = "";
            }
            // set minimum value property
            {   // scope for local, short-lived oss variable
                std::ostringstream oss; // resetting ostringstreams is not advised -> use a new object
                long minval = hist.second->GetAttributeMin(attrib);
                if (minval!=-1L) {
                    oss << minval;
                    attrprop.min_value = oss.str();
                }
                else attrprop.min_value = "";
                    
            }
            // set format property
            attrprop.format = hist.second->GetAttributeFormat(attrib);
            newattr->set_memorized_init(true);
            newattr->set_memorized();
            // set all these properties
            newattr->set_default_properties(attrprop);
            // add the attribute to our own map
            m_dyn_attr_map[name] = newattr;
            //m_dyn_attr_map[m_dyn_attr_names.back()]->set_class_properties()
            m_dyn_attr_long_vals[name] = hist.second->GetAttribute(attrib);
        }
    // m_dyn_attr_map is just our own custom dictionary of dynamic attributes
    // the tango control system doesn't know anything about m_dyn_attr_map
    // -------------------------------------------
    // now, add all attributes to this Tango server and register our callback function
    for (auto& iter : m_dyn_attr_map) { // iter is a pair, where first=key and second=CustomAttr
        this->add_attribute(iter.second);
        iter.second->SetReadCallback(this, &SurfaceConceptTDC::HistogramAttributeReadCallback);
        iter.second->SetWriteCallback(this, &SurfaceConceptTDC::HistogramAttributeWriteCallback);
    }
    SetFullRangeHistogramProperties();
}

void SurfaceConceptTDC::SetFullRangeHistogramProperties() {
    GeneralHistogram *h = m_hist_map.at("Hist_Full_XY");
    h->SetAttribute("ROI_X1", 0);
    h->SetAttribute("ROI_X2", 256);
    h->SetAttribute("ROI_Y1", 0);
    h->SetAttribute("ROI_Y2", 256);
    h->SetAttribute("ROI_T1", 0);
    h->SetAttribute("ROI_T2", 2147483647);
    h->SetAttribute("MODULO", 0);
    h->SetAttribute("BIN_T", 1);
    long binxy = 8;
    try {
        binxy = std::stoi(this->fullHistBinXY);
    }
    catch (std::invalid_argument exception) {
        std::cout << "FullHistBinXY property is set to an invalid value " << this->fullHistBinXY << std::endl;
        std::cout << "using default value " << binxy << std::endl;
    }
    h->SetAttribute("BIN_X", binxy);
    h->SetAttribute("BIN_Y", binxy);
    //std::cout << "Size data buffer Hist_Full_XY: "<< h->GetWidth()*h->GetHeight()*h->GetZSize() << std::endl;
    h = m_hist_map.at("Hist_Full_Counts");
    h->SetAttribute("BIN_T", 2147483648);
    h->SetAttribute("ROI_T1", 0);
    h->SetAttribute("ROI_T2", 0);
    // ###   Hist_Full_T        ##################################
    // ###   Hist_Full_T_BINT   ##################################
    h = m_hist_map.at("Hist_Full_T");
    long bint = 4194304;
    try {
        bint = std::stoi(this->fullHistBinT);
    }
    catch (std::invalid_argument exception) {
        std::cout << "FullHistBinT property is set to an invalid value " << this->fullHistBinT << std::endl;
        std::cout << "using default value " << bint << std::endl;
    }
    h->SetAttribute("BIN_T", bint);
    // ###   Hist_Full_T_TOFF   ################################## 
    long toff = 0;
    try {
        toff = std::stoi(this->fullHistTOff);
    }
    catch (std::invalid_argument exception) {
        std::cout << "FullHistBinT property is set to an invalid value " << this->fullHistTOff << std::endl;
        std::cout << "using default value " << toff << std::endl;
    }
    h->SetAttribute("ROI_TOFF", toff);
    // ###   Hist_Full_T_TSIZE   ##################################
    h->SetAttribute("ROI_TSIZE", fullHistTSize); // has been validated before
}


void SurfaceConceptTDC::AddImagePreviewPollingAttribute() {
    // adds two attributes
    // "Image_Trigger_PollPeriod"
    image_preview_polling_attr = new CustomAttr("Image_Preview_Polling", Tango::DEV_LONG, Tango::READ_WRITE, Tango::AssocWritNotSpec);
    Tango::UserDefaultAttrProp	attrprop2;
    attrprop2.max_value = "1000";
    attrprop2.min_value = "0";
    attrprop2.format    = "%4d";
    attrprop2.unit      = "ms";
    attrprop2.description = std::string("Period of internal timer which issues updates of live images and accumulation preview.");
    image_preview_polling_attr->set_default_properties(attrprop2);
    image_preview_polling_attr->set_memorized_init(true);
    image_preview_polling_attr->set_memorized();
    image_preview_polling_attr->SetWriteCallback(this, &SurfaceConceptTDC::ImagePreviewPollingWriteCallback);
    image_preview_polling_attr->SetReadCallback(this, &SurfaceConceptTDC::ImagePreviewPollingReadCallback);
    this->add_attribute(image_preview_polling_attr);
}

void SurfaceConceptTDC::HistogramAttributeReadCallback(Tango::DeviceImpl* dev, Tango::Attribute& att) {
    att.set_value(&(m_dyn_attr_long_vals[att.get_name()]));
}

void SurfaceConceptTDC::HistogramAttributeWriteCallback(Tango::DeviceImpl* dev, Tango::WAttribute& att) {
    //cout << "CustomWriteCallback called with attribute name: " << att.get_name() << endl;
    string name = att.get_name();
    Tango::DevLong64  w_val;
    att.get_write_value(w_val);
    HistogramAttributeWriteCallbackAction(name, w_val);
    HistogramAttributeWriteCallback_Hist_User_T_Links(name, w_val);
}

void SurfaceConceptTDC::HistogramAttributeWriteCallback_Hist_User_T_Links(std::string attname, long w_val) {
    try {
        if (attname.compare("Hist_User_T_ROI_TOFF")==0)
            hist_user_t_toff_raw_val = m_hist_map["Hist_User_T"]->bint*w_val;
        else if (attname.compare("Hist_User_T_MODULO")==0)
            hist_user_t_modbinned_val = w_val/m_hist_map["Hist_User_T"]->bint;
        else if (attname.compare("Hist_User_T_BIN_T")==0) {
            if (sync_hist_raw_toff_fixed_val)
                HistogramAttributeWriteCallbackAction("Hist_User_T_ROI_TOFF", hist_user_t_toff_raw_val/w_val);
            else
                hist_user_t_toff_raw_val = m_hist_map["Hist_User_T"]->roit1*w_val;
        }
    } catch (std::exception &e) {}; // ignore errors
}

void SurfaceConceptTDC::HistogramAttributeWriteCallbackAction(const std::string attrname, Tango::DevLong64 w_val) {
    // if the written-to attribute belongs to a pipeless histogram, writing is allowed (if-block is skipped)
    // (pipeless means, the databuffer is not written to by the surface concept library, but by our own integration
    // methods)
    string name = attrname;
    string hist_name = Helper::extract_hist_name(name);
    if (std::find(pipeless_histograms.begin(), pipeless_histograms.end(),hist_name)==pipeless_histograms.end()) {
        if (accumulation_running) // do not allow changes to pipe histograms during accumulation
            return;
        if (acquisition_running) { // if we are in live mode, we can stop, change pipe and start again
            _acquisition_stop();  // this may take as long as the exposure time
            std::tuple < std::string, Tango::DevLong64 > i(name, w_val);
            //std::cout << "pushing attributes for deferred writing" << std::endl;
            deferred_writes.push(i); // do not set histo attributes now, but in MeasurementCompleteCallback
            return;
        }
    }
    string subatt_name = Helper::extract_hist_remainder(name);
    // the following operation may take longer than 1 second and should be executed in a thread
    // such that the attribute write callback can return before timeout
    thread_pool.push([this, hist_name, subatt_name, w_val](int id){this->SetHistogramAttrLinked(hist_name, subatt_name, w_val);});
    //SetHistogramAttrLinked(hist_name, subatt_name, w_val);
}


void SurfaceConceptTDC::_SetHistogramAttribute(
    const std::string histname, const std::string histattrname, long value) 
{
    //std::cout << "Setting Histogram Attribute " << histname << "." << histattrname << " to " << value << std::endl;
    m_hist_map[histname]->SetAttribute(histattrname, value);
    m_dyn_attr_long_vals[histname+"_"+histattrname] = value;
}

void SurfaceConceptTDC::SetHistogramAttribute(
    const std::string histname, const std::string histattrname, long value) 
{
    if (histattrname.compare("ROI_T1")==0) {
        long t2 = m_hist_map[histname]->GetAttribute("ROI_T2");
        m_dyn_attr_long_vals[histname+"_ROI_TOFF"] = value;
        m_dyn_attr_long_vals[histname+"_ROI_TSIZE"] = t2-value;
    }
    if (histattrname.compare("ROI_T2")==0) {
        long t1 = m_hist_map[histname]->GetAttribute("ROI_T1");
        m_dyn_attr_long_vals[histname+"_ROI_TSIZE"] = value-t1;
    }
    if (histattrname.compare("ROI_TOFF")==0) {
        long tsize = m_hist_map[histname]->GetAttribute("ROI_TSIZE");
        m_dyn_attr_long_vals[histname+"_ROI_T1"] = value;
        m_dyn_attr_long_vals[histname+"_ROI_T2"] = value+tsize;
    }
    if (histattrname.compare("ROI_TSIZE")==0) {
        long t1 = m_hist_map[histname]->GetAttribute("ROI_T1");
        m_dyn_attr_long_vals[histname+"_ROI_T2"] = t1+value;
    }    
    _SetHistogramAttribute(histname, histattrname, value);
}

/**
 * void SetHistogramAttrFiltered(const std::string hist, const std::string attr, long value):
 * Some histogram attributes are linked, and must be forwarded to their targets.
 * For accumulated data sets, the axes of the accumulated XYT (3D) data set
 * should determine the resolved axes of the accumulated XY, XT, YT data sets
 * The ROI of the integration axes of the XY,XT,YT data sets should
 * be still freely choosable! The binning for all axes (including the integrated
 * axis), should be the same as the corresponding axes in the 3D/XYT data set.
 */
void SurfaceConceptTDC::SetHistogramAttrLinked(const std::string histname, const std::string histattrname, long value) {
    if (histname.find("Accu")!=std::string::npos) {
//        if (accu_buffers_mutex.try_lock())
//            std::cout << "SurfaceConceptTDC::SetHistogramAttrLinked: no interference with Integration task" << std::endl;
//        else {
//            std::cout << "SurfaceConceptTDC::SetHistogramAttrLinked: accu buffers are being integrated, have to wait" << std::endl;
//            long t1 = Helper::get_millisec();
//            accu_buffers_mutex.lock();
//            long t2 = Helper::get_millisec();
//            std::cout << "SurfaceConceptTDC::SetHistogramAttrLinked: waited " << t2-t1 << " ms." << std::endl;
//        }
        accu_buffers_mutex.lock();   
    }

    if (histname.compare("Hist_Accu_XYT")==0) {
        
        SetHistogramAttribute("Hist_Accu_XYT", histattrname, value); // set it for XYT
        // set the linked attributes of other accumulation histograms
        if (histattrname.compare("ROI_X1")==0 || histattrname.compare("ROI_X2")==0) {
            SetHistogramAttribute("Hist_Accu_XY", histattrname, value);
            SetHistogramAttribute("Hist_Accu_XT", histattrname, value);
        }
        else if (histattrname.compare("ROI_Y1")==0 || histattrname.compare("ROI_Y2")==0) {
            SetHistogramAttribute("Hist_Accu_XY", histattrname, value);
            SetHistogramAttribute("Hist_Accu_YT", histattrname, value);
        }
        else if (histattrname.compare("ROI_T1")==0 || histattrname.compare("ROI_T2")==0) {
            SetHistogramAttribute("Hist_Accu_XT", histattrname, value);
            SetHistogramAttribute("Hist_Accu_YT", histattrname, value);
            SetHistogramAttribute("Hist_Accu_T", histattrname, value);
        }
        else if (histattrname.compare("MODULO")==0 || 
                histattrname.compare("BIN_X")==0 || 
                histattrname.compare("BIN_Y")==0 || 
                histattrname.compare("BIN_T")==0 ) {
            SetHistogramAttribute("Hist_Accu_XY", histattrname, value);
            SetHistogramAttribute("Hist_Accu_XT", histattrname, value);
            SetHistogramAttribute("Hist_Accu_YT", histattrname, value);
            SetHistogramAttribute("Hist_Accu_T", histattrname, value);
        }
        Update_Info_Accu_XYT_Size();
    }
    else // no links from any other histogram
        SetHistogramAttribute(histname, histattrname, value);
    if (histname.find("Accu")!=std::string::npos)
        accu_buffers_mutex.unlock();
}

void SurfaceConceptTDC::Update_Info_Accu_XYT_Size() {
    GeneralHistogram* h = m_hist_map.at("Hist_Accu_XYT");
    long sz = h->GetZSize()*h->GetHeight()*h->GetWidth()*4; // 32-bit
    info_accu_xyt_bytesize_val = sz;
    std::string fsz = Helper::Format_Bytesize(sz, 2);
    strncpy(info_accu_xyt_formattedsize_val, fsz.c_str(), STRING_BUF_SIZE-1);
}
 
void SurfaceConceptTDC::LiveImageTriggerAction() {
    // get time
    
    long v = Helper::get_millisec();
    long delta = v - image_preview_last_timestamp; // correct even if millisec counter has had an overflow
    
    if (delta>=0 && delta<m_exposure_live_ms) // if not enough time passed, don't do anything
        return; // (however, delta < 0 means the time *difference* has overflown)

    image_preview_last_timestamp = v;

    std::mutex m;
    m.lock(); // protect the sequence of test and assignment of preview_refresh_task_busy against interference by another thread
    if (live_preview_refresh_task_busy) {
        std::cout << "SurfaceConceptTDC::LiveImageTriggerAction skipping concurrent refresh task" << std::endl;
        m.unlock();
        return; 
    }
    live_preview_refresh_task_busy = true; // the task_busy variable is set to false by the thread after it has finished its tasks
    m.unlock();
    if (live_preview_refresh_thread!=NULL) {
        delete live_preview_refresh_thread;
        live_preview_refresh_thread = NULL;
    }
    
    //live_preview_refresh_thread = new std::thread(&SurfaceConceptTDC::StaticLiveImageTriggerThreadedAction, this);
    //live_preview_refresh_thread->detach();
    thread_pool.push([this](int id){this->LiveImageTriggerThreadedAction();});
}

void SurfaceConceptTDC::LiveImageTriggerThreadedAction() {
    Helper::Finally finalaction([&]{live_preview_refresh_task_busy = false;}); // should work even when exceptions are thrown
    if (!taxes_initialized && livePreviewModeTangoActive) {
        m_hist_map.at("Hist_Full_T")->ProvideTAxis(hist_full_taxis_attr, devprop_pixel_size_t_val, hist_taxis_unit_internal);
        m_hist_map.at("Hist_Live_T")->ProvideTAxis(hist_live_taxis_attr, devprop_pixel_size_t_val, hist_taxis_unit_internal);
        m_hist_map.at("Hist_Accu_T")->ProvideTAxis(hist_accu_taxis_attr, devprop_pixel_size_t_val, hist_taxis_unit_internal);
        m_hist_map.at("Hist_User_T")->ProvideTAxis(hist_user_taxis_attr, devprop_pixel_size_t_val, hist_taxis_unit_internal);
        taxes_initialized = true;
    }
    // Statistics of image:
    LiveImageTriggerThreadedAction_ImageStat();
    // Write Databuffers to Tango attributes and files
    for (auto &hist : m_hist_map) 
    {
        if (hist.first.find("_Live_")!=std::string::npos || 
                hist.first.compare("Hist_Full_XY")==0 ||
                hist.first.compare("Hist_Full_T")==0) {
            //std::cout << "calling PerformActiveOutputs for " << hist.first << std::endl;
            hist.second->PerformActiveOutputs();
            if (livePreviewModeTangoActive) {
                if (accumulation_running)
                    hist.second->AddToTangoAccuBuffer(); // does nothing if it hasn't been activated before
                // send live buffer update via push_change_event
                Tango::DevLong* tbuf = hist.second->GetTangoBuffer();
                if (tbuf!=NULL) {
                    int h = hist.second->GetTangoBufferDataHeight();
                    if (h<2) h = 0; // (BAD) handles spectra correctly, but images with height 1 incorrectly!
                    try {
                        push_change_event(hist.first, tbuf, hist.second->GetTangoBufferDataWidth(), h);
                    }
                    catch (Tango::DevFailed e) {
                        std::cout << "DevFailed exception occured in LiveImageTriggerThreadedAction(...)" << std::endl;
                        Helper::cout_tango_devfailed_exception(e);
                        image_preview_update_thread.Stop();
                        image_preview_update_thread.Join();
                    }
                }
            }
            hist.second->ClearBuffer();
        }
    }
    LiveImageTriggerThreadedAction_Hist_User_T(); // Hist_User_T is just treated separately, did not fit well into the previous for loop
    // update time stamp in live subfolder
    if (livePreviewModeFileActive) {
        std::string timestamp_path = Helper::join_pathnames(this->livePreviewModeFilePath, "live", "timestamp");
        FILE *f = fopen(timestamp_path.c_str(), "w");
        long v = Helper::get_millisec();
        fwrite(&v, sizeof(v), 1, f);
        fclose(f);
    }
    //live_preview_refresh_task_busy = false;
}

void SurfaceConceptTDC::LiveImageTriggerThreadedAction_Hist_User_T() {
    GeneralHistogram* h = m_hist_map.at("Hist_User_T");
    h->PerformActiveOutputs();
    if (livePreviewModeTangoActive) {
        if (accumulation_running)
            h->AddToTangoAccuBuffer(); // does nothing if it hasn't been activated before
        // send live buffer update via push_change_event
        Tango::DevLong* tbuf = h->GetTangoBuffer();
        if (tbuf!=NULL) {
            try {
                push_change_event("Hist_Live_User_T", tbuf, h->GetTangoBufferDataWidth(), 0);
            }
            catch (Tango::DevFailed e) {
                std::cout << "DevFailed exception occured in LiveImageTriggerThreadedAction_Hist_User_T(...)" << std::endl;
                Helper::cout_tango_devfailed_exception(e);
                image_preview_update_thread.Stop();
                image_preview_update_thread.Join();
            }
        }
    }
    h->ClearBuffer();
}

void SurfaceConceptTDC::LiveImageTriggerThreadedAction_ImageStat() {
    for (std::string hname : {"Hist_Live_XY", "Hist_Live_XT", "Hist_Live_YT"}) {
        GeneralHistogram* h = m_hist_map[hname];
        h->UpdateStatisticsOfDatabuf();
        imagestat_vals[hname+"_Max"] = h->GetStatMax();
        imagestat_vals[hname+"_Q998"] = h->GetStatQuantile(0.998);
    }
}


void SurfaceConceptTDC::StaticLiveImageTriggerThreadedAction(void* Object) {
    ((SurfaceConceptTDC*) Object)->LiveImageTriggerThreadedAction();
}



void SurfaceConceptTDC::CountsPerSecUpdate() {
    // Timing logic: perform update only after 1 second since last update has passed
    long v = Helper::get_millisec();
    long delta = v - counts_per_sec_last_timestamp; // correct even if millisec counter has had an overflow
    if (delta>=0 && delta<1000) // if not enough time passed, don't do anything
        return; // (however, delta < 0 means the time *difference* has overflown)
    counts_per_sec_last_timestamp = v; 
    // now perform update
    GeneralHistogram *h = m_hist_map.at("Hist_Full_Counts");
    void* buf = h->GetDatabufPointer();
    if (buf!=NULL) {
        double c = (double) *((uint32_t*)buf) * 1000.0 / ((double) delta);
        counts_per_sec_val = (Tango::DevLong) c;
        *((uint32_t*)buf) = 0;
    }
}


void SurfaceConceptTDC::ImagePreviewUpdateAction() {
    LiveImageTriggerAction();
    CountsPerSecUpdate();
    AccuPreviewRefreshAction();
    AccumulatedTimeIncrementAction();
}

void SurfaceConceptTDC::StaticImagePreviewUpdateAction(void* Object) {
    ((SurfaceConceptTDC*) Object)->ImagePreviewUpdateAction();
}

void SurfaceConceptTDC::ImagePreviewPollingReadCallback(Tango::DeviceImpl* dev, Tango::Attribute& att) {
    att.set_value(&image_preview_polling_val);
}

void SurfaceConceptTDC::ImagePreviewPollingWriteCallback(Tango::DeviceImpl* dev, Tango::WAttribute& att) {
    att.get_write_value(image_preview_polling_val);
    image_preview_update_thread.SetPeriod(image_preview_polling_val);
}

void SurfaceConceptTDC::AccumulatedTimeIncrementAction() {
    if (accumulation_running) {
        long v = Helper::get_millisec();
        long delta = v - accumulation_last_time_stamp;
        if (delta>0) {
            accumulated_time_val += delta;
            accumulated_time_single += delta;
        }
        if (accumulated_time_single>=m_exposure_accu_ms) {
            accumulation_stop();
            if (request_save_after_accumulation.load()) {
                request_save_after_accumulation.store( false );
                if (save_after_accumulation_mode.load()==SAVE_AFTER_ACCUM_MODE_XYT)
                    save_xyt_to_tiff();
                else if (save_after_accumulation_mode.load()==SAVE_AFTER_ACCUM_MODE_XY)
                    save_xy_to_tiff();
                else if (save_after_accumulation_mode.load()==SAVE_AFTER_ACCUM_MODE_XY_TEXT)
                    save_xy_to_text();
            }
        }
        accumulation_last_time_stamp = v;
    }
}


void SurfaceConceptTDC::AddAccuPreviewRefreshAttribute() {
    accu_preview_refresh_attr = new CustomAttr("Accu_Preview_Refresh", Tango::DEV_LONG, Tango::READ_WRITE, Tango::AssocWritNotSpec);
    Tango::UserDefaultAttrProp	attrprop;
    attrprop.max_value = "2147483647";
    attrprop.min_value = "0";
    attrprop.format    = "%10d";
    attrprop.unit      = "ms";
    attrprop.set_description("Determines how often the slices through the accumulated xyz data stack are being updated.");
    accu_preview_refresh_attr->set_default_properties(attrprop);
    accu_preview_refresh_attr->set_memorized_init(true);
    accu_preview_refresh_attr->set_memorized();
    accu_preview_refresh_attr->SetWriteCallback(this, &SurfaceConceptTDC::AccuPreviewRefreshWriteCallback);
    accu_preview_refresh_attr->SetReadCallback(this, &SurfaceConceptTDC::AccuPreviewRefreshReadCallback);
    this->add_attribute(accu_preview_refresh_attr);
    //poll_attribute("Accu_Preview_Refresh", 1000);
}

void SurfaceConceptTDC::AccuPreviewRefreshReadCallback(Tango::DeviceImpl*, Tango::Attribute& att) {
    att.set_value(&accu_preview_refresh_val);
}
    
void SurfaceConceptTDC::AccuPreviewRefreshWriteCallback(Tango::DeviceImpl*dev, Tango::WAttribute& att) {
    att.get_write_value(accu_preview_refresh_val);
}

void SurfaceConceptTDC::AccuPreviewRefreshAction() {
    if (accu_preview_refresh_val==0) return;

    long v = Helper::get_millisec();

    long delta = v - accu_preview_refresh_timestamp; // correct even if millisec counter has had an overflow
    if (delta>=0 && delta<accu_preview_refresh_val) // if not enough time passed, don't do anything
        return;
    // if we decide that enough time has passed, update our timestamp
    accu_preview_refresh_timestamp = v;
    //std::cout << "SurfaceConceptTDC::PreviewRefreshAccuAction got called: " << v << std::endl;    
    if (m_hist_map.at("Hist_Accu_XYT")->GetDatabufPointer()==NULL)
        return;
    // make sure the time-consuming integration of data is not concurrently executed several times
    std::mutex m;
    m.lock(); // protect the sequence of test and assignment of preview_refresh_task_busy against interference by another thread
    if (accu_preview_refresh_task_busy) {
        std::cout << "SurfaceConceptTDC::AccuPreviewRefreshAction skipping concurrent integration task" << std::endl;
        m.unlock();
        return; 
    }
    accu_preview_refresh_task_busy = true; // the task_busy variable is set to false by the thread after it has finished its tasks
    m.unlock();

    thread_pool.push([this](int id){this->AccuPreviewRefreshThreadedAction();});
    
}

void SurfaceConceptTDC::AccuPreviewRefreshThreadedAction() {
    accu_buffers_mutex.lock();
    Helper::Finally finalaction([&]{accu_preview_refresh_task_busy = false; accu_buffers_mutex.unlock();});  // should work even when exceptions are thrown
    // this function must be called only from AccuPreviewRefreshAction
    // -------------------------------------------------------------------------
    long start = Helper::get_millisec();
    // Integrate the XYT data set to XY, XT, YT images
    int xy_retval =
        IntegrateXYT_T(*m_hist_map.at("Hist_Accu_XYT"), *m_hist_map.at("Hist_Accu_XY"),
            m_dyn_attr_long_vals["Hist_Accu_XY_ROI_T1"]-m_dyn_attr_long_vals["Hist_Accu_XYT_ROI_T1"], 
            m_dyn_attr_long_vals["Hist_Accu_XY_ROI_T2"]-m_dyn_attr_long_vals["Hist_Accu_XYT_ROI_T1"]);
    int xt_retval = 
    IntegrateXYT_Y(*m_hist_map.at("Hist_Accu_XYT"), *m_hist_map.at("Hist_Accu_XT"),
            m_dyn_attr_long_vals["Hist_Accu_XT_ROI_Y1"]-m_dyn_attr_long_vals["Hist_Accu_XYT_ROI_Y1"], 
            m_dyn_attr_long_vals["Hist_Accu_XT_ROI_Y2"]-m_dyn_attr_long_vals["Hist_Accu_XYT_ROI_Y1"]);
    int yt_retval = 
    IntegrateXYT_X(*m_hist_map.at("Hist_Accu_XYT"), *m_hist_map.at("Hist_Accu_YT"),
            m_dyn_attr_long_vals["Hist_Accu_YT_ROI_X1"]-m_dyn_attr_long_vals["Hist_Accu_XYT_ROI_X1"], 
            m_dyn_attr_long_vals["Hist_Accu_YT_ROI_X2"]-m_dyn_attr_long_vals["Hist_Accu_XYT_ROI_X1"]);
    int t_retval = 
    IntegrateXYT_XY(*m_hist_map.at("Hist_Accu_XYT"), *m_hist_map.at("Hist_Accu_T"),
            m_dyn_attr_long_vals["Hist_Accu_T_ROI_X1"]-m_dyn_attr_long_vals["Hist_Accu_XYT_ROI_X1"], 
            m_dyn_attr_long_vals["Hist_Accu_T_ROI_X2"]-m_dyn_attr_long_vals["Hist_Accu_XYT_ROI_X1"],
            m_dyn_attr_long_vals["Hist_Accu_T_ROI_Y1"]-m_dyn_attr_long_vals["Hist_Accu_XYT_ROI_Y1"], 
            m_dyn_attr_long_vals["Hist_Accu_T_ROI_Y2"]-m_dyn_attr_long_vals["Hist_Accu_XYT_ROI_Y1"]);
    if (livePreviewModeFileActive) {
        if (xy_retval==0) m_hist_map.at("Hist_Accu_XY")->WriteFile();
        if (xt_retval==0) m_hist_map.at("Hist_Accu_XT")->WriteFile();
        if (yt_retval==0) m_hist_map.at("Hist_Accu_YT")->WriteFile();
        if (t_retval==0) m_hist_map.at("Hist_Accu_T")->WriteFile();
        // update time stamp in accu subfolder
        std::string timestamp_path = Helper::join_pathnames(this->livePreviewModeFilePath, "accu", "timestamp");
        FILE *f = fopen(timestamp_path.c_str(), "w");
        long v = Helper::get_millisec();
        fwrite(&v, sizeof(v), 1, f);
        fclose(f);
    }
    if (livePreviewModeTangoActive) {
        GeneralHistogram* hist = m_hist_map.at("Hist_Accu_T");
        hist->WriteTangoBuffer();
        Tango::DevLong* tbuf = hist->GetTangoBuffer();
        if (tbuf!=NULL) {
            int h = hist->GetTangoBufferDataHeight();
            if (h<2) h = 0; // (BAD) handles spectra correctly, but images with height 1 incorrectly!
            try {
                push_change_event("Hist_Accu_T", tbuf, hist->GetTangoBufferDataWidth(), h);
            }
            catch (Tango::DevFailed e) {
                std::cout << "DevFailed exception occured in AccuPreviewRefreshThreadedAction(...)" << std::endl;
                Helper::cout_tango_devfailed_exception(e);
                image_preview_update_thread.Stop();
                image_preview_update_thread.Join();
            }
        }
        // Hist_Full_Accu_T
        hist = m_hist_map.at("Hist_Full_T");
        tbuf = hist->GetTangoAccuBuffer();
        
        if (tbuf!=NULL) {
            int h = hist->GetTangoBufferDataHeight();
            if (h<2) h = 0; // (BAD) handles spectra correctly, but images with height 1 incorrectly!
            try {
                push_change_event("Hist_Full_Accu_T", tbuf, hist->GetTangoBufferDataWidth(), h);
            }
            catch (Tango::DevFailed e) {
                std::cout << "DevFailed exception occured in AccuPreviewRefreshThreadedAction(...)" << std::endl;
                Helper::cout_tango_devfailed_exception(e);
                image_preview_update_thread.Stop(); // <-- NOT THE BEST REACTION
                image_preview_update_thread.Join(); // server stops working normally
            }
        }
        // Hist_User_Accu_T
        hist = m_hist_map.at("Hist_User_T");
        tbuf = hist->GetTangoAccuBuffer();
        
        if (tbuf!=NULL) {
            int h = hist->GetTangoBufferDataHeight();
            if (h<2) h = 0; // (BAD) handles spectra correctly, but images with height 1 incorrectly!
            try {
                push_change_event("Hist_Accu_User_T", tbuf, hist->GetTangoBufferDataWidth(), h);
            }
            catch (Tango::DevFailed e) {
                std::cout << "DevFailed exception occured in AccuPreviewRefreshThreadedAction(...)" << std::endl;
                Helper::cout_tango_devfailed_exception(e);
                image_preview_update_thread.Stop(); // <-- NOT THE BEST REACTION
                image_preview_update_thread.Join(); // server stops working normally
            }
        }
        // Accumulated Images XY, XT, YT
        for (std::string key : {"Hist_Accu_XY", "Hist_Accu_XT", "Hist_Accu_YT"}) {
            hist = m_hist_map.at(key);
            hist->WriteTangoBuffer();
            tbuf = hist->GetTangoBuffer();
            if (tbuf!=NULL) {
                try {
                    push_change_event(key, tbuf, hist->GetTangoBufferDataWidth(), hist->GetTangoBufferDataHeight());
                }
                catch (Tango::DevFailed e) {
                    std::cout << "DevFailed exception occured in AccuPreviewRefreshThreadedAction(...)" << std::endl;
                    Helper::cout_tango_devfailed_exception(e);
                    image_preview_update_thread.Stop(); // <-- NOT THE BEST REACTION
                    image_preview_update_thread.Join(); // server stops working normally
                }
            }
        }
    }
    AccuPreviewRefreshThreadedAction_ImageStat();
    long end = Helper::get_millisec();
    server_accu_int_duration_val = end-start;
    //std::cout << "Integration Task took " << end-start << " milliseconds." << std::endl;
}

void SurfaceConceptTDC::AccuPreviewRefreshThreadedAction_ImageStat() {
    for (std::string hname : {"Hist_Accu_XY", "Hist_Accu_XT", "Hist_Accu_YT"}) {
        GeneralHistogram* h = m_hist_map[hname];
        h->UpdateStatisticsOfDatabuf();
        imagestat_vals[hname+"_Max"] = h->GetStatMax();
        imagestat_vals[hname+"_Q998"] = h->GetStatQuantile(0.998);
    }
}


void SurfaceConceptTDC::StaticAccuPreviewRefreshThreadedAction(void* Object) {
    ((SurfaceConceptTDC*) Object)->AccuPreviewRefreshThreadedAction();
}

void SurfaceConceptTDC::AddAccumulatedTimeAttribute() {
    accumulated_time_attr = new CustomAttr("Accumulated_Time", Tango::DEV_LONG, Tango::READ, Tango::AssocWritNotSpec);
    Tango::UserDefaultAttrProp	attrprop;
    attrprop.max_value = "2147483647";
    attrprop.min_value = "0";
    attrprop.format    = "%10d";
    attrprop.unit      = "ms";
    attrprop.set_description("the currently accumulated exposure of the xyz data set");
    accumulated_time_attr->set_default_properties(attrprop);
    accumulated_time_attr->SetReadCallback(this, &SurfaceConceptTDC::AccumulatedTimeReadCallback);
    this->add_attribute(accumulated_time_attr);    
}

void SurfaceConceptTDC::AccumulatedTimeReadCallback(Tango::DeviceImpl* dev, Tango::Attribute& att) {
    att.set_value(&accumulated_time_val);
}

void SurfaceConceptTDC::AddDiagnosticAttributes() {
    server_databufs_reservedmem_attr = new CustomAttr("Server_Databuffers_ReservedMem", Tango::DEV_LONG64, Tango::READ, Tango::AssocWritNotSpec);
    Tango::UserDefaultAttrProp	attrprop;
    attrprop.max_value = "34359738368";
    attrprop.min_value = "0";
    attrprop.format    = "%11d";
    attrprop.unit      = "MB";
    attrprop.set_description("the memory currently reserved for data buffers");
    server_databufs_reservedmem_attr->set_default_properties(attrprop);
    server_databufs_reservedmem_attr->SetReadCallback(this, &SurfaceConceptTDC::DiagnosticAttributeReadCallback);
    this->add_attribute(server_databufs_reservedmem_attr);
    
    counts_per_sec_attr = new CustomAttr("Counts_Per_Sec", Tango::DEV_LONG, Tango::READ, Tango::AssocWritNotSpec);
    Tango::UserDefaultAttrProp	attrprop2;
    attrprop2.max_value = "2147483647";
    attrprop2.min_value = "0";
    attrprop2.format    = "%10d";
    attrprop2.unit      = "";
    attrprop2.set_description("the events per second in a predefined, hard-coded region of interest");
    counts_per_sec_attr->set_default_properties(attrprop2);
    counts_per_sec_attr->SetReadCallback(this, &SurfaceConceptTDC::DiagnosticAttributeReadCallback);
    this->add_attribute(counts_per_sec_attr);
    
    acquisition_running_attr = new CustomAttr("Acquisition_Running", Tango::DEV_BOOLEAN, Tango::READ, Tango::AssocWritNotSpec);
    acquisition_running_attr->SetReadCallback(this, &SurfaceConceptTDC::DiagnosticAttributeReadCallback);
    Tango::UserDefaultAttrProp ap1;
    ap1.set_description("true if an exposure is currently active");
    acquisition_running_attr->set_default_properties(ap1);
    this->add_attribute(acquisition_running_attr);
    
    
    accumulation_running_attr = new CustomAttr("Accumulation_Running", Tango::DEV_BOOLEAN, Tango::READ, Tango::AssocWritNotSpec);
    accumulation_running_attr->SetReadCallback(this, &SurfaceConceptTDC::DiagnosticAttributeReadCallback);
    Tango::UserDefaultAttrProp	attrprop_accumulation_running;
    attrprop_accumulation_running.set_description("true if an exposure is currently being accumulated");
    accumulation_running_attr->set_default_properties(attrprop_accumulation_running);
    this->add_attribute(accumulation_running_attr);
    
    devprop_pixel_size_t_attr = new CustomAttr("DevProp_PixelSize_T", Tango::DEV_DOUBLE, Tango::READ, Tango::AssocWritNotSpec);
    devprop_pixel_size_t_attr->SetReadCallback(this, &SurfaceConceptTDC::DevPropReadCallback);
    Tango::UserDefaultAttrProp	attrprop3;
    attrprop3.format    = "%8.4f";
    attrprop3.unit      = "ps";
    attrprop3.set_description("the pixel size of the time axis reported by the sctdc library");
    devprop_pixel_size_t_attr->set_default_properties(attrprop3);
    this->add_attribute(devprop_pixel_size_t_attr);
    devprop_pixel_size_x_attr = new CustomAttr("DevProp_PixelSize_X", Tango::DEV_DOUBLE, Tango::READ, Tango::AssocWritNotSpec);
    devprop_pixel_size_x_attr->SetReadCallback(this, &SurfaceConceptTDC::DevPropReadCallback);
    Tango::UserDefaultAttrProp	attrprop4;
    attrprop4.format    = "%8.4f";
    attrprop4.unit      = "µm";
    attrprop4.set_description("the pixel size of the x axis reported by the sctdc library");
    devprop_pixel_size_x_attr->set_default_properties(attrprop4);
    this->add_attribute(devprop_pixel_size_x_attr);
    devprop_pixel_size_y_attr = new CustomAttr("DevProp_PixelSize_Y", Tango::DEV_DOUBLE, Tango::READ, Tango::AssocWritNotSpec);
    devprop_pixel_size_y_attr->SetReadCallback(this, &SurfaceConceptTDC::DevPropReadCallback);
    attrprop4.set_description("the pixel size of the y axis reported by the sctdc library");
    devprop_pixel_size_y_attr->set_default_properties(attrprop4);
    this->add_attribute(devprop_pixel_size_y_attr);
    Tango::UserDefaultAttrProp	attrprop5;
    attrprop5.format    = "%12d";
    attrprop5.set_unit("pixels");
    attrprop5.set_description("device property reported by the sctdc library");
    devprop_detector_size_t_attr = new CustomAttr("DevProp_DetectorSize_T", Tango::DEV_LONG64, Tango::READ, Tango::AssocWritNotSpec);
    devprop_detector_size_t_attr->SetReadCallback(this, &SurfaceConceptTDC::DevPropReadCallback);
    devprop_detector_size_t_attr->set_default_properties(attrprop5);
    this->add_attribute(devprop_detector_size_t_attr);
    attrprop5.format    = "%10d";
    devprop_detector_size_x_attr = new CustomAttr("DevProp_DetectorSize_X", Tango::DEV_LONG, Tango::READ, Tango::AssocWritNotSpec);
    devprop_detector_size_x_attr->SetReadCallback(this, &SurfaceConceptTDC::DevPropReadCallback);
    devprop_detector_size_x_attr->set_default_properties(attrprop5);
    this->add_attribute(devprop_detector_size_x_attr);
    devprop_detector_size_y_attr = new CustomAttr("DevProp_DetectorSize_Y", Tango::DEV_LONG, Tango::READ, Tango::AssocWritNotSpec);
    devprop_detector_size_y_attr->SetReadCallback(this, &SurfaceConceptTDC::DevPropReadCallback);
    devprop_detector_size_y_attr->set_default_properties(attrprop5);
    this->add_attribute(devprop_detector_size_y_attr);
    
    server_accu_int_duration_attr = new CustomAttr("Server_Accu_Int_Duration", Tango::DEV_LONG, Tango::READ, Tango::AssocWritNotSpec);
    Tango::UserDefaultAttrProp ap1b;
    ap1b.set_description("reports how the long the last integration of the 3D data cube for the Accu XY,XT,YT slices has taken");
    ap1b.set_unit("ms");
    ap1b.set_format("%6d");
    server_accu_int_duration_attr->set_default_properties(ap1b);
    server_accu_int_duration_attr->SetReadCallback(this, &SurfaceConceptTDC::DiagnosticAttributeReadCallback);
    this->add_attribute(server_accu_int_duration_attr);
    
    server_save_file_busy_attr = new CustomAttr("Server_Save_File_Busy", Tango::DEV_BOOLEAN, Tango::READ, Tango::AssocWritNotSpec);
    Tango::UserDefaultAttrProp ap1c;
    ap1c.set_description("true if the server is currently in the process of saving a file");
    server_save_file_busy_attr->set_default_properties(ap1c);
    server_save_file_busy_attr->SetReadCallback(this, &SurfaceConceptTDC::DiagnosticAttributeReadCallback);
    this->add_attribute(server_save_file_busy_attr);
    
    info_accu_xyt_bytesize_attr = new CustomAttr("Info_Accu_XYT_Bytesize", Tango::DEV_LONG64, Tango::READ, Tango::AssocWritNotSpec);
    Tango::UserDefaultAttrProp ap2;
    ap2.set_description("The size in Bytes required to store the Accu XYT Buffer");
    ap2.set_unit("Byte");
    ap2.set_format("%10d");
    info_accu_xyt_bytesize_attr->set_default_properties(ap2);
    info_accu_xyt_bytesize_attr->SetReadCallback(this, &SurfaceConceptTDC::DiagnosticAttributeReadCallback);
    this->add_attribute(info_accu_xyt_bytesize_attr);
    
    info_accu_xyt_formattedsize_attr = new CustomAttr("Info_Accu_XYT_Formattedsize", Tango::DEV_STRING, Tango::READ, Tango::AssocWritNotSpec);
    Tango::UserDefaultAttrProp ap3;
    ap3.set_description("The size required to store the Accu XYT Buffer in human-readable units");
    info_accu_xyt_formattedsize_attr->set_default_properties(ap3);
    info_accu_xyt_formattedsize_attr->SetReadCallback(this, &SurfaceConceptTDC::DiagnosticAttributeReadCallback);
    this->add_attribute(info_accu_xyt_formattedsize_attr);
}

void SurfaceConceptTDC::DevPropReadCallback(Tango::DeviceImpl *dev, Tango::Attribute &attr) {
    std::string attrname = attr.get_name();
    if (attrname.compare("DevProp_PixelSize_T")==0) {
        attr.set_value(&devprop_pixel_size_t_val);
    }
    else if (attrname.compare("DevProp_PixelSize_X")==0) {
        attr.set_value(&devprop_pixel_size_x_val);
    }
    else if (attrname.compare("DevProp_PixelSize_Y")==0) {
        attr.set_value(&devprop_pixel_size_y_val);
    }
    else if (attrname.compare("DevProp_DetectorSize_T")==0) {
        attr.set_value(&devprop_detector_size_t_val);
    }
    else if (attrname.compare("DevProp_DetectorSize_X")==0) {
        attr.set_value(&devprop_detector_size_x_val);
    }
    else if (attrname.compare("DevProp_DetectorSize_Y")==0) {
        attr.set_value(&devprop_detector_size_y_val);
    }
}

void SurfaceConceptTDC::DiagnosticAttributeReadCallback(Tango::DeviceImpl *dev, Tango::Attribute &attr) {
    std::string attrname = attr.get_name();
    if (attrname.compare("Server_Accu_Int_Duration")==0) {
        attr.set_value(&server_accu_int_duration_val);
    }
    else if (attrname.compare("Accumulation_Running")==0) {
        attr.set_value(&accumulation_running_val);
    }
    else if (attrname.compare("Acquisition_Running")==0) {
        attr.set_value(&acquisition_running_val);
    }
    else if (attrname.compare("Info_Accu_XYT_Bytesize")==0) {
        attr.set_value(&info_accu_xyt_bytesize_val);
    }
    else if (attrname.compare("Info_Accu_XYT_Formattedsize")==0) {
        attr.set_value(&info_accu_xyt_formattedsize_val);
    }
    else if (attrname.compare("Counts_Per_Sec")==0) {
        attr.set_value(&counts_per_sec_val);
    }
    else if (attrname.compare("Server_Databuffers_ReservedMem")==0) {
        long long sum = 0;
        for (auto &hist : m_hist_map)
            sum += hist.second->GetDatabufSize();
        server_databufs_reservedmem_val = sum/1048576; // in MB
        attr.set_value(&server_databufs_reservedmem_val);
    }
    else if (attrname.compare("Server_Save_File_Busy")==0) {
        attr.set_value(&server_save_file_busy_val);
    }
}

void SurfaceConceptTDC::AddSavingAttributes() {
    save_directory_attr = new CustomAttr("Save_Directory", Tango::DEV_STRING, Tango::READ_WRITE, Tango::AssocWritNotSpec);
    save_directory_attr->SetReadCallback(this, &SurfaceConceptTDC::SaveDirectoryReadCallback);
    save_directory_attr->SetWriteCallback(this, &SurfaceConceptTDC::SaveDirectoryWriteCallback);
    save_directory_attr->set_memorized_init(true);
    save_directory_attr->set_memorized();
    save_filename_attr = new CustomAttr("Save_Filename", Tango::DEV_STRING, Tango::READ_WRITE, Tango::AssocWritNotSpec);
    save_filename_attr->SetReadCallback(this, &SurfaceConceptTDC::SaveFilenameReadCallback);
    save_filename_attr->SetWriteCallback(this, &SurfaceConceptTDC::SaveFilenameWriteCallback);
    save_filename_attr->set_memorized_init(true);
    save_filename_attr->set_memorized();
    save_filecounter_attr = new CustomAttr("Save_Filecounter", Tango::DEV_LONG, Tango::READ_WRITE, Tango::AssocWritNotSpec);
    Tango::UserDefaultAttrProp	attrprop;
    attrprop.format    = "%3d";
    save_filecounter_attr->set_default_properties(attrprop);
    save_filecounter_attr->SetReadCallback(this, &SurfaceConceptTDC::SaveFilecounterReadCallback);
    save_filecounter_attr->SetWriteCallback(this, &SurfaceConceptTDC::SaveFilecounterWriteCallback);
    save_filecounter_attr->set_memorized_init(true);
    save_filecounter_attr->set_memorized();
    this->add_attribute(save_directory_attr);
    this->add_attribute(save_filename_attr);
    this->add_attribute(save_filecounter_attr);
}

void SurfaceConceptTDC::SaveDirectoryReadCallback(Tango::DeviceImpl* dev, Tango::Attribute& att) {
    att.set_value(&save_directory_rval);
}

void SurfaceConceptTDC::SaveDirectoryWriteCallback(Tango::DeviceImpl* dev, Tango::WAttribute& att) {
    att.get_write_value(save_directory_wval);
    std::string sdir(save_directory_wval);
    // If directory does not start with a path separator (TODO does not handle Windows OS correctly!)
    // prepend the base directory preset in the device properties
    if (!(sdir.length()>0 && sdir.at(0)==Helper::path_sep))
        sdir = Helper::join_pathnames(this->saveBaseDir, sdir);
    memset(save_directory_rval, 0, STRING_BUF_SIZE);
    strncpy(save_directory_rval, sdir.c_str(), STRING_BUF_SIZE-1);
    
}

void SurfaceConceptTDC::SaveFilenameReadCallback(Tango::DeviceImpl* dev, Tango::Attribute& att) {
    att.set_value(&save_filename_val);
}

void SurfaceConceptTDC::SaveFilenameWriteCallback(Tango::DeviceImpl* dev, Tango::WAttribute& att) {
    att.get_write_value(save_filename_val);
}

void SurfaceConceptTDC::SaveFilecounterReadCallback(Tango::DeviceImpl* dev, Tango::Attribute& att) {
    att.set_value(&save_filecounter_val);
}

void SurfaceConceptTDC::SaveFilecounterWriteCallback(Tango::DeviceImpl* dev, Tango::WAttribute& att) {
    att.get_write_value(save_filecounter_val);
}

void SurfaceConceptTDC::MeasurementCompleteCallback(int i) {
    if (i!=1 && i!=2) { // don't print anything for code 1,2 (measurement time has passed or user has stopped)
		//std::cout << "MeasurementCompleteCallback called with code " << i << std::endl;
        if (i==4) return; // "early notification" : hardware has finished measurement but there is still data to transfer
        if (i==3) {
            std::cout << "'Measurement is complete because the buffer is full'" << i << std::endl;
        }
    }
// DEBUG
//    std::cout << "acquisition_running: " << acquisition_running << std::endl;
//    std::cout << "accumulation_running: " << accumulation_running << std::endl;
//    std::cout << "deferred_accumulation_start_request: " << deferred_accumulation_start_request << std::endl;
//    std::cout << "deferred_accumulation_continue_request: " << deferred_accumulation_continue_request << std::endl;
//    std::cout << "user_acquisition_active: " << user_acquisition_active << std::endl;
//    std::cout << "restore_acquisition: " << restore_acquisition << std::endl;
//    std::cout << "request_save_after_accumulation: " << request_save_after_accumulation << std::endl;
//    std::cout << "deferred_tdc_deinitialize_request: " << deferred_tdc_deinitialize_request << std::endl;
//    std::cout << "deferred_write_commonshift_request: " << deferred_write_commonshift_request << std::endl;
    
    // 
    acquisition_running = false;
    acquisition_running_val = false;

    if (deferred_xyt_pipe_close_request) {
        deferred_xyt_pipe_close_request = false;
        m_hist_map.at("Hist_Accu_XYT")->SetPipeActive(false);
    }
    
    if (deferred_tdc_deinitialize_request) {
        deferred_tdc_deinitialize_request = false;
        stop();
        return;
    }

    
    _read_tdc_statistics();
    
    if (deferred_writes.size()>0) {
        //std::cout << "doing deferred writes" << std::endl;
        Helper::sleep(10);
        while (deferred_writes.size()>0) {
            std::string name = std::get<0>(deferred_writes.front());
            Tango::DevLong64 w_val = std::get<1>(deferred_writes.front());
            string hist_name = Helper::extract_hist_name(name);
            string subatt_name = Helper::extract_hist_remainder(name);
            //std::cout << "setting attribute " << hist_name << " " << subatt_name << " " << w_val << std::endl;
            SetHistogramAttrLinked(hist_name, subatt_name, w_val);
            deferred_writes.pop();
        }
    }
    if (deferred_accumulation_start_request) {
        deferred_accumulation_start_request = false;
        accumulation_start();
        return;
    }
    if (deferred_accumulation_continue_request) {
        deferred_accumulation_continue_request = false;
        accumulation_continue();
        return;
    }
    if (user_acquisition_active || user_accumulation_active) {
        _acquisition_start();
    }
}

void SurfaceConceptTDC::StaticMeasurementCompleteCallback(void* p, int i) {
    ((SurfaceConceptTDC*) p)->MeasurementCompleteCallback(i);
}


void SurfaceConceptTDC::AddCommandTriggerAttributes() {
    cmd_trig_accumulation_start_attr = new CustomAttr("CmdTrig_Accumulation_Start", Tango::DEV_LONG, Tango::READ_WRITE, Tango::AssocWritNotSpec);
    cmd_trig_accumulation_stop_attr = new CustomAttr("CmdTrig_Accumulation_Stop", Tango::DEV_LONG, Tango::READ_WRITE, Tango::AssocWritNotSpec);
    cmd_trig_accumulation_continue_attr = new CustomAttr("CmdTrig_Accumulation_Continue", Tango::DEV_LONG, Tango::READ_WRITE, Tango::AssocWritNotSpec);
    cmd_trig_accumulation_start_save_attr = new CustomAttr("CmdTrig_Accumulation_Start_Save", Tango::DEV_LONG, Tango::READ_WRITE, Tango::AssocWritNotSpec);
    cmd_trig_acquisition_start_attr = new CustomAttr("CmdTrig_Acquisition_Start", Tango::DEV_LONG, Tango::READ_WRITE, Tango::AssocWritNotSpec);
    cmd_trig_acquisition_stop_attr =  new CustomAttr("CmdTrig_Acquisition_Stop", Tango::DEV_LONG, Tango::READ_WRITE, Tango::AssocWritNotSpec);
    cmd_trig_device_start_attr =  new CustomAttr("CmdTrig_Device_Start", Tango::DEV_LONG, Tango::READ_WRITE, Tango::AssocWritNotSpec);
    cmd_trig_device_stop_attr =   new CustomAttr("CmdTrig_Device_Stop", Tango::DEV_LONG, Tango::READ_WRITE, Tango::AssocWritNotSpec);
    cmd_trig_save_tiff_xyt_attr = new CustomAttr("CmdTrig_Save_XYT_To_TIFF", Tango::DEV_LONG, Tango::READ_WRITE, Tango::AssocWritNotSpec);
    cmd_trig_save_thist_accu_attr = new CustomAttr("CmdTrig_Save_THist_Accu", Tango::DEV_LONG, Tango::READ_WRITE, Tango::AssocWritNotSpec);
    cmd_trig_shrink_databufs_attr = new CustomAttr("CmdTrig_Shrink_Databufs", Tango::DEV_LONG, Tango::READ_WRITE, Tango::AssocWritNotSpec);
    cmd_trig_save_thist_user_accu_attr = new CustomAttr("CmdTrig_Save_THist_User_Accu", Tango::DEV_LONG, Tango::READ_WRITE, Tango::AssocWritNotSpec);
    cmd_trig_general = new CustomAttr("CmdTrig", Tango::DEV_STRING, Tango::READ_WRITE, Tango::AssocWritNotSpec);
            
    Tango::UserDefaultAttrProp	attrprop;
    attrprop.format    = "%3d";
    attrprop.set_description("attribute that triggers a command when written to");
    Tango::UserDefaultAttrProp	attrprop2;
    attrprop2.format    = "%3d";
    attrprop2.set_description("attribute that triggers an exposure and subsequently saves the xyz data set to a TIFF stack");
    cmd_trig_accumulation_start_attr->set_default_properties(attrprop);
    cmd_trig_accumulation_stop_attr->set_default_properties(attrprop);
    cmd_trig_accumulation_continue_attr->set_default_properties(attrprop);
    cmd_trig_accumulation_start_save_attr->set_default_properties(attrprop2);
    cmd_trig_acquisition_start_attr->set_default_properties(attrprop);
    cmd_trig_acquisition_stop_attr->set_default_properties(attrprop);
    cmd_trig_device_start_attr->set_default_properties(attrprop);
    cmd_trig_device_stop_attr->set_default_properties(attrprop);
    cmd_trig_save_tiff_xyt_attr->set_default_properties(attrprop);
    cmd_trig_shrink_databufs_attr->set_default_properties(attrprop);
    cmd_trig_save_thist_user_accu_attr->set_default_properties(attrprop);
    cmd_trig_general->set_default_properties(attrprop);
    for (CustomAttr* a : {cmd_trig_accumulation_start_attr, cmd_trig_accumulation_stop_attr, cmd_trig_accumulation_continue_attr,
            cmd_trig_accumulation_start_save_attr, cmd_trig_acquisition_start_attr, cmd_trig_acquisition_stop_attr,
            cmd_trig_device_start_attr, cmd_trig_device_stop_attr, cmd_trig_save_tiff_xyt_attr, cmd_trig_save_thist_accu_attr,
            cmd_trig_shrink_databufs_attr, cmd_trig_save_thist_user_accu_attr}) {
        a->SetReadCallback(this, &SurfaceConceptTDC::CommandTriggerReadCallback);
        a->SetWriteCallback(this, &SurfaceConceptTDC::CommandTriggerWriteCallback);
        this->add_attribute(a);
    }
    cmd_trig_general->SetReadCallback(this, &SurfaceConceptTDC::CommandTriggerGeneralReadCallback);
    cmd_trig_general->SetWriteCallback(this, &SurfaceConceptTDC::CommandTriggerGeneralWriteCallback);
    this->add_attribute(cmd_trig_general);
}

void SurfaceConceptTDC::CommandTriggerReadCallback(Tango::DeviceImpl*, Tango::Attribute& att) {
    att.set_value(&cmd_trig_dummy_val);
}

void SurfaceConceptTDC::CommandTriggerWriteCallback(Tango::DeviceImpl*, Tango::WAttribute& att) {
    std::string name = att.get_name();
    name = name.substr(8);
    if (name.compare("Accumulation_Start")==0) {
        accumulation_start();
    }
    else if (name.compare("Accumulation_Stop")==0) {
        accumulation_stop();
    }
    else if (name.compare("Accumulation_Continue")==0) {
        accumulation_continue();
    }
    else if (name.compare("Accumulation_Start_Save")==0) {
        save_after_accumulation_mode.store(SAVE_AFTER_ACCUM_MODE_XYT);
        request_save_after_accumulation.store(true);
        accumulation_start();
    }
    else if (name.compare("Acquisition_Start")==0) {
        acquisition_start();
    }
    else if (name.compare("Acquisition_Stop")==0) {
        acquisition_stop();
    }
    else if (name.compare("Device_Start")==0) {
        start();
    }
    else if (name.compare("Device_Stop")==0) {
        stop();
    }
    else if (name.compare("Save_XYT_To_TIFF")==0) {
        save_xyt_to_tiff();
    }
    else if (name.compare("Save_THist_Accu")==0) {
        save_thist_accu();
    }
    else if (name.compare("Save_THist_User_Accu")==0) {
        save_thist_user_accu();
    }
    else if (name.compare("Shrink_Databufs")==0) {
        shrink_databufs(); // FIXME: causes crashes :-(
    }
}


void SurfaceConceptTDC::CommandTriggerGeneralReadCallback(Tango::DeviceImpl*, Tango::Attribute& att) {
    att.set_value(&cmd_trig_general_val);
}

void SurfaceConceptTDC::CommandTriggerGeneralWriteCallback(Tango::DeviceImpl*, Tango::WAttribute& attr) {
    Tango::DevString	w_val;
    attr.get_write_value(w_val);
    std::string arg(w_val);
    std::istringstream iss(arg);
    std::string token1;
    iss >> token1;
    std::transform(token1.begin(), token1.end(), token1.begin(), [](unsigned char c){return std::toupper(c);});

    if (token1.compare("SAVEXYTOTIFF")==0) {
        save_xy_to_tiff();
    }
    else if (token1.compare("SAVEXYTTOTIFF")==0) {
        save_xyt_to_tiff();
    }
    else if (token1.compare("SAVEXYTOTEXT")==0) {
        save_xy_to_text();
    }
    else if (token1.compare("ACCUMSTARTANDSAVEXY")==0) {
        accum_start_and_save_xy();
    }
    else if (token1.compare("ACCUMSTARTANDSAVEXYTEXT")==0) {
        accum_start_and_save_xy_text();
    }
    
}



void SurfaceConceptTDC::AddMiscTDCAttributes() {
    tdc_common_shift_attr = new CustomAttr("TDC_CommonShift", Tango::DEV_LONG64, Tango::READ_WRITE, Tango::AssocWritNotSpec);
    tdc_common_shift_attr->SetReadCallback(this, &SurfaceConceptTDC::MiscTDCReadCallback);
    tdc_common_shift_attr->SetWriteCallback(this, &SurfaceConceptTDC::MiscTDCWriteCallback);
    Tango::UserDefaultAttrProp attrprop;
    attrprop.format = "%18d";
    attrprop.set_max_value("2147483647"); // common shift is only 32-bit int in the scTDC library
    attrprop.set_min_value("-2147483647");
    attrprop.set_description("This value shifts the time axis before a time modulo is applied");
    tdc_common_shift_attr->set_default_properties(attrprop);
    this->add_attribute(tdc_common_shift_attr);
    // TDC_IniFilePath : path to the ini file, nicer to have as a memorized attribute, changeable at runtime
    tdc_inifile_path_attr = new CustomAttr("TDC_IniFilePath", Tango::DEV_STRING, Tango::READ_WRITE, Tango::AssocWritNotSpec);
    tdc_inifile_path_attr->SetReadCallback(this, &SurfaceConceptTDC::MiscTDCReadCallback);
    tdc_inifile_path_attr->SetWriteCallback(this, &SurfaceConceptTDC::MiscTDCWriteCallback);
    tdc_inifile_path_attr->set_memorized();
    this->add_attribute(tdc_inifile_path_attr);
    // Server_Message : string attribute for the server to place messages in
    server_message_attr = new CustomAttr("Server_Message", Tango::DEV_STRING, Tango::READ_WRITE, Tango::AssocWritNotSpec);
    server_message_attr->SetReadCallback(this, &SurfaceConceptTDC::MiscTDCReadCallback);
    server_message_attr->SetWriteCallback(this, &SurfaceConceptTDC::MiscTDCWriteCallback);
    this->add_attribute(server_message_attr);
}

void SurfaceConceptTDC::MiscTDCReadCallback(Tango::DeviceImpl*, Tango::Attribute& attr) {
    std::string attrname = attr.get_name();
    if (attrname.compare("TDC_CommonShift")==0) {
        attr.set_value(&tdc_common_shift_val);
    }
    else if (attrname.compare("Hist_TAxis_Unit")==0) {
        attr.set_value(&hist_taxis_unit_val);
    }
    else if (attrname.compare("TDC_IniFilePath")==0) {
        attr.set_value(&tdc_inifile_path_val);
    }
    else if (attrname.compare("Server_Message")==0) {
        attr.set_value(&server_message_val);
    }
}

void SurfaceConceptTDC::MiscTDCWriteCallback(Tango::DeviceImpl*, Tango::WAttribute& attr) {
    std::string attrname = attr.get_name();
    if (attrname.compare("TDC_CommonShift")==0 && m_TDC_id>=0) {
        attr.get_write_value(tdc_common_shift_val);
        ::sc_tdc_set_common_shift2(m_TDC_id, (int)tdc_common_shift_val);
        //ostringstream oss;
        //oss << tdc_common_shift_val;
        //inifileOperations.SetModifiedKey("TDC", "CommonShift", oss.str());
        
        //if (m_TDC_id>=0) {
        //    deferred_write_commonshift_request = true;
        //    stop();
        //}
        //else
        //    _write_commonshift();
    }
    else if (attrname.compare("Hist_TAxis_Unit")==0) {
        attr.get_write_value(hist_taxis_unit_val);
        std::string unit(hist_taxis_unit_val);
        if (unit.compare("pixels")==0)
            hist_taxis_unit_internal = GeneralHistogram::taxis_unit_pixels;
        else if (unit.compare("unbinned pixels")==0)
            hist_taxis_unit_internal = GeneralHistogram::taxis_unit_unbinned;
        else if (unit.compare("s")==0)
            hist_taxis_unit_internal = GeneralHistogram::taxis_unit_s;
        else if (unit.compare("ms")==0)
            hist_taxis_unit_internal = GeneralHistogram::taxis_unit_ms;
        else if (unit.compare("us")==0)
            hist_taxis_unit_internal = GeneralHistogram::taxis_unit_us;
        else if (unit.compare("ns")==0)
            hist_taxis_unit_internal = GeneralHistogram::taxis_unit_ns;
        else if (unit.compare("ps")==0)
            hist_taxis_unit_internal = GeneralHistogram::taxis_unit_ps;
        else {
            hist_taxis_unit_internal = GeneralHistogram::taxis_unit_pixels;
            strncpy(hist_taxis_unit_val, "pixels", STRING_BUF_SIZE);
        }
        
        m_hist_map.at("Hist_Full_T")->SetTAxisUnit(hist_taxis_unit_internal);
        m_hist_map.at("Hist_Live_T")->SetTAxisUnit(hist_taxis_unit_internal);
        m_hist_map.at("Hist_Accu_T")->SetTAxisUnit(hist_taxis_unit_internal);
        m_hist_map.at("Hist_User_T")->SetTAxisUnit(hist_taxis_unit_internal);
    }
    else if (attrname.compare("TDC_IniFilePath")==0) {
        attr.get_write_value(tdc_inifile_path_val);
    }
    else if (attrname.compare("Server_Message")==0) {
        attr.get_write_value(server_message_val);
    }
}

void SurfaceConceptTDC::_write_commonshift() {
    std::string newinifile = inifileOperations.GetTargetIniFilePath();
    if (newinifile.length()>0) {
        inifileOperations.WriteModifiedFile();
        iniFilePath = newinifile;
    }
}

void SurfaceConceptTDC::AddTDCStatisticsAttributes() {
    tdc_stat_counters_attr = new CustomSpectrumAttr("TDC_Stat_Counters", Tango::DEV_LONG, Tango::READ, tdc_stat_counters_size);
    tdc_stat_counts_read_attr = new CustomSpectrumAttr("TDC_Stat_Counts_Read", Tango::DEV_LONG, Tango::READ, tdc_stat_counts_read_size);
    tdc_stat_counts_received_attr = new CustomSpectrumAttr("TDC_Stat_Counts_Received", Tango::DEV_LONG, Tango::READ, tdc_stat_counts_received_size);
    tdc_stat_events_found_attr = new CustomSpectrumAttr("TDC_Stat_Events_Found", Tango::DEV_LONG, Tango::READ, tdc_stat_events_found_size);
    tdc_stat_events_in_roi_attr = new CustomSpectrumAttr("TDC_Stat_Events_In_Roi", Tango::DEV_LONG, Tango::READ, tdc_stat_events_in_roi_size);
    tdc_stat_events_received_attr = new CustomSpectrumAttr("TDC_Stat_Events_Received", Tango::DEV_LONG, Tango::READ, tdc_stat_events_received_size);
    tdc_stat_summarized_attr = new CustomSpectrumAttr("TDC_Stat_Summarized", Tango::DEV_LONG, Tango::READ, tdc_stat_summarized_size);
    tdc_stat_max_raw_count_attr = new CustomAttr("TDC_Stat_Max_Raw_Count", Tango::DEV_LONG, Tango::READ, Tango::AssocWritNotSpec);
    tdc_stat_avg_raw_count_attr = new CustomAttr("TDC_Stat_Avg_Raw_Count", Tango::DEV_LONG, Tango::READ, Tango::AssocWritNotSpec);
    
    tdc_stat_counters_attr->SetReadCallback(this, &SurfaceConceptTDC::TDCStatisticsReadCallback);
    tdc_stat_counts_read_attr->SetReadCallback(this, &SurfaceConceptTDC::TDCStatisticsReadCallback);
    tdc_stat_counts_received_attr->SetReadCallback(this, &SurfaceConceptTDC::TDCStatisticsReadCallback);
    tdc_stat_events_found_attr->SetReadCallback(this, &SurfaceConceptTDC::TDCStatisticsReadCallback);
    tdc_stat_events_in_roi_attr->SetReadCallback(this, &SurfaceConceptTDC::TDCStatisticsReadCallback);
    tdc_stat_events_received_attr->SetReadCallback(this, &SurfaceConceptTDC::TDCStatisticsReadCallback);
    tdc_stat_summarized_attr->SetReadCallback(this, &SurfaceConceptTDC::TDCStatisticsReadCallback);
    tdc_stat_max_raw_count_attr->SetReadCallback(this, &SurfaceConceptTDC::TDCStatisticsReadCallback);
    tdc_stat_avg_raw_count_attr->SetReadCallback(this, &SurfaceConceptTDC::TDCStatisticsReadCallback);
    
    tdc_stat_counters_attr->set_change_event(true, false); // this server will push change events for this attribute
    tdc_stat_counts_read_attr->set_change_event(true, false);
    tdc_stat_counts_received_attr->set_change_event(true, false);
    tdc_stat_events_found_attr->set_change_event(true, false);
    tdc_stat_events_in_roi_attr->set_change_event(true, false);
    tdc_stat_events_received_attr->set_change_event(true, false);
    tdc_stat_summarized_attr->set_change_event(true, false);
    tdc_stat_max_raw_count_attr->set_change_event(true, false);
    tdc_stat_avg_raw_count_attr->set_change_event(true, false);

    Tango::UserDefaultAttrProp ap1;
    ap1.set_description("maps information in statistics_t::counters");
    tdc_stat_counters_attr->set_default_properties(ap1);
    Tango::UserDefaultAttrProp ap2;
    ap2.set_description("TDC data per channel recognized by FPGA");
    tdc_stat_counts_read_attr->set_default_properties(ap2);
    Tango::UserDefaultAttrProp ap3;
    ap3.set_description("TDC data per channel transferred from FPGA to CPU");
    tdc_stat_counts_received_attr->set_default_properties(ap3);
    Tango::UserDefaultAttrProp ap4;
    ap4.set_description("DLD events recognized in FPGA");
    tdc_stat_events_found_attr->set_default_properties(ap4);
    Tango::UserDefaultAttrProp ap5;
    ap5.set_description("DLD events recognized in FPGA that are within the hardware ROI");
    tdc_stat_events_in_roi_attr->set_default_properties(ap5);
    Tango::UserDefaultAttrProp ap6;
    ap6.set_description("DLD events transferred from FPGA to CPU");
    tdc_stat_events_received_attr->set_default_properties(ap6);
    Tango::UserDefaultAttrProp ap7;
    ap7.set_description("Summary of Statistics. Indices 0,8: zero, Index 1: events found, Indices 2-5: counts read on x1,x2,y1,y2, Index 6: events received, Index 7: events in ROI");
    tdc_stat_summarized_attr->set_default_properties(ap7);
    Tango::UserDefaultAttrProp ap8;
    ap8.set_description("The maximum value of the raw counts");
    tdc_stat_max_raw_count_attr->set_default_properties(ap8);
    Tango::UserDefaultAttrProp ap9;
    ap9.set_description("The average value of the raw counts");
    tdc_stat_max_raw_count_attr->set_default_properties(ap9);
    
    
    
    this->add_attribute(tdc_stat_counters_attr);
    this->add_attribute(tdc_stat_counts_read_attr);
    this->add_attribute(tdc_stat_counts_received_attr);
    this->add_attribute(tdc_stat_events_found_attr);
    this->add_attribute(tdc_stat_events_in_roi_attr);
    this->add_attribute(tdc_stat_events_received_attr);
    this->add_attribute(tdc_stat_summarized_attr);
    this->add_attribute(tdc_stat_max_raw_count_attr);
    this->add_attribute(tdc_stat_avg_raw_count_attr);
}

void SurfaceConceptTDC::TDCStatisticsReadCallback(Tango::DeviceImpl* dev, Tango::Attribute& attr) {
    std::string attrname = attr.get_name();
    if (attrname.compare("TDC_Stat_Counters")==0) {
        attr.set_value(tdc_stat_counters_val, tdc_stat_counters_size);
    }
    else if (attrname.compare("TDC_Stat_Counts_Read")==0) {
        attr.set_value(tdc_stat_counts_read_val, tdc_stat_counts_read_size);
    }
    else if (attrname.compare("TDC_Stat_Counts_Received")==0) {
        attr.set_value(tdc_stat_counts_received_val, tdc_stat_counts_received_size);
    }
    else if (attrname.compare("TDC_Stat_Events_Found")==0) {
        attr.set_value(tdc_stat_events_found_val, tdc_stat_events_found_size);
    }
    else if (attrname.compare("TDC_Stat_Events_In_Roi")==0) {
        attr.set_value(tdc_stat_events_in_roi_val, tdc_stat_events_in_roi_size);
    }
    else if (attrname.compare("TDC_Stat_Events_Received")==0) {
        attr.set_value(tdc_stat_events_received_val, tdc_stat_events_received_size);
    }
    else if (attrname.compare("TDC_Stat_Summarized")==0) {
        attr.set_value(tdc_stat_summarized_val, tdc_stat_summarized_size);
    }
    else if (attrname.compare("TDC_Stat_Max_Raw_Count")==0) {
        attr.set_value(&tdc_stat_max_raw_count_val);
    }
    else if (attrname.compare("TDC_Stat_Avg_Raw_Count")==0) {
        attr.set_value(&tdc_stat_avg_raw_count_val);
    }
}

void SurfaceConceptTDC::_read_tdc_statistics() {
    if (!tdc_statistics_active) return;
    int result = 0;
    if (m_TDC_id>=0) {
		//result = ::sc_tdc_get_statistics2(m_TDC_id, &_tdc_statistics_sc); // deprecated
		result = 0;
		stat_pipe.GetStatistics(&_tdc_statistics_sc);
	}
    else
        return;
    if (result!=0) {
        std::cout << "Error in SurfaceConceptTDC::_read_tdc_statistics():" << std::endl; 
        std::cout << " sc_tdc_get_statistics2(...) returned error code" << std::endl;
        Helper::cout_sc_err_message(result);
        return;
    }
    // copy all values (int to Tango::DevLong conversion -> do it element-wise)
    for (int i = 0; i < tdc_stat_counters_size; i++)
        tdc_stat_counters_val[i] = _tdc_statistics_sc.counters[i/16][i%16];
    for (int i = 0; i < tdc_stat_counts_read_size; i++)
        tdc_stat_counts_read_val[i] = _tdc_statistics_sc.counts_read[i/16][i%16];
    for (int i = 0; i < tdc_stat_counts_received_size; i++)
        tdc_stat_counts_received_val[i] = _tdc_statistics_sc.counts_received[i/16][i%16];
    for (int i = 0; i < tdc_stat_events_found_size; i++)
        tdc_stat_events_found_val[i] = _tdc_statistics_sc.events_found[i];
    for (int i = 0; i < tdc_stat_events_in_roi_size; i++)
        tdc_stat_events_in_roi_val[i] = _tdc_statistics_sc.events_in_roi[i];
    for (int i = 0; i < tdc_stat_events_received_size; i++)
        tdc_stat_events_received_val[i] = _tdc_statistics_sc.events_received[i];
    // fill the summarized statistics spectrum
    tdc_stat_summarized_val[0] = 0.0; // 0 and unused left value is good for bar-graph plotting
    tdc_stat_summarized_val[1] = tdc_stat_events_found_val[0];
    tdc_stat_summarized_val[2] = 0;
    tdc_stat_summarized_val[3] = tdc_stat_counts_read_val[0];
    if (tdc_stat_counts_read_val[1]>0)
        tdc_stat_summarized_val[4] = tdc_stat_counts_read_val[1];
    else
        tdc_stat_summarized_val[4] = tdc_stat_counts_read_val[4];
    if (tdc_stat_counts_read_val[2]>0)
        tdc_stat_summarized_val[5] = tdc_stat_counts_read_val[2];
    else
        tdc_stat_summarized_val[5] = tdc_stat_counts_read_val[8];
    if (tdc_stat_counts_read_val[3]>0)
        tdc_stat_summarized_val[6] = tdc_stat_counts_read_val[3];
    else 
        tdc_stat_summarized_val[6] = tdc_stat_counts_read_val[12];
    //tdc_stat_summarized_val[6] = tdc_stat_events_received_val[0];
    //tdc_stat_summarized_val[7] = tdc_stat_events_in_roi_val[0];
    tdc_stat_summarized_val[7] = 0.0;
    tdc_stat_summarized_val[8] = 0.0;
    // ### calculate maximum and average #######################################
    tdc_stat_max_raw_count_val = 0;
    tdc_stat_avg_raw_count_val = 0;
    Tango::DevLong avg_divisor = 0;
    for (std::size_t i=0; i<64; i++) {
      if (tdc_stat_counts_read_val[i] > tdc_stat_max_raw_count_val)
        tdc_stat_max_raw_count_val = tdc_stat_counts_read_val[i];
      if (tdc_stat_counts_read_val[i] > 0) {
        tdc_stat_avg_raw_count_val += tdc_stat_counts_read_val[i];
        avg_divisor++;
      }
    }
    if (avg_divisor > 0)
      tdc_stat_avg_raw_count_val = tdc_stat_avg_raw_count_val / avg_divisor;
    // ### push change events ##################################################
    try {
        //std::cout << "sending TDC statistics change events" << std::endl;
        //long t1 = Helper::get_millisec();
        push_change_event("TDC_Stat_Counters", tdc_stat_counters_val, tdc_stat_counters_size, 0);
        push_change_event("TDC_Stat_Counts_Read", tdc_stat_counts_read_val, tdc_stat_counts_read_size, 0);
        push_change_event("TDC_Stat_Counts_Received", tdc_stat_counts_received_val, tdc_stat_counts_received_size, 0);
        push_change_event("TDC_Stat_Events_Found", tdc_stat_events_found_val, tdc_stat_events_found_size, 0);
        push_change_event("TDC_Stat_Events_In_Roi", tdc_stat_events_in_roi_val, tdc_stat_events_in_roi_size, 0);
        push_change_event("TDC_Stat_Events_Received", tdc_stat_events_received_val, tdc_stat_events_received_size, 0);
        push_change_event("TDC_Stat_Summarized", tdc_stat_summarized_val, tdc_stat_summarized_size, 0);
        push_change_event("TDC_Stat_Max_Raw_Count", &tdc_stat_max_raw_count_val, 1, 0);
        push_change_event("TDC_Stat_Avg_Raw_Count", &tdc_stat_avg_raw_count_val, 1, 0);
        //long t2 = Helper::get_millisec();
        //std::cout << "sending TDC statistics took " << t2-t1 << " ms" << std::endl;
    }
    catch (Tango::DevFailed e) {
        std::cout << "Tango exception while pushing change events" << std::endl;
        int len = e.errors.length();
        for (int i=0; i<len; i++) {
            std::cout << " desc:" << e.errors[i].desc << std::endl;
            std::cout << " origin:" << e.errors[i].origin << std::endl;
            std::cout << " reason:" << e.errors[i].reason << std::endl;
        }
    }
    
}

void SurfaceConceptTDC::_zero_tdc_statistics() {
    for (int i = 0; i < tdc_stat_counters_size; i++)
        tdc_stat_counters_val[i] = 0;
    for (int i = 0; i < tdc_stat_counts_read_size; i++)
        tdc_stat_counts_read_val[i] = 0;
    for (int i = 0; i < tdc_stat_counts_received_size; i++)
        tdc_stat_counts_received_val[i] = 0;
    for (int i = 0; i < tdc_stat_events_found_size; i++)
        tdc_stat_events_found_val[i] = 0;
    for (int i = 0; i < tdc_stat_events_in_roi_size; i++)
        tdc_stat_events_in_roi_val[i] = 0;
    for (int i = 0; i < tdc_stat_events_received_size; i++)
        tdc_stat_events_received_val[i] = 0;
    for (int i = 0; i < tdc_stat_summarized_size; i++)
        tdc_stat_summarized_val[i] = 0;
    tdc_stat_avg_raw_count_val = 0;
    tdc_stat_max_raw_count_val = 0;
}

void SurfaceConceptTDC::Hist_Full_T_ReadCallback(Tango::DeviceImpl* dev, Tango::Attribute& att) {
    //std::cout << "Hist_Full_T_ReadCallback(...) got called" << std::endl;
    GeneralHistogram* h = m_hist_map.at("Hist_Full_T");
    Tango::DevLong* tbuf = h->GetTangoBuffer();
    if (tbuf!=NULL) {
        //std::cout << "Hist_Full_T_ReadCallback(...) sent real data" << std::endl;
        att.set_value(tbuf, h->GetTangoBufferDataWidth());
    }
    else {
        //std::cout << "Hist_Full_T_ReadCallback(...) sent dummy buffer" << std::endl;
        Tango::DevLong dummybuf[2] = {0,0};
        att.set_value(dummybuf, 2, 0);
    }
}

void SurfaceConceptTDC::Hist_Full_Accu_T_ReadCallback(Tango::DeviceImpl* dev, Tango::Attribute& att) {
    GeneralHistogram* h = m_hist_map.at("Hist_Full_T");
    Tango::DevLong* tbuf = h->GetTangoAccuBuffer();
    if (tbuf!=NULL) {
        //std::cout << "Hist_Full_T_ReadCallback(...) sent real data" << std::endl;
        att.set_value(tbuf, h->GetTangoBufferDataWidth());
    }
    else {
        //std::cout << "Hist_Full_T_ReadCallback(...) sent dummy buffer" << std::endl;
        Tango::DevLong dummybuf[2] = {0,0};
        att.set_value(dummybuf, 2, 0);
    }
}

void SurfaceConceptTDC::Hist_Live_User_T_ReadCallback(Tango::DeviceImpl* dev, Tango::Attribute& att) {
    GeneralHistogram* h = m_hist_map.at("Hist_User_T");
    Tango::DevLong* tbuf = h->GetTangoBuffer();
    if (tbuf!=NULL) {
        att.set_value(tbuf, h->GetTangoBufferDataWidth());
    }
    else {
        Tango::DevLong dummybuf[2] = {0,0};
        att.set_value(dummybuf, 2, 0);
    }
}

void SurfaceConceptTDC::Hist_Accu_User_T_ReadCallback(Tango::DeviceImpl* dev, Tango::Attribute& att) {
    GeneralHistogram* h = m_hist_map.at("Hist_User_T");
    Tango::DevLong* tbuf = h->GetTangoAccuBuffer();
    if (tbuf!=NULL) {
        att.set_value(tbuf, h->GetTangoBufferDataWidth());
    }
    else {
        Tango::DevLong dummybuf[2] = {0,0};
        att.set_value(dummybuf, 2, 0);
    }
}

void SurfaceConceptTDC::HistUserTAux_ReadCallback(Tango::DeviceImpl*, Tango::Attribute& att) {
    std::string attrname = att.get_name();
    if (attrname.compare("Hist_User_T_ModBinned")==0) {
        att.set_value(&hist_user_t_modbinned_val);
    }
    else if (attrname.compare("Hist_User_T_TOFF_Raw")==0) {
        att.set_value(&hist_user_t_toff_raw_val);
    }

}

void SurfaceConceptTDC::HistUserTAux_WriteCallback(Tango::DeviceImpl*, Tango::WAttribute& att) {
    std::string attrname = att.get_name();
    long bint = -1; // need current binning for fanning out either Hist_User_T_ModBinned or Hist_User_T_TOFF
    try {
        bint = m_hist_map["Hist_User_T"]->bint;
    } catch (std::exception& e) {};
    if (attrname.compare("Hist_User_T_ModBinned")==0) {
        att.get_write_value(hist_user_t_modbinned_val);
        if (bint>0) {
            HistogramAttributeWriteCallbackAction("Hist_User_T_MODULO", hist_user_t_modbinned_val*bint);
        }
    }
    else if (attrname.compare("Hist_User_T_TOFF_Raw")==0) {
        att.get_write_value(hist_user_t_toff_raw_val);
        if (bint>0) {
            HistogramAttributeWriteCallbackAction("Hist_User_T_ROI_TOFF", hist_user_t_toff_raw_val/bint);
        }
    }
}


void SurfaceConceptTDC::Hist_Live_T_ReadCallback(Tango::DeviceImpl*, Tango::Attribute& att) {
    GeneralHistogram* h = m_hist_map.at("Hist_Live_T");
    Tango::DevLong* tbuf = h->GetTangoBuffer();
    if (tbuf!=NULL) {
        att.set_value(tbuf, h->GetTangoBufferDataWidth());
    }
    else {
        Tango::DevLong dummybuf[2] = {0,0};
        att.set_value(dummybuf, 2, 0);
    }
}


void SurfaceConceptTDC::Hist_Accu_T_ReadCallback(Tango::DeviceImpl* dev, Tango::Attribute& att) {
    GeneralHistogram* h = m_hist_map.at("Hist_Accu_T");
    Tango::DevLong* tbuf = h->GetTangoBuffer();
    if (tbuf!=NULL) {
        //std::cout << "Hist_Full_T_ReadCallback(...) sent real data" << std::endl;
        att.set_value(tbuf, h->GetTangoBufferDataWidth());
    }
    else {
        //std::cout << "Hist_Full_T_ReadCallback(...) sent dummy buffer" << std::endl;
        Tango::DevLong dummybuf[2] = {0,0};
        att.set_value(dummybuf, 2, 0);
    }
}

void SurfaceConceptTDC::Hist_Full_TAxis_ReadCallback(Tango::DeviceImpl* dev, Tango::Attribute& att) {
    GeneralHistogram* h = m_hist_map.at("Hist_Full_T");
    Tango::DevDouble* tbuf = h->getTAxis();
    if (tbuf!=NULL)
        att.set_value(tbuf, h->GetTangoBufferDataWidth());
    else {
        Tango::DevDouble dummybuf[2] = {0};
        att.set_value(dummybuf, 2, 0);
    }
}

void SurfaceConceptTDC::Hist_Accu_TAxis_ReadCallback(Tango::DeviceImpl* dev, Tango::Attribute& att) {
    GeneralHistogram* h = m_hist_map.at("Hist_Accu_T");
    Tango::DevDouble* tbuf = h->getTAxis();
    if (tbuf!=NULL)
        att.set_value(tbuf, h->GetTangoBufferDataWidth());
    else {
        Tango::DevDouble dummybuf[2] = {0,0};
        att.set_value(dummybuf, 2, 0);
    }
}

void SurfaceConceptTDC::Hist_Live_TAxis_ReadCallback(Tango::DeviceImpl* dev, Tango::Attribute& att) {
    GeneralHistogram* h = m_hist_map.at("Hist_Live_T");
    Tango::DevDouble* tbuf = h->getTAxis();
    if (tbuf!=NULL)
        att.set_value(tbuf, h->GetTangoBufferDataWidth());
    else {
        Tango::DevDouble dummybuf[2] = {0,0};
        att.set_value(dummybuf, 2, 0);
    }
}

void SurfaceConceptTDC::Hist_User_Taxis_ReadCallback(Tango::DeviceImpl* dev, Tango::Attribute& att) {
    GeneralHistogram* h = m_hist_map.at("Hist_User_T");
    Tango::DevDouble* tbuf = h->getTAxis();
    if (tbuf!=NULL)
        att.set_value(tbuf, h->GetTangoBufferDataWidth());
    else {
        Tango::DevDouble dummybuf[2] = {0};
        att.set_value(dummybuf, 2, 0);
    }
}

void SurfaceConceptTDC::AddHistUserTAuxAttributes() {
    // Hist_User_T_ModBinned
    hist_user_t_modbinned_attr = new CustomAttr("Hist_User_T_ModBinned", Tango::DEV_LONG64, Tango::READ_WRITE, Tango::AssocWritNotSpec);
    hist_user_t_modbinned_attr->SetReadCallback(this, &SurfaceConceptTDC::HistUserTAux_ReadCallback);
    hist_user_t_modbinned_attr->SetWriteCallback(this, &SurfaceConceptTDC::HistUserTAux_WriteCallback);
    Tango::UserDefaultAttrProp ap;
    ap.set_description("Time modulo in binned pixels");
    hist_user_t_modbinned_attr->set_default_properties(ap);
    hist_user_t_modbinned_attr->set_memorized();
    this->add_attribute(hist_user_t_modbinned_attr);
    // Hist_User_T_TOFF_Raw
    hist_user_t_toff_raw_attr = new CustomAttr("Hist_User_T_TOFF_Raw", Tango::DEV_LONG64, Tango::READ_WRITE, Tango::AssocWritNotSpec);
    hist_user_t_toff_raw_attr->SetReadCallback(this, &SurfaceConceptTDC::HistUserTAux_ReadCallback);
    hist_user_t_toff_raw_attr->SetWriteCallback(this, &SurfaceConceptTDC::HistUserTAux_WriteCallback);
    Tango::UserDefaultAttrProp ap2;
    ap2.set_description("Time offset in unbinned pixels");
    hist_user_t_toff_raw_attr->set_default_properties(ap2);
    hist_user_t_toff_raw_attr->set_memorized();
    this->add_attribute(hist_user_t_toff_raw_attr);
}



void SurfaceConceptTDC::AddCSSAttributes() {
    Tango::UserDefaultAttrProp ap;
    ap.set_description("Storage attribute for clients. Has no effect on server functionality.");
    for (std::string attrname : {"CSS_Live_XY_Max", "CSS_Live_XY_Max_Log",
            "CSS_Live_XT_Max", "CSS_Live_XT_Max_Log", 
            "CSS_Live_YT_Max", "CSS_Live_YT_Max_Log", 
            "CSS_Accu_XY_Max", "CSS_Accu_XY_Max_Log", 
            "CSS_Accu_XT_Max", "CSS_Accu_XT_Max_Log",
            "CSS_Accu_YT_Max", "CSS_Accu_YT_Max_Log", 
            "CSS_Live_Max", "CSS_Live_Max_Log",
            "CSS_Accu_Max", "CSS_Accu_Max_Log",
            "CSS_AutoBC"}) {
                CustomAttr* n = new CustomAttr(attrname.c_str(), Tango::DEV_DOUBLE, Tango::READ_WRITE, Tango::AssocWritNotSpec);
                n->SetReadCallback(this, &SurfaceConceptTDC::CSS_ReadCallback);
                n->SetWriteCallback(this, &SurfaceConceptTDC::CSS_WriteCallback);
                n->set_default_properties(ap);
                n->set_memorized();
                css_attrs[attrname] = n;
                //css_values[attrname] = 0;
                this->add_attribute(n);
            }
}

void SurfaceConceptTDC::CSS_ReadCallback(Tango::DeviceImpl* dev, Tango::Attribute& attr) {
    std::string attrname = attr.get_name();
    Tango::DevDouble rval = 0; // use this value, if attribute not yet in the map
    try {
        rval = css_values.at(attrname);
    }
    catch (const std::out_of_range& oor) {
        css_values[attrname] = rval;
    }
    attr.set_value(&rval);    
}

void SurfaceConceptTDC::CSS_WriteCallback(Tango::DeviceImpl*, Tango::WAttribute& attr) {
    std::string attrname = attr.get_name();
    Tango::DevDouble wval = 0;
    attr.get_write_value(wval);
    css_values[attrname] = wval;
    // now write related values
    // fan to log-scale attributes (if linear-scale attribute was written)
    if (attrname.compare("CSS_Live_XY_Max")==0 || 
            attrname.compare("CSS_Live_XT_Max")==0 ||
            attrname.compare("CSS_Live_YT_Max")==0 || 
            attrname.compare("CSS_Accu_XY_Max")==0 ||
            attrname.compare("CSS_Accu_XT_Max")==0 ||
            attrname.compare("CSS_Accu_YT_Max")==0 ||
            attrname.compare("CSS_Live_Max")==0 || 
            attrname.compare("CSS_Accu_Max")==0 )
            css_values[attrname+"_Log"] = (wval>=1.0 ? ::log10(wval) : 0.0);
    // fan to linear-scale attributes (if log-scale attribute was written)
    if (attrname.substr(attrname.length()-4).compare("_Log")==0)
        css_values[attrname.substr(0,attrname.length()-4)] = (wval>0.0 ? ::pow10(wval) : 0.0);
    // CSS_Live_Max fans to individual xy,xt,yt live-image max values
    if (attrname.compare("CSS_Live_Max")==0)
        for (std::string targname : {"CSS_Live_XY_Max", "CSS_Live_XT_Max", "CSS_Live_YT_Max"}) {
            css_values[targname] = wval;
            css_values[targname+"_Log"] = (wval>=1.0 ? ::log10(wval) : 0.0);
        };
    // CSS_Accu_Max fans to individual xy,xt,yt accu-image max values
    if (attrname.compare("CSS_Accu_Max")==0)
        for (std::string targname : {"CSS_Accu_XY_Max", "CSS_Accu_XT_Max", "CSS_Accu_YT_Max"}) {
            css_values[targname] = wval;
            css_values[targname+"_Log"] = (wval>=1.0 ? ::log10(wval) : 0.0);
        };
    // CSS_Live_Max_Log fans to individual xy,xt,yt live-image max values
    if (attrname.compare("CSS_Live_Max_Log")==0)
        for (std::string targname : {"CSS_Live_XY_Max", "CSS_Live_XT_Max", "CSS_Live_YT_Max"}) {
            css_values[targname] = (wval>0.0 ? ::pow10(wval) : 0.0);
            css_values[targname+"_Log"] = wval;
        };
    // CSS_Accu_Max_Log fans to individual xy,xt,yt accu-image max values       
    if (attrname.compare("CSS_Accu_Max_Log")==0)
        for (std::string targname : {"CSS_Accu_XY_Max", "CSS_Accu_XT_Max", "CSS_Accu_YT_Max"}) {
            css_values[targname] = (wval>0.0 ? ::pow10(wval) : 0.0);
            css_values[targname+"_Log"] = wval;
        };
}


void SurfaceConceptTDC::AddStatusAttributes() {
    // Status_Spectrum_Saved
    status_spectrum_saved_attr = new CustomAttr("Status_Spectrum_Saved", Tango::DEV_BOOLEAN, Tango::READ, Tango::AssocWritNotSpec);    
    status_spectrum_saved_attr->SetReadCallback(this, &SurfaceConceptTDC::Status_ReadCallback);
    Tango::UserDefaultAttrProp ap1;
    ap1.set_description("true if the Hist_Accu_T data has been saved to a text file via CmdTrig_Save_THist_Accu");
    status_spectrum_saved_attr->set_default_properties(ap1);
    this->add_attribute(status_spectrum_saved_attr);
    // Status_Spectrum2_Saved
    status_spectrum2_saved_attr = new CustomAttr("Status_Spectrum2_Saved", Tango::DEV_BOOLEAN, Tango::READ, Tango::AssocWritNotSpec);    
    status_spectrum2_saved_attr->SetReadCallback(this, &SurfaceConceptTDC::Status_ReadCallback);
    Tango::UserDefaultAttrProp ap1b;
    ap1b.set_description("true if the Hist_Accu_User_T data has been saved to a text file via CmdTrig_Save_THist_User_Accu");
    status_spectrum2_saved_attr->set_default_properties(ap1b);
    this->add_attribute(status_spectrum2_saved_attr);
    // Status_XYT_Saved
    status_xyt_saved_attr = new CustomAttr("Status_XYT_Saved", Tango::DEV_BOOLEAN, Tango::READ, Tango::AssocWritNotSpec);    
    status_xyt_saved_attr->SetReadCallback(this, &SurfaceConceptTDC::Status_ReadCallback);
    Tango::UserDefaultAttrProp ap2;
    ap2.set_description("true if the XYT data set has been saved to a TIFF stack");
    status_xyt_saved_attr->set_default_properties(ap2);
    this->add_attribute(status_xyt_saved_attr);
    // Status_XY_Saved
    status_xy_saved_attr = new CustomAttr("Status_XY_Saved", Tango::DEV_BOOLEAN, Tango::READ, Tango::AssocWritNotSpec);    
    status_xy_saved_attr->SetReadCallback(this, &SurfaceConceptTDC::Status_ReadCallback);
    Tango::UserDefaultAttrProp ap2b;
    ap2b.set_description("true if the XY image has been saved.");
    status_xy_saved_attr->set_default_properties(ap2b);
    this->add_attribute(status_xy_saved_attr);
    
}


void SurfaceConceptTDC::Status_ReadCallback(Tango::DeviceImpl* dev, Tango::Attribute& attr) {
    std::string attrname = attr.get_name();
    if (attrname.compare("Status_Spectrum_Saved")==0) {
        attr.set_value(&status_spectrum_saved_val);
    }
    if (attrname.compare("Status_Spectrum2_Saved")==0) {
        attr.set_value(&status_spectrum2_saved_val);
    }
    else if (attrname.compare("Status_XYT_Saved")==0) {
        attr.set_value(&status_xyt_saved_val);
    }
    else if (attrname.compare("Status_XY_Saved")==0) {
        attr.set_value(&status_xy_saved_val);
    }
}

} // namespace
