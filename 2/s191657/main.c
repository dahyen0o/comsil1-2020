#include "head.h"

int main(){
    int t, n; //t: 테스트 케이스 개수, n:각각의 페이지 수
    scanf("%d", &t);
    int **arr = (int **)malloc(t * sizeof(int *));
	//케이스 마다 결과를 기록할 배열을 만든다

    for(int i = 0;i < t;i++){
        scanf("%d", &n);
		*(arr + i) = (int *)calloc(10, sizeof(int));
		//0~9 숫자들의 개수를 셀 배열을 만든다
        Page(*(arr + i), n);
		//페이지를 이루는 숫자들의 개수를 세는 함수
    }

	for(int i = 0;i < t;i++){
        Print(*(arr + i));
		//화면에 결과를 출력하는 함수
		free(*(arr + i));
		//실행이 끝나면 해당 배열을 free한다
	}
    free(arr);
    return 0;
}    
