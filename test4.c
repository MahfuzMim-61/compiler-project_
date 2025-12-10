#include <stdio.h>

int addFn(int _x1a) {
    int _y2b = _x1a + 3;
    return _y2b;
}

int main(void) {
    int _in1x = 0; // changed to 0 so the loop body executes for demo
    int _out2y = addFn(_in1x);

    loop_count01:
    while (_in1x < 3) {
        printf("Value: %d\n", _out2y);
        break;
    }

    return 0;
}
