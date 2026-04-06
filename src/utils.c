#include "utils.h"
#include <stdlib.h>

void ShuffleArray(int * array, int size) {
    
    for (int i = size - 1; i > 0; i--) {
        int j = rand() % (i + 1);

        int temp = array[i];
        array[i] = array[j];
        array[j] = temp;
    }
}