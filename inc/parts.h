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

#ifndef PARTS_H
#define PARTS_H

#include <iostream>     // for IO-Operations
#include <cstdint>      // int8_t, ...
#include <vector>       //
#include <algorithm>

#include "pugixml/pugixml.hpp"

//#define _DEBUG_ 0

/*
grobe Struktur einer QET-Datei
<project
  <properties                  // Name, Datum, etc. des Projekts
  <newdiagrams                 // für hier zur Zeit uninteressant
  <diagram                     // entspricht EINEM "Folio" = Schaltplanseite
    <elements                  // Obergruppe enthält die Elemente des Folios
      <element                 // "Instanz" eines Elements aus der importierten Collection
      ...
    </elements
    <conductors                // Obergruppe: Verbindungen innerhalb des Folios
      <conductor               // Verbindungsleitung
      ...
    </conductors
  </diagram
  (weitere "<diagram")
  <collection                  // importierte Elemente der Bauteilsammlung. Kann beliebig tief verschachtelt sein!
    <category
</project
*/

//
// --- function-prototypes for ... ---------------------------------------------
//


//
//--- START - definition of Properties of qet-file -----------------------------
//
class Property {
    private:
        //
    protected:
        std::string sSaveDate    {"01.01.00"};
        std::string sSaveTime    {"00:00"};
        std::string sTitle       {"unnamed"};
        std::string sFileName    {"unnamed"};
        std::string sFilePath    {"unnamed.qet"};
    public:
        void SetTitle(std::string s) { sTitle = s; }
        void ReadProperties(pugi::xml_node);
};
//
//--- END - definition of Properties of qet-file -------------------------------
//


//
//--- START - definition of Diagram in qet-file --------------------------------
//
class Diagram {
    private:
    protected:
        std::string sAuthor           {""};
        std::string sDate             {""};
        std::string sIndexrev         {""};
        std::string sLocmach          {""};
        uint64_t    uiOrder           { 0 };
        std::string sPlant            {""};
        std::string sTitle            {""};
        std::string sDefCondSection   {""};
        std::string sDefCondColor     {""};
    public:
        uint64_t    uiCols            { 0 };
        uint64_t    uiColSize         { 0 };
        uint64_t    uiRows            { 0 };
        uint64_t    uiRowSize         { 0 };
        std::string GetTitle(void) { return sTitle; }
        std::string GetDefCondSection(void) { return sDefCondSection; }
        std::string GetDefCondColor(void) { return sDefCondColor; }
        uint64_t    GetOrder(void) { return uiOrder; }
        void        ReadFromNode(pugi::xml_node);
        void        SetTitle(std::string s) { sTitle = s; }
};
//
//--- END - definition of Diagram in qet-file ----------------------------------
//



//
//--- START - definition of Klemme in qet-file ---------------------------------
//
struct KlemmenAnschluss {
    uint64_t    uiNummer     {0};
    std::string sFunktion    {""}; // function
    std::string sText        {""}; // num
    std::string sProtokoll   {""}; // tension_protocol
    std::string sQuerschnitt {""}; // conductor_section
    std::string sFarbe       {""}; // conductor_color
    std::string sKabel       {""}; // cable
};
//
class Klemme {
    private:
        //
    protected:
        //
    public:
        uint64_t                col;
        uint64_t                row;
        std::string             uuid;
        uint64_t                folio;
        std::string             typ;
        bool                    led;
	    std::string             fkt;
	    std::string             lbl;
        std::vector<KlemmenAnschluss>   vAnschluss;
};
//
//--- END - definition of Klemme in qet-file -----------------------------------
//



#endif  //#ifndef PARTS_H
