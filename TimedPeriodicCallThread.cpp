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

#include "TimedPeriodicCallThread.h"
#include "Helper.h"


TimedPeriodicCallThread::TimedPeriodicCallThread() {
    
}

TimedPeriodicCallThread::TimedPeriodicCallThread(void (*func)(void* arg), void* func_arg, long milliseconds) {
    callback     = func;
    callback_arg = func_arg;
    period       = milliseconds;
}

void TimedPeriodicCallThread::SetCallback(void(*func)(void* arg), void* func_arg) {
    callback     = func;
    callback_arg = func_arg;
}

void TimedPeriodicCallThread::SetPeriod(long milliseconds) {
    if (milliseconds<=0) 
        period = 1;
    else
        period = milliseconds;
}

void TimedPeriodicCallThread::Start() {
    if (thread!=NULL) {
        delete thread;
        thread = NULL;
    }
    running = true;
    thread = new std::thread(&StaticThreadLoop, this);
}

void TimedPeriodicCallThread::Stop() {
    running = false;
}


void TimedPeriodicCallThread::ThreadLoop() {
    while (running) {
        Helper::sleep(period);
        if (callback!=NULL)
            callback(callback_arg);
    }
}

void TimedPeriodicCallThread::StaticThreadLoop(void* object) {
    ((TimedPeriodicCallThread*) object)->ThreadLoop();
}

TimedPeriodicCallThread::~TimedPeriodicCallThread() {
    if (thread!=NULL)
        delete thread;
}

void TimedPeriodicCallThread::Join() {
    if (thread!=NULL)
        thread->join();
}

void TimedPeriodicCallThread::Detach() {
    if (thread!=NULL)
        thread->detach();
}

