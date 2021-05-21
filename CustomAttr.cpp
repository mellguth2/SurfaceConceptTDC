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


// strange... if tango/attrdesc.h is the only include file, compilation fails
#include <tango/tango.h>
#include <tango/attrdesc.h>

#include "CustomAttr.h"

namespace SurfaceConceptTDC_ns {

CustomAttr::CustomAttr(const char *name,long data_type, Tango::AttrWriteType w_type, const char *assoc) 
  : Tango::Attr(name, data_type, w_type, assoc)
{
}

CustomAttr::~CustomAttr() {
}

void CustomAttr::read(Tango::DeviceImpl *dev, Tango::Attribute &att) {
    if (rcb==NULL || rcb_targetobject==NULL) return;
    (rcb_targetobject->*rcb)(dev, att);
}

void CustomAttr::write(Tango::DeviceImpl *dev, Tango::WAttribute &att) {
    if (wcb==NULL || wcb_targetobject==NULL) return;
    (wcb_targetobject->*wcb)(dev, att);
}

void CustomAttr::SetReadCallback(SurfaceConceptTDC* targetobject, ReadCallback rcb_) {
    rcb = rcb_;
    rcb_targetobject = targetobject;
}

void CustomAttr::SetWriteCallback(SurfaceConceptTDC* targetobject, WriteCallback wcb_) {
    wcb = wcb_;
    wcb_targetobject = targetobject;
}

// #############################################################################


CustomSpectrumAttr::CustomSpectrumAttr(const char* name, long data_type, Tango::AttrWriteType w_type, int maxsize) 
  : Tango::SpectrumAttr(name, data_type, w_type, maxsize)
{
}

CustomSpectrumAttr::~CustomSpectrumAttr() {
}


void CustomSpectrumAttr::SetReadCallback(SurfaceConceptTDC* targetobject, ReadCallback rcb_) {
    rcb = rcb_;
    rcb_targetobject = targetobject;
}

void CustomSpectrumAttr::SetWriteCallback(SurfaceConceptTDC* targetobject, WriteCallback wcb_) {
    wcb = wcb_;
    wcb_targetobject = targetobject;
}

void CustomSpectrumAttr::read(Tango::DeviceImpl* dev, Tango::Attribute& att) {
    if (rcb==NULL || rcb_targetobject==NULL) return;
    (rcb_targetobject->*rcb)(dev, att);
}

void CustomSpectrumAttr::write(Tango::DeviceImpl* dev, Tango::WAttribute& att) {
    if (wcb==NULL || wcb_targetobject==NULL) return;
    (wcb_targetobject->*wcb)(dev, att);
}


// #############################################################################

CustomImageAttr::CustomImageAttr(const char* name, long data_type, Tango::AttrWriteType w_type, long max_x, long max_y)
 : Tango::ImageAttr(name, data_type, w_type, max_x, max_y)
{
}

CustomImageAttr::~CustomImageAttr() {
}

void CustomImageAttr::SetReadCallback(SurfaceConceptTDC* targetobject, ReadCallback rcb_) {
    rcb = rcb_;
    rcb_targetobject = targetobject;
}

void CustomImageAttr::SetWriteCallback(SurfaceConceptTDC* targetobject, WriteCallback wcb_) {
    wcb = wcb_;
    wcb_targetobject = targetobject;
}

void CustomImageAttr::read(Tango::DeviceImpl* dev, Tango::Attribute& att) {
    if (rcb==NULL || rcb_targetobject==NULL) return;
    (rcb_targetobject->*rcb)(dev, att);
}

void CustomImageAttr::write(Tango::DeviceImpl* dev, Tango::WAttribute& att) {
    if (wcb==NULL || wcb_targetobject==NULL) return;
    (wcb_targetobject->*wcb)(dev, att);
}

} // namespace SurfaceConceptTDC_ns
