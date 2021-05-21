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
static const char *RcsId = "$Id:  $";

#include <SurfaceConceptTDC.h>
#include <SurfaceConceptTDCClass.h>
#include <attrdesc.h>
#include "Helper.h"
#include "GeneralHistogram.h"

#ifdef __linux__
  string TERMRED = "\x1B[31m";
  string TERMGREEN = "\x1B[32m";
  string TERMRST = "\x1B[0m";
  string TERMSUCCESFUL = "\x1B[32msuccesful\x1B[0m";
  string TERMERROR = "\x1B[31merror\x1B[0m";
#else
  string TERMRED = "";
  string TERMGREEN = "";
  string TERMRST = "";
  string TERMSUCCESFUL = "succesful";
  string TERMERROR = "error";
#endif

/*----- PROTECTED REGION END -----*/	//	SurfaceConceptTDC.cpp

/**
 *  SurfaceConceptTDC class description:
 *    
 */

//================================================================
//  The following table gives the correspondence
//  between command and method names.
//
//  Command name      |  Method name
//================================================================
//  State             |  Inherited (no method)
//  Status            |  Inherited (no method)
//  Start             |  start
//  Stop              |  stop
//  AcquisitionStart  |  acquisition_start
//  AcquisitonStop    |  acquisiton_stop
//================================================================

//================================================================
//  Attributes managed are:
//================================================================
//  DeviceID      |  Tango::DevLong	Scalar
//  ExposureLive  |  Tango::DevDouble	Scalar
//  ExposureAccu  |  Tango::DevDouble	Scalar
//================================================================

namespace SurfaceConceptTDC_ns
{
/*----- PROTECTED REGION ID(SurfaceConceptTDC::namespace_starting) ENABLED START -----*/

//	static initializations

/*----- PROTECTED REGION END -----*/	//	SurfaceConceptTDC::namespace_starting

//--------------------------------------------------------
/**
 *	Method      : SurfaceConceptTDC::SurfaceConceptTDC()
 *	Description : Constructors for a Tango device
 *                implementing the classSurfaceConceptTDC
 */
//--------------------------------------------------------
SurfaceConceptTDC::SurfaceConceptTDC(Tango::DeviceClass *cl, string &s)
 : TANGO_BASE_CLASS(cl, s.c_str()), thread_pool(2)
{
	/*----- PROTECTED REGION ID(SurfaceConceptTDC::constructor_1) ENABLED START -----*/
    init_device();
    

    /*----- PROTECTED REGION END -----*/	//	SurfaceConceptTDC::constructor_1
}
//--------------------------------------------------------
SurfaceConceptTDC::SurfaceConceptTDC(Tango::DeviceClass *cl, const char *s)
 : TANGO_BASE_CLASS(cl, s), thread_pool(2)
{
	/*----- PROTECTED REGION ID(SurfaceConceptTDC::constructor_2) ENABLED START -----*/
    init_device();
    
    /*----- PROTECTED REGION END -----*/	//	SurfaceConceptTDC::constructor_2
}
//--------------------------------------------------------
SurfaceConceptTDC::SurfaceConceptTDC(Tango::DeviceClass *cl, const char *s, const char *d)
 : TANGO_BASE_CLASS(cl, s, d), thread_pool(2)
{
	/*----- PROTECTED REGION ID(SurfaceConceptTDC::constructor_3) ENABLED START -----*/
    init_device();
    
    /*----- PROTECTED REGION END -----*/	//	SurfaceConceptTDC::constructor_3
}

//--------------------------------------------------------
/**
 *	Method      : SurfaceConceptTDC::delete_device()
 *	Description : will be called at device destruction or at init command
 */
//--------------------------------------------------------
void SurfaceConceptTDC::delete_device()
{
	DEBUG_STREAM << "SurfaceConceptTDC::delete_device() " << device_name << endl;
	/*----- PROTECTED REGION ID(SurfaceConceptTDC::delete_device) ENABLED START -----*/
    
    //	Delete device allocated objects
        image_preview_update_thread.Stop();
        image_preview_update_thread.Join(); // need this to prevent core dump
    /*----- PROTECTED REGION END -----*/	//	SurfaceConceptTDC::delete_device
	delete[] attr_DeviceID_read;
	delete[] attr_ExposureLive_read;
	delete[] attr_ExposureAccu_read;
        //delete[] save_directory_val;
        //delete[] save_filename_val;
}

//--------------------------------------------------------
/**
 *	Method      : SurfaceConceptTDC::init_device()
 *	Description : will be called at device initialization.
 */
//--------------------------------------------------------
void SurfaceConceptTDC::init_device()
{
	DEBUG_STREAM << "SurfaceConceptTDC::init_device() create device " << device_name << endl;
	/*----- PROTECTED REGION ID(SurfaceConceptTDC::init_device_before) ENABLED START -----*/
    
    //	Initialization before get_device_property() call
    
    /*----- PROTECTED REGION END -----*/	//	SurfaceConceptTDC::init_device_before
	

	//	Get the device properties from database
	get_device_property();
	
	attr_DeviceID_read = new Tango::DevLong[1];
	attr_ExposureLive_read = new Tango::DevDouble[1];
	attr_ExposureAccu_read = new Tango::DevDouble[1];
        // ------------------------------------------------------
        // allocate buffers for char* variables holding tango string values
        for (char** p : {&save_directory_wval, 
                &save_directory_rval, 
                &save_filename_val,
                &info_accu_xyt_formattedsize_val,
                &hist_taxis_unit_val,
                &tdc_inifile_path_val,
                &cmd_trig_general_val,
                &server_message_val}) {
            *p = new char[STRING_BUF_SIZE];
            (*p)[0] = '\0';
        }
        // ------------------------------------------------------
        // tdc statistics, variables holding the attribute values
        tdc_stat_counters_val        = new Tango::DevLong[tdc_stat_counters_size];
        tdc_stat_counts_read_val     = new Tango::DevLong[tdc_stat_counts_read_size];
        tdc_stat_counts_received_val = new Tango::DevLong[tdc_stat_counts_received_size];
        tdc_stat_events_found_val    = new Tango::DevLong[tdc_stat_events_found_size];
        tdc_stat_events_in_roi_val   = new Tango::DevLong[tdc_stat_events_in_roi_size];
        tdc_stat_events_received_val = new Tango::DevLong[tdc_stat_events_received_size];
        tdc_stat_summarized_val      = new Tango::DevLong[tdc_stat_summarized_size];
        _zero_tdc_statistics();
        // ------------------------------------------------------
        image_preview_update_thread.SetCallback(&StaticImagePreviewUpdateAction, this);
        image_preview_update_thread.SetPeriod(200);
        image_preview_update_thread.Start();
        set_state(Tango::OFF);
        
        inifileOperations.SetSourceIniFilePath(iniFilePath);
        if (iniFilePath.length()>4)
            inifileOperations.SetTargetIniFilePath(iniFilePath.substr(0,iniFilePath.length()-4)+"_servercopy"+iniFilePath.substr(iniFilePath.length()-4));
        
        tdc_statistics_active = true;

	/*----- PROTECTED REGION ID(SurfaceConceptTDC::init_device) ENABLED START -----*/
    
    //	Initialize device
    
    /*----- PROTECTED REGION END -----*/	//	SurfaceConceptTDC::init_device
}

//--------------------------------------------------------
/**
 *	Method      : SurfaceConceptTDC::get_device_property()
 *	Description : Read database to initialize property data members.
 */
//--------------------------------------------------------
void SurfaceConceptTDC::get_device_property()
{
	/*----- PROTECTED REGION ID(SurfaceConceptTDC::get_device_property_before) ENABLED START -----*/
    
    //	Initialize property data members
    
    /*----- PROTECTED REGION END -----*/	//	SurfaceConceptTDC::get_device_property_before


	//	Read device properties from database.
	Tango::DbData	dev_prop;
	dev_prop.push_back(Tango::DbDatum("IniFilePath"));
	dev_prop.push_back(Tango::DbDatum("LivePreviewModeFilePath"));
        dev_prop.push_back(Tango::DbDatum("LivePreviewModeFileActive"));
        dev_prop.push_back(Tango::DbDatum("LivePreviewModeTangoActive"));
        dev_prop.push_back(Tango::DbDatum("SaveBaseDir"));
        dev_prop.push_back(Tango::DbDatum("FullHistBinXY"));
        dev_prop.push_back(Tango::DbDatum("FullHistBinT"));
        dev_prop.push_back(Tango::DbDatum("FullHistTOff"));
        dev_prop.push_back(Tango::DbDatum("FullHistTSize"));
        dev_prop.push_back(Tango::DbDatum("fullHistTPGMPreviewWidth"));
        dev_prop.push_back(Tango::DbDatum("CSS_Support_Active"));
        

	//	is there at least one property to be read ?
	if (dev_prop.size()>0)
	{
		//	Call database and extract values
		if (Tango::Util::instance()->_UseDb==true)
			get_db_device()->get_property(dev_prop);
	
		//	get instance on SurfaceConceptTDCClass to get class property
		Tango::DbDatum	def_prop, cl_prop;
		SurfaceConceptTDCClass	*ds_class =
			(static_cast<SurfaceConceptTDCClass *>(get_device_class()));
		int	i = -1;
                // -------------------------------------------------------------
		//	Try to initialize IniFilePath from class property
		cl_prop = ds_class->get_class_property(dev_prop[++i].name);
		if (cl_prop.is_empty()==false)	cl_prop  >>  iniFilePath;
		else {
			//	Try to initialize IniFilePath from default device value
			def_prop = ds_class->get_default_device_property(dev_prop[i].name);
			if (def_prop.is_empty()==false)	def_prop  >>  iniFilePath;
		}
		//	And try to extract IniFilePath value from database
		if (dev_prop[i].is_empty()==false)	dev_prop[i]  >>  iniFilePath;
                // in any case write this value back to our DbData structure
                dev_prop[i] << iniFilePath;
                // -------------------------------------------------------------
		//	Try to initialize LivePreviewModeFilePath from class property
		cl_prop = ds_class->get_class_property(dev_prop[++i].name);
		if (cl_prop.is_empty()==false)	cl_prop  >>  livePreviewModeFilePath;
		else {
			def_prop = ds_class->get_default_device_property(dev_prop[i].name);
			if (def_prop.is_empty()==false)	def_prop  >>  livePreviewModeFilePath;
		}
		if (dev_prop[i].is_empty()==false)	dev_prop[i]  >>  livePreviewModeFilePath;
                dev_prop[i] << livePreviewModeFilePath;
                // -------------------------------------------------------------
		//	Try to initialize LivePreviewModeFileActive from class property
		cl_prop = ds_class->get_class_property(dev_prop[++i].name);
		if (cl_prop.is_empty()==false)	cl_prop  >>  livePreviewModeFileActive;
		else {
			def_prop = ds_class->get_default_device_property(dev_prop[i].name);
			if (def_prop.is_empty()==false)	def_prop  >>  livePreviewModeFileActive;
		}
		if (dev_prop[i].is_empty()==false)	dev_prop[i]  >>  livePreviewModeFileActive;
                if (cl_prop.is_empty() && def_prop.is_empty() && dev_prop[i].is_empty()) {
                    livePreviewModeFileActive = false; // hard-coded value if everything else fails
                }
                dev_prop[i] << (livePreviewModeFileActive?"true":"false");
                // -------------------------------------------------------------
		//	Try to initialize LivePreviewModeTangoActive from class property
		cl_prop = ds_class->get_class_property(dev_prop[++i].name);
		if (cl_prop.is_empty()==false)	cl_prop  >>  livePreviewModeTangoActive;
		else {
			def_prop = ds_class->get_default_device_property(dev_prop[i].name);
			if (def_prop.is_empty()==false)	def_prop  >>  livePreviewModeTangoActive;
		}
		if (dev_prop[i].is_empty()==false)	dev_prop[i]  >>  livePreviewModeTangoActive;
                if (cl_prop.is_empty() && def_prop.is_empty() && dev_prop[i].is_empty()) {
                    livePreviewModeTangoActive = false; // hard-coded value if everything else fails
                }
                dev_prop[i] << (livePreviewModeTangoActive?"true":"false");
                // -------------------------------------------------------------
		//	Try to initialize SaveBaseDir from class property
		cl_prop = ds_class->get_class_property(dev_prop[++i].name);
		if (cl_prop.is_empty()==false)	cl_prop  >>  saveBaseDir;
		else {
			def_prop = ds_class->get_default_device_property(dev_prop[i].name);
			if (def_prop.is_empty()==false)	def_prop  >>  saveBaseDir;
		}
		if (dev_prop[i].is_empty()==false)	dev_prop[i]  >>  saveBaseDir;
                //      use hard-coded value if all of these options failed
                if (cl_prop.is_empty() && def_prop.is_empty() && dev_prop[i].is_empty()) {
                    saveBaseDir = Helper::get_user_home_dir();
                    std::cout << "Property SaveBaseDir not found in database." << std::endl;
                    std::cout << "Setting it to default value " << saveBaseDir << std::endl;                    
                }
                dev_prop[i] << saveBaseDir;
                // -------------------------------------------------------------
		//	Try to initialize FullHistBinXY from class property
		cl_prop = ds_class->get_class_property(dev_prop[++i].name);
		if (cl_prop.is_empty()==false)	cl_prop  >>  fullHistBinXY;
		else {
			def_prop = ds_class->get_default_device_property(dev_prop[i].name);
			if (def_prop.is_empty()==false)	def_prop  >>  fullHistBinXY;
		}
		if (dev_prop[i].is_empty()==false)	dev_prop[i]  >>  fullHistBinXY;
                //      use hard-coded value if all of these options failed
                if (cl_prop.is_empty() && def_prop.is_empty() && dev_prop[i].is_empty()) {
                    fullHistBinXY = "8";
                    std::cout << "Property FullHistBinXY not found in database." << std::endl;
                    std::cout << "Setting it to default value " << fullHistBinXY << std::endl;                    
                }
                dev_prop[i] << fullHistBinXY;
                // -------------------------------------------------------------
		//	Try to initialize FullHistBinT from class property
		cl_prop = ds_class->get_class_property(dev_prop[++i].name);
		if (cl_prop.is_empty()==false)	cl_prop  >>  fullHistBinT;
		else {
			def_prop = ds_class->get_default_device_property(dev_prop[i].name);
			if (def_prop.is_empty()==false)	def_prop  >>  fullHistBinT;
		}
		if (dev_prop[i].is_empty()==false)	dev_prop[i]  >>  fullHistBinT;
                //      use hard-coded value if all of these options failed
                if (cl_prop.is_empty() && def_prop.is_empty() && dev_prop[i].is_empty()) {
                    fullHistBinT = "4194304";
                    std::cout << "Property FullHistBinT not found in database." << std::endl;
                    std::cout << "Setting it to default value " << fullHistBinT << std::endl;                    
                }
                dev_prop[i] << fullHistBinT;
                // -------------------------------------------------------------
		//	Try to initialize FullHistTOff from class property
		cl_prop = ds_class->get_class_property(dev_prop[++i].name);
		if (cl_prop.is_empty()==false)	cl_prop  >>  fullHistTOff;
		else {
			def_prop = ds_class->get_default_device_property(dev_prop[i].name);
			if (def_prop.is_empty()==false)	def_prop  >>  fullHistTOff;
		}
		if (dev_prop[i].is_empty()==false)	dev_prop[i]  >>  fullHistTOff;                
                //      use hard-coded value if all of these options failed
                if (cl_prop.is_empty() && def_prop.is_empty() && dev_prop[i].is_empty()) {
                    fullHistTOff = "0";
                    std::cout << "Property FullHistTOff not found in database." << std::endl;
                    std::cout << "Setting it to default value " << fullHistTOff << std::endl;                    
                }
                dev_prop[i] << fullHistTOff;
                // ----------------------------------------------------------------
		//	Try to initialize FullHistTSize from class property
		cl_prop = ds_class->get_class_property(dev_prop[++i].name);
		if (cl_prop.is_empty()==false)	cl_prop  >>  fullHistTSizeStr;
		else {
			def_prop = ds_class->get_default_device_property(dev_prop[i].name);
			if (def_prop.is_empty()==false)	def_prop  >>  fullHistTSizeStr;
		}
		if (dev_prop[i].is_empty()==false)	dev_prop[i]  >>  fullHistTSizeStr;                
                //      use hard-coded value if all of these options failed
                if (cl_prop.is_empty() && def_prop.is_empty() && dev_prop[i].is_empty()) {
                    fullHistTSizeStr = "600";
                    std::cout << "Property FullHistTSize not found in database." << std::endl;
                    std::cout << "Setting it to default value " << fullHistTSizeStr << std::endl;                    
                }
                // validate FullHistTSize
                fullHistTSize = 600;
                try {
                    fullHistTSize = std::stoi(this->fullHistTSizeStr);
                }
                catch (std::invalid_argument exception) {
                    std::cout << "FullHistTSize property is set to an invalid value " << this->fullHistTSizeStr << std::endl;
                    std::cout << "using default value " << fullHistTSize << std::endl;
                }
                dev_prop[i] << fullHistTSizeStr;
                // ----------------------------------------------------------------
		//	Try to initialize FullHistTPGMPreviewWidth from class property
		cl_prop = ds_class->get_class_property(dev_prop[++i].name);
		if (cl_prop.is_empty()==false)	cl_prop  >>  fullHistTPGMPreviewWidthStr;
		else {
			def_prop = ds_class->get_default_device_property(dev_prop[i].name);
			if (def_prop.is_empty()==false)	def_prop  >>  fullHistTPGMPreviewWidthStr;
		}
		if (dev_prop[i].is_empty()==false)	dev_prop[i]  >>  fullHistTPGMPreviewWidthStr;                
                //      use hard-coded value if all of these options failed
                if (cl_prop.is_empty() && def_prop.is_empty() && dev_prop[i].is_empty()) {
                    fullHistTPGMPreviewWidthStr = "600";
                    std::cout << "Property FullHistTPGMPreviewWidth not found in database." << std::endl;
                    std::cout << "Setting it to default value " << fullHistTPGMPreviewWidthStr << std::endl;                    
                }
                // validate FullHistTPGMPreviewWidth
                int pgmwidth = 600; // default value
                try {
                    pgmwidth = std::stoi(this->fullHistTPGMPreviewWidthStr);
                }
                catch (std::invalid_argument exception) {
                    std::cout << "fullHistTPGMPreviewWidth property is set to an invalid value " << this->fullHistTPGMPreviewWidthStr << std::endl;
                    std::cout << "using default value " << pgmwidth << std::endl;
                }
                fullHistTPGMPreviewWidth = pgmwidth;
                dev_prop[i] << fullHistTPGMPreviewWidthStr;                
                // ----------------------------------------------------------------
		//	Try to initialize CSS_Support_Active from class property
		cl_prop = ds_class->get_class_property(dev_prop[++i].name);
		if (cl_prop.is_empty()==false)	cl_prop  >>  cssSupportActive;
		else {
			def_prop = ds_class->get_default_device_property(dev_prop[i].name);
			if (def_prop.is_empty()==false)	def_prop  >>  cssSupportActive;
		}
		if (dev_prop[i].is_empty()==false)	dev_prop[i]  >>  cssSupportActive;                
                //      use hard-coded value if all of these options failed
                if (cl_prop.is_empty() && def_prop.is_empty() && dev_prop[i].is_empty()) {
                    cssSupportActive = true;
                    std::cout << "Property CSS_Support_Active not found in database." << std::endl;
                    std::cout << "Setting it to default value " << cssSupportActive << std::endl;                    
                }
                dev_prop[i] << (cssSupportActive?"true":"false");
                // ----------------------------------------------------------------
                // ----------------------------------------------------------------
                // now write everything back to the database (workaround for bug in server wizard)
                write_device_properties(dev_prop);
                
	}

	/*----- PROTECTED REGION ID(SurfaceConceptTDC::get_device_property_after) ENABLED START -----*/
    
    //	Check device property data members init
    
    /*----- PROTECTED REGION END -----*/	//	SurfaceConceptTDC::get_device_property_after
}

void SurfaceConceptTDC::write_device_properties(Tango::DbData& dbdata) {
    if (Tango::Util::instance()->_UseDb==true)
            get_db_device()->put_property(dbdata);
}


//--------------------------------------------------------
/**
 *	Method      : SurfaceConceptTDC::always_executed_hook()
 *	Description : method always executed before any command is executed
 */
//--------------------------------------------------------
void SurfaceConceptTDC::always_executed_hook()
{
	//INFO_STREAM << "SurfaceConceptTDC::always_executed_hook()  " << device_name << endl;
	/*----- PROTECTED REGION ID(SurfaceConceptTDC::always_executed_hook) ENABLED START -----*/
    
    //	code always executed before all requests
    
    /*----- PROTECTED REGION END -----*/	//	SurfaceConceptTDC::always_executed_hook
}

//--------------------------------------------------------
/**
 *	Method      : SurfaceConceptTDC::read_attr_hardware()
 *	Description : Hardware acquisition for attributes
 */
//--------------------------------------------------------
void SurfaceConceptTDC::read_attr_hardware(TANGO_UNUSED(vector<long> &attr_list))
{
	//DEBUG_STREAM << "SurfaceConceptTDC::read_attr_hardware(vector<long> &attr_list) entering... " << endl;
	/*----- PROTECTED REGION ID(SurfaceConceptTDC::read_attr_hardware) ENABLED START -----*/
    
    //	Add your own code
    
    /*----- PROTECTED REGION END -----*/	//	SurfaceConceptTDC::read_attr_hardware
}
//--------------------------------------------------------
/**
 *	Method      : SurfaceConceptTDC::write_attr_hardware()
 *	Description : Hardware writing for attributes
 */
//--------------------------------------------------------
void SurfaceConceptTDC::write_attr_hardware(TANGO_UNUSED(vector<long> &attr_list))
{
	DEBUG_STREAM << "SurfaceConceptTDC::write_attr_hardware(vector<long> &attr_list) entering... " << endl;
	/*----- PROTECTED REGION ID(SurfaceConceptTDC::write_attr_hardware) ENABLED START -----*/
    
    //	Add your own code
    
    /*----- PROTECTED REGION END -----*/	//	SurfaceConceptTDC::write_attr_hardware
}

//--------------------------------------------------------
/**
 *	Read attribute DeviceID related method
 *	Description: 
 *
 *	Data type:	Tango::DevLong
 *	Attr type:	Scalar
 */
//--------------------------------------------------------
void SurfaceConceptTDC::read_DeviceID(Tango::Attribute &attr)
{
	DEBUG_STREAM << "SurfaceConceptTDC::read_DeviceID(Tango::Attribute &attr) entering... " << endl;
	/*----- PROTECTED REGION ID(SurfaceConceptTDC::read_DeviceID) ENABLED START -----*/
    //	Set the attribute value
    attr.set_value(attr_DeviceID_read);
    
    /*----- PROTECTED REGION END -----*/	//	SurfaceConceptTDC::read_DeviceID
}
//--------------------------------------------------------
/**
 *	Read attribute ExposureLive related method
 *	Description: 
 *
 *	Data type:	Tango::DevDouble
 *	Attr type:	Scalar
 */
//--------------------------------------------------------
void SurfaceConceptTDC::read_ExposureLive(Tango::Attribute &attr)
{
	DEBUG_STREAM << "SurfaceConceptTDC::read_ExposureLive(Tango::Attribute &attr) entering... " << endl;
	/*----- PROTECTED REGION ID(SurfaceConceptTDC::read_ExposureLive) ENABLED START -----*/
	//	Set the attribute value
	attr.set_value(attr_ExposureLive_read);
	
	/*----- PROTECTED REGION END -----*/	//	SurfaceConceptTDC::read_ExposureLive
}
//--------------------------------------------------------
/**
 *	Write attribute ExposureLive related method
 *	Description: 
 *
 *	Data type:	Tango::DevDouble
 *	Attr type:	Scalar
 */
//--------------------------------------------------------
void SurfaceConceptTDC::write_ExposureLive(Tango::WAttribute &attr)
{
	DEBUG_STREAM << "SurfaceConceptTDC::write_ExposureLive(Tango::WAttribute &attr) entering... " << endl;
	//	Retrieve write value
	Tango::DevDouble	w_val;
	attr.get_write_value(w_val);
	/*----- PROTECTED REGION ID(SurfaceConceptTDC::write_ExposureLive) ENABLED START -----*/
        m_exposure_live_ms = (long)(w_val*1000.0);
        *attr_ExposureLive_read = w_val;
	
	
	/*----- PROTECTED REGION END -----*/	//	SurfaceConceptTDC::write_ExposureLive
}
//--------------------------------------------------------
/**
 *	Read attribute ExposureAccu related method
 *	Description: 
 *
 *	Data type:	Tango::DevDouble
 *	Attr type:	Scalar
 */
//--------------------------------------------------------
void SurfaceConceptTDC::read_ExposureAccu(Tango::Attribute &attr)
{
	DEBUG_STREAM << "SurfaceConceptTDC::read_ExposureAccu(Tango::Attribute &attr) entering... " << endl;
	/*----- PROTECTED REGION ID(SurfaceConceptTDC::read_ExposureAccu) ENABLED START -----*/
	//	Set the attribute value
	attr.set_value(attr_ExposureAccu_read);
	
	/*----- PROTECTED REGION END -----*/	//	SurfaceConceptTDC::read_ExposureAccu
}
//--------------------------------------------------------
/**
 *	Write attribute ExposureAccu related method
 *	Description: 
 *
 *	Data type:	Tango::DevDouble
 *	Attr type:	Scalar
 */
//--------------------------------------------------------
void SurfaceConceptTDC::write_ExposureAccu(Tango::WAttribute &attr)
{
	DEBUG_STREAM << "SurfaceConceptTDC::write_ExposureAccu(Tango::WAttribute &attr) entering... " << endl;
	//	Retrieve write value
	Tango::DevDouble	w_val;
	attr.get_write_value(w_val);
	/*----- PROTECTED REGION ID(SurfaceConceptTDC::write_ExposureAccu) ENABLED START -----*/
        m_exposure_accu_ms = (long)(w_val*1000.0);
        *attr_ExposureAccu_read = w_val;
	
	
	/*----- PROTECTED REGION END -----*/	//	SurfaceConceptTDC::write_ExposureAccu
}

//--------------------------------------------------------
/**
 *	Method      : SurfaceConceptTDC::add_dynamic_attributes()
 *	Description : Create the dynamic attributes if any
 *                for specified device.
 */
//--------------------------------------------------------
void SurfaceConceptTDC::add_dynamic_attributes()
{
    /*----- PROTECTED REGION ID(SurfaceConceptTDC::add_dynamic_attributes) ENABLED START -----*/
    // all of the following are implemented in SurfaceConceptTDC_DynAttr.cpp
    AddHistogramAttributes();
    AddImagePreviewPollingAttribute();
    AddDiagnosticAttributes();
    AddAccuPreviewRefreshAttribute();
    AddAccumulatedTimeAttribute();
    AddSavingAttributes();
    AddCommandTriggerAttributes();
    AddMiscTDCAttributes();
    AddTDCStatisticsAttributes();
    AddSyncHistAttributes();
    AddImageAttributes();
    AddStatusAttributes();
    if (cssSupportActive) AddCSSAttributes();
    AddImageStatAttributes();
    AddHistUserTAuxAttributes();
    /*----- PROTECTED REGION END -----*/	//	SurfaceConceptTDC::add_dynamic_attributes
}

//--------------------------------------------------------
/**
 *	Command Start related method
 *	Description: call initialization function of the Surface concept TDC library
 *
 */
//--------------------------------------------------------
void SurfaceConceptTDC::start() // implements the command DeviceStart
{
    DEBUG_STREAM << "SurfaceConceptTDC::Start()  - " << device_name << endl;
    /*----- PROTECTED REGION ID(SurfaceConceptTDC::start) ENABLED START -----*/

    //	Add your own code
    if (m_TDC_id>=0) {
        INFO_STREAM << "SurfaceConceptTDC::start(): device has already been initialized!" << endl;
        return;
    }
    set_state(Tango::INIT);
    thread_pool.push([this](int id){this->startThreadAction();});

}

void SurfaceConceptTDC::startThreadAction() 
{
    int retval = 0;
    if (strlen(tdc_inifile_path_val)>0)
        retval = ::sc_tdc_init_inifile(tdc_inifile_path_val);
    else
        retval = ::sc_tdc_init_inifile(this->iniFilePath.c_str());
    INFO_STREAM << endl;
    if (retval>=0) {
        set_state(Tango::ON);
        memset(server_message_val, 0, STRING_BUF_SIZE-1);
        m_TDC_id = retval;
        *attr_DeviceID_read = m_TDC_id;
        // Registering Statistics Pipe
        stat_pipe.Register(m_TDC_id);
        // Activating Live Pipes!
        for (auto &hist : m_hist_map) {
            hist.second->SetDeviceDescriptor(m_TDC_id);
            // the following line means "if hist.first is not contained in pipeless_histograms"
            if (std::find(pipeless_histograms.begin(), pipeless_histograms.end(),hist.first)==pipeless_histograms.end())
            {
                hist.second->SetPipeActive(true);
            }
        }
        m_hist_map["Hist_Accu_XYT"]->SetPipeActive(false); // no accumulation at start up
        ::sc_tdc_set_complete_callback2(m_TDC_id, this, &SurfaceConceptTDC::StaticMeasurementCompleteCallback);
        INFO_STREAM << "SurfaceConceptTDC::start(): sc_tdc_init_inifile(...) " << TERMSUCCESFUL << ". Device ID " << retval << endl;
    }
    else {
        INFO_STREAM << "SurfaceConceptTDC::start(): sc_tdc_init_inifile(...) returned " << TERMERROR << endl;
        std::string errmsg = Helper::get_sc_err_message(retval);
        std::cout << errmsg << std::endl;
        strncpy(server_message_val, errmsg.c_str(), STRING_BUF_SIZE-1);
        set_state(Tango::OFF);
    }
    // get pixel sizes (x and y in mm, time in ns) from the library
    ::sc_DeviceProperties1 devprop;
    ::sc_tdc_get_device_properties(m_TDC_id, 1, &devprop);
    // devprop_pixel_size_t_val = devprop.pixel_size_t*1000.0; // display in picoseconds, seems to be the only non-zero value so far
    
    
    //devprop_pixel_size_t_val = devprop.pixel_size_t*1000.0; // display in picoseconds, seems to be the only non-zero value so far
    // use a fix value, workaround for scTDC
    devprop_pixel_size_t_val = 6.85871056241;             // display in picoseconds, seems to be the only non-zero value so far
    devprop_pixel_size_x_val = devprop.pixel_size_x*1000.0; // display in micrometers
    devprop_pixel_size_y_val = devprop.pixel_size_y*1000.0;
    devprop_detector_size_t_val = devprop.detector_size.time; // these detector size values seem to be missing altogether
    devprop_detector_size_x_val = devprop.detector_size.x;
    devprop_detector_size_y_val = devprop.detector_size.y;
    taxes_initialized = false;
    
    /*----- PROTECTED REGION END -----*/	//	SurfaceConceptTDC::start
}
//--------------------------------------------------------
/**
 *	Command Stop related method
 *	Description: call Deinitialization function of the Surface Concept TDC library
 *
 */
//--------------------------------------------------------
void SurfaceConceptTDC::stop() // implements the command DeviceStop
{
	DEBUG_STREAM << "SurfaceConceptTDC::Stop()  - " << device_name << endl;
	/*----- PROTECTED REGION ID(SurfaceConceptTDC::stop) ENABLED START -----*/
    
    //	Add your own code
    if (m_TDC_id>=0) {
        for (auto &hist : m_hist_map) {
            hist.second->SetPipeActive(false);
            hist.second->SetDeviceDescriptor(-1);
        }
        if (acquisition_running) {
            deferred_tdc_deinitialize_request = true;
            acquisition_stop();
            return;
        }
            
        int retval = ::sc_tdc_deinit2(m_TDC_id);
        if (retval==0) {
            set_state(Tango::OFF);
            m_TDC_id = -1;
            INFO_STREAM << "SurfaceConceptTDC::stop(): sc_tdc_deinit2(...) " << TERMSUCCESFUL << "." << endl;
        }
        else {
            set_state(Tango::FAULT);
            INFO_STREAM << "SurfaceConceptTDC::stop(): sc_tdc_deinit2(...) returned an " << TERMERROR << endl;
            Helper::cout_sc_err_message(retval);
        }
        if (deferred_write_commonshift_request) {
            _write_commonshift();
            start();
        }
    }
    else {
    	INFO_STREAM << "SurfaceConceptTDC::stop(): no device has been initialized." << endl;
    	return;
    }
    
    /*----- PROTECTED REGION END -----*/	//	SurfaceConceptTDC::stop
}
//--------------------------------------------------------
/**
 *	Command AcquisitionStart related method
 *	Description: Start exposure
 *
 */
//--------------------------------------------------------
void SurfaceConceptTDC::acquisition_start()
{
    DEBUG_STREAM << "SurfaceConceptTDC::AcquisitionStart()  - " << device_name << endl;
    /*----- PROTECTED REGION ID(SurfaceConceptTDC::acquisition_start) ENABLED START -----*/
    
    //	Add your own code
    user_acquisition_active = true;
    _acquisition_start();
    /*----- PROTECTED REGION END -----*/	//	SurfaceConceptTDC::acquisition_start
}
//--------------------------------------------------------
/**
 *	Command AcquisitonStop related method
 *	Description: Stop the exposure
 *
 */
//--------------------------------------------------------
void SurfaceConceptTDC::acquisition_stop()
{
    DEBUG_STREAM << "SurfaceConceptTDC::AcquisitonStop()  - " << device_name << endl;
    /*----- PROTECTED REGION ID(SurfaceConceptTDC::acquisiton_stop) ENABLED START -----*/
    
    //	Add your own code
    user_acquisition_active = false;
    _acquisition_stop();
    /*----- PROTECTED REGION END -----*/	//	SurfaceConceptTDC::acquisiton_stop
}

/*----- PROTECTED REGION ID(SurfaceConceptTDC::namespace_ending) ENABLED START -----*/

void SurfaceConceptTDC::_acquisition_start()
{
    //std::cout << "_acquisition_start() got called" << std::endl;
    int retval = ::sc_tdc_start_measure2(m_TDC_id, 1000); // 1 second
    if (retval!=0) {
        INFO_STREAM << "SurfaceConceptTDC::acquisition_start(): sc_tdc_start_measure2(...) returned an " << TERMERROR << endl;
        Helper::cout_sc_err_message(retval);
    }
    else {
        acquisition_running = true;
        acquisition_running_val = true;
        INFO_STREAM << "SurfaceConceptTDC::acquisition_start(): sc_tdc_start_measure2(...) " << TERMSUCCESFUL << "." << endl;
    }
}

void SurfaceConceptTDC::_acquisition_stop()
{
    int retval = ::sc_tdc_interrupt2(m_TDC_id);
    if (retval!=0) {
        INFO_STREAM << "SurfaceConceptTDC::acquisiton_stop(): sc_tdc_interrupt2(...) returned an " << TERMERROR << endl;
        Helper::cout_sc_err_message(retval);
    }
    else {
        INFO_STREAM << "SurfaceConceptTDC::acquisiton_stop(): sc_tdc_interrupt2(...) " << TERMSUCCESFUL << "." << endl;
    }
}


int SurfaceConceptTDC::m_allocator_cb(void *owner, void **ptr_to_mem) {
    cout << "m_allocator_cb got called!" << endl;
    *ptr_to_mem = (void*) ((SurfaceConceptTDC*)(owner))->m_databuf;
    return 0;
}

void SurfaceConceptTDC::Write_Own_DevLong64_Attribute(std::string attrname, Tango::DevLong64 w_val) {
    if (self_device_proxy==NULL) 
        self_device_proxy = new Tango::DeviceProxy(get_name());
    Tango::DeviceAttribute devattr(attrname.c_str(), w_val);
    self_device_proxy->write_attribute(devattr);
}

void SurfaceConceptTDC::Write_Own_DevLong_Attribute(std::string attrname, Tango::DevLong w_val) {
    if (self_device_proxy==NULL) 
        self_device_proxy = new Tango::DeviceProxy(get_name());
    Tango::DeviceAttribute devattr(attrname.c_str(), w_val);
    self_device_proxy->write_attribute(devattr);
}

/*----- PROTECTED REGION END -----*/	//	SurfaceConceptTDC::namespace_ending
} //	namespace
