#ifndef PROJECT_LEXER_C
#define PROJECT_LEXER_C
/* project_lexer.c
    MSVC-compatible C89 version
    Validates custom language syntax and produces token stream
*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define MAXLINE 1024
#define MAXTOK 1024

int is_variable(const char *s)
{
    int n, i;
    n = strlen(s);
    if (n < 4) return 0;
    if (s[0] != '_') return 0;
    i = 1;
    if (!isalpha(s[i])) return 0;
    while (i < n && isalpha(s[i])) i++;
    if (i >= n || !isdigit(s[i])) return 0;
    i++;
    if (i >= n || !isalpha(s[i])) return 0;
    i++;
    return i == n;
}

int is_function_name(const char *s)
{
    int n, len, i;
    n = strlen(s);
    if (n < 3) return 0;
    len = n - 2;
    if (strcmp(s + len, "Fn") != 0) return 0;
    for (i = 0; i < len; i++)
        if (!isalpha(s[i])) return 0;
    return 1;
}

int is_include_line(const char *s)
{
    char tmp[MAXLINE];
    int n;
    strcpy(tmp, s);
    while (*tmp && isspace((unsigned char)*tmp))
        memmove(tmp, tmp + 1, strlen(tmp));
    n = strlen(tmp);
    while (n > 0 && isspace((unsigned char)tmp[n - 1]))
        tmp[--n] = 0;
    return strcmp(tmp, "#include<stdio.h>") == 0 || 
           strcmp(tmp, "#include <stdio.h>") == 0;
}

int is_comment_line(const char *s)
{
    const char *p;
    p = s;
    while (*p && isspace((unsigned char)*p)) p++;
    if (p[0] == '/' && p[1] == '/') {
        p += 2;
        while (*p) {
            if (!(*p == ' ' || isalpha((unsigned char)*p) || isspace((unsigned char)*p)))
                return 0;
            p++;
        }
        return 1;
    }
    return 0;
}

void emit(const char *tok, const char *lex)
{
    if (lex)
        printf("%-20s : %s\n", tok, lex);
    else
        printf("%s\n", tok);
}

int main(int argc, char **argv)
{
    FILE *f;
    char line[MAXLINE];
    char tmp[1024];
    char id[256];
    char buf[256];
    char num[64];
    char tmp2[16];
    char *s;
    char *ptrim;
    int lineno, ok, saw_main, tokc;
    int allspace, i, j, oklabel, letters;
    char tokens[MAXTOK][256];

    if (argc < 2) {
        printf("Usage: %s <source-file>\n", argv[0]);
        return 1;
    }

    f = fopen(argv[1], "r");
    if (!f) {
        perror("fopen");
        return 1;
    }

    lineno = 0;
    ok = 1;
    saw_main = 0;
    tokc = 0;

    while (fgets(line, sizeof(line), f)) {
        lineno++;
        line[strcspn(line, "\r\n")] = 0;

        if (lineno == 1) {
            if (!is_include_line(line)) {
                printf("Error: first line must be #include<stdio.h>\n");
                ok = 0;
                break;
            }
            emit("INCLUDE", "#include<stdio.h>");
            strcpy(tokens[tokc++], "INCLUDE");
            continue;
        }

        strcpy(tmp, line);
        allspace = 1;
        for (i = 0; i < (int)strlen(tmp); i++) {
            if (!isspace((unsigned char)tmp[i])) {
                allspace = 0;
                break;
            }
        }
        if (allspace) continue;

        ptrim = tmp;
        while (*ptrim && isspace((unsigned char)*ptrim)) ptrim++;
        /* allow other preprocessor/include lines after first line (e.g. #include <string.h>) */
        if (ptrim[0] == '#') {
            /* emit the whole trimmed line as INCLUDE */
            emit("INCLUDE", ptrim);
            strcpy(tokens[tokc++], "INCLUDE");
            continue;
        }
        if (ptrim[0] == '/' && ptrim[1] == '/') {
            if (!is_comment_line(ptrim)) {
                printf("Error: invalid comment at line %d\n", lineno);
                ok = 0;
                break;
            }
            emit("COMMENT", ptrim + 2);
            strcpy(tokens[tokc++], "COMMENT");
            continue;
        }

        s = tmp;
        while (*s) {
            if (isspace((unsigned char)*s)) {
                s++;
                continue;
            }

            if (strncmp(s, "loop_", 5) == 0) {
                i = 0;
                while (s[i] && !isspace((unsigned char)s[i]) && s[i] != '{' && s[i] != '(') {
                    buf[i] = s[i];
                    i++;
                }
                buf[i] = 0;
                if (buf[strlen(buf) - 1] == ':') {
                    oklabel = 1;
                    j = 5;
                    letters = 0;
                    while (buf[j] && isalpha((unsigned char)buf[j])) {
                        letters++;
                        j++;
                    }
                    if (letters < 1) oklabel = 0;
                    if (!(isdigit((unsigned char)buf[j]) && isdigit((unsigned char)buf[j + 1]) &&
                          buf[j + 2] == ':'))
                        oklabel = 0;
                    if (!oklabel) {
                        printf("Error: invalid loop label at line %d\n", lineno);
                        ok = 0;
                        break;
                    }
                    emit("LOOP_LABEL", buf);
                    strcpy(tokens[tokc++], "LOOP_LABEL");
                    s += i;
                    continue;
                }
            }

            if (isalpha((unsigned char)*s) || *s == '_') {
                i = 0;
                while (s[i] && (isalnum((unsigned char)s[i]) || s[i] == '_')) {
                    id[i] = s[i];
                    i++;
                }
                id[i] = 0;
                if (strcmp(id, "int") == 0 || strcmp(id, "dec") == 0) {
                    emit("TYPE", id);
                    strcpy(tokens[tokc++], "TYPE");
                } else if (strcmp(id, "while") == 0) {
                    emit("WHILE", "while");
                    strcpy(tokens[tokc++], "WHILE");
                } else if (strcmp(id, "printf") == 0) {
                    emit("PRINTF", "printf");
                    strcpy(tokens[tokc++], "PRINTF");
                } else if (strcmp(id, "return") == 0) {
                    emit("RETURN", "return");
                    strcpy(tokens[tokc++], "RETURN");
                } else if (strcmp(id, "break") == 0) {
                    emit("BREAK", "break");
                    strcpy(tokens[tokc++], "BREAK");
                } else if (is_function_name(id)) {
                    emit("FUNC_NAME", id);
                    strcpy(tokens[tokc++], "FUNC_NAME");
                } else if (is_variable(id)) {
                    emit("VAR", id);
                    strcpy(tokens[tokc++], "VAR");
                } else if (strcmp(id, "main") == 0) {
                    emit("MAIN", "main");
                    strcpy(tokens[tokc++], "MAIN");
                    saw_main = 1;
                } else {
                    emit("IDENT", id);
                    strcpy(tokens[tokc++], "IDENT");
                }
                s += i;
                continue;
            }

            if (isdigit((unsigned char)*s)) {
                i = 0;
                while (isdigit((unsigned char)s[i])) {
                    num[i] = s[i];
                    i++;
                }
                num[i] = 0;
                emit("NUM", num);
                strcpy(tokens[tokc++], "NUM");
                s += i;
                continue;
            }

            if (s[0] == '.' && s[1] == '.') {
                emit("STMT_END", "..");
                strcpy(tokens[tokc++], "STMT_END");
                s += 2;
                continue;
            }

            /* handle C-style string literals ("...") by capturing them as STRING tokens */
            if (s[0] == '"') {
                int si = 0;
                char strlit[512];
                s++; /* skip opening quote */
                while (*s) {
                    if (*s == '\\' && s[1]) {
                        /* keep escaped char */
                        if (si < (int)sizeof(strlit)-2) {
                            strlit[si++] = *s;
                            strlit[si++] = s[1];
                        }
                        s += 2;
                        continue;
                    }
                    if (*s == '"') { s++; break; }
                    if (si < (int)sizeof(strlit)-1) strlit[si++] = *s;
                    s++;
                }
                strlit[si] = 0;
                emit("STRING", strlit);
                strcpy(tokens[tokc++], "STRING");
                continue;
            }

            /* handle C-style character literals ('a' or '\n') */
            if (s[0] == '\'') {
                int si = 0;
                char charlit[16];
                s++; /* skip opening quote */
                if (*s == '\\' && s[1]) {
                    /* escaped char */
                    charlit[si++] = *s; charlit[si++] = s[1]; s += 2;
                } else if (*s) {
                    charlit[si++] = *s; s++;
                }
                /* skip closing quote if present */
                if (*s == '\'') s++;
                charlit[si] = 0;
                emit("CHAR", charlit);
                strcpy(tokens[tokc++], "CHAR");
                continue;
            }

            /* accept additional C operators/symbols: ? | [ ] and keep existing ones */
            if (strchr("(){}=,+-*/<>;:,?[]|", *s)) {
                snprintf(tmp2, sizeof(tmp2), "SYM(%c)", *s);
                emit(tmp2, NULL);
                strcpy(tokens[tokc++], tmp2);
                s++;
                continue;
            }

            printf("Error: invalid character '%c' at line %d\n", *s, lineno);
            ok = 0;
            break;
        }

        if (!ok) break;
    }

    if (!ok) {
        printf("Lexical analysis failed.\n");
        fclose(f);
        return 1;
    }

    printf("\n==== TOKEN STREAM ====\n");
    for (i = 0; i < tokc; i++) {
        printf("%s ", tokens[i]);
        if ((i + 1) % 10 == 0) printf("\n");
    }
    printf("\n==== END TOKEN STREAM ====\n");

    fclose(f);
    return 0;
}

#endif /* PROJECT_LEXER_C */
