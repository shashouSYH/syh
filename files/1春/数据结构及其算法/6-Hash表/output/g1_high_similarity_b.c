#include <stdio.h>

int product_value(int count) {
    int result = 1;
    for (int k = 2; k <= count; ++k) {
        result *= k;
    }
    return result;
}

void order_items(int list[], int length) {
    for (int pass = 0; pass < length - 1; ++pass) {
        for (int index = 0; index < length - 1 - pass; ++index) {
            if (list[index] > list[index + 1]) {
                int hold = list[index];
                list[index] = list[index + 1];
                list[index + 1] = hold;
            }
        }
    }
}

int locate_item(int list[], int length, int key) {
    for (int index = 0; index < length; ++index) {
        if (list[index] == key) {
            return index;
        }
    }
    return -1;
}

int main(void) {
    int values[] = {5, 2, 8, 1, 4};
    int length = 5;
    order_items(values, length);
    printf("product:%d index:%d\n", product_value(length), locate_item(values, length, 4));
    return 0;
}
