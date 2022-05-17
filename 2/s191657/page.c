#include "head.h"

void Page(int *a, int n){
    //i: page number, 현재 페이지
    for(int i = 1;i <= n;i++){
        int k = i; //i의 정보를 k에 저장
        for(int j = 1;k > 0;j *= 10){
            a[k % 10]++;
            k = k / 10;
        } 
		//k를 맨 오른쪽 자리수부터 분해하며 해당하는 숫자의 배열값을 하나 더한다.
    }
}
