@echo off

IF NOT EXIST ../../../build_console_win mkdir ..\..\..\build_console_win
pushd ..\..\..\build_console_win
cl ..\snake\snake_console_win\code\cpp_snake.cpp
popd
