#include <stdio.h>

int factorial_loop(int n) {
    int ans = 1;
    for (int i = 2; i <= n; ++i) {
        ans *= i;
    }
    return ans;
}

int search_linear(int data[], int n, int target) {
    for (int i = 0; i < n; ++i) {
        if (data[i] == target) {
            return i;
        }
    }
    return -1;
}

int sum_for(int data[], int n) {
    int total = 0;
    for (int i = 0; i < n; ++i) {
        total += data[i];
    }
    return total;
}

int main(void) {
    int a[] = {1, 2, 3, 4, 5, 6};
    printf("%d %d %d\n", factorial_loop(6), search_linear(a, 6, 4), sum_for(a, 6));
    return 0;
}
