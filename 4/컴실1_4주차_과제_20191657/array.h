#ifndef __ARRAY__
#define __ARRAY__

#include <iostream>
#include <cstdlib>

using namespace std;

template <typename T>
class Array{
	protected:
		T *data; //배열
		int len; //배열의 길이
	public:
		Array(int size){
			if(size <= 0){
				cerr << "Error" << endl;
				exit(-1);
			}
			else{
				data = new T[size];
				len = size;
			}
		} //size를 배열의 길이로 하는 T형 배열 생성

		~Array(){
			delete[] data;
		} //소멸자

		int length() const{
			return len;
		} //배열의 길이 len을 return

		virtual T& operator[](int i){
			static T tmp;
			if(i < len && 0 <= i)
				return data[i];
			else{
				cerr << "Error" << endl;
				return tmp;
			}
		} //인덱스가 배열의 길이 내에 있으면 해당 인덱스의 배열값을 return, 아니면 error 출력


		virtual T operator[](int i) const{
			if(i < len && 0 <= i)
				return data[i];
			else{
				cerr << "Error" << endl;
				return 0;
			}
		}

		void print(){
			cout << "[";
			for(int i = 0;i < len;i++){
				if(i < len - 1)
					cout << data[i] << " ";
				else
					cout << data[i] << "]" << endl;
			}
		}
		//주어진 형식대로 배열을 차례대로 출력한다
};
#endif
