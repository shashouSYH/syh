#include <stdio.h>

int count_even(int data[], int n) {
    int count = 0;
    for (int i = 0; i < n; ++i) {
        if (data[i] % 2 == 0) {
            ++count;
        }
    }
    return count;
}

int main(void) {
    int a[] = {1, 2, 3, 4};
    printf("%d\n", count_even(a, 4));
    return 0;
}
