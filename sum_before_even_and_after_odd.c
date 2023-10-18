#include <stdio.h>
#include "sum_before_even_and_after_odd.h"
#include <stdlib.h>
#include "index_first_even.h"
#include "index_last_odd.h"

int sum_before_even_and_after_odd(int arr[], int n) {
    int summa_before = 0;
    int summa_after = 0;
	int first= index_first_even(arr,n);
	int last= index_last_odd(arr,n);

    for (int i = 0; i < first; i++) {
        summa_before += abs(arr[i]);
    }

    for (int i = last; i < n; i++) {
        summa_after += abs(arr[i]);
    }

    return summa_before + summa_after;
}
