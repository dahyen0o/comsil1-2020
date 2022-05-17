#include "head.h"

void Print(int *a){
    for(int i = 0;i < 10;i++)
        printf("%d ", a[i]);
	//결과를 담은 배열을 형식에 맞게 출력한다.
    printf("\n");
}
