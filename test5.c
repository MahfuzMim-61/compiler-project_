#include <stdio.h>

int incFn(int _a1x) {
    int _b2y = _a1x + 1;
    return _b2y;
}

int mulFn(int _v1a) {
    int _m2y = _v1a + 2;
    return _m2y;
}

int main(void) {
    int _num1k = 10;
    int _res2p = mulFn(_num1k);
    printf("Result: %d\n", _res2p);
    return 0;
}
