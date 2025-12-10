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
