#include <stdio.h>
#include <stdlib.h>
#include <time.h>

void merge_sort(int i, int j, int a[], int aux[]) {
    if (j <= i) {
        // if the subsection is empty or a single element
        return;
    }
    int mid = (i + j) / 2;

    // left sub-array is a[i .. mid]
    // right sub-array is a[mid + 1 .. j]

    merge_sort(i, mid, a, aux);         // sort the left sub-array recursively
    merge_sort(mid + 1, j, a, aux);     // sort the right sub-array recursively

    int pointer_left = i;               // pointer_left points to the beginning of the left sub-array
    int pointer_right = mid + 1;        // pointer_right points to the beginning of the right sub-array
    int k;                              // k is the loop counter

    // we loop from i to j to fill each element of the final merged array
    for (k = i; k <= j; k++) {
        if (pointer_left == mid + 1) {  // left pointer has reached the limit
            aux[k] = a[pointer_right];
            pointer_right++;
        } else if (pointer_right == j + 1) {  // right pointer has reached the limit
            aux[k] = a[pointer_left];
            pointer_left++;
        } else if (a[pointer_left] < a[pointer_right]) {  // pointer left points to smaller element
            aux[k] = a[pointer_left];
            pointer_left++;
        } else {                        // pointer right points to smaller element
            aux[k] = a[pointer_right];
            pointer_right++;
        }
    }
    // copy the elements from aux[] to a[]
    for (k = i; k <= j; k++) {
        a[k] = aux[k];
    }
}

#define MAXRANGE 1000

// main method
int main() {
    clock_t start, elapsed1, elapsed2;
    int size, i;

    printf("Enter N: ");
    if (scanf("%d", &size) != 1 || size <= 0) {
        printf("Invalid input!\n");
        return 1;
    }

    int array[size];
    int aux[size];

    start = clock();
    for (i = 0; i < size; i++) {
        array[i] = rand() % MAXRANGE;
    }
    elapsed1 = clock() - start;

    start = clock();
    merge_sort(0, size - 1, array, aux);
    elapsed2 = clock() - start;

    printf("initialisation time: %12.3f\n",
           (double)elapsed1 * 1000.0 / CLOCKS_PER_SEC);
    printf("    merge sort time: %12.3f\n",
           (double)elapsed2 * 1000.0 / CLOCKS_PER_SEC);

    for (i = 1; i < size; i++) {
        if (array[i - 1] > array[i])
            break;
    }

    if (i == size) {
        printf("array sorted\n");
    } else {
        printf("sort error: array[%i] = %d > array[%i] = %d\n",
               i - 1, array[i - 1], i, array[i]);
    }
    return 0;
}