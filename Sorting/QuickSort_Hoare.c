
int hoare_partition(int arr[], int low, int high) {
    int pivot = arr[low];
    int i = low - 1, j = high + 1;

    while (1) {
        do { i++; } while (arr[i] < pivot);
        do { j--; } while (arr[j] > pivot);

        if (i >= j) return j;  

        swap(&arr[i], &arr[j]);
    }
}

void quicksort_hoare(int arr[], int low, int high) {
    if (low < high) {
        int p = hoare_partition(arr, low, high);
        quicksort_hoare(arr, low, p);
        quicksort_hoare(arr, p + 1, high);
    }
}
