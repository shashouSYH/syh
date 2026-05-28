#include <stdio.h>

long factorial_recursive(long n) {
    if (n <= 1) {
        return 1;
    }
    return n * factorial_recursive(n - 1);
}

unsigned search_binary(const long data[], unsigned n, long target) {
    unsigned left = 0;
    unsigned right = n - 1;
    while (left <= right) {
        unsigned mid = left + (right - left) / 2;
        if (data[mid] == target) {
            return mid;
        } else if (data[mid] < target) {
            left = mid + 1;
        } else {
            right = mid - 1;
        }
    }
    return n;
}

long sum_while(const long data[], unsigned n) {
    unsigned i = 0;
    long total = 0;
    while (i < n) {
        total += data[i];
        ++i;
    }
    return total;
}

int main(void) {
    const long a[] = {1, 2, 3, 4, 5, 6};
    printf("%ld %u %ld\n", factorial_recursive(6), search_binary(a, 6, 4), sum_while(a, 6));
    return 0;
}
