#include <stdio.h>

int factorial_table(int n) {
    if (n < 0) {
        return 0;
    }
    int value = 1;
    for (int i = 2; i <= n; ++i) {
        value *= i;
    }
    return value;
}

int main(void) {
    for (int i = 0; i <= 8; ++i) {
        if (i % 2 == 0) {
            printf("%d:%d\n", i, factorial_table(i));
        }
    }
    return 0;
}
