#include <iostream>
#include <thread>
#include <ctime>
#include <windows.h>
using namespace std;
int bbb = 1;
class Foo {
public:
	void begin();
	void printt(int i, int);
	void kkk();
};
void Foo::kkk() {
	Sleep(1000);
	bbb = 0;
}
void Foo::printt(int i, int j) {
	while (bbb){
		cout << i << "Xx" << endl;
	}
}

void Foo::begin() {
	thread TT(&Foo::printt, this, 1, 2);
	thread T(&Foo::kkk, this);
	TT.detach();
	T.detach();
	cout << "end" << endl;
}


int main() {
	Foo K;
	K.begin();
	return 0;
	getchar();
}