#include "utils.h"

char valueinarray(int val, int *arr, size_t n) {
    for (size_t i = 0; i < n; i++)
    {
        if (arr[i] == val)
            return 1;
    }
    return 0;
}
