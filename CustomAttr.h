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
 * File:   CustomAttr.h
 *
 * Created on 22. April 2016, 18:52
 */

#ifndef CUSTOMATTR_H
#define	CUSTOMATTR_H

#include <tango.h>

class GeneralHistogram;

#include "SurfaceConceptTDC.h"

namespace SurfaceConceptTDC_ns
{
    class SurfaceConceptTDC;
    typedef void (SurfaceConceptTDC::* ReadCallback) (Tango::DeviceImpl *, Tango::Attribute &); 
    typedef void (SurfaceConceptTDC::* WriteCallback) (Tango::DeviceImpl *, Tango::WAttribute &);

    class CustomAttr  : public Tango::Attr {
    public:
        CustomAttr(const char *name,long data_type,
                 Tango::AttrWriteType w_type = Tango::READ,
                 const char *assoc = Tango::AssocWritNotSpec);
        CustomAttr(const CustomAttr& orig);
        virtual ~CustomAttr();



        void SetReadCallback(SurfaceConceptTDC* targetobject, ReadCallback rcb_);
        void SetWriteCallback(SurfaceConceptTDC* targetobject, WriteCallback wcb_);


    private:
        void read(Tango::DeviceImpl *, Tango::Attribute &);
        void write(Tango::DeviceImpl *, Tango::WAttribute &);
        bool is_allowed(Tango::DeviceImpl *, Tango::AttReqType) {return true;}
        ReadCallback rcb = NULL;
        WriteCallback wcb = NULL;
        SurfaceConceptTDC *rcb_targetobject = NULL;
        SurfaceConceptTDC *wcb_targetobject = NULL;

    };

// #############################################################################


    class CustomSpectrumAttr  : public Tango::SpectrumAttr {
    public:
        CustomSpectrumAttr(const char *name,long data_type,
                 Tango::AttrWriteType w_type, int maxsize);
        CustomSpectrumAttr(const CustomSpectrumAttr& orig);
        virtual ~CustomSpectrumAttr();

        void SetReadCallback(SurfaceConceptTDC* targetobject, ReadCallback rcb_);
        void SetWriteCallback(SurfaceConceptTDC* targetobject, WriteCallback wcb_);

    private:
        void read(Tango::DeviceImpl *, Tango::Attribute &);
        void write(Tango::DeviceImpl *, Tango::WAttribute &);
        bool is_allowed(Tango::DeviceImpl *, Tango::AttReqType) {return true;}
        ReadCallback rcb = NULL;
        WriteCallback wcb = NULL;
        SurfaceConceptTDC *rcb_targetobject = NULL;
        SurfaceConceptTDC *wcb_targetobject = NULL;

    };

// #############################################################################
    
    class CustomImageAttr  : public Tango::ImageAttr {
    public:
        CustomImageAttr(const char *name, long data_type,
                 Tango::AttrWriteType w_type, long max_x, long max_y);
        CustomImageAttr(const CustomImageAttr& orig);
        virtual ~CustomImageAttr();

        void SetReadCallback(SurfaceConceptTDC* targetobject, ReadCallback rcb_);
        void SetWriteCallback(SurfaceConceptTDC* targetobject, WriteCallback wcb_);

    private:
        void read(Tango::DeviceImpl *, Tango::Attribute &);
        void write(Tango::DeviceImpl *, Tango::WAttribute &);
        bool is_allowed(Tango::DeviceImpl *, Tango::AttReqType) {return true;}
        ReadCallback rcb = NULL;
        WriteCallback wcb = NULL;
        SurfaceConceptTDC *rcb_targetobject = NULL;
        SurfaceConceptTDC *wcb_targetobject = NULL;
    };
    
}

#endif	/* CUSTOMATTR_H */

