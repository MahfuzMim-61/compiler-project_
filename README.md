# Lexer & Parser Project - Complete Documentation

## Project Overview
This project contains a custom language lexer, parser, and C test programs compiled with MSVC.

---

## 1. LEXER (project_lexer.exe)

**Purpose**: Tokenizes custom language source files

**Custom Language Syntax Rules**:
- First line: `#include<stdio.h>` or `#include <stdio.h>`
- Comments: `// letters and spaces only`
- Variable names: `_[A-Za-z]+[0-9][A-Za-z]` (e.g., `_var1a`, `_num2b`)
- Function names: `[A-Za-z]+Fn` (e.g., `testFn`, `computeFn`)
- Types: `int` or `dec`
- Statement terminator: `..` (double dot)
- Loop labels: `loop_main01:` pattern
- Keywords: `while`, `printf`, `return`, `break`

**Usage**:
```bash
.\project_lexer.exe <source-file>
```

**Example Output**:
```
INCLUDE              : #include<stdio.h>
COMMENT              :  This is a simple test
TYPE                 : int
VAR                  : _var1a
STMT_END             : ..
FUNC_NAME            : testFn
MAIN                 : main
```

---

## 2. PARSER (project_parser.exe)

**Purpose**: Validates custom language syntax and structure

**Validation Rules**:
- INCLUDE statement must be first line
- Variable declarations must have `..` terminator
- printf statements: `printf(_variable)..`
- while loops: `while (type var < number..) { ... }`
- Function names must follow pattern
- main() function must exist

**Usage**:
```bash
.\project_parser.exe <source-file>
```

**Example Output**:
```
PARSE SUCCESS: Program ACCEPTED
```

or

```
PARSE ERROR: structure validation failed
```

---

## 3. STANDARD C PROGRAMS

### test1.c
Demonstrates:
- Function definition and calls
- Loops
- Integer arithmetic

**Source Code**:
```c
#include<stdio.h>

int computeValueFn(int _val1a) {
    int _temp2x = _val1a + 5;
    return _temp2x;
}

int main() {
    int _input3k = 10;
    int _result4m = computeValueFn(_input3k);

    int _loopin0x = 0;
    while (_loopin0x < 3) {
        printf("Result: %d\n", _result4m);
        _loopin0x++;
    }

    return 0;
}
```

**Compilation**:
```bash
cl.exe test1.c /Fetest1.exe
```

**Output**:
```
Result: 15
Result: 15
Result: 15
```

---

### test2.c
Demonstrates:
- Variable initialization
- Loop iteration
- Formatted output

**Source Code**:
```c
#include<stdio.h>

int main() {
    int _input3k = 10;
    int _result4m = 2;

    int _m7x = 0;
    while (_m7x < 3) {
        printf("Loop iteration: %d\n", _m7x);
        _m7x++;
    }

    return 0;
}
```

**Compilation**:
```bash
cl.exe test2.c /Fetest2.exe
```

**Output**:
```
Loop iteration: 0
Loop iteration: 1
Loop iteration: 2
```

---

## 4. TEST INPUT FILE (test_input.txt)

**Format**: Custom language syntax for testing lexer/parser

**Content**:
```
#include<stdio.h>
// This is a simple test
int _var1a..
dec _num2b..
testFn() { }
main() { }
```

**Lexer Output**:
```
INCLUDE              : #include<stdio.h>
COMMENT              :  This is a simple test
TYPE                 : int
VAR                  : _var1a
STMT_END             : ..
TYPE                 : dec
VAR                  : _num2b
STMT_END             : ..
FUNC_NAME            : testFn
SYM(()
SYM())
SYM({)
SYM(})
MAIN                 : main
SYM(()
SYM())
SYM({)
SYM(})

==== TOKEN STREAM ====
INCLUDE COMMENT TYPE VAR STMT_END TYPE VAR STMT_END FUNC_NAME SYM(()
SYM()) SYM({) SYM(}) MAIN SYM(() SYM()) SYM({) SYM(})
==== END TOKEN STREAM ====
```

**Parser Output**:
```
PARSE SUCCESS: Program ACCEPTED
```

---

## 5. COMPILATION & BUILD SETUP

### Compiler Configuration
- **Compiler**: MSVC (cl.exe) from Visual Studio 2022
- **Architecture**: x64
- **Batch Setup**: `C:\Program Files\Microsoft Visual Studio\2022\Community\VC\Auxiliary\Build\vcvarsall.bat x64`

### VS Code Tasks (tasks.json)
Two build tasks configured:
1. **C/C++: compile with MSVC** - Compiles single file
2. **run executable** - Runs the compiled executable

### VS Code Settings (settings.json)
- MSVC configured as default compiler
- Proper warning flags (/W4)
- Debugging support with cdb.exe

---

## 6. FILES IN PROJECT

| File | Type | Description |
|------|------|-------------|
| project_lexer.c | Source | Custom language tokenizer |
| project_lexer.exe | Executable | Compiled lexer |
| project_parser.c | Source | Custom language validator |
| project_parser.exe | Executable | Compiled parser |
| test1.c | Source | Test program with function |
| test1.exe | Executable | Compiled test1 |
| test2.c | Source | Test program with loop |
| test2.exe | Executable | Compiled test2 |
| test_input.txt | Data | Sample custom language file |

---

## 7. QUICK START GUIDE

### Compile C Programs
```bash
cd "c:\mingw64\Cmsys64mingw64bin\New folder\"
cl.exe test1.c /Fetest1.exe
cl.exe test2.c /Fetest2.exe
```

### Run C Programs
```bash
.\test1.exe
.\test2.exe
```

### Test Custom Language
```bash
.\project_lexer.exe test_input.txt
.\project_parser.exe test_input.txt
```

### Full Build with Environment Setup
```bash
cmd /c "call "C:\Program Files\Microsoft Visual Studio\2022\Community\VC\Auxiliary\Build\vcvarsall.bat" x64 && cl.exe test1.c /Fetest1.exe && test1.exe"
```

---

## 8. SYSTEM STATUS

✅ **All systems fully functional**:
- Custom language lexer working
- Custom language parser working
- C compiler (MSVC) configured correctly
- All test programs compile and run
- All output verified

---

## 9. TROUBLESHOOTING

### If compilation fails with "'cl' is not recognized":
Solution: Run with full MSVC environment setup
```bash
cmd /c "call "C:\Program Files\Microsoft Visual Studio\2022\Community\VC\Auxiliary\Build\vcvarsall.bat" x64 && cl.exe <file>"
```

### If custom language validation fails:
- Check variable names follow pattern: `_[A-Za-z]+[0-9][A-Za-z]`
- Ensure all statements end with `..` (double dot)
- Verify first line is `#include<stdio.h>`

---

## 10. PROJECT GOALS ACHIEVED

✓ Lexical analyzer implemented and working
✓ Parser validator implemented and working
✓ Custom language syntax fully functional
✓ C compilation environment configured
✓ Test programs created and tested
✓ All output verified and documented
✓ Build system properly configured with MSVC
✓ No GCC flags conflicts
✓ Complete documentation provided

---

**Project Status**: COMPLETE ✅
**All Components**: WORKING ✅
**Documentation**: COMPREHENSIVE ✅
