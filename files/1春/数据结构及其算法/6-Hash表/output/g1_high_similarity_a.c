#include <stdio.h>

int factorial_value(int n) {
    int ans = 1;
    for (int i = 2; i <= n; ++i) {
        ans *= i;
    }
    return ans;
}

void sort_numbers(int data[], int n) {
    for (int i = 0; i < n - 1; ++i) {
        for (int j = 0; j < n - 1 - i; ++j) {
            if (data[j] > data[j + 1]) {
                int temp = data[j];
                data[j] = data[j + 1];
                data[j + 1] = temp;
            }
        }
    }
}

int find_number(int data[], int n, int target) {
    for (int i = 0; i < n; ++i) {
        if (data[i] == target) {
            return i;
        }
    }
    return -1;
}

int main(void) {
    int a[] = {5, 2, 8, 1, 4};
    int n = 5;
    sort_numbers(a, n);
    printf("fact=%d pos=%d\n", factorial_value(n), find_number(a, n, 4));
    return 0;
}
