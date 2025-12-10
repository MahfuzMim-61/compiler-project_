#include <stdio.h>

int main() {
    // DFA:
    // q0 --a--> q1
    // q1 --b--> q1
    // q1 --c--> q1

    // Equivalent NFA:
    // Same states + explicit transitions

    printf("NFA Equivalent Transitions:\n");
    printf("q0 --a--> q1\n");
    printf("q1 --b--> q1\n");
    printf("q1 --c--> q1\n");
    printf("q0 --ε--> q0 (self epsilon)\n");
    printf("q1 --ε--> q1 (self epsilon)\n");

    return 0;
}
