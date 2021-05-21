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

#include <stdint.h>

#include "GeneralHistogram.h"
#include "Helper.h"
#include "PGM_Export.h"
#include <arpa/inet.h>

namespace SurfaceConceptTDC_ns {
    
    const std::string GeneralHistogram::AllAttributes[] = {"ROI_X1", "ROI_X2", "ROI_Y1", "ROI_Y2", 
            "ROI_T1", "ROI_T2", "ROI_TOFF", "ROI_TSIZE", "BIN_X", "BIN_Y", "BIN_T", "MODULO", ""};
    
    const std::map<std::string, long> GeneralHistogram::AttributesMin = 
    {{"ROI_X1", 0}, {"ROI_X2", 0}, 
     {"ROI_Y1", 0},  {"ROI_Y2", 0}, 
     {"ROI_T1", -1}, {"ROI_T2", -1},
     {"ROI_TOFF", -1}, {"ROI_TSIZE", 1},
     {"BIN_X", 1}, {"BIN_Y", 1}, {"BIN_T", 1}, 
     {"MODULO", 0}};

    const std::map<std::string, long> GeneralHistogram::AttributesMax = 
    {{"ROI_X1", 4096}, {"ROI_X2", 4096}, 
     {"ROI_Y1", 4096}, {"ROI_Y2", 4096}, 
     {"ROI_T1", -1}, {"ROI_T2", -1},
     {"ROI_TOFF", -1}, {"ROI_TSIZE", -1},
     {"BIN_X", 1024}, {"BIN_Y", 1024}, {"BIN_T", -1}, 
     {"MODULO", -1}};

    const std::map<std::string, std::string> GeneralHistogram::AttributesFormat = 
    {{"ROI_X1", "%4d"}, {"ROI_X2", "%4d"}, 
     {"ROI_Y1", "%4d"}, {"ROI_Y2", "%4d"}, 
     {"ROI_T1", "%18d"}, {"ROI_T2", "%18d"},
     {"ROI_TOFF", "%18d"}, {"ROI_TSIZE", "%18d"},
     {"BIN_X", "%4d"}, {"BIN_Y", "%4d"}, {"BIN_T", "%18d"}, 
     {"MODULO", "%18d"}};
    
    GeneralHistogram::GeneralHistogram(::sc_pipe_type_t pipe_type_) {
        pipe_type = pipe_type_;
        for (int i = 0; !GeneralHistogram::AllAttributes[i].empty(); i++)
            Attributes.push_back(AllAttributes[i]);
        // the initialization values for each parameter will be overwritten
        // when the Tango Database already has values and sets the attributes
        // at server startup
        if (pipe_type==::sc_pipe_type_t::DLD_IMAGE_XY) {
            bint = 1;
            roit2 = max_size_t; // full time range
            hist_par = (void*) new ::sc_pipe_dld_image_xy_params_t();
            ::sc_pipe_dld_image_xy_params_t *h = (::sc_pipe_dld_image_xy_params_t*) hist_par;
            h->accumulation_ms = std_accumulation;
            h->allocator_cb = &(GeneralHistogram::AllocatorCallback);
            h->allocator_owner = (void*) this;
            h->binning.time = bint;
            h->binning.x = binx;
            h->binning.y = biny;
            h->channel = -1;
            h->depth = Helper::bitsize_to_enum(depth);
            h->modulo = modulo*MODULO_FACTOR;
            h->roi.offset.time = ROIOff(roit1, roit2);
            h->roi.offset.x = ROIOff(roix1, roix2);
            h->roi.offset.y = ROIOff(roiy1, roiy2);
            h->roi.size.time = ROISize(roit1, roit2);
            h->roi.size.x = ROISize(roix1, roix2);
            h->roi.size.y = ROISize(roiy1, roiy2);
        }
        else if (pipe_type==::sc_pipe_type_t::DLD_IMAGE_XT) {
            bint = 16777216; // pow(2,24)
            roit2 = 512;
            biny = 1;   // integration axis doesn't need binning
            roiy2 = max_size_y; // use full range as standard setting
            hist_par = (void*) new ::sc_pipe_dld_image_xt_params_t();
            ::sc_pipe_dld_image_xt_params_t *h = (::sc_pipe_dld_image_xt_params_t*) hist_par;
            h->accumulation_ms = std_accumulation;
            h->allocator_cb = &(GeneralHistogram::AllocatorCallback);
            h->allocator_owner = (void*) this;
            h->binning.time = bint;
            h->binning.x = binx;
            h->binning.y = biny;
            h->channel = -1;
            h->depth = Helper::bitsize_to_enum(depth);
            h->modulo = modulo*MODULO_FACTOR; 
            h->roi.offset.time = ROIOff(roit1, roit2);
            h->roi.offset.x = ROIOff(roix1, roix2);
            h->roi.offset.y = ROIOff(roiy1, roiy2);
            h->roi.size.time = ROISize(roit1, roit2);
            h->roi.size.x = ROISize(roix1, roix2);
            h->roi.size.y = ROISize(roiy1, roiy2);
        }
        else if (pipe_type==::sc_pipe_type_t::DLD_IMAGE_YT) {
            bint = 16777216; // pow(2,24)
            roit2 = 512;
            binx = 1;           // integration axis doesn't need binning
            roix2 = max_size_x; // use full integration range as standard setting
            hist_par = (void*) new ::sc_pipe_dld_image_yt_params_t();
            ::sc_pipe_dld_image_yt_params_t *h = (::sc_pipe_dld_image_yt_params_t*) hist_par;
            h->accumulation_ms = std_accumulation;
            h->allocator_cb = &(GeneralHistogram::AllocatorCallback);
            h->allocator_owner = (void*) this;
            h->binning.time = bint;
            h->binning.x = binx;
            h->binning.y = biny;
            h->channel = -1;
            h->depth = Helper::bitsize_to_enum(depth);
            h->modulo = modulo*MODULO_FACTOR; 
            h->roi.offset.time = ROIOff(roit1, roit2);
            h->roi.offset.x = ROIOff(roix1, roix2);
            h->roi.offset.y = ROIOff(roiy1, roiy2);
            h->roi.size.time = ROISize(roit1, roit2);
            h->roi.size.x = ROISize(roix1, roix2);
            h->roi.size.y = ROISize(roiy1, roiy2);
        }
        else if (pipe_type==::sc_pipe_type_t::DLD_IMAGE_3D) {
            bint = 16777216; // pow(2,24)
            roit2 = 512;
            hist_par = (void*) new ::sc_pipe_dld_image_3d_params_t;
            ::sc_pipe_dld_image_3d_params_t *h = (::sc_pipe_dld_image_3d_params_t*) hist_par;
            h->accumulation_ms = std_accumulation;
            h->allocator_cb = &(GeneralHistogram::AllocatorCallback);
            h->allocator_owner = (void*) this;
            h->binning.time = bint;
            h->binning.x = binx;
            h->binning.y = biny;
            h->channel = -1;
            h->depth = Helper::bitsize_to_enum(depth);
            h->modulo = modulo*MODULO_FACTOR; 
            h->roi.offset.time = ROIOff(roit1, roit2);
            h->roi.offset.x = ROIOff(roix1, roix2);
            h->roi.offset.y = ROIOff(roiy1, roiy2);
            h->roi.size.time = ROISize(roit1, roit2);
            h->roi.size.x = ROISize(roix1, roix2);
            h->roi.size.y = ROISize(roiy1, roiy2);
        }
        else if (pipe_type==::sc_pipe_type_t::DLD_SUM_HISTO) {
            // here we just define default values
            // fully resolved (non-binned) time axis with full range
            bint = 4194304;
            roit2 = 600;
            hist_par = (void*) new ::sc_pipe_dld_sum_histo_params_t;
            ::sc_pipe_dld_sum_histo_params_t *h = (::sc_pipe_dld_sum_histo_params_t*) hist_par;
            h->accumulation_ms = std_accumulation;
            h->allocator_cb = &(GeneralHistogram::AllocatorCallback);
            h->allocator_owner = (void*) this;
            h->binning.time = bint;
            h->binning.x = 2048;
            h->binning.y = 2048;
            h->binning.time = bint;
            h->channel = -1;
            h->depth = Helper::bitsize_to_enum(depth);
            h->modulo = modulo*MODULO_FACTOR;
            h->roi.offset.time = 0;
            h->roi.offset.x = 0;
            h->roi.offset.y = 0;
            h->roi.size.time = ROISize(roit1, roit2); // multiplied with 2048 binning is pow(2,31)
            h->roi.size.x = 1;
            h->roi.size.y = 1;
            
        }
        else return;
        UpdateSCTDCHistoPipe();
    }
    
    GeneralHistogram::~GeneralHistogram() {
        if (pipe_id>-1 && device_descriptor>-1)
            ::sc_pipe_close2(device_descriptor, pipe_id);
        if (databuf!=NULL) {
            free(databuf);
            databuf = NULL;
        }
        if (stathist!=NULL) {
            delete stathist;
            stathist = NULL;
        }
        //if (tangobuf!=NULL) { // that was before it became a vector
        //    free(tangobuf);
        //    tangobuf = NULL;
        //}
        if (pipe_type==::sc_pipe_type_t::DLD_IMAGE_XY)
            delete (::sc_pipe_dld_image_xy_params_t*) hist_par;
        else if (pipe_type==::sc_pipe_type_t::DLD_IMAGE_XT)
            delete (::sc_pipe_dld_image_xt_params_t*) hist_par;
        else if (pipe_type==::sc_pipe_type_t::DLD_IMAGE_YT)
            delete (::sc_pipe_dld_image_yt_params_t*) hist_par;
        else if (pipe_type==::sc_pipe_type_t::DLD_IMAGE_3D)
            delete (::sc_pipe_dld_image_3d_params_t*) hist_par;
        else if (pipe_type==::sc_pipe_type_t::DLD_SUM_HISTO)
            delete (::sc_pipe_dld_sum_histo_params_t*) hist_par;
    }
    
    long GeneralHistogram::ROIOff(const long x1, const long x2) {
        return x1<x2?x1:x2;
    }

    long GeneralHistogram::ROISize(const long x1, const long x2) {
        return x1<x2?x2-x1+1:x1-x2+1;
    }
    
    template <typename T>
    void GeneralHistogram::_setAttribute(const std::string identifier, long value) {
        if (identifier.compare("ROI_X1")==0) {
            roix1 = value;
            ((T*) hist_par)->roi.offset.x = ROIOff(roix1, roix2);
            ((T*) hist_par)->roi.size.x = ROISize(roix1, roix2);
        }
        else if (identifier.compare("ROI_X2")==0) {
            roix2 = value;
            ((T*) hist_par)->roi.offset.x = ROIOff(roix1, roix2);
            ((T*) hist_par)->roi.size.x = ROISize(roix1, roix2);
        }
        else if (identifier.compare("ROI_Y1")==0) {
            roiy1 = value;
            ((T*) hist_par)->roi.offset.y = ROIOff(roiy1, roiy2);
            ((T*) hist_par)->roi.size.y = ROISize(roiy1, roiy2);
        }
        else if (identifier.compare("ROI_Y2")==0) {
            roiy2 = value;
            ((T*) hist_par)->roi.offset.y = ROIOff(roiy1, roiy2);
            ((T*) hist_par)->roi.size.y = ROISize(roiy1, roiy2);
        }
        else if (identifier.compare("ROI_T1")==0) {
            roit1 = value;
            ((T*) hist_par)->roi.offset.time = ROIOff(roit1, roit2);
            ((T*) hist_par)->roi.size.time = ROISize(roit1, roit2);
        }
        else if (identifier.compare("ROI_T2")==0) {
            roit2 = value;
            ((T*) hist_par)->roi.offset.time = ROIOff(roit1, roit2);
            ((T*) hist_par)->roi.size.time = ROISize(roit1, roit2);
        }
        else if (identifier.compare("ROI_TOFF")==0) {
            ((T*) hist_par)->roi.offset.time = value;
            roit1 = value;
            roit2 = value + ((T*) hist_par)->roi.size.time;
        }
        else if (identifier.compare("ROI_TSIZE")==0) {
            ((T*) hist_par)->roi.size.time = value;
            roit1 = ((T*) hist_par)->roi.offset.time;
            roit2 = roit1 + value - 1;
        }
        else if (identifier.compare("BIN_X")==0) {
            binx = value;
            ((T*) hist_par)->binning.x = value;
        }
        else if (identifier.compare("BIN_Y")==0) {
            biny = value;
            ((T*) hist_par)->binning.y = value;
        }
        else if (identifier.compare("BIN_T")==0) {
            bint = value;
            ((T*) hist_par)->binning.time = value;
        }
        else if (identifier.compare("MODULO")==0) {
            modulo = value;
            ((T*) hist_par)->modulo = value*MODULO_FACTOR;
        }
    }

    template <typename T>
    long GeneralHistogram::_getAttribute(const std::string identifier) {
        if (identifier.compare("ROI_X1")==0)
            return ((T*) hist_par)->roi.offset.x;
        else if (identifier.compare("ROI_X2")==0)
            return ((T*) hist_par)->roi.offset.x + ((T*) hist_par)->roi.size.x - 1;
        else if (identifier.compare("ROI_Y1")==0)
            return ((T*) hist_par)->roi.offset.y;
        else if (identifier.compare("ROI_Y2")==0)
            return ((T*) hist_par)->roi.offset.y + ((T*) hist_par)->roi.size.y - 1;
        else if (identifier.compare("ROI_T1")==0)
            return ((T*) hist_par)->roi.offset.time;
        else if (identifier.compare("ROI_T2")==0)
            return ((T*) hist_par)->roi.offset.time + ((T*) hist_par)->roi.size.time - 1;
        else if (identifier.compare("ROI_TOFF")==0)
            return ((T*) hist_par)->roi.offset.time;
        else if (identifier.compare("ROI_TSIZE")==0)
            return ((T*) hist_par)->roi.size.time;
        else if (identifier.compare("BIN_X")==0)
            return ((T*) hist_par)->binning.x;
        else if (identifier.compare("BIN_Y")==0)
            return ((T*) hist_par)->binning.y;
        else if (identifier.compare("BIN_T")==0)
            return ((T*) hist_par)->binning.time;
        else if (identifier.compare("MODULO")==0)
            return ((T*) hist_par)->modulo/MODULO_FACTOR;
        else return -1;
    }    
    
    long GeneralHistogram::GetAttribute(const std::string identifier) {
        if (pipe_type==::sc_pipe_type_t::DLD_IMAGE_XY)
            return _getAttribute< ::sc_pipe_dld_image_xy_params_t > (identifier);
        else if (pipe_type==::sc_pipe_type_t::DLD_IMAGE_XT)
            return _getAttribute< ::sc_pipe_dld_image_xt_params_t > (identifier);
        else if (pipe_type==::sc_pipe_type_t::DLD_IMAGE_YT)
            return _getAttribute< ::sc_pipe_dld_image_yt_params_t > (identifier);
        else if (pipe_type==::sc_pipe_type_t::DLD_IMAGE_3D)
            return _getAttribute< ::sc_pipe_dld_image_3d_params_t > (identifier);
        else if (pipe_type==::sc_pipe_type_t::DLD_SUM_HISTO)
            return _getAttribute< ::sc_pipe_dld_sum_histo_params_t > (identifier);
        else return -1;
        
    }

    void GeneralHistogram::SetAttribute(const std::string identifier, long value) {
        if (pipe_type==::sc_pipe_type_t::DLD_IMAGE_XY)
            _setAttribute< ::sc_pipe_dld_image_xy_params_t > (identifier, value);
        else if (pipe_type==::sc_pipe_type_t::DLD_IMAGE_XT)
            _setAttribute< ::sc_pipe_dld_image_xt_params_t > (identifier, value);
        else if (pipe_type==::sc_pipe_type_t::DLD_IMAGE_YT)
            _setAttribute< ::sc_pipe_dld_image_yt_params_t > (identifier, value);
        else if (pipe_type==::sc_pipe_type_t::DLD_IMAGE_3D)
            _setAttribute< ::sc_pipe_dld_image_3d_params_t > (identifier, value);
        else if (pipe_type==::sc_pipe_type_t::DLD_SUM_HISTO)
            _setAttribute< ::sc_pipe_dld_sum_histo_params_t > (identifier, value);
        else return;
        UpdateSCTDCHistoPipe();
    }
    
    long GeneralHistogram::GetAttributeMin(const std::string identifier) {
        if ( AttributesMin.count(identifier)==1 ) 
            return GeneralHistogram::AttributesMin.at(identifier);
        else
            return -1;
    }

    long GeneralHistogram::GetAttributeMax(const std::string identifier) {
        if ( AttributesMax.count(identifier)==1 ) 
            return GeneralHistogram::AttributesMax.at(identifier);
        else
            return -1;
    }
    
    std::string GeneralHistogram::GetAttributeFormat(const std::string identifier) {
        if ( AttributesFormat.count(identifier)==1 ) 
            return GeneralHistogram::AttributesFormat.at(identifier);
        else
            return std::string("");
    }
    
    long GeneralHistogram::GetWidth() {
        if (pipe_type==::sc_pipe_type_t::DLD_IMAGE_XY ||
            pipe_type==::sc_pipe_type_t::DLD_IMAGE_XT ||
            pipe_type==::sc_pipe_type_t::DLD_IMAGE_3D) {
            return ROISize(roix1, roix2);
        }
        else if (pipe_type==::sc_pipe_type_t::DLD_IMAGE_YT) {
            return ROISize(roiy1, roiy2);
        }
        else if (pipe_type==::sc_pipe_type_t::DLD_SUM_HISTO) {
            return ROISize(roit1, roit2);
        }
        return -1; // should not be reached for supported histo types
    }

    void GeneralHistogram::SetWidth(long newwidth) {
        if (pipe_type==::sc_pipe_type_t::DLD_IMAGE_XY ||
            pipe_type==::sc_pipe_type_t::DLD_IMAGE_XT ||
            pipe_type==::sc_pipe_type_t::DLD_IMAGE_3D) {
            roix2 = roix1 + newwidth - 1;
        }
        else if (pipe_type==::sc_pipe_type_t::DLD_IMAGE_YT) {
            roiy2 = roiy1 + newwidth - 1;
        }
        else if (pipe_type==::sc_pipe_type_t::DLD_SUM_HISTO) {
            roit2 = roit1 + newwidth - 1;
        }
    }

    
    
    void GeneralHistogram::SetAbscissaOffset(long newoffset) {
        if (pipe_type==::sc_pipe_type_t::DLD_IMAGE_XY ||
            pipe_type==::sc_pipe_type_t::DLD_IMAGE_XT ||
            pipe_type==::sc_pipe_type_t::DLD_IMAGE_3D) {
            roix2 = newoffset + ROISize(roix1, roix2) - 1;
            roix1 = newoffset;
        }
        else if (pipe_type==::sc_pipe_type_t::DLD_IMAGE_YT) {
            long w = ROISize(roiy1, roiy2);
            roiy2 = newoffset + ROISize(roiy1, roiy2) - 1;
            roiy1 = newoffset;
        }
        else if (pipe_type==::sc_pipe_type_t::DLD_SUM_HISTO) {
            long w = ROISize(roit1, roit2);
            roit2 = newoffset + ROISize(roit1, roit2) - 1;
            roit1 = newoffset;
        }
    }

    long GeneralHistogram::GetAbscissaOffset() {
        if (pipe_type==::sc_pipe_type_t::DLD_IMAGE_XY ||
            pipe_type==::sc_pipe_type_t::DLD_IMAGE_XT ||
            pipe_type==::sc_pipe_type_t::DLD_IMAGE_3D)
            return ROIOff(roix1, roix2);
        else if (pipe_type==::sc_pipe_type_t::DLD_IMAGE_YT)
            return ROIOff(roiy1, roiy2);
        else if (pipe_type==::sc_pipe_type_t::DLD_SUM_HISTO)
            return ROIOff(roit1, roit2);
        return -1; // only reached if invalid type
    }

    
    long GeneralHistogram::GetHeight() {
        if (pipe_type==::sc_pipe_type_t::DLD_IMAGE_XY ||
            pipe_type==::sc_pipe_type_t::DLD_IMAGE_3D) {
            return ROISize(roiy1, roiy2);
        }
        else if (pipe_type==::sc_pipe_type_t::DLD_IMAGE_XT ||
                 pipe_type==::sc_pipe_type_t::DLD_IMAGE_YT) {
            return ROISize(roit1, roit2);
        }
        else if (pipe_type==::sc_pipe_type_t::DLD_SUM_HISTO)
            return 1;
        return -1; // only reached if invalid type
    }

    void GeneralHistogram::SetHeight(long newheight) {
        if (pipe_type==::sc_pipe_type_t::DLD_IMAGE_XY ||
            pipe_type==::sc_pipe_type_t::DLD_IMAGE_3D) {
            roiy2 = roiy1 + newheight - 1;
        }
        else if (pipe_type==::sc_pipe_type_t::DLD_IMAGE_XT ||
                 pipe_type==::sc_pipe_type_t::DLD_IMAGE_YT) {
            roit2 = roit1 + newheight - 1;
        }
    }

    void GeneralHistogram::SetOrdinateOffset(long newoffset) {
        if (pipe_type==::sc_pipe_type_t::DLD_IMAGE_XY ||
            pipe_type==::sc_pipe_type_t::DLD_IMAGE_3D) {
            roiy2 = newoffset + ROISize(roiy1, roiy2) - 1;
            roiy1 = newoffset;
        }
        else if (pipe_type==::sc_pipe_type_t::DLD_IMAGE_XT ||
                 pipe_type==::sc_pipe_type_t::DLD_IMAGE_YT) {
            roit2 = roit1 + ROISize(roit1, roit2) - 1;
            roit1 = newoffset;
        }
    }

    long GeneralHistogram::GetOrdinateOffset() {
        if (pipe_type==::sc_pipe_type_t::DLD_IMAGE_XY ||
            pipe_type==::sc_pipe_type_t::DLD_IMAGE_3D)
            return ROIOff(roiy1, roiy2);
        else if (pipe_type==::sc_pipe_type_t::DLD_IMAGE_XT ||
                 pipe_type==::sc_pipe_type_t::DLD_IMAGE_YT)
            return ROIOff(roit1, roit2);
        else if (pipe_type==::sc_pipe_type_t::DLD_SUM_HISTO)
            return 0;
        return -1; // only reached if invalid type
    }
    
    long GeneralHistogram::GetZSize() {
        if (pipe_type==::sc_pipe_type_t::DLD_IMAGE_XY ||
            pipe_type==::sc_pipe_type_t::DLD_IMAGE_XT ||
            pipe_type==::sc_pipe_type_t::DLD_IMAGE_YT)
            return 1;
        else if (pipe_type==::sc_pipe_type_t::DLD_IMAGE_3D) {
            return ROISize(roit1, roit2);
        }
        else if (pipe_type==::sc_pipe_type_t::DLD_SUM_HISTO)
            return 1;
        return -1;  // only reached if invalid type
    }

    void GeneralHistogram::SetZSize(long newzsize) {
        if (pipe_type==::sc_pipe_type_t::DLD_IMAGE_3D)
            roit2 = roit1 + newzsize - 1;
    }

    void GeneralHistogram::SetZOffset(long newoffset) {
        if (pipe_type==::sc_pipe_type_t::DLD_IMAGE_3D) {
            roit2 = newoffset + ROISize(roit1, roit2) - 1;
            roit1 = newoffset;
        }
    }
    
    long GeneralHistogram::GetZOffset() {
        if (pipe_type==::sc_pipe_type_t::DLD_IMAGE_3D)
            return ROIOff(roit1, roit2);
        else
            return 0;
    }

    
    void GeneralHistogram::SetDeviceDescriptor(int devdesc) {
        if (pipe_id>=0) {
            std::cout << "ERROR: GeneralHistogram::SetDeviceDescriptor:" << std::endl;
            std::cout << " changing device descriptor while pipe is open." << std::endl;
            return;
        }
        //else if (devdesc<0) { // should be allowed to mark the device descriptor unset
        //    std::cout << "ERROR: GeneralHistogram::SetDeviceDescriptor:" << std::endl;
        //    std::cout << " trying to set a negative-valued device descriptor." << std::endl;
        //    return;
        //}
        else device_descriptor = devdesc;
    }
    
    int GeneralHistogram::GetDeviceDescriptor() {
        return device_descriptor;
    }
    
    void GeneralHistogram::SetPipeID(int new_pipe_id) {
        pipe_id = new_pipe_id;
    }
    
    int GeneralHistogram::GetPipeID() {
        return pipe_id;
    }
        
    void GeneralHistogram::SetPipeActive(bool state) {
        if (state==pipe_active) return;
        if (state==false) { // request to deactivate pipe
            if (device_descriptor>=0 && pipe_id>=0) {
                ::sc_pipe_close2(device_descriptor, pipe_id);
            }
            pipe_id = -1;
            pipe_active = false;
        }
        else { // request to activate pipe
            if (device_descriptor<0) {
                std::cout << "ERROR: GeneralHistogram::SetPipeActive:" << std::endl;
                std::cout << " trying to start a pipe with no device descriptor set." << std::endl;
                return;
            }
            else if (pipe_id>=0) {
                std::cout << "ERROR: GeneralHistogram::SetPipeActive:" << std::endl;
                std::cout << " trying to open a pipe, although a pipe is already open." << std::endl;
                return;
            }
            else { // everything is ok
                pipe_active = true;
                UpdateSCTDCHistoPipe();
            }
        }
    }
    
    bool GeneralHistogram::GetPipeActive() {
        return pipe_active;
    }

    /**
     * ensure that the data buffer is big enough
     * if not, reallocate it (this automatically zeroes the buffer, even if zerobuf==false)
     * @param zerobuf : if true, zero the data buffer, even if no reallocation is necessary
     */
    void GeneralHistogram::AccomodateDatabufSize(bool zerobuf) {
        long databufsize_ = GetWidth()*GetHeight()*GetZSize()*(depth/8);
        //std::cout << "GeneralHistogram::AccomodateDatabufSize: " << std::endl;
        //std::cout << " ... width = " << GetWidth() << ", height = " << GetHeight() << ", zsize = " << GetZSize() << ", bytes/pixel = " << (depth/8) << std::endl;
        //std::cout << " ... size of data buffer: " << databufsize_ << std::endl;
        if (databuf == NULL) {
            databuf = (void*) calloc(databufsize_, 1);
            if (databuf==NULL) {
                std::cout << "ERROR: GeneralHistogram::AccomodateDatabufSize:" << std::endl;
                std::cout << " unable to reserve memory (" << databufsize_ << " bytes)" << std::endl;
                return;
            }
            else databufsize = databufsize_;
        }
        else {
            if (databufsize<databufsize_) { // if we need a bigger data buffer, reallocate it
                //std::cout << "GeneralHistogram::AccomodateDatabufSize: need to reallocate buffer" << std::endl;
                //std::cout << "new size: " << databufsize_ << std::endl;
                free(databuf);
                databuf=NULL;
                databufsize=0;
                AccomodateDatabufSize();
            } // if current data buffer is big enough or bigger than necessary, leave it
            // zero the data buffer if explicitly asked for
            if (zerobuf) {
                int* p = (int *) databuf; // natural size of architecture, may be faster
                int intsize = sizeof(int); // we need conversion to signed int
                for (long i=0; i<databufsize/intsize; i++)
                    p[i] = 0;
                char* p2 = (char *) databuf; // need to clear the remainder bytewise
                for (long i=0; i<databufsize % intsize; i++)
                    p[databufsize-i-1] = 0;
            }
        }
    }

    void GeneralHistogram::ReleaseDatabuf() {
        if (databuf == NULL)
            return;
        free(databuf);
        databuf = NULL;
        databufsize = 0;
    }

    
    void GeneralHistogram::UpdateSCTDCHistoPipe() {
        if (device_descriptor<0)
            return;
        if (pipe_id > -1) {
            ::sc_pipe_close2(device_descriptor, pipe_id);
            pipe_id = -1;
        }

        AccomodateDatabufSize(); // ensure that our data buffer is big enough
        if (!pipe_active)
            return;

        if (databuf==NULL) {
            std::cout << "ERROR: GeneralHistogram::UpdateSCTDCHistoPipe:" << std::endl;
            std::cout << " unable to allocate data buffer" << std::endl;
            return;              // allocation of data buffer did not succeed, don't open pipe
        }

        int retval = ::sc_pipe_open2(device_descriptor, pipe_type, hist_par);
        if (retval<0) {
            std::cout << "ERROR: GeneralHistogram::UpdateSCTDCHistoPipe:" << std::endl;
            std::cout << " sc_pipe_open2() returned an error" << std::endl;
            Helper::cout_sc_err_message(retval);
            pipe_id = -1;
        }
        else {
            //std::cout << "GeneralHistogram::UpdateSCTDCHistoPipe(): succesfully opened a pipe" << std::endl;
            pipe_id = retval;
        }
    }
    
    void* GeneralHistogram::GetDatabufPointer() {
        return databuf;
    }
    
    int GeneralHistogram::AllocatorCallback(void *object, void **bufpointer) {
        //std::cout << "GeneralHistogram::AllocatorCallback(...) was called" << std::endl;
        *bufpointer = malloc(sizeof(void*));
        *bufpointer = ((GeneralHistogram*)object)->GetDatabufPointer();
        return 0;
    }
    
    void GeneralHistogram::SetFileOutputActive(bool state) {
        if (file_path.size()==0)
            return;
        if (state==true) {
            file_output_active = false; // first set to false, 
            // check if everything works and only then set to true
            if (!Helper::ensure_directory_exists(Helper::extract_dirname(file_path)))
                return;
            if (!Helper::test_file_writable(file_path))
                return;
            file_ptr = fopen(file_path.c_str(), "wb"); // keep the file open
            if (file_ptr!=NULL)
                file_output_active = true;
        }
        else {
            if (file_ptr!=NULL) {
                fclose(file_ptr);
                file_ptr = NULL;
            }
            file_output_active = false;
        }
    }
    
    void GeneralHistogram::SetFilePath(std::string new_file_path) {
        if (file_output_active) {
            SetFileOutputActive(false);
            file_path = new_file_path;
            SetFileOutputActive(true);
        }
        else file_path = new_file_path;
    }
    
    void GeneralHistogram::WriteFile() {
        //std::cout << "GeneralHistogram::WriteFile() called" << std::endl;
        if (!file_output_active || file_ptr==NULL || databuf==NULL) 
            return;
        if (file_output_big_endian) {
            _write_file_big_endian();
        }
        else {
            rewind(file_ptr); // set file pointer to beginning of the file
            int32_t w = GetWidth();
            int32_t h = GetHeight();
            int bytesz = depth/8;
            int32_t d = bytesz;
            fwrite(&w, sizeof(w), 1, file_ptr);
            fwrite(&h, sizeof(h), 1, file_ptr);
            fwrite(&d, sizeof(d), 1, file_ptr);
            fwrite(databuf, bytesz, w*h*GetZSize(), file_ptr);
            fflush(file_ptr);
        }
    }
    
    void GeneralHistogram::SetPGMOutputActive(bool state, int width, int height) {
        pgm_output_active = state;
        pgm_width = width;
        pgm_height = height;
    }
    
    void GeneralHistogram::SetPGMPath(std::string new_file_path) {
        if (new_file_path.size()==0)
            pgm_path = "";
        else
            pgm_path = Helper::ensure_extension(new_file_path, ".pgm");
    }
    
    void GeneralHistogram::WritePGM() {
        if (!pgm_output_active || databuf==NULL || pgm_path.size()==0) 
            return;
        if (pipe_type==::sc_pipe_type_t::DLD_SUM_HISTO)
            WritePGM1DPlot();
        else
            PGM_Export_from_uint32buf_autoBC(pgm_path.c_str(), (uint32_t*) databuf, GetWidth(), GetHeight());
    }
    
    void GeneralHistogram::WritePGM1DPlot(uint32_t maxvalue) {
        if (!pgm_output_active || databuf==NULL || pgm_path.size()==0) 
            return;
        //std::cout << "GetWidth():" << GetWidth() << " ; pgm_width:" << pgm_width << std::endl;
        uint32_t* binned = AutoBin_uint32Spectrum_To_New_Size((uint32_t*) databuf, GetWidth(), pgm_width);
        if (maxvalue==0)
            PGM_Export_from_uint32buf_Plot_autoMax(pgm_path.c_str(), binned, pgm_width, pgm_width, pgm_height);
        else
            PGM_Export_from_uint32buf_Plot(pgm_path.c_str(), binned, pgm_width, pgm_width, pgm_height, maxvalue);
    }

    void GeneralHistogram::PerformActiveOutputs() {
        // these functions only do something if the corresponding bool variable is true
        WriteFile();
        WritePGM();
        WriteTangoBuffer(); // only does something if tango attribute has been set
        UpdateStatisticsOfDatabuf();
    }

    void GeneralHistogram::UpdateStatisticsOfDatabuf() {
        if (databuf!=NULL) {
            if (stathist==NULL) stathist = new StatisticsHist(100);
            stathist->Update((uint32_t*) databuf, GetWidth()*GetHeight()*GetZSize());
        }
            
    }

    uint32_t GeneralHistogram::GetStatMax() {
        if (stathist!=NULL)
            return stathist->GetMax();
        else 
            return 0;
    }
    
    uint32_t GeneralHistogram::GetStatQuantile(double p) {
        if (stathist!=NULL)
            return stathist->GetQuantile(p);
        else
            return 0;
    }

    void GeneralHistogram::SetFileOutputBigEndian(bool state) {
        file_output_big_endian = state;
    }
    
    bool GeneralHistogram::GetFileOutputBigEndian() {
        return file_output_big_endian;
    }
    
    void GeneralHistogram::_write_file_big_endian() {
        //if (!file_output_active || file_ptr==NULL)  // test was already performed
        //    return;                                 // in WriteFile
        rewind(file_ptr); // set file pointer to beginning of the file
        uint32_t w_bigE = htonl(GetWidth());
        uint32_t h_bigE = htonl(GetHeight());
        int bytesz = depth/8;
        uint32_t d_bigE = htonl(bytesz);
        fwrite(&w_bigE, sizeof(w_bigE), 1, file_ptr);
        fwrite(&h_bigE, sizeof(h_bigE), 1, file_ptr);
        fwrite(&d_bigE, sizeof(d_bigE), 1, file_ptr);
        // write the buffer data --- the ugly part
        long zsize = GetZSize();
        long h = GetHeight();
        long w = GetWidth();
        uint32_t datum;
        uint8_t* p = (uint8_t*)&datum; // pointer into the bytes of datum for blanking
        uint8_t* pointer = (uint8_t*) databuf; // used for byte-granular addressing in the databuf array
        
        for (long z = 0; z<zsize; z++)
            for (long y = 0; y<h; y++)
                for (long x = 0; x<w; x++) {
                    datum = 0;
                    memcpy(&datum, pointer, bytesz);
                    datum = htonl(datum); // swap endianness if necessary
                    // now we know, that it is big endian
                    fwrite(p+4-bytesz, bytesz, 1, file_ptr);
                    pointer += bytesz;
                }
        fflush(file_ptr);
    }
    
    bool GeneralHistogram::GetFileOutputActive() {
        return file_output_active;
    }
    
    std::string GeneralHistogram::GetFilePath() {
        return file_path;
    }
    
    void GeneralHistogram::ClearBuffer() {
        if (databuf==NULL) 
            return;
        memset(databuf, '\0', databufsize);
    }
    
    long GeneralHistogram::GetDatabufSize() {
        return databufsize;
    }
    
    void GeneralHistogram::SetPGMSize(int width, int height) {
        if (width>0) pgm_width = width;
        if (height>0) pgm_height = height;
    }

    int GeneralHistogram::GetPGMHeight() {
        return pgm_height;
    }
    
    int GeneralHistogram::GetPGMWidth() {
        return pgm_width;
    }

    void GeneralHistogram::_write_taxis(int w_) {
        if (taxis_active && tango_taxis_attr!=NULL) {
            int max_x = tango_taxis_attr->get_max_x();
            w_ = w_>max_x?max_x:w_;
            tangobuf_taxis.resize(w_);
            //AccomodateTangobuffer<Tango::DevDouble>(&tangobuf_taxis, w_, h_, false);
            if (taxis_unit==taxis_unit_s)
                for (int x=0; x<w_; x++)
                    tangobuf_taxis[x] = (Tango::DevDouble) (((double)(x+roit1))*taxis_pixelsize*bint*1e-12);
            else if (taxis_unit==taxis_unit_ms)
                for (int x=0; x<w_; x++)
                    tangobuf_taxis[x] = (Tango::DevDouble) (((double)(x+roit1))*taxis_pixelsize*bint*1e-9);
            else if (taxis_unit==taxis_unit_us)
                for (int x=0; x<w_; x++)
                    tangobuf_taxis[x] = (Tango::DevDouble) (((double)(x+roit1))*taxis_pixelsize*bint*1e-6);
            else if (taxis_unit==taxis_unit_ns)
                for (int x=0; x<w_; x++)
                    tangobuf_taxis[x] = (Tango::DevDouble) (((double)(x+roit1))*taxis_pixelsize*bint*1e-3);
            else if (taxis_unit==taxis_unit_ps)
                for (int x=0; x<w_; x++)
                    tangobuf_taxis[x] = (Tango::DevDouble) (((double)(x+roit1))*taxis_pixelsize*bint);            
            else if (taxis_unit==taxis_unit_pixels)
                for (int x=0; x<w_; x++)
                    tangobuf_taxis[x] = (Tango::DevDouble) x+roit1;
            else if (taxis_unit==taxis_unit_unbinned)
                for (int x=0; x<w_; x++)
                    tangobuf_taxis[x] = ((Tango::DevDouble) x+roit1)*((Tango::DevDouble) bint);
        }
        
    }
    
    void GeneralHistogram::WriteTangoBufferDevLong(int max_x, int max_y) {
        // ONLY SUPPORTS BIT DEPTH OF 32!!
        // w and h are the dimensions of the Tango attribute
        if (databuf==NULL) return;
        int w_ = max_x>GetWidth()?GetWidth():max_x;   // minimum of our dimensions and the tango attribute: 
        int h_ = max_y>GetHeight()?GetHeight():max_y; // cannot write more than we have, and cannot write more than the Tango attribute allows
        h_ = h_<2?1:h_; // h=0 is used in Tango to signal a spectrum (or scalar?) attribute (no y axis)
        tangobuf.resize(w_*h_);
        //AccomodateTangobuffer<Tango::DevLong>(&tangobuf, w_, h_, false);
        // do the copying
        int x,y;
        int databufw = GetWidth();
        uint32_t* buf = (uint32_t*) databuf;
        for (y=0; y<h_; y++)
            for (x=0; x<w_; x++) {
                tangobuf[y*w_+x] = (Tango::DevLong) buf[x+y*databufw];
            }
        _write_taxis(w_);
        tangobuf_datawidth = w_;
        tangobuf_dataheight = h_;
    }
    
    void GeneralHistogram::AddToTangoAccuBufferDevLong(int max_x, int max_y) {
        // ONLY SUPPORTS BIT DEPTH OF 32!!
        // w and h are the dimensions of the Tango attribute
        if (databuf==NULL) return;
        int w_ = max_x>GetWidth()?GetWidth():max_x;   // minimum of our dimensions and the tango attribute: 
        int h_ = max_y>GetHeight()?GetHeight():max_y; // cannot write more than we have, and cannot write more than the Tango attribute allows
        h_ = h_<2?1:h_; // h=0 is used in Tango to signal a spectrum (or scalar?) attribute (no y axis)
        tangobuf_accu.resize(w_,h_);
        //AccomodateTangobuffer<Tango::DevLong>(&tangobuf_accu, w_, h_, true); // rereserves memory if expanding
        // do the copying
        int x,y;
        int databufw = GetWidth();
        uint32_t* buf = (uint32_t*) databuf;
        for (y=0; y<h_; y++)
            for (x=0; x<w_; x++) {
                tangobuf_accu[y*w_+x] += (Tango::DevLong) buf[x+y*databufw];
            }
        _write_taxis(w_);
        // update the data x and y sizes reported by GetTangoBufferDataWidth()/Height()
        tangobuf_datawidth = w_;
        tangobuf_dataheight = h_; 
    }

    void GeneralHistogram::ZeroTangoAccuBufferDevLong() {
        if (tangobuf_accu.size()==0) return;
        for (std::size_t i=0; i<tangobuf_accu.size(); i++)
            tangobuf_accu[i] = 0;
    }

    void GeneralHistogram::SetTangoAccuBufferActive(bool active_) {
        tangobuf_accu_active = active_;
    }
    
    void GeneralHistogram::SetTangoOutAttribute(CustomSpectrumAttr* attr) {
        tango_spectrum_attr = attr;
    }
    
    void GeneralHistogram::SetTangoOutAttribute(CustomImageAttr* attr) {
        tango_image_attr = attr;
    }
    
    void GeneralHistogram::WriteTangoBuffer() {
        //std::cout << "WriteTangoBuffer() got called" << std::endl;
        if (tango_spectrum_attr!=NULL) {
            //std::cout << "tango_spectrum_attr is set" << std::endl;
            long max_x = tango_spectrum_attr->get_max_x();
            WriteTangoBufferDevLong(max_x, 1);
        }
        else if (tango_image_attr!=NULL) {
            //std::cout << "tango_image_attr is set" << std::endl;
            long max_x = tango_image_attr->get_max_x();
            long max_y = tango_image_attr->get_max_y();
            WriteTangoBufferDevLong(max_x, max_y);
        }
    }

    void GeneralHistogram::AddToTangoAccuBuffer() {
        if (tango_spectrum_attr!=NULL && tangobuf_accu_active) {
            long max_x = tango_spectrum_attr->get_max_x();
            AddToTangoAccuBufferDevLong(max_x, 1);
        }
        else if (tango_image_attr!=NULL && tangobuf_accu_active) {
            long max_x = tango_image_attr->get_max_x();
            long max_y = tango_image_attr->get_max_y();
            AddToTangoAccuBufferDevLong(max_x, max_y);
        }
    }
    
    long GeneralHistogram::GetTangoBufferDataWidth() {
        return tangobuf_datawidth;
    }

    long GeneralHistogram::GetTangoBufferDataHeight() {
        return tangobuf_dataheight;
    }

    Tango::DevLong* GeneralHistogram::GetTangoBuffer() {
        return tangobuf.data();
    }

    Tango::DevLong* GeneralHistogram::GetTangoAccuBuffer() {
        return tangobuf_accu.data();
    }
    
    Tango::DevDouble* GeneralHistogram::getTAxis() {
        return tangobuf_taxis.data();
    }
    
    void GeneralHistogram::ProvideTAxis(CustomSpectrumAttr* taxis_attr, double pixelsize, int unit) {
        taxis_active = (taxis_attr!=NULL);
        tango_taxis_attr = taxis_attr;
        taxis_pixelsize = pixelsize;
        taxis_unit = unit;
    }
    
    void GeneralHistogram::SetTAxisUnit(int unit) {
        taxis_unit = unit;
    }


}