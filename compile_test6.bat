.\project_lexer.exe test1.c ; .\project_parser.exe test1.c@echo off
call "C:\Program Files\Microsoft Visual Studio\2022\Community\VC\Auxiliary\Build\vcvarsall.bat" x64
cl.exe "test6.c" /Fetest6.exe /W4 /std:c11
