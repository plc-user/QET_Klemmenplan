#!/bin/bash

# create sub-directories
mkdir -p obj/inc/pugixml

# delete the old stuff:
rm obj/inc/pugixml/*.o  &> /dev/null
rm obj/inc/*.o          &> /dev/null
rm obj/*.o              &> /dev/null
rm QET_Klemmenplan      &> /dev/null

echo ""
echo ""
echo ""
echo "--------------------------------------------------------------------------------"

#CompileOptions="-Wall -Wextra -Wpedantic -Werror -Wunused-variable -Wmaybe-uninitialized -std=c++17 -O2"
CompileOptions="-Wall -Wpedantic -std=c++17 -O2"

# compile
g++ $CompileOptions  -c inc/pugixml/pugixml.cpp -o obj/inc/pugixml/pugixml.o
g++ $CompileOptions  -c inc/helpers.cpp -o obj/inc/helpers.o
g++ $CompileOptions  -c inc/parts.cpp -o obj/inc/parts.o
g++ $CompileOptions  -c main.cpp -o obj/main.o

# link:
g++  -o QET_Klemmenplan \
        obj/inc/pugixml/pugixml.o \
        obj/inc/helpers.o \
        obj/inc/parts.o \
        obj/main.o  \
     -s

# delete object-files again:
rm obj/inc/pugixml/*.o  &> /dev/null
rm obj/inc/*.o          &> /dev/null
rm obj/*.o              &> /dev/null

echo "--------------------------------------------------------------------------------"
echo ""
echo ""
echo ""
