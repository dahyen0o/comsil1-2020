#include <iostream>
#include <cstring>
#include "str.h"

using namespace std;

Str::Str(int leng){
	if(leng <= 0)
		cout << "Error" << endl;
	else{
		len = leng;
		str = new char[len + 1];
	}
	//문자열의 길이가 0이하이면 에러, 아니면 len + 1 크기의 char 배열 선언
}

Str::Str(const char *neyong){
	if(strlen(neyong) > 0){
		len = strlen(neyong);
		str = new char[len + 1];
		strcpy(str, neyong);
	}
	else
		cout << "Error" << endl;
	//문자열의 길이가 0이하이면 에러, 아니면 neyong 배열을 str배열로 만들어줌
}

Str::~Str(void){
	delete[] str;
	//str 배열 해제
}

int Str::length(void){
	return strlen(str);
	//str 배열의 길이 return
}

char* Str::contents(void){
	return str;
	// str 배열을 return
}

int Str::compare(class Str &a){
	return strcmp(str, a.contents());
	//배열과Str 안의 contents 함수로 return 된 배열을 strcmp로 비교한 값을 return
}

int Str::compare(const char *a){
	return strcmp(str, a);
	//위 함수와 마찬가지로 두 배열을 strcmp로 비교후 결과를 return
}

void Str::operator=(const char *a){
	delete[] str;
	len = strlen(a);
	//str 배열을 해제한 후 a의 배열 길이 len에 저장

	if(len > 0){
		str = new char[len + 1];
		strcpy(str, a);
	}
	else
		cout << "Error" << endl;
	//len의 길이가 0이하면 에러, 아니면 새로운 배열을 할당해주고 a의 내용을 새 배열에 복사
}

void Str::operator=(class Str &a){
	delete[] str;
	len = a.length();

	if(len > 0){
		str = new char[len + 1];
		strcpy(str, a.contents());
	}
	else
		cout << "Error" << endl;
	//위 함수와 같은 방식이지만 이번에는 복사하는 배열이 Str 클래스 안에 있음
}
