#include <iostream>
#include "array.h"
#include "growablearray.h"

using namespace std;

int main(){
	//배열이 int 형일때의 growablearray
	cout << "GrowableArray<int> Test" << endl;
	GrowableArray<int> g(10);
	for(int i = 0;i < g.length();i++)
		g[i] = 2 * i + 3;
	cout << "g(10)"; 
	g.print();
	g[13] = 13;
	cout << "g(26)";
	g.print();

	//배열이 double 형일때의 growablearray
	cout << "GrowableArray<double> Test" << endl;
	GrowableArray<double> dg(10);
	for(int i = 0;i < dg.length();i++)
		dg[i] = 2 * i + 3.14;
	cout << "dg(10)"; 
	dg.print();
	dg[13] = 13.31;
	cout << "dg(26)";
	dg.print();

	return 0;
}
