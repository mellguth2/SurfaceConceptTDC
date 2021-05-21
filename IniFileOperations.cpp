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

#include <vector>
#include <fstream>
#include <string>

#include "IniFileOperations.h"
#include "Helper.h"

IniFileOperations::IniFileOperations() {

}

IniFileOperations::IniFileOperations(std::string _inifilepath) {
    SetSourceIniFilePath(_inifilepath);
}

void IniFileOperations::ClearModifiedKeys() {
    mods.clear();
}

std::string IniFileOperations::GetSourceIniFilePath() {
    return inifilepath;
}

void IniFileOperations::SetModifiedKey(std::string section, std::string key, std::string value) {
    for (ConfigLine cl : mods)
        // in case that section.key already exists, overwrite value
        if (cl.section.compare(section)==0 && cl.key.compare(key)==0) {
            cl.value = value;
            return;
        }
    // else add a new config line            
    ConfigLine cl;
    cl.section = section;
    cl.key = key;
    cl.value = value;
    mods.push_back(cl);
}

void IniFileOperations::SetSourceIniFilePath(std::string _inifilepath) {
    inifilepath = _inifilepath;
}

std::string IniFileOperations::GetTargetIniFilePath() {
    return inifileoutpath;
}

void IniFileOperations::SetTargetIniFilePath(std::string _inifileoutpath) {
    inifileoutpath = _inifileoutpath;
}


bool IniFileOperations::WriteModifiedFile() {
    std::string current_section;
    std::string line;
    std::string origline;
    if (inifileoutpath=="" || inifilepath=="") {
        return true;
    }
    std::ifstream inif(inifilepath, std::ifstream::in);
    if (!inif.is_open()) 
        return false;
    std::ofstream outf(inifileoutpath, std::ofstream::out);
    if (!outf.is_open()) 
        return false;
    while (std::getline(inif, origline)) { // getline drops end-of-line character
        line = origline;
        // restore end of line character
        char lastchar = origline[origline.length()-1];
        if (lastchar=='\x0D')
            origline = origline + "\x0A"; // line had cr+lf ending, lf was lost
        else 
            origline = origline + "\x0D"; // line had cr ending, cr was lost
        // --
        Helper::trim(line);
        if (line.length()==0) { // skip empty line
            outf << origline;
            continue;
        }
        if (line.at(0)==';') { // skip comment line
            outf << origline;
            continue;
        }
        // test whether a new section is starting
        if (line.at(0)=='[') {
            std::string::size_type endpos = line.find(']');
            if (endpos==std::string::npos) {
                outf << origline;
                continue;
            }
            std::string::size_type cpos = line.find(';');
            if (cpos!=std::string::npos && cpos<endpos) {
                outf << origline;
                continue;
            }
            current_section = line.substr(1, endpos-1); // set new section
            outf << origline;
            continue;
        }
        // test whether line contains a '=' symbol
        std::string::size_type eqpos = line.find('=');
        if (eqpos==std::string::npos) {
            outf << origline; // did not find '=' in line, write this line and continue
            continue;
        }
        // found a '='. split line to the left and right of the '='
        std::vector<std::string> elements = Helper::split(line, '=');
        Helper::trim(elements[0]);
        bool match = false;
        for (auto mod : mods) {
            // check whether a key is matching for the current section
            if (mod.key.compare(elements[0])==0 && current_section.compare(mod.section)==0) {
                // found a matching key, write modified value
                outf << mod.key << " = " << mod.value << std::endl;
                match = true;
            }
        }
        // did not find a match : write original line
        if (!match) outf << origline;
    }
    return true;
}
    

