@echo off
setlocal enabledelayedexpansion

echo ============================================
echo COMPILER DESIGN PROJECT - LEXER & PARSER
echo ============================================
echo.

cd /d "%~dp0"

echo Running Lexer and Parser on all test files...
echo.

for %%f in (test_input.txt test1.c test2.c test3.c test4.c test5.c test6.c nfa.dfa.c) do (
    if exist %%f (
        echo.
        echo ============================================
        echo FILE: %%f
        echo ============================================
        echo.
        echo --- LEXER OUTPUT ---
        call .\project_lexer.exe %%f
        echo.
        echo --- PARSER OUTPUT ---
        call .\project_parser.exe %%f
        echo.
    ) else (
        echo [SKIP] File not found: %%f
        echo.
    )
)

echo.
echo ============================================
echo All tests complete.
echo ============================================
pause
