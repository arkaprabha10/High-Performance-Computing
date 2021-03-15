#!/bin/bash
#
g++ -c emps.C >& compiler.txt
if [ $? -ne 0 ]; then
  echo "Errors compiling emps.C."
  exit
fi
rm compiler.txt
#
g++ emps.o
if [ $? -ne 0 ]; then
  echo "Errors loading emps.o."
  exit
fi
#
rm emps.o
#
chmod ugo+x a.out
mv a.out ~/bincpp/$ARCH/emps
#
echo "A new version of emps has been created."
