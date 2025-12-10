# Compiler Design Project - Terminal Command Reference

## Project Location
```
C:\mingw64\Cmsys64mingw64bin\compiler_design_project\
```

---

## 1. BUILD COMMANDS

### Build Lexer & Parser
```powershell
cd 'C:\mingw64\Cmsys64mingw64bin\compiler_design_project'
.\build_lexer_parser.bat
```
**Output:** Compiles `project_lexer.exe` and `project_parser.exe` using MSVC

### Build Individual Test Programs
```powershell
.\compile_test3.bat
.\compile_test4.bat
.\compile_test5.bat
.\compile_test6.bat
```
**Output:** Compiles `test3.exe`, `test4.exe`, etc. using MSVC

### Build test7.c
```powershell
.\compile_test7.bat
```
**Output:** Compiles `test7.exe` (fixed from DSL to valid C syntax)

---

## 2. LEXER COMMANDS

### Tokenize a Single File
```powershell
.\project_lexer.exe test_input.txt
.\project_lexer.exe test1.c
.\project_lexer.exe test2.c
.\project_lexer.exe test3.c
.\project_lexer.exe test4.c
.\project_lexer.exe test5.c
.\project_lexer.exe test6.c
.\project_lexer.exe nfa.dfa.c
.\project_lexer.exe input.c
```
**Output:** Token stream and detailed token listing

### Save Lexer Output to File
```powershell
.\project_lexer.exe test_input.txt > lexer_test_input_full.txt
.\project_lexer.exe nfa.dfa.c > lexer_nfa_dfa_full.txt
.\project_lexer.exe input.c > lexer_input.txt
```

---

## 3. PARSER COMMANDS

### Validate a Single File
```powershell
.\project_parser.exe test_input.txt
.\project_parser.exe test1.c
.\project_parser.exe test2.c
.\project_parser.exe test3.c
.\project_parser.exe test4.c
.\project_parser.exe test5.c
.\project_parser.exe test6.c
.\project_parser.exe nfa.dfa.c
.\project_parser.exe input.c
```
**Output:** `PARSE SUCCESS: Program ACCEPTED` or error message

### Save Parser Output to File
```powershell
.\project_parser.exe test_input.txt > parser_test_input.txt
.\project_parser.exe nfa.dfa.c > parser_nfa_dfa.txt
.\project_parser.exe input.c > parser_input.txt
```

---

## 4. RUN COMPILED EXECUTABLES

### Run Test Programs
```powershell
.\test1.exe
.\test2.exe
.\test3.exe
.\test4.exe
.\test5.exe
.\test6.exe
.\test7.exe
```
**Output:** Program-specific output (e.g., "Result: 15", "Loop iteration: 0")

### Run NFA/DFA Programs
```powershell
.\build\Debug\outDebug.exe
```
**Requires:** `input.c` file in project directory
**Output:** Token count statistics (Keywords, Identifiers, Numbers, Operators, Delimiters)

---

## 5. COMBINED WORKFLOW COMMANDS

### Lexer + Parser on Single File
```powershell
.\project_lexer.exe test2.c ; .\project_parser.exe test2.c
```

### Compile + Run Test
```powershell
.\compile_test3.bat ; .\test3.exe
```

### Compile, Analyze, and Run
```powershell
.\compile_test3.bat ; .\test3.exe ; .\project_lexer.exe test3.c ; .\project_parser.exe test3.c
```

### Run All Tests (Lexer + Parser)
```powershell
.\run_all_tests.bat
```
**Output:** Iterates through all test files and shows lexer/parser results for each

---

## 6. TERMINAL DISPLAY COMMANDS

### Show Lexer Output in Terminal (First 60 Lines)
```powershell
.\project_lexer.exe nfa.dfa.c | Select-Object -First 60
```

### Show Both Lexer and Parser in Sequence
```powershell
Write-Host "=== LEXER ===" -ForegroundColor Cyan; .\project_lexer.exe input.c; Write-Host "`n=== PARSER ===" -ForegroundColor Cyan; .\project_parser.exe input.c
```

### Show Token Count
```powershell
Write-Host "=== TOKEN COUNT ===" -ForegroundColor Cyan; .\build\Debug\outDebug.exe
```

---

## 7. FILE VIEWING COMMANDS

### View Saved Outputs
```powershell
notepad .\lexer_test_input.txt
notepad .\parser_test_input.txt
notepad .\lexer_nfa_dfa.txt
notepad .\parser_nfa_dfa.txt
```

### View Source Files
```powershell
notepad .\input.c
notepad .\test_input.txt
notepad .\nfa.dfa.c
```

---

## 8. DIRECTORY NAVIGATION

### Navigate to Project Folder
```powershell
cd 'C:\mingw64\Cmsys64mingw64bin\compiler_design_project'
Set-Location 'C:\mingw64\Cmsys64mingw64bin\compiler_design_project'
```

### List All Files
```powershell
Get-ChildItem
ls
dir
```

### List Only Executables
```powershell
Get-ChildItem -Filter "*.exe"
```

---

## 9. MSVC ENVIRONMENT SETUP (If Needed)

### Set Up MSVC Environment and Compile
```powershell
cmd /c "call ""C:\Program Files\Microsoft Visual Studio\2022\Community\VC\Auxiliary\Build\vcvarsall.bat"" x64 && cl.exe input.c /Feinput.exe /W4"
```

### Quick Compile with MSVC
```powershell
cmd /c "call vcvarsall.bat x64 && cl.exe test7.c /Fetest7.exe && test7.exe"
```

---

## 10. QUICK REFERENCE - MOST USED COMMANDS

### 1. Rebuild Lexer & Parser
```powershell
.\build_lexer_parser.bat
```

### 2. Analyze a File (Lexer + Parser)
```powershell
.\project_lexer.exe nfa.dfa.c
.\project_parser.exe nfa.dfa.c
```

### 3. Count Tokens in input.c
```powershell
.\build\Debug\outDebug.exe
```

### 4. Run All Tests
```powershell
.\run_all_tests.bat
```

### 5. Compile a Test Program
```powershell
.\compile_test3.bat
.\test3.exe
```

---

## FILES REFERENCED IN COMMANDS

| File | Type | Purpose |
|------|------|---------|
| `project_lexer.exe` | Executable | Tokenizes source code |
| `project_parser.exe` | Executable | Validates program structure |
| `test1.c` - `test6.c` | Source | Test programs |
| `nfa.dfa.c` | Source | NFA/DFA example |
| `input.c` | Source | Input for token counter |
| `test_input.txt` | Source | DSL test syntax |
| `build\Debug\outDebug.exe` | Executable | Token counter/analyzer |
| `*.bat` | Script | Build and test scripts |

---

## NOTES

- All commands assume you are in the project directory: `C:\mingw64\Cmsys64mingw64bin\compiler_design_project\`
- MSVC compiler is required (Visual Studio 2022 Community Edition)
- Use `-ForegroundColor Cyan` in PowerShell for colored output
- Redirect output with `> filename.txt` to save to file
- Use `|` to pipe output (e.g., `Select-Object -First N`)
- Commands use `.bat` scripts which internally call `vcvarsall.bat` for MSVC environment setup

