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


// Extrahiert aus einer QET-Datei die Klemmen und gibt sie sortiert auf STDOUT aus.
// Dabei ist es unerheblich, wie die Klemmen im Schaltplan bezeichnet sind:
// Alle Elemente, die als "Klemme" deklariert sind, werden in die Liste aufgenommen.

//
// QET_Klemmenplan is a commandline-tool to extract
// terminals (Klemmen) from Schematic-Files of QET
//
// usage:
// QET_Klemmenplan [options] <file>
//
// compiles with C++17 enabled on
// - Debian/GNU Linux (un-)stable
// - ReactOS (0.4.16-dev)
// - ???
// (see compile.sh and/or compile.cmd)
//
//
// +----------------------------------------------------------+
// | This software uses pugixml library (https://pugixml.org) |
// | pugixml is Copyright (C) 2006-2023 Arseny Kapoulkine.    |
// +----------------------------------------------------------+
//

// own headers:
#include "inc/pugixml/pugixml.hpp"
#include "inc/helpers.h"
#include "inc/parts.h"
#include "main.h"

// In den Folios (diagrams) interessieren uns nur die Klemmen:
    std::vector<Diagram> vDiagrams;
    std::vector<Klemme>  vKlemmen;

// ============================================================================
// implementation
// ============================================================================

int main(int argc, char **argv)
{
    // read and check the Commandline-Parameters
    int iRetVal = parseCommandline(argc, argv);
    if (_DEBUG_) std::cerr << "iRetVal: " << iRetVal << std::endl;

    if (xPrintHelp == true) {
        PrintHelp(argv[0], sVersion);
        return 0;
    }

    if (xStopWithError == true) {
        std::cerr << "check options and try again!" << std::endl;
        return -1;
    }

    // load Element from stdin or XML-File
    pugi::xml_document doc;
    pugi::xml_parse_result result;
    if (xReadFromStdIn){
        result = doc.load(std::cin);
    } else {
        if ((QETFile != "") && (std::filesystem::exists(QETFile))) {
          //ElementFile = argv[1];
          result = doc.load_file(QETFile.c_str());
        } else
        if ((argc>1)&&(std::filesystem::exists(argv[argc-1]))) {
          QETFile = argv[argc-1];
          result = doc.load_file(QETFile.c_str());
        } else
        {
            PrintHelp(argv[0], sVersion);
            return -1;
        }
    }
    // check the result of "doc.load"-Function
    if (!result){
        std::cerr << "file \"" << QETFile << "\" could not be loaded: " << result.description() << std::endl;
        return -1;
    } else {
        if (_DEBUG_) std::cerr << "QET-File loaded successfully.\n";
    }

// Ein QET-Projekt hat ein paar Eigenschaften:
    Property ProjectProperties;
    ProjectProperties.SetTitle(doc.child("project").attribute("title").value());
    ProjectProperties.ReadProperties(doc.child("project").child("properties"));

// durch die diagrams gehen, um die Elemente und Leiter zu lesen
    node = doc.child("project");
    for (pugi::xml_node dnode: node.children("diagram")) {
        Diagram TempDiagram;
        TempDiagram.ReadFromNode(dnode);
        //std::cout << "Diagram-Eigenschaften: " << TempDiagram << std::endl;
        vDiagrams.push_back(TempDiagram);
        // Elemente des Diagramms in einer Schleife lesen:
        for (pugi::xml_node enode: dnode.child("elements").children("element")) {
            // alle Klemmen separat speichern:
            if (enode.child("properties")) {
                Klemme TempKlemme;
                TempKlemme.uuid = enode.attribute("uuid").as_string();
                TempKlemme.folio = TempDiagram.GetOrder();
                TempKlemme.fkt = enode.child("properties").child("element_type").attribute("terminal_function").as_string();
                TempKlemme.led = enode.child("properties").child("element_type").attribute("terminal_led").as_bool();
                TempKlemme.typ = enode.child("properties").child("element_type").attribute("terminal_type").as_string();
                TempKlemme.col = enode.attribute("x").as_uint() / TempDiagram.uiColSize + 1;
                TempKlemme.row = enode.attribute("y").as_uint() / TempDiagram.uiRowSize + 1;
                // die dynamischen Texte durchgehen und das Label finden:
                for (pugi::xml_node dtnode: enode.child("dynamic_texts").children("dynamic_elmt_text")) {
                    //std::cout << "--> dynamic_elmt_text\n";
                    if (std::string(dtnode.child_value("info_name")) == "label") {
                        TempKlemme.lbl = dtnode.child_value("text");
                        //std::cout << "--> found label\n";
                    }
                }
                // die Anschlüsse der Klemme zuordnen:
                for (pugi::xml_node knode: enode.child("terminals").children("terminal")) {
                    KlemmenAnschluss Anschluss;
                    Anschluss.uiNummer = knode.attribute("id").as_uint();
                    // pro Anschluss das diagram durchgehen und potenzial und funktion der Leiter bestimmen:
                    for (pugi::xml_node cnode: dnode.child("conductors").children("conductor")) {
                        //std::cout << "conductor: " << cnode.attribute("element1").value() << "\n";
                        if ((TempKlemme.uuid == std::string(cnode.attribute("element1").value())) ||
                            (cnode.attribute("element2").value() == TempKlemme.uuid)) {
                            //std::cout << "found UUID\n";
                            Anschluss.sFunktion    = cnode.attribute("function").value();
                            Anschluss.sText        = cnode.attribute("num").value();
                            Anschluss.sProtokoll   = cnode.attribute("tension_protocol").value();
                            Anschluss.sQuerschnitt = cnode.attribute("conductor_section").value();
                            Anschluss.sFarbe       = cnode.attribute("conductor_color").value();
                            Anschluss.sKabel       = cnode.attribute("cable").value();
                        }
                    } // Conductors des Diagramms eingelesen
                    TempKlemme.vAnschluss.push_back(Anschluss);
                }
                if ((xOnlyWithLabel == true) && (TempKlemme.lbl == "")) {
                    ; // ich soll keine "leeren" Klemmen speichern...
                } else {
                    //std::cout << TempKlemme << "\n";
                    vKlemmen.push_back(TempKlemme);
                }
            } // "properties" der Klemme gelesen
        } // Elemente des Diagramms eingelesen
    } // Diagrams eingelesen

    // wenn es keine Klemmen in den Vector geschafft haben, ist hier schon Schluß!
    if (vKlemmen.empty()) {
        std::cerr << "\n * * * No (labelled) terminals found!\n\n";
        return 0;
    }

    // wir sortieren zuerst nach Klemmleiste:
   	std::sort(vKlemmen.begin(), vKlemmen.end(),
		[](const auto& lhs, const auto& rhs) {
             return left_trim(lhs.lbl) < left_trim(rhs.lbl);
             });

    // die Anfänge einer neuen Klemme finden:
    std::vector<size_t> vBlockStart;
    if (vKlemmen.size() > 1) {
        vBlockStart.push_back(0);
        for (size_t i=0; i < (vKlemmen.size()-1); i++) {
            if ((LeftOfInterpunct(vKlemmen[i].lbl) != LeftOfInterpunct(vKlemmen[i+1].lbl)))
                vBlockStart.push_back(i+1);
        }
        vBlockStart.push_back(vKlemmen.size());
    }


    // Die einzelnen Blöcke sortieren:
    for (size_t n=0; n < (vBlockStart.size()-1); n++) {
        //std::cerr << "sortiere Klemmen " << vBlockStart[n] << " .. " << vBlockStart[n+1]-1 << "\n";
        std::sort(vKlemmen.begin()+vBlockStart[n], vKlemmen.begin()+vBlockStart[n+1],
                [](const auto& lhs, const auto& rhs) { return zero_padding(lhs.lbl) < zero_padding(rhs.lbl); });
    }



    if (vKlemmen.size() > 0) {
        // alle Klemmen sortiert ausgeben:
        std::cout << sCSSstyle    << "\n\n";
        // Tabellen-Header und Spalten-Überschriften:
        std::cout << sTableHeader() << "\n";
        for (size_t i = 0; i < vKlemmen.size(); i++) {
            // HTML-Ausgabe einer Tabellenzeile:
            std::cout << "  <tr>";
            std::cout << sTDtbold(vKlemmen[i].lbl); //<< "Label: "
            std::cout << sTDtext(std::to_string(vKlemmen[i].folio) + "-"
                                 + NumberToLetters(vKlemmen[i].row)
                                 + std::to_string(vKlemmen[i].col)); //<< "Position: "
            std::cout << sTDctext("&nbsp;") << sTDctext("&nbsp;") << sTDctext("&nbsp;") << sTDctext("&nbsp;"); //<< "Brücker: "
            std::cout << sTDtext(vKlemmen[i].vAnschluss[0].sFunktion); //<< "Funktion: "
            std::cout << sTDtext(vKlemmen[i].vAnschluss[0].sText); //<< "Text: "
            std::cout << sTDtext(vKlemmen[i].vAnschluss[0].sProtokoll); //<< "Protokoll: "
            std::cout << (xAddCommentCol ? sTDtext("") : ""); // Kommentar nur auf Anfrage
            std::cout << "</tr>\n"; // Ende der Tabellenzeile
            // ENDE - HTML-Ausgabe

            // gibt es weitere Klemmen und ist das ein neuer Block?
            // Dann hier eine neue Tabelle anfangen lassen!
            if ((i < (vKlemmen.size()-1)) && (LeftOfInterpunct(vKlemmen[i].lbl) != LeftOfInterpunct(vKlemmen[i+1].lbl))) {
                std::cout << sStartNewTable() << "\n";
            }
        }
        std::cout << sTableFooter << "\n";
    } else {
        std::cout << "* * * no terminals found * * *\n";
    }

    return 0;
}
