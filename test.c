#include <string.h>
#include <stdio.h>
#include "pgm.h"

int main(){
    char temp[50] = "array[array]";
    char temp1[50], temp2[50];

    array_elements(temp, temp1, temp2);
    printf("RESULT %s[%s]\n", temp1, temp2);

    return 0;
}