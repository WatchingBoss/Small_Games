@echo off

IF NOT EXIST ../../build mkdir ..\..\build
pushd ..\..\build
cl -Zi ..\sudoku\code\sudoku.c
popd


