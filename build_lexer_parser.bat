@echo off
call "C:\Program Files\Microsoft Visual Studio\2022\Community\VC\Auxiliary\Build\vcvarsall.bat" x64
cl.exe "project_lexer.c" /Feproject_lexer.exe /W4 /std:c11
cl.exe "project_parser.c" /Feproject_parser.exe /W4 /std:c11
