#include<stdio.h>


int transition(int _state1a, char _input1a) {
    switch (state) {
        case 0:
            if (_input1a == 97)
                return 1;
            else
                return -1;

        case 1:
            if (_input1a == 98)
                return 1;
            else if (_input1a == 99)
                return 1;
            else
                return -1;
    }
    return -1;
}

int main() {
    char * _str1a = "abcbc";
    int _state1a = 0;

    for (int _i1a = 0; _i1a < strlen(_str1a); _i1a++) {
        _state1a = transition(_state1a, *(_str1a + _i1a));
        if (_state1a == -1) break;
    }

    if (_state1a == 1)
        printf("ACCEPTED (Matches: a(b|c)* )");
    else
        printf("REJECTED");

    return 0;
}
