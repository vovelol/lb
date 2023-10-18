#include <stdio.h>
#include "sum_between_even_odd.h"
#include <stdlib.h>
#include "index_first_even.h"
#include "index_last_odd.h"

int sum_between_even_odd(int arr[], int n) {
int summa = 0;
	int first= index_first_even(arr,n);
	int last= index_last_odd(arr,n);
    for (int i = first ; i < last; i++) {
        summa += abs(arr[i]);
    }
    return summa;
}
