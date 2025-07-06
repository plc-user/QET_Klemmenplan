/*
 * Copyright (c) 2025 plc-user
 *
 * It is NOT allowed to use this software or any of the
 * associated files as sample data for the purposes of
 * building machine learning models.
 *
 * Permission is hereby granted, free of charge, to any person
 * obtaining a copy of this software and associated documentation
 * files (the "Software"), to deal in the Software without
 * restriction, including without limitation the rights to use,
 * copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the
 * Software is furnished to do so, subject to the following
 * conditions:
 *
 * The above copyright notice and this permission notice shall be
 * included in all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
 * EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES
 * OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
 * NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT
 * HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY,
 * WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
 * FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR
 * OTHER DEALINGS IN THE SOFTWARE.
 */

#ifndef MAIN_H
#define MAIN_H

#include <iostream>     // for IO-Operations
#include <fstream>      // for file-Operations
#include <cstdint>      // int8_t, ...
#include <string>       // we handle strings here
#include <getopt.h>     // for Commandline-Parameters
#include <filesystem>   // for exe-filename
#include <vector>       // to store the Elements and Conductors
#include <algorithm>    // any_of, ...

#define _DEBUG_ 0

// ============================================================================
// global variables
// ============================================================================

const std::string sVersion = "v0.0.1beta3";

// the qet-file to process:
static std::string QETFile      = "";

// we need a "Pugi-Node":
static pugi::xml_node node;

static size_t uiNumber = 0;

// some Bits for control
static bool xPrintHelp          = false;
static bool xReadFromStdIn      = false;
static bool xOnlyWithLabel      = false;
static bool xAddCommentCol      = false;
static bool xAddNumberCol       = false;
static bool xAddJumpers         = true;
static bool xAddFunction        = true;
static bool xAddText            = true;
static bool xAddProtocol        = true;
static bool xStopWithError      = false;


//
// variables + functions for HTML-output
//
std::string sCSSstyle = "<style type=\"text/css\">\n\
table {\n\
    font-size: 9pt;\n\
}\n\
td {\n\
    padding-left: 5px;\n\
    padding-right: 5px;\n\
}\n\
th {\n\
    font-size: 11pt;\n\
    line-height: 23px;\n\
    text-align: left;\n\
}\n\
.ctext {\n\
    text-align: center;\n\
    valign: middle;\n\
    font-weight: normal;\n\
}\n\
.rtext {\n\
    text-align: right;\n\
    valign: middle;\n\
    font-weight: normal;\n\
}\n\
.text {\n\
    text-align: left;\n\
    font-weight: normal;\n\
}\n\
.tbold {\n\
    text-align: left;\n\
    font-weight: bold;\n\
}\n\
.pos {\n\
    font-size: 8pt;\n\
    line-height: 19px;\n\
    vertical-align: middle;\n\
}\n\
</style>";

std::string sTDctext (std::string s) { return "<td class=\"ctext\">"               + s + "</td>"; }
std::string sTDrtext (std::string s) { return "<td class=\"rtext\">"               + s + "</td>"; }
std::string sTDtext  (std::string s) { return "<td class=\"text\">"                + s + "</td>"; }
std::string sTDtbold (std::string s) { return "<td class=\"tbold\">"               + s + "</td>"; }
std::string sTDtbcs2 (std::string s) { return "<td class=\"tbold\" colspan=\"2\">" + s + "</td>"; }
std::string sTDtbcs4 (std::string s) { return "<td class=\"tbold\" colspan=\"4\">" + s + "</td>"; }
std::string sTDpos   (std::string s) { return "<td class=\"pos\">"                 + s + "</td>"; }

std::string sTableHeader(void){
    return
        "<table border=\"1\" cellspacing=\"0\" cellpadding=\"0\">\n"
        + std::string("  <tr>")
        + (xAddNumberCol  ? sTDtbcs2("")         : sTDtbold("")) // Name der Klemmleiste
        + sTDtbold("Position")
        + (xAddJumpers    ? sTDtbcs4("Jumpers")  : "")
        + (xAddFunction   ? sTDtbold("Function") : "")
        + (xAddText       ? sTDtbold("Text")     : "")
        + (xAddProtocol   ? sTDtbold("Protocol") : "")
        + (xAddCommentCol ? sTDtbold("Comment")  : "")
        + "</tr>";
}

std::string sTableFooter = "</table>";

std::string sStartNewTable(void) {
    return sTableFooter + "\n\n<br>\n\n"
         + sTableHeader();
}

//
// END: variables + functions for HTML-output
//


//
// --- function-prototypes -----------------------------------------------------
//
int parseCommandline(int argc, char *argv[]);
void PrintHelp(const std::string &s, const std::string &v);

static const char cOptions[] = "cf:hlnio";

// the possible Commandlineparameters:
static struct option long_options[]={
    {"comment",no_argument,nullptr,'c'},
    {"file",required_argument,nullptr,'f'},
    {"help",no_argument,nullptr,'h'},
    {"label",no_argument,nullptr,'l'},
    {"number",no_argument,nullptr,'n'},
    {"stdin",no_argument,nullptr,'i'},
    {"NoJumpers",no_argument,nullptr,1000},   // "long-opt" only!!!
    {"NoFunction",no_argument,nullptr,1001},  // "long-opt" only!!!
    {"NoProtocol",no_argument,nullptr,1002},  // "long-opt" only!!!
    {"NoText",no_argument,nullptr,1003},      // "long-opt" only!!!
    {0,0,0,0}
  };





/******************************************************************************/
int parseCommandline(int argc, char *argv[]) {
    int c;
    int option_index = 12345;
    std::string sTmp = "";
    while ((c=getopt_long(argc,argv,cOptions,long_options,&option_index))!=-1) {
        if (_DEBUG_) std::cerr << "long-opt-name: " << long_options[option_index].name << std::endl;
        switch(c) {
            case 0:
                if (long_options[option_index].flag != 0){
                    if (_DEBUG_)
                        std::cerr << "(long_options[option_index].flag != 0)\n";
                    break;
                }
            case 1000:
                if (_DEBUG_)
                    std::cerr << "No Jumper-Columns in table\n";
                xAddJumpers = false;
                break;
            case 1001:
                if (_DEBUG_)
                    std::cerr << "No Function-Column in table\n";
                xAddFunction = false;
                break;
            case 1002:
                if (_DEBUG_)
                    std::cerr << "No Protocol-Column in table\n";
                xAddProtocol = false;
                break;
            case 1003:
                if (_DEBUG_)
                    std::cerr << "No Text-Column in table\n";
                xAddText = false;
                break;
            case 'c':
                if (_DEBUG_)
                    std::cerr << "Add Comment-Column to table\n";
                xAddCommentCol = true;
                break;
            case 'i':
                if (_DEBUG_)
                    std::cerr << "Read from STDIN\n";
                xReadFromStdIn = true;
                break;
            case 'h':
                if (_DEBUG_)
                    std::cerr << "Help wanted...\n";
                xPrintHelp = true;
                break;
            case 'l':
                if (_DEBUG_)
                    std::cerr << "Output only terminals with label\n";
                xOnlyWithLabel = true;
                break;
            case 'n':
                if (_DEBUG_)
                    std::cerr << "Add column \"consecutive number\" to table\n";
                xAddNumberCol = true;
                break;
            case 'f':
                if (_DEBUG_)
                    std::cerr << "use filename \"" << optarg << "\"\n";
                QETFile = std::string(optarg);
                break;
            case '?':
                std::cerr << " * * * there were non-handled option(s)!"<< std::endl;
                xStopWithError = true;
                break;
        }
    }

    if (_DEBUG_) std::cerr << "optind=" << optind << "; argc="<<argc << std::endl;
    // are there "non-option-Arguments"?
    int iDiff = argc - optind;
    if (optind < argc) {
        if (_DEBUG_) std::cerr << "non-option ARGV-elements:" << std::endl;
        while (optind < argc) {
            if (_DEBUG_) std::cerr << optind << ": " << argv[optind] << std::endl;
            optind++;
        }
        if (_DEBUG_) std::cerr << std::endl;
    }
    // return number of non-option-arguments:
    return iDiff;
}
/******************************************************************************/


/******************************************************************************/
void PrintHelp(const std::string &s, const std::string &v){
    std::stringstream ss;
    ss << std::filesystem::path(s).filename();
    std::string sExeName = ss.str();
    std::cout << std::endl
    << sExeName << " version " << v << " needs arguments!" << std::endl
    << std::endl
    << "usage:" << std::endl
    << sExeName << "  [options]  FILENAME" << std::endl
    << std::endl
    << "   -i | --stdin     input-data is read from stdin,     \n"
    << "                    a given filename is ignored        \n"
    << "   -f FILENAME      or                                 \n"
    << "   --file FILENAME  the file that will be used         \n"
    << "   -c | --comment   add comment-column to table        \n"
    << "   -l | --label     only add terminals with label      \n"
    << "   -n | --number    add column with consecutive number \n"
    << "   --NoJumpers      suppress jumper-columns            \n"
    << "   --NoFunction     suppress function-column           \n"
    << "   --NoText         suppress text-column               \n"
    << "   --NoProtocol     suppress protocol-column           \n"
    << "   -h | --help      show this help                     \n"
    << std::endl
    << "As always with free software: Use it at your own risk!\n\n";
}
/******************************************************************************/




#endif  //#ifndef MAIN_H
