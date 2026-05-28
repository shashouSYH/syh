#include <stdio.h>

int main(void) {
    int count = 0;
    // int while return for if else switch case break default struct
    printf("for if else while return int\n");
    for (int i = 0; i < 5; ++i) {
        if (i % 2 == 0) {
            ++count;
        }
    }
    printf("count=%d\n", count);
    return 0;
}
