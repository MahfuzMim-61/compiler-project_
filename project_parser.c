/* project_parser.c
   A pragmatic parser/validator that:
   - expects INCLUDE token first
   - checks optional functions before main
   - checks main exists
   - verifies statements end with STMT_END (..)
   - verifies printf and while patterns in a simplified way
   - outputs ACCEPTED or REJECTED
   Note: uses the token textual output of project_lexer (or re-reads source and re-tokenizes)
*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define MAXLINE 1024

// We'll reuse simplified tokenization for validator so you can run parser alone with same source
// For reliability, parser will just scan file and check higher-level structure.

int is_variable(const char *s){
    int n = strlen(s);
    if(n < 4) return 0;
    if(s[0] != '_') return 0;
    int i = 1;
    if(!isalpha((unsigned char)s[i])) return 0;
    while(i < n && isalpha((unsigned char)s[i])) i++;
    if(i >= n || !isdigit((unsigned char)s[i])) return 0;
    i++;
    if(i >= n || !isalpha((unsigned char)s[i])) return 0;
    i++;
    if(i != n) return 0;
    return 1;
}

int is_c_identifier(const char *s){
    if(!s || !s[0]) return 0;
    if(!(isalpha((unsigned char)s[0]) || s[0]=='_')) return 0;
    for(int i=1; s[i]; i++) if(!(isalnum((unsigned char)s[i]) || s[i]=='_')) return 0;
    return 1;
}

int is_function_name(const char *s){
    int n = strlen(s);
    if(n < 3) return 0;
    int len = n - 2;
    if(strcmp(s + len, "Fn") != 0) return 0;
    for(int i=0;i<len;i++) if(!isalpha((unsigned char)s[i])) return 0;
    return 1;
}

int check_include_first(FILE *f){
    char line[MAXLINE];
    if(!fgets(line, sizeof(line), f)) return 0;
    line[strcspn(line, "\r\n")] = 0;
    while(isspace((unsigned char)line[0])) memmove(line, line+1, strlen(line));
    if(strcmp(line, "#include<stdio.h>")==0 || strcmp(line, "#include <stdio.h>")==0) return 1;
    return 0;
}

int has_main_function(FILE *f){
    // naive search for "int main(" or "int main ()" or "int main()"
    char line[MAXLINE];
    rewind(f);
    while(fgets(line, sizeof(line), f)){
        if(strstr(line, "main(") || strstr(line, "main (")) return 1;
    }
    return 0;
}

int validate_structure(FILE *f){
    rewind(f);
    char line[MAXLINE];
    int lineno = 0;
    int ok = 1;
    while(fgets(line, sizeof(line), f)){
        lineno++;
        // skip first line
        if(lineno == 1) continue;
        // trim
        line[strcspn(line, "\r\n")] = 0;
        char *p = line;
        while(*p && isspace((unsigned char)*p)) p++;
        if(*p == 0) continue;
        // comment?
        if(p[0]=='/' && p[1]=='/'){
            // ensure only letters and spaces after //
            p += 2;
            while(*p){
                if(!isalpha((unsigned char)*p) && *p!=' ' && *p!='\t'){ printf("Line %d: invalid comment characters\n", lineno); return 0; }
                p++;
            }
            continue;
        }
        // if this line is a function header (e.g. "int addFn(int _x1a) {"), skip it
        if ((strncmp(p, "int ", 4) == 0 || strncmp(p, "dec ", 4) == 0) && strchr(p, '(')) {
            /* ensure this looks like a function header (has '(' before '=' or ';') */
            char *paren = strchr(p, '(');
            char *eq = strchr(p, '=');
            char *semi = strchr(p, ';');
            if (paren && ((eq == NULL) || (paren < eq)) && ((semi == NULL) || (paren < semi))) {
                continue;
            }
        }

        // check printf pattern: printf( <var> )..
        if(strstr(line, "printf(")){
            char *q = strstr(line, "printf(");
            q += strlen("printf(");
            // find next ')'
            char *r = strchr(q, ')');
            if(!r){ printf("Line %d: printf missing closing parenthesis\n", lineno); return 0; }
            // extract inside
            char inside[256]; int len = (int)(r - q);
            if (len >= (int)sizeof(inside)) len = (int)sizeof(inside)-1;
            strncpy(inside, q, len); inside[len]=0;
            // trim
            char *s = inside; while(*s && isspace((unsigned char)*s)) s++;
            char *t = inside + strlen(inside) - 1; while(t>=inside && isspace((unsigned char)*t)) *t--=0;
            // If printf has comma-separated args (C-style), take the last argument as variable
            if (strchr(s, ',')) {
                char *last = s + strlen(s) - 1;
                while(last > s && isspace((unsigned char)*last)) last--;
                /* move back to char after previous comma */
                while(last > s && *last != ',') last--;
                if (*last == ',') last++; /* move to char after comma */
                while(*last && isspace((unsigned char)*last)) last++;
                s = last;
            }
            /* Accept either a variable or a string literal as printf argument (single-arg printf) */
            if(!(s[0] == '"' || is_variable(s))){ printf("Line %d: printf argument not valid variable '%s'\n", lineno, s); return 0; }
            // check terminator ".." or C-style ";" somewhere after r
            if(strstr(r, "..")==NULL && strstr(r, ";")==NULL){ printf("Line %d: statement missing '..' or ';' terminator\n", lineno); return 0; }
            continue;
        }
        // check variable declarations like: dec _input3k = 10..
        if(strstr(line, "dec ") || strstr(line, "int ")){
            // ensure variable name matches and terminator exists
            /* if this is the function header for main, skip it */
            if (strstr(line, "main(") || strstr(line, "main (")) continue;

            char *typ = strstr(line, "dec ") ? strstr(line, "dec ") : strstr(line, "int ");
            if(typ){
                char buf[256];
                strncpy(buf, typ, sizeof(buf)-1); buf[sizeof(buf)-1]=0;
                // tokenise: type var = value..
                char type[16], varname[128];
                if(sscanf(buf, "%15s %127s", type, varname) >= 2){
                    /* varname may contain trailing chars like "= 10.." so extract upto non-var chars (space or '=' or ';' or '.') */
                    char vn[128]; int i=0;
                    while (i < (int)strlen(varname) && varname[i] != '=' && varname[i] != ';' && varname[i] != '.' && !isspace((unsigned char)varname[i])) {
                        vn[i] = varname[i];
                        i++;
                    }
                    vn[i] = 0;
                    if(!is_variable(vn) && !is_c_identifier(vn)){ printf("Line %d: invalid variable name '%s'\n", lineno, vn); return 0; }
                    if(strstr(line, "..") == NULL && strstr(line, ";") == NULL){ printf("Line %d: missing '..' or ';' terminator\n", lineno); return 0; }
                }
            }
            continue;
        }
        // check while loop pattern: while (dec _loopin0x < 3..) { 
        if(strstr(line, "while")){
            // must contain '(' and ')' and comparator <
            if(strstr(line, "(") == NULL || strstr(line, ")") == NULL){ printf("Line %d: while parenthesis missing\n", lineno); return 0; }
            if(strstr(line, "<") == NULL){ printf("Line %d: while comparator expected '<'\n", lineno); return 0; }
            /* Allow C-style while with opening brace after the condition: "while (cond) {"
               In that case don't require DSL terminator '..' or ';' in the same line. */
            {
                char *rp = strchr(line, ')');
                if (rp) {
                    char *brace_after = strchr(rp, '{');
                    if (brace_after == NULL && strstr(line, "..") == NULL && strstr(line, ";") == NULL) {
                        printf("Line %d: while condition missing '..' or ';'\n", lineno); return 0;
                    }
                }
            }
            /* Scan the original line for a variable token that matches is_variable pattern */
            {
                int found = 0;
                int llen = (int)strlen(line);
                int si;
                for(si = 0; si < llen; si++){
                    if(line[si] == '_'){
                        /* extract token of alnum/_ chars starting at si */
                        int sj = si;
                        char cand[128]; int ck = 0;
                        while(sj < llen && (isalnum((unsigned char)line[sj]) || line[sj] == '_')){
                            if(ck < (int)sizeof(cand)-1) cand[ck++] = line[sj];
                            sj++;
                        }
                        cand[ck] = 0;
                        if(ck > 0 && (is_variable(cand) || is_c_identifier(cand))){
                            found = 1;
                            break;
                        }
                    }
                }
                if(!found){ printf("Line %d: while variable not found\n", lineno); return 0; }
            }
            continue;
        }

        // check return statement like return 0..
        if(strstr(line, "return")){
            if(strstr(line, "..") == NULL && strstr(line, ";") == NULL){ printf("Line %d: return missing '..' or ';'\n", lineno); return 0; }
            continue;
        }

        // if open brace or close brace or break with terminator
        if(strstr(line, "break")){
            if(strstr(line, "..") == NULL && strstr(line, ";") == NULL){ printf("Line %d: break missing '..' or ';'\n", lineno); return 0; }
            continue;
        }

        // for other lines, ensure if they contain '.' token then it's OK else assume it's code block or braces
        continue;
    }
    return ok;
}

int main(int argc, char **argv){
    if(argc < 2){ printf("Usage: %s <source-file>\n", argv[0]); return 1; }
    FILE *f = fopen(argv[1], "r");
    if(!f){ perror("fopen"); return 1; }

    if(!check_include_first(f)){
        printf("PARSE ERROR: first line must be #include<stdio.h>\n");
        fclose(f);
        return 1;
    }
    if(!has_main_function(f)){
        printf("PARSE ERROR: main function not found\n");
        fclose(f);
        return 1;
    }
    if(!validate_structure(f)){
        printf("PARSE ERROR: structure validation failed\n");
        fclose(f);
        return 1;
    }
    printf("PARSE SUCCESS: Program ACCEPTED\n");
    fclose(f);
    return 0;
}
