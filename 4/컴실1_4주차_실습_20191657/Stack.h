#include "LinkedList.h"


//1. 템플릿 클래스로 확장해야함
//2. Stack형식으로 Delete 함수 재정의해야함
//주의: first, current_size는 class의 멤버 변수이기 때문에 this 포인터를 사용하여 가져와야함

//LinkedList class를 상속받음
template <class N>
class Stack : public LinkedList<N>{
	public:
		bool Delete (N &element){
			//first가 0이면 false반환
			if(!this->first)
				return false;

			// LinkedList와 달리 Stack은 current가 가리키는 곳을 삭제
			element = this->first->data;
			Node<N> *de = this->first;
			this->first = this->first->link;
			
			delete de;
			this->current_size--;

			return true;
			}
};
