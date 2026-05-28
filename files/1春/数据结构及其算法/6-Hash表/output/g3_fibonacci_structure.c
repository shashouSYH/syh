#include <stdio.h>

int fibonacci_table(int n) {
    if (n < 0) {
        return 0;
    }
    if (n <= 1) {
        return n;
    }
    int prev = 0;
    int curr = 1;
    for (int i = 2; i <= n; ++i) {
        int next = prev + curr;
        prev = curr;
        curr = next;
    }
    return curr;
}

int main(void) {
    for (int i = 0; i <= 8; ++i) {
        if (i % 2 == 0) {
            printf("%d:%d\n", i, fibonacci_table(i));
        }
    }
    return 0;
}
