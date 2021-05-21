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
 * File:   scTDCinterface_.h
 *
 * Created on 25. April 2016, 17:27
 */

#ifndef GENERALHISTOGRAM__H
#define	GENERALHISTOGRAM__H

#include <scTDC.h>
#include <tango.h>
#include <climits>
#include "CustomAttr.h"
#include "StatisticsHist.h"

using std::vector;

namespace SurfaceConceptTDC_ns {
    
    class CustomSpectrumAttr;
    class CustomImageAttr;
    
    class GeneralHistogram {
    public:
        static const int MODULO_FACTOR = 1; // multiply user input of modulos by this number at the sctdc pipe level
        static const std::string AllAttributes[];

        void *hist_par=NULL; // pointer to one of the sc_pipe_dld_***_params_t structs
        
        ::sc_pipe_type_t pipe_type;
        static const Tango::DevLong64 std_bin_x = 2;
        static const Tango::DevLong64 std_bin_y = 2;
        static const Tango::DevLong64 std_bin_t = 1024;
        static const Tango::DevLong64 std_size_x = 512;
        static const Tango::DevLong64 std_size_y = 512;
        static const Tango::DevLong64 std_size_t = 512;
        static const Tango::DevLong64 max_size_x = 1024;
        static const Tango::DevLong64 max_size_y = 1024;
        static const Tango::DevLong64 max_size_t = 9223372036854775807L;
        static const Tango::DevLong64 std_depth = 32;
        static const unsigned int std_accumulation = UINT_MAX;
        
        static const int         taxis_unit_pixels      = 0;
        static const int         taxis_unit_unbinned    = 1;
        static const int         taxis_unit_s           = 2;
        static const int         taxis_unit_ms          = 3;
        static const int         taxis_unit_us          = 4;
        static const int         taxis_unit_ns          = 5;
        static const int         taxis_unit_ps          = 6;
        static const int         taxis_unit_max         = 6;          
        
        std::vector<std::string> Attributes;
        Tango::DevDouble accumulation = 0.0;
        Tango::DevLong64   binx = std_bin_x;
        Tango::DevLong64   biny = std_bin_y;
        Tango::DevLong64   bint = std_bin_t;
        Tango::DevLong64   roix1 = 0;
        Tango::DevLong64   roix2 = std_size_x-1;
        Tango::DevLong64   roiy1 = 0;
        Tango::DevLong64   roiy2 = std_size_y-1;
        Tango::DevLong64   roit1 = 0;
        Tango::DevLong64   roit2 = std_size_t-1;
        Tango::DevLong64   depth = std_depth;
        Tango::DevLong64   modulo = 0; // = pow(2,31) - 1
        
        
        
        GeneralHistogram(::sc_pipe_type_t pipe_type);
        ~GeneralHistogram();
        
        /**
         * Sets the value of the attribute, the change is propagated
         * to the ..._pipe_params structures of the scTDC library
         * @param identifier   name of the attribute
         * @param value        new value of the attribute
         */
        void SetAttribute(const std::string identifier, long value);

        /**
         * Get the value of the attribute
         * @param identifier   name of the attribute
         */
        long GetAttribute(const std::string identifier);
        
        /**
         * Get the minimum allowed value of the attribute
         * @param identifier   name of the attribute
         */
        long GetAttributeMin(const std::string identifier);
        
        /**
         * Get the maximum allowed value of the attribute
         * @param identifier   name of the attribute
         */
        long GetAttributeMax(const std::string identifier);
        
        /**
         * Get the preferred Tango Format string for the attribute
         * @param identifier   name of the attribute
         */
        std::string GetAttributeFormat(const std::string identifier);

        /**
         * return the smaller value of x1, x2
         */
        static long ROIOff(long x1, long x2);

        /**
         * return the positive (or zero) distance between x1, x2
         */
        static long ROISize(long x1, long x2);
        
        /**
         * return the width of the image
         */
        long GetWidth();
        void SetWidth(long newwidth);
        long GetAbscissaOffset();
        void SetAbscissaOffset(long newoffset);

        /**
         * return the height of the image
         */
        long GetHeight();
        void SetHeight(long newheight);
        long GetOrdinateOffset();
        void SetOrdinateOffset(long newoffset);
        
        /**
         * return the pixel size along the third axis
         */
        long GetZSize();
        void SetZSize(long newzsize);
        long GetZOffset();
        void SetZOffset(long newoffset);
        
        // #####################################################################
        // #####################################################################
        
        void SetDeviceDescriptor(int devdesc);
        int  GetDeviceDescriptor();
        
        // #####################################################################
        // #####################################################################
        
        int GetPipeID();
        void SetPipeActive(bool state);
        bool GetPipeActive();
        
        // #####################################################################
        // #####################################################################
        
        void SetFilePath(std::string new_file_path);
        std::string GetFilePath();
        
        void SetPGMPath(std::string new_file_path);
        
        /**
         * Activate/Deactivate file output, for activation, SetFilePath should be called before
         */
        void SetFileOutputActive(bool state);
        void SetPGMOutputActive(bool state, int width=256, int height=128);
        void SetTangoOutAttribute(CustomSpectrumAttr* attr=NULL);  // overloaded function for spectrum attributes
        void SetTangoOutAttribute(CustomImageAttr* attr=NULL); // overloaded function for image attributes
        void ProvideTAxis(CustomSpectrumAttr* taxis_attr=NULL, double pixelsize=1.0, int unit=taxis_unit_pixels);
        void SetTAxisUnit(int unit);
        Tango::DevDouble* getTAxis();

        
        /**
         * Returns the active/inactive state of file output.
         * Can be used to check whether SetFileOutputActive(true) was succesful
         * @returns true, if file output is active, otherwise false
         */
        bool GetFileOutputActive();
        
        void SetFileOutputBigEndian(bool state); // if false, use architectural standard
        bool GetFileOutputBigEndian();           // (... might be big endian as well)

        /**
         * Write the image to the file passed by SetFilePath(...), if file_output_active==true
         */
        void WriteFile();   // auto-choose endianness depending on file_output_big_endian
        void WritePGM();
        void WritePGM1DPlot(uint32_t maxvalue=0);
        void SetPGMSize(int width=-1, int height=-1);
        int  GetPGMWidth();
        int  GetPGMHeight();
        
        void WriteTangoBuffer();
        
        void WriteTangoBufferDevLong(int w, int h);
        void SetTangoAccuBufferActive(bool active_);
        void AddToTangoAccuBuffer();
        void AddToTangoAccuBufferDevLong(int w, int h);
        void ZeroTangoAccuBufferDevLong();
        Tango::DevLong* GetTangoBuffer();
        Tango::DevLong* GetTangoAccuBuffer();
        long GetTangoBufferDataWidth();
        long GetTangoBufferDataHeight();
        
        void UpdateStatisticsOfDatabuf();
        uint32_t GetStatMax();
        uint32_t GetStatQuantile(double p);
        
        
        void PerformActiveOutputs();

        // #####################################################################
        // #####################################################################
        
        static int AllocatorCallback(void *object, void **bufpointer); // to be called by the scTDC library
        void* GetDatabufPointer();
        long GetDatabufSize();
        void ClearBuffer();
        void AccomodateDatabufSize(bool zerobuf=false);
        void ReleaseDatabuf();
        
        // #####################################################################
        // #####################################################################
        
    private:
        static const std::map<std::string, long> AttributesMin;
        static const std::map<std::string, long> AttributesMax;
        static const std::map<std::string, std::string> AttributesFormat;
        
        StatisticsHist* stathist = NULL;

        int device_descriptor = -1;
        int pipe_id           = -1;
        bool pipe_active      = false;
        
        bool file_output_active     = false;
        bool file_output_big_endian = true;
        std::string file_path       = "";
        FILE *file_ptr              = NULL; // FILE* io is not the c++ way, but may have best performance
        
        bool pgm_output_active      = false;
        int pgm_width               = 256;
        int pgm_height              = 128;
        std::string pgm_path        = "";
        
        void *databuf = NULL;  // pointer to the data buffer object
        long databufsize = 0;  // size of the allocated memory for the data buffer in bytes
        
        vector<Tango::DevLong>   tangobuf;
        long                     tangobuf_width         = -1;  // capacity width of the Tango buffer
        long                     tangobuf_height        = -1;  // capacity height of the Tango buffer
        long                     tangobuf_datawidth     = -1;  // actual width of the data in the Tango buffer
        long                     tangobuf_dataheight    = -1;  // actual height of the data in the Tango buffer
        vector<Tango::DevLong>   tangobuf_accu;
        bool                     tangobuf_accu_active   = false;
        CustomSpectrumAttr*      tango_spectrum_attr    = NULL;
        CustomImageAttr*         tango_image_attr       = NULL;

        vector<Tango::DevDouble> tangobuf_taxis;
        CustomSpectrumAttr*      tango_taxis_attr       = NULL;
        bool                     taxis_active           = false;
        double                   taxis_pixelsize        = 0.0;
        int                      taxis_unit             = false;
      
        
        template <typename T> void _setAttribute(const std::string identifier, long value);
        template <typename T> long _getAttribute(const std::string identifier);
        void UpdateSCTDCHistoPipe();
        void SetPipeID(int new_pipe_id);
        
        // _write_file_big_endian is private and called by WriteFile if necessary
        // ( users of the class can control this via SetFileOutputBigEndian(true/false))
        void _write_file_big_endian(); // write file in big-endian byte order
        void _write_taxis(int w_);
        
        //template <typename T> void AccomodateTangobuffer(T** buf, int w, int h, bool zero=false);
        
    };
};

#endif	/* GENERALHISTOGRAM__H */

