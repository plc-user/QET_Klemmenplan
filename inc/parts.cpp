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

#include "helpers.h"
#include "parts.h"

#include <sstream>      // for String-Streams


//
//--- START - implementation of Property in qet-file ---------------------------
//
void Property::ReadProperties(const pugi::xml_node node){
    for (pugi::xml_node node: node.children("property"))
    {
        for (pugi::xml_attribute attr: node.attributes())
        {
            if ((std::string)attr.value() == "saveddate")     sSaveDate = node.child_value();
            if ((std::string)attr.value() == "savedtime")     sSaveTime = node.child_value();
            if ((std::string)attr.value() == "savedfilename") sFileName = node.child_value();
            if ((std::string)attr.value() == "savedfilepath") sFilePath = node.child_value();
        }
    }
}
//
//--- END - implementation of Property in qet-file -----------------------------
//



//
//--- START - implementation of Diagram in qet-file ----------------------------
//
void Diagram::ReadFromNode(const pugi::xml_node node){
    sAuthor   = node.attribute("author").value();
    sDate     = node.attribute("date").value();
    sIndexrev = node.attribute("indexrev").value();
    sLocmach  = node.attribute("locmach").value();
    uiOrder   = node.attribute("order").as_uint();
    sPlant    = node.attribute("plant").value();
    sTitle    = node.attribute("title").value();
    uiCols    = node.attribute("cols").as_uint();
    uiColSize = node.attribute("colsize").as_uint();
    uiRows    = node.attribute("rows").as_uint();
    uiRowSize = node.attribute("rowsize").as_uint();
    sDefCondSection = node.child("defaultconductor").attribute("conductor_section").as_string();
    sDefCondColor = node.child("defaultconductor").attribute("conductor_color").as_string();
}
//
//--- END - implementation of Diagram in qet-file ------------------------------
//



/****************************************************************************/
/****************************************************************************/
/****************************************************************************/
