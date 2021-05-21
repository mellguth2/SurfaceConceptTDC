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
 * File:   TimedPeriodicCallThread.h
 *
 * Created on 19. Mai 2016, 12:18
 */

#ifndef TIMEDPERIODICCALLTHREAD_H
#define	TIMEDPERIODICCALLTHREAD_H

#include <iostream>
#include <thread>


class TimedPeriodicCallThread {
private:
    void (*callback)(void* arg)   = NULL;
    void* callback_arg            = NULL;
    long period                   = 1000;
    bool running                  = false;
    std::thread *thread           = NULL;
    
    void ThreadLoop();
    static void StaticThreadLoop(void* object);
    
public:
    TimedPeriodicCallThread();
    TimedPeriodicCallThread(void (*func)(void* arg), void* func_arg, long milliseconds);
    void SetCallback(void (*func)(void* arg), void* func_arg);
    void SetPeriod(long milliseconds);
    void Start();
    void Stop();
    void Join();
    void Detach();
    ~TimedPeriodicCallThread();

};

#endif	/* TIMEDPERIODICCALLTHREAD_H */

