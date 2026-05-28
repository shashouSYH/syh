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

int count_positive(int data[], int n) {
    int count = 0;
    for (int i = 0; i < n; ++i) {
        if (data[i] > 0) {
            ++count;
        }
    }
    return count;
}

int count_large(int data[], int n) {
    int count = 0;
    for (int i = 0; i < n; ++i) {
        if (data[i] > 10) {
            ++count;
        }
    }
    return count;
}

int main(void) {
    int a[] = {1, 2, 13, 24, -5, 30};
    printf("%d %d %d\n", count_even(a, 6), count_positive(a, 6), count_large(a, 6));
    return 0;
}
