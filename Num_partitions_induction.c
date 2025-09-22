#include <stdio.h>

unsigned long long stirling(unsigned int n, unsigned int k) {
    if (n == 0 && k == 0) return 1;
    if (n == 0 || k == 0 || k > n) return 0;

    return k * stirling(n - 1, k) + stirling(n - 1, k - 1);
}

int main() {
    unsigned int n, k;
    printf("Enter n (set size): ");
    scanf("%u", &n);
    printf("Enter k (number of subsets): ");
    scanf("%u", &k);

    printf("Number of ways to partition %u elements into %u non-empty subsets: %llu\n", n, k, stirling(n, k));
    return 0;
}
