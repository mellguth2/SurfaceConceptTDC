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
 * Helper.h
 * 
 * Created on: 07.04.2016
 */

#ifndef HELPER_H_
#define HELPER_H_

#include <string>
#include <algorithm>
#include <sstream>
#include <scTDC_types.h>
#include <tango.h>

namespace Helper {
    const static char path_sep = '/';
    
    bitsize_t bitsize_to_enum(long Bitsize);
    long enum_to_bitsize(bitsize_t bs);

    std::string extract_hist_name(const std::string s);
    std::string extract_hist_remainder(const std::string s);
    void cout_sc_err_message(int errcode);
    std::string get_sc_err_message(int errcode);
    
    bool ensure_directory_exists(const std::string s);
    bool test_file_writable(const std::string s);
    bool test_file_exists(const std::string s);
    std::string extract_dirname(const std::string s);
    std::string join_pathnames(const std::string p1, const std::string p2);
    std::string join_pathnames(const std::string p1, const std::string p2, const std::string p3);
    std::string insert_before_extension(const std::string path, const std::string ext, const std::string insertstr);
    std::string ensure_extension(const std::string path, const std::string ext);
    std::string get_user_home_dir();
    
    std::string to_lower(const std::string);
    
    std::string get_date_string(std::string separator);
    std::string get_time_string(std::string separator);
    
    void cout_tango_devfailed_exception(Tango::DevFailed e);
    
    void sleep(unsigned milliseconds);
    long get_millisec();
    
    std::string Format_Bytesize(long sz, int precision);
    
    // -------------------------------------------------------------------------
    class Finally
    {
        std::function<void(void)> functor;
    public:
        Finally(const std::function<void(void)> &functor) : functor(functor) {}
        ~Finally() { functor(); }
    };
    // -------------------------------------------------------------------------


    // trim from start
    static inline std::string &ltrim(std::string &s) {
            s.erase(s.begin(), std::find_if(s.begin(), s.end(), std::not1(std::ptr_fun<int, int>(std::isspace))));
            return s;
    }

    // trim from end
    static inline std::string &rtrim(std::string &s) {
            s.erase(std::find_if(s.rbegin(), s.rend(), std::not1(std::ptr_fun<int, int>(std::isspace))).base(), s.end());
            return s;
    }

    // trim from both ends
    static inline std::string &trim(std::string &s) {
            return ltrim(rtrim(s));
    }
    
        // trim from start (copying)
    static inline std::string ltrimmed(std::string s) {
        ltrim(s);
        return s;
    }

    // trim from end (copying)
    static inline std::string rtrimmed(std::string s) {
        rtrim(s);
        return s;
    }

    // trim from both ends (copying)
    static inline std::string trimmed(std::string s) {
        trim(s);
        return s;
    }

    std::vector<std::string> &split(const std::string &s, char delim, std::vector<std::string> &elems);
    std::vector<std::string> split(const std::string &s, char delim);

    
    
}

#endif /* HELPER_H_ */
