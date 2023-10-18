#include <stdio.h>
#include <stdlib.h>
#include "index_last_odd.h"

int index_last_odd(int arr[], int n) {
    for (int i = n - 1; i >= 0; i--) {
        if (arr[i] % 2 != 0) {
            return i;
        }
    }
    return -1;
}
