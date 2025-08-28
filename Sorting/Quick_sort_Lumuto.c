#include <stdio.h>
#include <stdlib.h>

// Swap helper
void swap(int *a, int *b) {
    int t = *a; *a = *b; *b = t;
}

// Lomuto partition (pivot = last element)
int lomuto_partition(int arr[], int low, int high) {
    int pivot = arr[high];
    int i = low - 1;
    for (int j = low; j < high; j++) {
        if (arr[j] <= pivot) {
            i++;
            swap(&arr[i], &arr[j]);
        }
    }
    swap(&arr[i+1], &arr[high]);
    return i+1;  // pivot index (final position)
}

// Quicksort using Lomuto
void quicksort_lomuto(int arr[], int low, int high) {
    if (low < high) {
        int p = lomuto_partition(arr, low, high);
        quicksort_lomuto(arr, low, p - 1);
        quicksort_lomuto(arr, p + 1, high);
    }
}
