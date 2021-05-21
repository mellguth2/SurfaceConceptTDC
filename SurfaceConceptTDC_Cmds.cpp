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
// This file contains implementations of our own commands,
// i.e. commands which were not created by POGO

#include "SurfaceConceptTDC.h"
#include "SaveXYTtoTiff.h"
#include "SaveXYtoText.h"
#include "Helper.h"
#include <sstream>
#include <iomanip>
#include <mutex>

namespace SurfaceConceptTDC_ns {

    void SurfaceConceptTDC::save_xyt_to_tiff()
    {
        //std::cout << "save_xyt_to_tiff() got called" << std::endl;
        if (m_hist_map.at("Hist_Accu_XYT")->GetDatabufPointer()==NULL)
            return;
        
        { // lock_guard scope
            std::lock_guard<std::mutex> lock(save_task_busy_mutex);
            if (save_task_busy) {
                std::cout << "save_xyt_to_tiff(): still busy with previous save command" << std::endl;
                return; 
            }
            save_task_busy = true;
            server_save_file_busy_val = true;
        }
        
        if (save_thread!=NULL) {
            delete save_thread;
            save_thread = NULL;
        }
        save_thread = new std::thread(&SurfaceConceptTDC::StaticSaveThreadedAction, this);
        save_thread->detach();
    }
    
    bool SurfaceConceptTDC::is_SaveXytToTiff_allowed(const CORBA::Any &any) {
        if (m_hist_map.at("Hist_Accu_XYT")->GetDatabufPointer()!=NULL)
            return true;
        else 
            return false;
    }
    
    void SurfaceConceptTDC::save_xy_to_tiff()
    {
        if (m_hist_map.at("Hist_Accu_XY")->GetDatabufPointer()==NULL)
            return;
        
        { // lock_guard scope
            std::lock_guard<std::mutex> lock(save_task_busy_mutex);
            if (save_task_busy) {
                std::cout << "save_xy_to_tiff(): still busy with previous save command" << std::endl;
                return; 
            }
            save_task_busy = true;
            server_save_file_busy_val = true;
        }
        
        if (save_thread!=NULL) {
            delete save_thread;
            save_thread = NULL;
        }
        save_thread = new std::thread(&SurfaceConceptTDC::StaticSaveXYThreadedAction, this);
        save_thread->detach();    
    }    
    
    bool SurfaceConceptTDC::is_SaveXyToTiff_allowed(const CORBA::Any &any) {
        if (m_hist_map.at("Hist_Accu_XY")->GetDatabufPointer()!=NULL)
            return true;
        else 
            return false;
    }    

    void SurfaceConceptTDC::accum_start_and_save_xy() {
        save_after_accumulation_mode.store(SAVE_AFTER_ACCUM_MODE_XY);
        request_save_after_accumulation.store(true);
        accumulation_start();
    }

    void SurfaceConceptTDC::accum_start_and_save_xy_text() {
        save_after_accumulation_mode.store(SAVE_AFTER_ACCUM_MODE_XY_TEXT);
        request_save_after_accumulation.store(true);
        accumulation_start();
    }
    

    
    void SurfaceConceptTDC::save_xy_to_text()
    {
        if (m_hist_map.at("Hist_Accu_XY")->GetDatabufPointer()==NULL)
            return;
        
        { // lock_guard scope
            std::lock_guard<std::mutex> lock(save_task_busy_mutex);
            if (save_task_busy) {
                std::cout << "save_xy_to_text(): still busy with previous save command" << std::endl;
                return; 
            }
            save_task_busy = true;
            server_save_file_busy_val = true;
        }
        
        if (save_thread!=NULL) {
            delete save_thread;
            save_thread = NULL;
        }
        save_thread = new std::thread(&SurfaceConceptTDC::StaticSaveXYTextThreadedAction, this);
        save_thread->detach();    
    }    
    
    bool SurfaceConceptTDC::is_SaveXyToText_allowed(const CORBA::Any &any) {
        if (m_hist_map.at("Hist_Accu_XY")->GetDatabufPointer()!=NULL)
            return true;
        else 
            return false;
    }    

    void SurfaceConceptTDC::SaveThreadedAction() {
        long start = Helper::get_millisec();
        std::string filename(save_filename_val);
        std::replace(filename.begin(), filename.end(), ' ', '_');
        std::string directory(save_directory_rval);
        directory = Helper::join_pathnames(directory, Helper::get_date_string("_"));
        if (!Helper::ensure_directory_exists(directory)) {
            save_task_busy = false;
            server_save_file_busy_val = false;
            return;
        }
        std::ostringstream oss;
        oss << std::setfill('0') << std::setw(3) << save_filecounter_val << "_" << filename;
        std::string filename_w_ctr = oss.str();
        filename_w_ctr = Helper::ensure_extension(filename_w_ctr, ".tif");
        if (SaveXYTtoTiff(*m_hist_map.at("Hist_Accu_XYT"), directory, filename_w_ctr)) {
            long end = Helper::get_millisec();
            std::cout << "Saved TIFF to file " << filename_w_ctr << " in " << end-start << " milliseconds." << std::endl;
            save_last_filename = filename_w_ctr;
            // write measurement info
            std::string infofilename = filename_w_ctr.substr(0, filename_w_ctr.length()-4)+"_info.txt";
            std::string infofilepath = Helper::join_pathnames(directory, infofilename);
            status_xyt_saved_val = true;
            SaveMeasurementInformation(infofilepath);
            std::string msg("Dataset saved to ");
            msg = msg + directory + "/" + filename_w_ctr;
            strncpy(server_message_val, msg.c_str(), STRING_BUF_SIZE-1);
        } else {
            strncpy(server_message_val, "Error: Failed attempt to save the dataset to a TIFF file", STRING_BUF_SIZE-1);
        }
        // 
        save_task_busy = false;
        server_save_file_busy_val = false;
    }

    void SurfaceConceptTDC::StaticSaveThreadedAction(void* Object) {
        ((SurfaceConceptTDC*) Object)->SaveThreadedAction();
    }

    void SurfaceConceptTDC::StaticSaveXYThreadedAction(void* Object) {
        static_cast<SurfaceConceptTDC*>(Object)->SaveXYThreadedAction();
    }
    
    void SurfaceConceptTDC::StaticSaveXYTextThreadedAction(void* Object) {
        static_cast<SurfaceConceptTDC*>(Object)->SaveXYTextThreadedAction();
    }
    

    void SurfaceConceptTDC::SaveXYThreadedAction() {
        long start = Helper::get_millisec();
        std::string filename(save_filename_val);
        std::replace(filename.begin(), filename.end(), ' ', '_');
        std::string directory(save_directory_rval);
        directory = Helper::join_pathnames(directory, Helper::get_date_string("_"));
        if (!Helper::ensure_directory_exists(directory)) {
            save_task_busy = false;
            server_save_file_busy_val = false;
            return;
        }
        std::ostringstream oss;
        oss << std::setfill('0') << std::setw(3) << save_filecounter_val << "_" << filename;
        std::string filename_w_ctr = oss.str();
        filename_w_ctr = Helper::ensure_extension(filename_w_ctr, "_XY.tif");
        if (SaveXYtoTiff(*m_hist_map.at("Hist_Accu_XY"), directory, filename_w_ctr)) {
            long end = Helper::get_millisec();
            std::cout << "Saved TIFF to file " << filename_w_ctr << " in " << end-start << " milliseconds." << std::endl;
            save_last_filename = filename_w_ctr;
            // write measurement info
            std::string infofilename = filename_w_ctr.substr(0, filename_w_ctr.length()-4)+"_info.txt";
            std::string infofilepath = Helper::join_pathnames(directory, infofilename);
            status_xy_saved_val = true;
            SaveMeasurementInformation(infofilepath);
            std::string msg("XY image saved to ");
            msg = msg + directory + "/" + filename_w_ctr;
            strncpy(server_message_val, msg.c_str(), STRING_BUF_SIZE-1);
        } else {
            strncpy(server_message_val, "Error: Failed attempt to save the XY image to a TIFF file", STRING_BUF_SIZE-1);
        }
        // 
        save_task_busy = false;
        server_save_file_busy_val = false;

    }

    void SurfaceConceptTDC::SaveXYTextThreadedAction() {
        long start = Helper::get_millisec();
        std::string filename(save_filename_val);
        std::replace(filename.begin(), filename.end(), ' ', '_');
        std::string directory(save_directory_rval);
        directory = Helper::join_pathnames(directory, Helper::get_date_string("_"));
        if (!Helper::ensure_directory_exists(directory)) {
            save_task_busy = false;
            server_save_file_busy_val = false;
            return;
        }
        std::ostringstream oss;
        oss << std::setfill('0') << std::setw(3) << save_filecounter_val << "_" << filename;
        std::string filename_w_ctr = oss.str();
        filename_w_ctr = Helper::ensure_extension(filename_w_ctr, "_XY.dat");
        if (SaveXYtoText(*m_hist_map.at("Hist_Accu_XY"), directory, filename_w_ctr, accumulated_time_val)) {
            long end = Helper::get_millisec();
            std::cout << "Saved XY image to text file " << filename_w_ctr << " in " << end-start << " milliseconds." << std::endl;
            save_last_filename = filename_w_ctr;
            // write measurement info
            std::string infofilename = filename_w_ctr.substr(0, filename_w_ctr.length()-4)+"_info.txt";
            std::string infofilepath = Helper::join_pathnames(directory, infofilename);
            status_xy_saved_val = true;
            SaveMeasurementInformation(infofilepath);
            std::string msg("XY image saved to ");
            msg = msg + directory + "/" + filename_w_ctr;
            strncpy(server_message_val, msg.c_str(), STRING_BUF_SIZE-1);
        } else {
            strncpy(server_message_val, "Error: Failed attempt to save the XY image to a text file", STRING_BUF_SIZE-1);
        }
        // 
        save_task_busy = false;
        server_save_file_busy_val = false;

    }
    
    void SurfaceConceptTDC::SaveMeasurementInformation(std::string fullpath) {
        ofstream f;
        f.open(fullpath, ios::out);
        GeneralHistogram *h = m_hist_map["Hist_Accu_XYT"];
        f << "Information for File " << save_last_filename << std::endl;
        f << "Originally saved in " << std::string(save_directory_rval) << std::endl;
        f << "Date: " << Helper::get_date_string("-") << std::endl;
        f << "Time: " << Helper::get_time_string(":") << std::endl;
        f << "Accumulated Time: " << accumulated_time_val << " ms" << std::endl;
        f << "XYT data set: " << std::endl;
        f << "    binning x: " << h->binx << std::endl;
        f << "    binning y: " << h->biny << std::endl;
        f << "    binning t: " << h->bint << std::endl;
        f << "    region of interest x: " << h->roix1 << " to " << h->roix2 << std::endl;
        f << "    region of interest y: " << h->roiy1 << " to " << h->roiy2 << std::endl;
        f << "    region of interest t: " << h->roit1 << " to " << h->roit2 << std::endl;
        f << "    modulo: " << h->modulo << std::endl;
        f.close();
    }
    
    void SurfaceConceptTDC::accumulation_start()
    {
        if (m_TDC_id<0) return; // device not initialized
        accumulation_running_val = true; // (value of Tango attribute) it is better to set this immediately to true
        // if users of this server want to trigger to the end of the accumulation by testing
        // the accumulation_running state, this should be immediately true after the accumulation_start command
        // has been called.
        user_acquisition_active = false;
        user_accumulation_active = true;
        if (acquisition_running) {
            deferred_accumulation_start_request = true;
            // we need to do this, so that our data buffer can be reallocated, if necessary
            _acquisition_stop(); // accumulation_start will be called again by Measurement_Complete_Callback
            return;
        }
        // if this spot is reached, acquisition is not running
        // now is a good time to reallocate memory if necessary (done by SetPipeActive)
        if (!m_hist_map.at("Hist_Accu_XYT")->GetPipeActive())
            m_hist_map.at("Hist_Accu_XYT")->SetPipeActive(true);
        if (!m_hist_map.at("Hist_Accu_XYT")->GetPipeActive()) // succesful?
            return;
        m_hist_map.at("Hist_Accu_XYT")->ClearBuffer();
        m_hist_map.at("Hist_Full_T")->ZeroTangoAccuBufferDevLong();
        m_hist_map.at("Hist_User_T")->ZeroTangoAccuBufferDevLong();
        _acquisition_start(); 
        accumulated_time_val = 0;    // this is exposed to the user and represents total accumulated time (accumulation continue)
        accumulated_time_single = 0; // internal: reset to 0 when accumulation start or accumulation continue is called
        // accumulated_time_single is needed to autostop the accumulation when the time given in m_exposure_accu_ms has passed
        accumulation_last_time_stamp = Helper::get_millisec();
        _update_filecounter_and_save_states();
        accumulation_running = true; // controls write access to pipe histograms, accumulation time counter, and accumulation of tango spectra

    }

    void SurfaceConceptTDC::_update_filecounter_and_save_states() {
        // *** LOGIC TO INCREMENT THE SAVE_FILECOUNTER ONLY IF SOMETHING HAD BEEN SAVED ***
        if (status_spectrum_saved_val || status_xyt_saved_val || status_spectrum2_saved_val || status_xy_saved_val) {
            save_filecounter_val = (save_filecounter_val+1) % 1000;
            Write_Own_DevLong_Attribute("Save_Filecounter", save_filecounter_val);
        }        
        status_spectrum_saved_val  = false;
        status_spectrum2_saved_val = false;
        status_xyt_saved_val       = false;
        status_xy_saved_val        = false;
    }


    bool SurfaceConceptTDC::is_AccumulationStart_allowed(const CORBA::Any &any) {
        return true;
    }
    

    void SurfaceConceptTDC::accumulation_continue()
    {
        accumulation_running_val = true; // it is better to set this immediately to true
        user_acquisition_active = false;
        user_accumulation_active = true;
        if (acquisition_running) {
            deferred_accumulation_continue_request = true;
            _acquisition_stop(); // accumulation_continue will be called again by Measurement_Complete_Callback
            return;
        }        
        if (!m_hist_map.at("Hist_Accu_XYT")->GetPipeActive())
            m_hist_map.at("Hist_Accu_XYT")->SetPipeActive(true);
        if (!m_hist_map.at("Hist_Accu_XYT")->GetPipeActive()) // succesful?
            return;
        _acquisition_start();
        accumulation_last_time_stamp = Helper::get_millisec();
        accumulated_time_single  = 0;
        accumulation_running = true; // controls write access to pipe histograms, accumulation time counter, and accumulation of tango spectra
        _update_filecounter_and_save_states();
    }

    bool SurfaceConceptTDC::is_AccumulationContinue_allowed(const CORBA::Any &any) {
        return true;
    }
    
    void SurfaceConceptTDC::accumulation_stop()
    {
        // m_hist_map.at("Hist_Accu_XYT")->SetPipeActive(false); // -> dangerous, might segfault, need to defer this to measurement complete callback
        if (accumulation_running)
            deferred_xyt_pipe_close_request = true;
        else
            m_hist_map.at("Hist_Accu_XYT")->SetPipeActive(false); // xyt pipe should not be open anyway (?)
        accumulation_running = false;
        accumulation_running_val = false;
        user_accumulation_active = false;
        user_acquisition_active = false; // stop live images as well
        _acquisition_stop();
    }
    
    bool SurfaceConceptTDC::is_AccumulationStop_allowed(const CORBA::Any &any) {
        return true;
    }

    bool SurfaceConceptTDC::is_AccumStartAndSaveXYText_allowed(const CORBA::Any& any) {
        return true;
    }

    bool SurfaceConceptTDC::is_AccumStartAndSaveXY_allowed(const CORBA::Any& any) {
        return true;
    }

    
    void SurfaceConceptTDC::save_thist_accu() {
        std::string filename(save_filename_val);
        std::replace(filename.begin(), filename.end(), ' ', '_');
        std::string directory(save_directory_rval);
        directory = Helper::join_pathnames(directory, Helper::get_date_string("_"));
        if (!Helper::ensure_directory_exists(directory))
            return;
        std::string filectr;
        std::ostringstream oss;
        oss << std::setfill('0') << std::setw(3) << save_filecounter_val << "_" << filename << "_t1";
        std::string filename_w_ctr = oss.str();
        filename_w_ctr = Helper::ensure_extension(filename_w_ctr, ".txt");
        if (SaveSpectrum(*m_hist_map.at("Hist_Accu_T"), directory, filename_w_ctr, false))
            status_spectrum_saved_val = true;;
    }

    void SurfaceConceptTDC::save_thist_user_accu() {
        std::string filename(save_filename_val);
        std::replace(filename.begin(), filename.end(), ' ', '_');
        std::string directory(save_directory_rval);
        directory = Helper::join_pathnames(directory, Helper::get_date_string("_"));
        if (!Helper::ensure_directory_exists(directory))
            return;
        std::string filectr;
        std::ostringstream oss;
        oss << std::setfill('0') << std::setw(3) << save_filecounter_val << "_" << filename << "_t2";
        std::string filename_w_ctr = oss.str();
        filename_w_ctr = Helper::ensure_extension(filename_w_ctr, ".txt");
        if (SaveSpectrum(*m_hist_map.at("Hist_User_T"), directory, filename_w_ctr, true))
            status_spectrum2_saved_val = true;
    }


    bool SurfaceConceptTDC::SaveSpectrum(GeneralHistogram& hist, const std::string path, const std::string filename, bool from_tango_accu_buf) {
        uint32_t *pthist = (uint32_t*) hist.GetDatabufPointer();
        Tango::DevLong* p_accuhist = hist.GetTangoAccuBuffer();
        if (!from_tango_accu_buf && pthist==NULL) return false;
        if (from_tango_accu_buf && p_accuhist==NULL) return false;
        if (hist.depth!=32) {
            std::cout << "SaveSpectrum: bit depths other than 32 not implemented yet!" << std::endl;
            std::cout << " Cancelling saving of spectrum." << std::endl;
            return false;
        }
        std::string fullpath = Helper::join_pathnames(path, filename);
        if (Helper::test_file_exists(fullpath)) {
            std::cout << "SaveSpectrum: file already exists!" << std::endl;
            std::cout << " " << fullpath;
            std::cout << " Cancelling saving of spectrum." << std::endl;
            std::string msg("Spectrum not saved. File already exists: ");
            msg = msg + fullpath;
            strncpy(server_message_val, msg.c_str(), STRING_BUF_SIZE-1);
            return false;
        }
        if (!Helper::test_file_writable(fullpath)) {
            std::cout << "SaveSpectrum: cannot write to file" << std::endl;
            std::cout << " " << fullpath;
            std::string msg("Spectrum not saved. File not writable: ");
            msg = msg + fullpath;
            strncpy(server_message_val, msg.c_str(), STRING_BUF_SIZE-1);
            return false;
        }
        long w = hist.GetWidth();
        
        std::ofstream outf(fullpath, std::ofstream::out);
        outf << "# Accumulated time spectrum" << std::endl;
        outf << "# Date: " << Helper::get_date_string("-") << std::endl;
        outf << "# Time: " << Helper::get_time_string(":") << std::endl;
        outf << "# Accumulated Time: " << accumulated_time_val << " ms" << std::endl;
        outf << "# Parameters: " << std::endl;
        outf << "#     binning x: " << hist.binx << std::endl;
        outf << "#     binning y: " << hist.biny << std::endl;
        outf << "#     binning t: " << hist.bint << std::endl;
        outf << "#     selected x range: " << hist.roix1 << " to " << hist.roix2 << std::endl;
        outf << "#     selected y range: " << hist.roiy1 << " to " << hist.roiy2 << std::endl;
        outf << "#     region of interest t (binned units): " << hist.roit1 << " to " << hist.roit2 << std::endl;
        outf << "#     modulo: " << hist.modulo << std::endl;
        outf << "#     pre-modulo time shift: " << tdc_common_shift_val << std::endl;
        outf << "### Column1: binned pixels" << std::endl;
        outf << "### Column2: unbinned pixels" << std::endl;
        outf << "### Column3: time (s)" << std::endl;
        outf << "### Column4: intensity" << std::endl;
        outf << "### " << std::endl;
        outf << "### START OF DATA" << std::endl;
        
        if (from_tango_accu_buf) {
            for (int x = 0; x<w; x++) {
                long xbinned = x+hist.roix1;
                outf << xbinned << "\t" << xbinned*hist.bint << "\t" << ((double)xbinned*hist.bint)*devprop_pixel_size_t_val*1e-12 << "\t" << p_accuhist[x] << std::endl;
            }
        }
        else {
            for (int x = 0; x<w; x++) {
                long xbinned = x+hist.roix1;
                outf << xbinned << "\t" << xbinned*hist.bint << "\t" << ((double)xbinned*hist.bint)*devprop_pixel_size_t_val*1e-12 << "\t" << pthist[x] << std::endl;
            }
        }
        outf.close();
        std::string msg("Spectrum saved to: ");
        msg = msg + fullpath;
        strncpy(server_message_val, msg.c_str(), STRING_BUF_SIZE-1);
        return true;
    }
    
    void SurfaceConceptTDC::shrink_databufs() {
        if (accumulation_running || acquisition_running)
            return;
        for (auto &h : m_hist_map) {
            bool old_active_state = h.second->GetPipeActive();
            h.second->SetPipeActive(false);
            if (h.second->GetPipeActive()==false) {
                h.second->ReleaseDatabuf();
                if (old_active_state)
                    h.second->SetPipeActive(true);
            }
        }
            
    }

}
