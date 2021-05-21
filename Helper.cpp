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
 * Helper.cpp
 * 
 * Created on: 07.04.2016
 */

// Surface concept includes
#include <scTDC_types.h>
#include <scTDC.h>

// standard library includes
#include <iostream>
#include <string.h>
#include <vector>
#include <algorithm>
#include <string>
#include <ctime>
#include <functional> 
#include <cctype>
#include <locale>

// Tango includes
#include <tango.h>

#ifdef _WIN32
// Windows includes (none yet)
#include <windows.h>
#else
// Linux includes
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <libgen.h>
#include <time.h>
#include <pwd.h>
#endif

// my own includes
#include <Helper.h>
#include <iomanip>

namespace Helper {
    //const static char path_sep = '/';

    bitsize_t bitsize_to_enum(long Bitsize) {
            if (Bitsize == 8)
                    return BS8;
            else if (Bitsize == 16)
                    return BS16;
            else if (Bitsize == 32)
                    return BS32;
            else if (Bitsize == 64)
                    return BS64;
            else {
                std::cout << "Helper::bitsize_to_enum called with illegal argument: " << Bitsize << std::endl;
                return (bitsize_t) -1;
            }
    }

    long enum_to_bitsize(bitsize_t bs) {
            if (bs == BS8)
                    return 8;
            else if (bs == BS16)
                    return 16;
            else if (bs == BS32)
                    return 32;
            else if (bs == BS64)
                    return 64;
            else {
                std::cout << "Helper::enum_to_bitsize called with illegal argument: " << bs << std::endl;
                return -1;
            }
    }
    
    void cout_sc_err_message(int errcode) {
        char my_str_buf[ERRSTRLEN];
        ::sc_get_err_msg(errcode, my_str_buf);
        std::cout << "scTDC ERROR (code " << errcode << "): " << my_str_buf << std::endl;
    }
    
    std::string get_sc_err_message(int errcode) {
        char my_str_buf[ERRSTRLEN];
        ::sc_get_err_msg(errcode, my_str_buf);
        ostringstream oss;
        oss << "scTDC ERROR (code " << errcode << "): " << my_str_buf;
        return oss.str();
    }

    bool ensure_directory_exists(const std::string s) {
        //std::cout << "ensure_directory_exists called with " << s << std::endl;
        struct stat st;
        memset(&st, 0, sizeof(st));
        
        if (stat(s.c_str(), &st) == -1) { // does not exist yet
            std::string base = extract_dirname(s); // recursively try to make parent directories
            if (base.size()>0) {
                if (ensure_directory_exists(base)!=true) // if parent directories can't be made, then no chance
                    return false;
            }
            bool success = (mkdir(s.c_str(), 0700)==0);
            if (!success) std::cout << " ... unable to create directory" << std::endl;
            return success;
        }
        else {                          // exists
            if (S_ISDIR(st.st_mode))    // check if it is a directory
                return true;
            else {
                std::cout << " ... path exists, but is not a directory" << std::endl;
                return false;
            }
        }
        return false; // this point can never be reached (?!), prevent compiler warning
    }
    
    bool test_file_exists(const std::string s) {
        //std::cout << "test_file_writable called with " << s << std::endl;
        FILE* f = fopen(s.c_str(), "r"); // appending mode does not erase file
        if (f!=NULL) {
            fclose(f);
            //std::cout << " ... is writable" << std::endl;
            return true;
        }
        else {
            return false;
        }
    }

    bool test_file_writable(const std::string s) {
        //std::cout << "test_file_writable called with " << s << std::endl;
        FILE* f = fopen(s.c_str(), "a"); // appending mode does not erase file
        if (f!=NULL) {
            fclose(f);
            //std::cout << " ... is writable" << std::endl;
            return true;
        }
        else {
            std::cout << "WARNING: " << s << " is not writable" << std::endl;
            return false;
        }
    }
    
    std::string extract_dirname(const std::string s) {
        //std::cout << "extract_dirname called with " << s << std::endl;
        std::string::size_type pos = s.find_last_of(path_sep);
        std::string retval;
        if (pos==std::string::npos) // no path separator -> return empty string
            retval = "";
        else
            retval = s.substr(0,pos); // return everything before the last path separator
        //std::cout << " ... returns " << retval << std::endl;
        return retval;
    }
    
    std::string join_pathnames(const std::string p1, const std::string p2) {
        // treat cases, where at least one of the two is empty
        if (p1.size()==0 && p2.size()==0)
            return "";
        else if (p1.size()!=0 && p2.size()==0)
            return p1;
        else if (p1.size()==0 && p2.size()!=0)
            return p2;
        else { // both are non-empty, 
            // decide whether we have to remove duplicate path_sep, 
            // or insert missing path_sep, or just concatenate
            char p1last = p1.at(p1.size()-1);
            char p2first = p2.at(0);
            if (p1last==path_sep && p2first==path_sep)
                return p1 + p2.substr(1);
            else if ((p1last!=path_sep && p2first==path_sep) || (p1last==path_sep && p2first!=path_sep ))
                return p1 + p2;
            else 
                return p1 + path_sep + p2;
        }
        // this point cannot be reached
        return p1 + path_sep + p2;
    }
    
    std::string join_pathnames(const std::string p1, const std::string p2, const std::string p3) {
        return join_pathnames(p1, join_pathnames(p2,p3));
    }
    
    
    // #################################################################
    // #################################################################
    
    std::vector<std::string> *__hlist = NULL;

    void __initialize_hlist();
    void __initialize_hlist() {
        if (__hlist==NULL) {
            __hlist = new std::vector<std::string>();
            __hlist->push_back("Hist_Live_XY");
            __hlist->push_back("Hist_Live_XT");
            __hlist->push_back("Hist_Live_YT");
            __hlist->push_back("Hist_Live_T");
            __hlist->push_back("Hist_Accu_XYT");
            __hlist->push_back("Hist_Accu_XY");
            __hlist->push_back("Hist_Accu_XT");
            __hlist->push_back("Hist_Accu_YT");
            __hlist->push_back("Hist_Accu_T");
            __hlist->push_back("Hist_Full_XY");
            __hlist->push_back("Hist_User_T");
        }
    }    
     /**
     * Extract the Histogram name from the Tango server property
     * @param s Tango server property
     * @return Histogram name
     */
    std::string extract_hist_name(const std::string s) {
        __initialize_hlist();
        for (std::string h : *__hlist) {
            if (s.find(h)==0)
                return h;
        }
        return "";
    }
    
     /**
     * Extract the part following the Histogram name from the Tango server property
     * @param s Tango server property
     * @return the part following the Histogram name without leading underscore
     */
    std::string extract_hist_remainder(const std::string s) {
        __initialize_hlist();
        for (std::string h : *__hlist) {
            h = h + "_";
            if (s.find(h)==0)
                return s.substr(h.length());
        }
        return "";
    }

    long get_millisec(){
        struct timespec t;
        clock_gettime(CLOCK_MONOTONIC_COARSE, &t);
        //std::cout << "clock_gettime gave us " << t.tv_sec << " seconds and " << t.tv_nsec << " nanoseconds" << std::endl;
        return t.tv_sec*1000+t.tv_nsec/1000000;
    }
  
    
    std::string to_lower(const std::string src) {
        std::string result(src);
        std::transform(result.begin(), result.end(), result.begin(), ::tolower);
        return result;
    }
    
    std::string insert_before_extension(const std::string path, const std::string ext, 
            const std::string insertstr) {
        std::string e(ext);
        if (path.length()==0)
            return insertstr;
        if (ext.length()==0)
            return path+insertstr;
        if (path.length()<ext.length()) 
            e = ext.substr(0, path.length());
        std::string pathext = path.substr(path.length()-e.length());
        if (Helper::to_lower(pathext).compare(e)==0)
            return path.substr(0, path.length()-e.length()) + insertstr + e;
        else
            return path + insertstr;
    }
    
    std::string ensure_extension(const std::string path, const std::string ext) {
        std::string e(ext);
        if (path.length()==0)
            return ext;
        if (ext.length()==0)
            return path;
        if (path.length()<ext.length()) 
            return path+ext;
        std::string pathext = path.substr(path.length()-e.length());
        if (Helper::to_lower(pathext).compare(e)==0)
            return path;
        else
            return path + ext;
    }
    
    #ifdef _WIN32
        void sleep(unsigned milliseconds)
        {
            Sleep(milliseconds);
        }
    #else
        void sleep(unsigned milliseconds)
        {
            usleep(milliseconds * 1000); // takes microseconds
        }
    #endif    

        
        std::string get_date_string(std::string separator) {
            ostringstream oss;
            time_t t = time(0);
            struct tm *now = localtime(&t);
            oss << now->tm_year + 1900 << separator;
            oss << std::setw(2) << std::setfill('0') << (now->tm_mon + 1) << separator;
            oss << std::setw(2) << std::setfill('0') << now->tm_mday;
            return oss.str();
        }
        
        std::string get_time_string(std::string separator) {
            ostringstream oss;
            time_t t = time(0);
            struct tm *now = localtime(&t);
            oss << std::setw(2) << std::setfill('0') << now->tm_hour << separator << std::setw(2) << std::setfill('0') << now->tm_min;
            return oss.str();
        }
        
        std::string get_user_home_dir() {
            const char *homedir;

            if ((homedir = getenv("HOME")) == NULL) {
                homedir = getpwuid(getuid())->pw_dir;
            }
            std::string home(homedir);
            return home;
        }
    
    std::vector<std::string> &split(const std::string &s, char delim, std::vector<std::string> &elems) {
        std::stringstream ss(s);
        std::string item;
        while (std::getline(ss, item, delim)) {
            elems.push_back(item);
        }
        return elems;
    }


    std::vector<std::string> split(const std::string &s, char delim) {
        std::vector<std::string> elems;
        split(s, delim, elems);
        return elems;
    }        
    
     void cout_tango_devfailed_exception(Tango::DevFailed e)
     {
        for (_CORBA_ULong i = 0; i<e.errors.length(); i++) {
            std::cout << "desc: " << e.errors[i].desc << std::endl;
            std::cout << "origin: " << e.errors[i].origin << std::endl;
            std::cout << "reason: " << e.errors[i].reason << std::endl;
        }
    }
     
    std::string Format_Bytesize(long sz, int precision) {
        std::ostringstream fsz;
        std::string unit;
        double dsz = (double) sz;
        if (sz/1073741824L>0) { // more than 1 GB?
            dsz = dsz / 1073741824.0;
            unit = "GB";
        }
        else if (sz/1048576L>0) {
            dsz = dsz / 1048576.0;
            unit = "MB";
        }
        else if (sz/1024>0) {
            dsz = dsz / 1024.0;
            unit = "KB";
        }
        else
            unit = "Byte";
        fsz.setf(ios::fixed, ios::floatfield);
        fsz << std::setw(6) << std::setprecision(precision) << dsz << " " << unit;
        return fsz.str();
    }
}
