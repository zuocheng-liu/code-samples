#include <stdio.h>
#include <stdlib.h>

int quick_sort(int *a, int l) {
  if (l <= 1) return 0;
  int bench = a[0];
  int *p = a;
  int *q = a + l -1;
  while (p < q) {
    while (*q >= bench && p < q) {
      --q;
    }
    if (*q < bench && p < q) {
      *p = *q;
      ++p;
    }    
    while (*p <=bench && p < q) {
      ++p;
    }
    if (*p > bench && p < q) {
      *q = *p;
      --q;
    }
  }
  *p = bench;
  quick_sort(a,p-a);
  quick_sort(p + 1, l -(p - a) + 1);
  return 0;
}

int main() {
  return 0;
}
