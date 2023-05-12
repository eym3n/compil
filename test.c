#include <string.h>
#include <stdio.h>


int is_array(char* op){
    return strstr(op, "[") != NULL;
}


void array_elements(char* op, char* arr, char* ind){
    int i = 0;
    while(op[i] != '['){
        arr[i] = op[i];
        i++;
    }
    arr[i] = '\0';

    i++;
    int j = 0;
    while(op[i+j] != ']'){
        ind[j] = op[i+j];
        j++;
    }
    ind[j] = '\0';
}

int main(){
    char temp[50] = "array[array]";
    char temp1[50], temp2[50];

    array_elements(temp, temp1, temp2);
    printf("RESULT %s[%s]\n", temp1, temp2);

    return 0;
}