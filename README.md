# QET_Klemmenplan

&quot;QET_Klemmenplan&quot; erstellt aus den Klemmen (Terminals) in QElectroTech - Schaltplan-Dateien (*.qet) eine Tabelle pro Klemmleiste im HTML-Format.
Die Ausgabe des Programms kann anschließend mit Hilfe eines Texteditors (Notepad++, Mousepad, Kate, etc.) und dem QET-internen Quelltexteditor in ein Textfeld eingefügt und weiter bearbeitet werden.

allgemeiner Aufruf auf der Kommandozeile mit Ausgabe auf stdout:

```
QET_Klemmenplan Schaltplan.qet
```

Eine Ausgabe-Umleitung in eine Datei funktioniert selbstverständlich auch: So kann die Ausgabe mit einem Editor noch angepasst werden, falls nötig:

```
QET_Klemmenplan Schaltplan.qet > Klemmen.txt
```

Vollständige Liste der verfügbaren Optionen:
```
usage:
"QET_Klemmenplan"  [options]  FILENAME

   -i | --stdin     input-data is read from stdin, a given
                    filename is ignored
   -f FILENAME      or
   --file FILENAME  the file that will be used
   -c | --comment   add comment-column to table
   -l | --label     only add terminals with label
   -h | --help      show this help
```

Das Einfügen in den Schaltplan von QElectroTech geht wie folgt:

*  Im Schaltplaneditor von QElectroTech ein Textfeld anlegen.
*  Textfeld markieren und "Fortgeschrittener Editor" aufrufen.
*  TAB "Quelltext" öffnen, kompletten Inhalt auswählen <Strg>+A und löschen!
*  Datei "Klemmen.txt" mit externem Texteditor öffnen, den kompletten Inhalt markieren <Strg>+A und in die Zwischenablage kopieren <Strg>+C
*  zum Quelltexteditor von QET zurückwechseln und den Inhalt der Zwischenablage einfügen <Strg>+V
*  Nach Schließen des Dialogs mit "OK" erscheint im Textfeld eine Tabelle mit den Klemmleisten.


QET_Klemmenplan uses Arseny Kapoulkine's &quot;pugixml&quot; (https://github.com/zeux/pugixml)
to handle the content of the Element-File. Thank you, Arseny! <br>
 <br>
Many thanks, of course, to the developers and contributors of QElectroTech – a free software to create electric diagrams. (https://qelectrotech.org) <br>
