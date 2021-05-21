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

#ifndef SurfaceConceptTDC_H
#define SurfaceConceptTDC_H

#include <queue>
#include <thread>
#include <mutex>
#include <atomic>

#include <scTDC.h>
#include <tango.h>

#include "CustomAttr.h"
#include "GeneralHistogram.h"
#include "TimedPeriodicCallThread.h"
#include "IniFileOperations.h"
#include "ctpl/ctpl_stl.h"
#include "StatPipe.h"
#include "SaveAfterAccumModes.h"

/*----- PROTECTED REGION END -----*/	//	SurfaceConceptTDC.h

/**
 *  SurfaceConceptTDC class description:
 *    
 */

namespace SurfaceConceptTDC_ns
{
/*----- PROTECTED REGION ID(SurfaceConceptTDC::Additional Class Declarations) ENABLED START -----*/

    class CustomAttr;
    class CustomSpectrumAttr;
    class CustomImageAttr;

/*----- PROTECTED REGION END -----*/	//	SurfaceConceptTDC::Additional Class Declarations

class SurfaceConceptTDC : public TANGO_BASE_CLASS
{

/*----- PROTECTED REGION ID(SurfaceConceptTDC::Data Members) ENABLED START -----*/

    static const int STRING_BUF_SIZE = 4096;
    
    Tango::DeviceProxy* self_device_proxy = NULL;
    
    int m_TDC_id = -1;
    std::map<std::string, CustomAttr*> m_dyn_attr_map;
    std::map<std::string, Tango::DevLong64> m_dyn_attr_long_vals;
    std::map<std::string, GeneralHistogram*> m_hist_map;
    std::vector<std::string> m_dyn_attr_names;
    
    long m_exposure_live_ms = 1000;   // live exposure in milliseconds
    long m_exposure_accu_ms = 10000;  // accumulation exposure in milliseconds
    ::sc_pipe_dld_image_xy_params_t *m_hist_xy_params = NULL;
    int m_hist_xy_pipe_id = -1;
    char *m_databuf = NULL;
    
    CustomAttr*             image_preview_polling_attr     = NULL;
    Tango::DevLong          image_preview_polling_val      = 50;
    long                    image_preview_last_timestamp   = 0;
    TimedPeriodicCallThread image_preview_update_thread;
    
    std::thread*            live_preview_refresh_thread    = NULL;
    bool                    live_preview_refresh_task_busy = false;

    
    CustomAttr*      accu_preview_refresh_attr      = NULL;
    Tango::DevLong   accu_preview_refresh_val       = 0;
    long             accu_preview_refresh_timestamp = 0;
    bool             accu_preview_refresh_task_busy      = false;
    std::thread*     accu_preview_refresh_thread         = NULL;
    
    CustomAttr*      accumulated_time_attr          = NULL;
    Tango::DevLong   accumulated_time_val           = 0;
    // accumulation_running = internal variable, accumulation_running_val=Tango attribute value
    bool             accumulation_running           = false;
    CustomAttr*      accumulation_running_attr      = NULL;
    Tango::DevBoolean accumulation_running_val      = false;
    CustomAttr*       acquisition_running_attr      = NULL;
    Tango::DevBoolean acquisition_running_val       = false;
    long             accumulation_last_time_stamp   = 0;
    long             accumulated_time_single        = 0;
    bool             deferred_accumulation_start_request = false;
    bool             deferred_accumulation_continue_request = false;
    bool             deferred_xyt_pipe_close_request = false;
    
    
    CustomAttr*      save_directory_attr            = NULL;
    Tango::DevString save_directory_wval            = NULL;
    Tango::DevString save_directory_rval            = NULL;    
    CustomAttr*      save_filename_attr             = NULL;
    Tango::DevString save_filename_val              = NULL;
    CustomAttr*      save_filecounter_attr          = NULL;
    Tango::DevLong   save_filecounter_val           = 0;
    bool             save_task_busy                 = false;
    std::thread*     save_thread                    = NULL;
    std::string      save_last_filename;
    
    // diagnostic attributes
    CustomAttr*      counts_per_sec_attr            = NULL;
    Tango::DevLong   counts_per_sec_val             = 0;
    long             counts_per_sec_last_timestamp  = 0;
    
    CustomAttr*      devprop_pixel_size_x_attr      = NULL;
    Tango::DevDouble devprop_pixel_size_x_val       = 1.0;
    CustomAttr*      devprop_pixel_size_y_attr      = NULL;
    Tango::DevDouble devprop_pixel_size_y_val       = 1.0;
    CustomAttr*      devprop_pixel_size_t_attr      = NULL;
    Tango::DevDouble devprop_pixel_size_t_val       = 1.0;
    CustomAttr*      devprop_detector_size_x_attr   = NULL;
    Tango::DevLong   devprop_detector_size_x_val    = 0;
    CustomAttr*      devprop_detector_size_y_attr   = NULL;
    Tango::DevLong   devprop_detector_size_y_val    = 0;    
    CustomAttr*      devprop_detector_size_t_attr   = NULL;
    Tango::DevLong64 devprop_detector_size_t_val    = 0;  
    
    CustomAttr*       server_databufs_reservedmem_attr = NULL;
    Tango::DevLong64  server_databufs_reservedmem_val  = 0;
    CustomAttr*       server_accu_int_duration_attr    = NULL;
    Tango::DevLong    server_accu_int_duration_val     = 0;
    CustomAttr*       info_accu_xyt_bytesize_attr      = NULL;
    Tango::DevLong64  info_accu_xyt_bytesize_val       = 0;
    CustomAttr*       info_accu_xyt_formattedsize_attr = NULL;
    Tango::DevString  info_accu_xyt_formattedsize_val  = NULL;
    CustomAttr*       server_save_file_busy_attr       = NULL;
    Tango::DevBoolean server_save_file_busy_val        = false;
    CustomAttr*       status_xyt_saved_attr          = NULL;
    Tango::DevBoolean status_xyt_saved_val           = false;
    CustomAttr*       status_xy_saved_attr           = NULL;
    Tango::DevBoolean status_xy_saved_val            = false;
    CustomAttr*       status_spectrum_saved_attr     = NULL;
    Tango::DevBoolean status_spectrum_saved_val      = false;
    CustomAttr*       status_spectrum2_saved_attr    = NULL;
    Tango::DevBoolean status_spectrum2_saved_val     = false;
    
    std::vector<std::string> pipeless_histograms;
    
    bool              acquisition_running            = false;
    bool              user_acquisition_active        = false; // set to true/false according to user commands AcquisitionStart/Stop
    bool              user_accumulation_active       = false; // set to true/false according to user commands AccumulationStart/Stop
    std::queue< std::tuple < std::string, Tango::DevLong64 > > deferred_writes;
    
    CustomAttr*      cmd_trig_accumulation_start_attr    = NULL;
    CustomAttr*      cmd_trig_accumulation_continue_attr = NULL;
    CustomAttr*      cmd_trig_accumulation_stop_attr     = NULL;
    CustomAttr*      cmd_trig_accumulation_start_save_attr = NULL;
    CustomAttr*      cmd_trig_acquisition_start_attr     = NULL;
    CustomAttr*      cmd_trig_acquisition_stop_attr      = NULL;
    CustomAttr*      cmd_trig_device_start_attr          = NULL;
    CustomAttr*      cmd_trig_device_stop_attr           = NULL;
    CustomAttr*      cmd_trig_save_tiff_xyt_attr         = NULL;
    CustomAttr*      cmd_trig_save_thist_accu_attr       = NULL;
    CustomAttr*      cmd_trig_shrink_databufs_attr       = NULL;
    CustomAttr*      cmd_trig_save_thist_user_accu_attr  = NULL;
    CustomAttr*      cmd_trig_general                    = NULL;
    Tango::DevLong   cmd_trig_dummy_val                  = 0;
    Tango::DevString cmd_trig_general_val                = NULL;
    std::atomic_bool request_save_after_accumulation     = {false};
    std::atomic_int  save_after_accumulation_mode        = {SAVE_AFTER_ACCUM_MODE_XYT};

    CustomAttr*      tdc_common_shift_attr              = NULL;
    Tango::DevLong64 tdc_common_shift_val               = 0;
    bool             deferred_tdc_deinitialize_request  = false;
    bool             deferred_write_commonshift_request = false;
    Tango::DevString tdc_inifile_path_val               = NULL;
    CustomAttr*      tdc_inifile_path_attr              = NULL;
    Tango::DevString server_message_val                 = NULL;
    CustomAttr*      server_message_attr                = NULL;
    
    IniFileOperations inifileOperations;
    
    bool                tdc_statistics_active          = false; // set to true after device has been initialized
    static const int    tdc_stat_counts_read_size      = 64;
    CustomSpectrumAttr* tdc_stat_counts_read_attr      = NULL;
    Tango::DevLong*     tdc_stat_counts_read_val       = NULL;
    static const int    tdc_stat_counts_received_size  = 64;
    CustomSpectrumAttr* tdc_stat_counts_received_attr  = NULL;
    Tango::DevLong*     tdc_stat_counts_received_val   = NULL;
    static const int    tdc_stat_events_found_size     = 4;
    CustomSpectrumAttr* tdc_stat_events_found_attr     = NULL;
    Tango::DevLong*     tdc_stat_events_found_val      = NULL;
    static const int    tdc_stat_events_in_roi_size    = 4;
    CustomSpectrumAttr* tdc_stat_events_in_roi_attr    = NULL;
    Tango::DevLong*     tdc_stat_events_in_roi_val     = NULL;
    static const int    tdc_stat_events_received_size  = 4;
    CustomSpectrumAttr* tdc_stat_events_received_attr  = NULL;
    Tango::DevLong*     tdc_stat_events_received_val   = NULL;
    static const int    tdc_stat_counters_size         = 64;
    CustomSpectrumAttr* tdc_stat_counters_attr         = NULL;
    Tango::DevLong*     tdc_stat_counters_val          = NULL;
    static const int    tdc_stat_summarized_size       = 9;
    CustomSpectrumAttr* tdc_stat_summarized_attr       = NULL;
    Tango::DevLong*     tdc_stat_summarized_val        = NULL;
    CustomAttr*         tdc_stat_max_raw_count_attr    = NULL;
    Tango::DevLong      tdc_stat_max_raw_count_val     = 0;
    CustomAttr*         tdc_stat_avg_raw_count_attr    = NULL;
    Tango::DevLong      tdc_stat_avg_raw_count_val     = 0;
    ::statistics_t      _tdc_statistics_sc; // structure of surface concept library
    
    CustomSpectrumAttr* hist_full_t_attr               = NULL;
    CustomSpectrumAttr* hist_accu_full_t_attr          = NULL;
    CustomSpectrumAttr* hist_accu_t_attr               = NULL;
    CustomSpectrumAttr* hist_live_t_attr               = NULL;
    CustomSpectrumAttr* hist_live_user_t_attr          = NULL;
    CustomSpectrumAttr* hist_accu_user_t_attr          = NULL;
    CustomSpectrumAttr* hist_full_taxis_attr           = NULL;
    CustomSpectrumAttr* hist_live_taxis_attr           = NULL;
    CustomSpectrumAttr* hist_accu_taxis_attr           = NULL;
    CustomSpectrumAttr* hist_user_taxis_attr           = NULL;
    
    CustomAttr*         hist_taxis_unit_attr           = NULL;
    Tango::DevString    hist_taxis_unit_val            = nullptr;
    int                 hist_taxis_unit_internal       = 0;
    static const int    hist_accu_t_max_size           = 4194304; // 2^22 apprx 4 mega
    bool                taxes_initialized              = false;
    
    CustomImageAttr*    hist_live_xy_attr              = NULL;
    CustomImageAttr*    hist_live_xt_attr              = NULL;
    CustomImageAttr*    hist_live_yt_attr              = NULL;
    CustomImageAttr*    hist_accu_xy_attr              = NULL;
    CustomImageAttr*    hist_accu_xt_attr              = NULL;
    CustomImageAttr*    hist_accu_yt_attr              = NULL;
    static const int    hist_images_max_xsize          = 2048;
    static const int    hist_images_max_ysize          = 2048;
    static const int    hist_images_max_tsize          = 2048;
    
    // attributes for selection of ROI, BINNING and INTEGRATION RANGES
    // these are forwarded to 3 live histograms (XY,XT,YT), 3 accumulation histograms (XY,XT,YT),
    // and to the 3D accumulated data cube
    CustomAttr*         sync_hist_bin_x_attr           = NULL;
    CustomAttr*         sync_hist_bin_y_attr           = NULL;
    CustomAttr*         sync_hist_bin_t_attr           = NULL;
    CustomAttr*         sync_hist_roi_x1_attr          = NULL;
    CustomAttr*         sync_hist_roi_x2_attr          = NULL;
    CustomAttr*         sync_hist_roi_y1_attr          = NULL;
    CustomAttr*         sync_hist_roi_y2_attr          = NULL;
    CustomAttr*         sync_hist_roi_t1_attr          = NULL;
    CustomAttr*         sync_hist_roi_t2_attr          = NULL;
    CustomAttr*         sync_hist_roi_toff_attr        = NULL;
    CustomAttr*         sync_hist_roi_tsize_attr       = NULL;
    CustomAttr*         sync_hist_int_x1_attr          = NULL;
    CustomAttr*         sync_hist_int_x2_attr          = NULL;
    CustomAttr*         sync_hist_int_y1_attr          = NULL;
    CustomAttr*         sync_hist_int_y2_attr          = NULL;
    CustomAttr*         sync_hist_int_t1_attr          = NULL;
    CustomAttr*         sync_hist_int_t2_attr          = NULL;
    CustomAttr*         sync_hist_modulo_attr          = NULL;
    CustomAttr*         sync_hist_raw_toff_attr        = NULL;
    CustomAttr*         sync_hist_raw_toff_fixed_attr = NULL;

    Tango::DevLong64      sync_hist_bin_x_val          = 4L;
    Tango::DevLong64      sync_hist_bin_y_val          = 4L;
    Tango::DevLong64      sync_hist_bin_t_val          = 256L;
    Tango::DevLong64      sync_hist_roi_x1_val         = 0L;
    Tango::DevLong64      sync_hist_roi_x2_val         = 511L;
    Tango::DevLong64      sync_hist_roi_y1_val         = 0L;
    Tango::DevLong64      sync_hist_roi_y2_val         = 511L;
    Tango::DevLong64      sync_hist_roi_t1_val         = 0L;
    Tango::DevLong64      sync_hist_roi_t2_val         = 511L;
    Tango::DevLong64      sync_hist_roi_toff_val       = 0L;
    Tango::DevLong64      sync_hist_roi_tsize_val      = 512L;
    Tango::DevLong64      sync_hist_int_x1_val         = 0L;
    Tango::DevLong64      sync_hist_int_x2_val         = 511L;
    Tango::DevLong64      sync_hist_int_y1_val         = 0L;
    Tango::DevLong64      sync_hist_int_y2_val         = 511L;
    Tango::DevLong64      sync_hist_int_t1_val         = 0L;
    Tango::DevLong64      sync_hist_int_t2_val         = 511L;
    Tango::DevLong64      sync_hist_modulo_val         = 511L;
    Tango::DevLong64      sync_hist_raw_toff_val       = 0L;
    Tango::DevBoolean     sync_hist_raw_toff_fixed_val = true;
    
    Tango::DevLong64      hist_user_t_modbinned_val    = 0;
    CustomAttr*           hist_user_t_modbinned_attr   = NULL;
    Tango::DevLong64      hist_user_t_toff_raw_val     = 0;
    CustomAttr*           hist_user_t_toff_raw_attr    = NULL;
    
    
    std::map<std::string, CustomAttr*> css_attrs;
    std::map<std::string, Tango::DevDouble> css_values;
    
    std::map<std::string, Tango::DevLong> imagestat_vals;
    std::map<std::string, CustomAttr*> imagestat_attrs;
    
    std::mutex          accu_buffers_mutex;
    std::mutex          save_task_busy_mutex;
    
    ctpl::thread_pool   thread_pool;
    
    StatPipe            stat_pipe;

/*----- PROTECTED REGION END -----*/	//	SurfaceConceptTDC::Data Members

//	Device property data members
public:
	//	IniFilePath:	      Path and Filename of the Surface Concept TDC .ini file
	string	          iniFilePath;
	//	DefaultLiveOut:	      Default Target for live output in file mode
	string	          livePreviewModeFilePath;
        // livePreviewModeFileActive: whether file output is active
        Tango::DevBoolean livePreviewModeFileActive;
        Tango::DevBoolean livePreviewModeTangoActive;
        Tango::DevBoolean cssSupportActive;
        string  fullHistBinXY;
        string  fullHistBinT;
        string  fullHistTOff;
        string  fullHistTSizeStr;
        int     fullHistTSize;
        string  fullHistTPGMPreviewWidthStr;
        int     fullHistTPGMPreviewWidth;
        string saveBaseDir;

//	Attribute data members
public:
	Tango::DevLong	     *attr_DeviceID_read;
	Tango::DevDouble     *attr_ExposureLive_read;
	Tango::DevDouble     *attr_ExposureAccu_read;

//	Constructors and destructors
public:
	/**
	 * Constructs a newly device object.
	 *
	 *	@param cl	Class.
	 *	@param s 	Device Name
	 */
	SurfaceConceptTDC(Tango::DeviceClass *cl,string &s);
	/**
	 * Constructs a newly device object.
	 *
	 *	@param cl	Class.
	 *	@param s 	Device Name
	 */
	SurfaceConceptTDC(Tango::DeviceClass *cl,const char *s);
	/**
	 * Constructs a newly device object.
	 *
	 *	@param cl	Class.
	 *	@param s 	Device name
	 *	@param d	Device description.
	 */
	SurfaceConceptTDC(Tango::DeviceClass *cl,const char *s,const char *d);
	/**
	 * The device object destructor.
	 */	
	~SurfaceConceptTDC() {delete_device();};


//	Miscellaneous methods
public:
	/*
	 *	will be called at device destruction or at init command.
	 */
	void delete_device();
	/*
	 *	Initialize the device
	 */
	virtual void init_device();
	/*
	 *	Read the device properties from database
	 */
	void get_device_property();
        void write_device_properties(Tango::DbData& dbdata); // NOT FROM POGO!
	/*
	 *	Always executed method before execution command method.
	 */
	virtual void always_executed_hook();


//	Attribute methods
public:
	//--------------------------------------------------------
	/*
	 *	Method      : SurfaceConceptTDC::read_attr_hardware()
	 *	Description : Hardware acquisition for attributes.
	 */
	//--------------------------------------------------------
	virtual void read_attr_hardware(vector<long> &attr_list);
	//--------------------------------------------------------
	/*
	 *	Method      : SurfaceConceptTDC::write_attr_hardware()
	 *	Description : Hardware writing for attributes.
	 */
	//--------------------------------------------------------
	virtual void write_attr_hardware(vector<long> &attr_list);

/**
 *	Attribute DeviceID related methods
 *	Description: 
 *
 *	Data type:	Tango::DevLong
 *	Attr type:	Scalar
 */
	virtual void read_DeviceID(Tango::Attribute &attr);
	virtual bool is_DeviceID_allowed(Tango::AttReqType type);
/**
 *	Attribute ExposureLive related methods
 *	Description: 
 *
 *	Data type:	Tango::DevDouble
 *	Attr type:	Scalar
 */
	virtual void read_ExposureLive(Tango::Attribute &attr);
	virtual void write_ExposureLive(Tango::WAttribute &attr);
	virtual bool is_ExposureLive_allowed(Tango::AttReqType type);
/**
 *	Attribute ExposureAccu related methods
 *	Description: 
 *
 *	Data type:	Tango::DevDouble
 *	Attr type:	Scalar
 */
	virtual void read_ExposureAccu(Tango::Attribute &attr);
	virtual void write_ExposureAccu(Tango::WAttribute &attr);
	virtual bool is_ExposureAccu_allowed(Tango::AttReqType type);


	//--------------------------------------------------------
	/**
	 *	Method      : SurfaceConceptTDC::add_dynamic_attributes()
	 *	Description : Add dynamic attributes if any.
	 */
	//--------------------------------------------------------
	void add_dynamic_attributes();



//	Command related methods
public:
	/**
	 *	Command Start related method
	 *	Description: call initialization function of the Surface concept TDC library
	 *
	 */
	virtual void start();
	virtual bool is_Start_allowed(const CORBA::Any &any);
        void startThreadAction();
	/**
	 *	Command Stop related method
	 *	Description: call Deinitialization function of the Surface Concept TDC library
	 *
	 */
	virtual void stop();
	virtual bool is_Stop_allowed(const CORBA::Any &any);
	/**
	 *	Command AcquisitionStart related method
	 *	Description: Start exposure
	 *
	 */
	virtual void acquisition_start();
	virtual bool is_AcquisitionStart_allowed(const CORBA::Any &any);
	/**
	 *	Command AcquisitonStop related method
	 *	Description: Stop the exposure
	 *
	 */
	virtual void acquisition_stop();
	virtual bool is_AcquisitonStop_allowed(const CORBA::Any &any);


/*----- PROTECTED REGION ID(SurfaceConceptTDC::Additional Method prototypes) ENABLED START -----*/
public:
        virtual void save_xyt_to_tiff();
        virtual bool is_SaveXytToTiff_allowed(const CORBA::Any &any);
        virtual void save_xy_to_tiff();
        virtual bool is_SaveXyToTiff_allowed(const CORBA::Any &any);
        virtual void save_xy_to_text();
        virtual bool is_SaveXyToText_allowed(const CORBA::Any &any);
        virtual void accumulation_start();
        virtual bool is_AccumulationStart_allowed(const CORBA::Any &any);
        virtual void accumulation_continue();
        virtual bool is_AccumulationContinue_allowed(const CORBA::Any &any);
        virtual void accumulation_stop();
        virtual bool is_AccumulationStop_allowed(const CORBA::Any &any);
        virtual void accum_start_and_save_xy();
        virtual bool is_AccumStartAndSaveXY_allowed(const CORBA::Any &any);
        virtual void accum_start_and_save_xy_text();
        virtual bool is_AccumStartAndSaveXYText_allowed(const CORBA::Any &any);        
        virtual void save_thist_accu();
        virtual void save_thist_user_accu();
        bool SaveSpectrum(GeneralHistogram& hist, const std::string path, const std::string filename, bool from_tango_accu_buf);
        virtual void shrink_databufs();
        

private:
    static int	m_allocator_cb(void *owner, void **ptr_to_mem);
    
    void Write_Own_DevLong64_Attribute(std::string attrname, Tango::DevLong64 wval);
    void Write_Own_DevLong_Attribute(std::string attrname, Tango::DevLong wval);

    void _acquisition_start();
    void _acquisition_stop(); 
    
    void AddHistogramAttributes();
    void AddImageAttributes();
    void SetFullRangeHistogramProperties();
    void HistogramAttributeReadCallback(Tango::DeviceImpl *, Tango::Attribute &); 
    void HistogramAttributeWriteCallback(Tango::DeviceImpl *, Tango::WAttribute &);
    void HistogramAttributeWriteCallbackAction(const std::string attrname, Tango::DevLong64 w_val);
    void SetHistogramAttrLinked(const std::string hist, const std::string attr, long value);
    void SetHistogramAttribute(const std::string histname, const std::string histattrname, long w_val);
    void _SetHistogramAttribute(const std::string histname, const std::string histattrname, long w_val);
    void HistogramAttributeWriteCallback_Hist_User_T_Links(std::string attname, long w_val);
    
    void AddImagePreviewPollingAttribute();
    void ImagePreviewPollingReadCallback(Tango::DeviceImpl *, Tango::Attribute &);
    void ImagePreviewPollingWriteCallback(Tango::DeviceImpl *, Tango::WAttribute &);
    void ImagePreviewUpdateAction();
    static void StaticImagePreviewUpdateAction(void* Object);

    void LiveImageTriggerAction();
    void LiveImageTriggerThreadedAction();
    void LiveImageTriggerThreadedAction_Hist_User_T();
    void LiveImageTriggerThreadedAction_ImageStat();
    
    static void StaticLiveImageTriggerThreadedAction(void* Object);

    void AddDiagnosticAttributes();
    void DiagnosticAttributeReadCallback(Tango::DeviceImpl *, Tango::Attribute &);
    void DevPropReadCallback(Tango::DeviceImpl *, Tango::Attribute &);
    void Update_Info_Accu_XYT_Size();
    
    void AddAccuPreviewRefreshAttribute();
    void AccuPreviewRefreshReadCallback(Tango::DeviceImpl *, Tango::Attribute &);
    void AccuPreviewRefreshWriteCallback(Tango::DeviceImpl *, Tango::WAttribute &);
    void AccuPreviewRefreshAction();
    void AccuPreviewRefreshThreadedAction();
    void AccuPreviewRefreshThreadedAction_ImageStat();
    static void StaticAccuPreviewRefreshThreadedAction(void* Object);
    
    void AddAccumulatedTimeAttribute();
    void AccumulatedTimeReadCallback(Tango::DeviceImpl *, Tango::Attribute &);
    void AccumulatedTimeIncrementAction();
    
    void AddSavingAttributes();
    void SaveDirectoryReadCallback(Tango::DeviceImpl *, Tango::Attribute &);
    void SaveDirectoryWriteCallback(Tango::DeviceImpl *, Tango::WAttribute &);
    void SaveFilenameReadCallback(Tango::DeviceImpl *, Tango::Attribute &);
    void SaveFilenameWriteCallback(Tango::DeviceImpl *, Tango::WAttribute &);
    void SaveFilecounterReadCallback(Tango::DeviceImpl *, Tango::Attribute &);
    void SaveFilecounterWriteCallback(Tango::DeviceImpl *, Tango::WAttribute &);
    void SaveThreadedAction();
    void SaveXYThreadedAction();
    void SaveXYTextThreadedAction();
    void SaveMeasurementInformation(std::string fullpath);
    static void StaticSaveThreadedAction(void* Object);
    static void StaticSaveXYThreadedAction(void* Object);
    static void StaticSaveXYTextThreadedAction(void* Object);
    void _update_filecounter_and_save_states();
    
    void CountsPerSecUpdate();
    
    void MeasurementCompleteCallback(int i);
    static void StaticMeasurementCompleteCallback(void *p, int i);
    
    void AddCommandTriggerAttributes();
    void CommandTriggerReadCallback(Tango::DeviceImpl *, Tango::Attribute &);
    void CommandTriggerWriteCallback(Tango::DeviceImpl *, Tango::WAttribute &);
    void CommandTriggerGeneralReadCallback(Tango::DeviceImpl *, Tango::Attribute &);
    void CommandTriggerGeneralWriteCallback(Tango::DeviceImpl *, Tango::WAttribute &);
    
    void AddMiscTDCAttributes();
    void MiscTDCReadCallback(Tango::DeviceImpl *, Tango::Attribute &);
    void MiscTDCWriteCallback(Tango::DeviceImpl *, Tango::WAttribute &);
    void _write_commonshift();
    
    void AddTDCStatisticsAttributes();
    void TDCStatisticsReadCallback(Tango::DeviceImpl *, Tango::Attribute &);
    void _read_tdc_statistics();
    void _zero_tdc_statistics();
    
    void Hist_Full_T_ReadCallback(Tango::DeviceImpl *, Tango::Attribute &);
    void Hist_Full_Accu_T_ReadCallback(Tango::DeviceImpl *, Tango::Attribute &);
    void Hist_Live_T_ReadCallback(Tango::DeviceImpl *, Tango::Attribute &);
    void Hist_Accu_T_ReadCallback(Tango::DeviceImpl *, Tango::Attribute &);
    void Hist_Live_User_T_ReadCallback(Tango::DeviceImpl *, Tango::Attribute &);
    void Hist_Accu_User_T_ReadCallback(Tango::DeviceImpl *, Tango::Attribute &);
    void Hist_Full_TAxis_ReadCallback(Tango::DeviceImpl *, Tango::Attribute &);
    void Hist_Live_TAxis_ReadCallback(Tango::DeviceImpl *, Tango::Attribute &);
    void Hist_Accu_TAxis_ReadCallback(Tango::DeviceImpl *, Tango::Attribute &);
    void Hist_User_Taxis_ReadCallback(Tango::DeviceImpl *, Tango::Attribute &);
    
    
    void AddSyncHistAttributes();
    void Sync_Hist_ReadCallback(Tango::DeviceImpl *, Tango::Attribute &);
    void Sync_Hist_WriteCallback(Tango::DeviceImpl *, Tango::WAttribute &);
    
    void Hist_Live_Accu_Image_ReadCallback(Tango::DeviceImpl *, Tango::Attribute &);
    
    void AddCSSAttributes();
    void CSS_ReadCallback(Tango::DeviceImpl *, Tango::Attribute &);
    void CSS_WriteCallback(Tango::DeviceImpl *, Tango::WAttribute &);
    
    void AddStatusAttributes();
    void Status_ReadCallback(Tango::DeviceImpl *, Tango::Attribute &);
    
    void AddImageStatAttributes();
    void ImageStat_ReadCallback(Tango::DeviceImpl *, Tango::Attribute &);
    
    void AddHistUserTAuxAttributes();
    void HistUserTAux_ReadCallback(Tango::DeviceImpl *, Tango::Attribute &);
    void HistUserTAux_WriteCallback(Tango::DeviceImpl *, Tango::WAttribute &);
    
/*----- PROTECTED REGION END -----*/	//	SurfaceConceptTDC::Additional Method prototypes
};

/*----- PROTECTED REGION ID(SurfaceConceptTDC::Additional Classes Definitions) ENABLED START -----*/

//	Additional Classes Definitions

/*----- PROTECTED REGION END -----*/	//	SurfaceConceptTDC::Additional Classes Definitions

}	//	End of namespace

#endif   //	SurfaceConceptTDC_H
