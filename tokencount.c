#include <stdio.h>
#include <ctype.h>
#include <string.h>

int main() {
    FILE *fp = fopen("input.c", "r");
    if (!fp) {
        printf("File not found!\n");
        return 0;
    }

    int keywords = 0, identifiers = 0, numbers = 0;
    int operators = 0, delimiters = 0;

    char word[100];
    char c;

    char *keywordList[] = {"int", "float", "return", "while", "printf", "dec"};
    
    while ((c = fgetc(fp)) != EOF) {

        if (isalpha(c) || c == '_') {
            int i = 0;
            word[i++] = c;

            while (isalnum(c = fgetc(fp)) || c=='_')
                word[i++] = c;

            word[i] = '\0';

            int isKey = 0;
            for (int k = 0; k < 6; k++) {
                if (strcmp(word, keywordList[k]) == 0) {
                    isKey = 1;
                    break;
                }
            }

            if (isKey)
                keywords++;
            else
                identifiers++;

            ungetc(c, fp);
        }

        else if (isdigit(c)) {
            numbers++;
            while (isdigit(c = fgetc(fp)));
            ungetc(c, fp);
        }

        else if (c == '+' || c == '-' || c == '*' || c == '/' || c == '=')
            operators++;

        else if (c == ';' || c == ',' || c == '(' || c == ')'
              || c == '{' || c == '}')
            delimiters++;
    }

    printf("Token Count:\n");
    printf("Keywords   = %d\n", keywords);
    printf("Identifiers= %d\n", identifiers);
    printf("Numbers    = %d\n", numbers);
    printf("Operators  = %d\n", operators);
    printf("Delimiters = %d\n", delimiters);

    fclose(fp);
    return 0;
}
