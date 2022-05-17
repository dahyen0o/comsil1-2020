#include "array.h"

template <class T>
class GrowableArray:public Array<T>{
	public:
		GrowableArray(int x):Array<T>(x){}

		~GrowableArray(){}
		
		virtual T operator[](int i) const{
			return Array<T>::operator[](i);
		};

		virtual T& operator[](int i){
			if(i >= (this->len)){
				int l = i * 2, k;
				T *t;
			    t = new T[l];
				for(k = 0;k < l;k++)
					t[k] = 0;
				for(k = 0;k < (this->len);k++)
					t[k] = (this->data)[k];
				delete[] (this->data);
				this->len = l;
				this->data = t;
			}
			return Array<T>::operator[](i);
		}
};
