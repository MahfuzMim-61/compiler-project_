#include <stdio.h>
#include <string.h>

int transition(int state, char input) {
    switch (state) {
        case 0:
            return (input == 'a') ? 1 : -1;

        case 1:
            if (input == 'b' || input == 'c')
                return 1;
            else
                return -1;
    }
    return -1;
}

int main() {
    char str[100];
    int state = 0;

    printf("Enter string: ");
    scanf("%s", str);

    for (int i = 0; i < strlen(str); i++) {
        state = transition(state, str[i]);
        if (state == -1) break;
    }

    if (state == 1)
        printf("ACCEPTED (Matches: a(b|c)* )");
    else
        printf("REJECTED");

    return 0;
}
