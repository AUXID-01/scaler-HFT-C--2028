#include <iostream>

// 0 1 2 3 4 5 6 7 8 9 10 11 12 13 14 15 16 17 18

// a a a a b       a a a a b       

// Obj obj;;
// dhdsjgsdjdf
//dfbdsnjsd
//sdbsjn:wqds
// int x
struct Z{
	int  a;
	char b;
};
struct Person{
	int a;
	int b;
};

struct Any{
	int a;
	char b[3];
};

struct X{
	char a;
	int* p;
};

struct Y{
	char a;
	int p;
};
int main(){
	std::cout << sizeof(Z)  << std::endl;
	std::cout << sizeof(Person) << std::endl;
	std::cout << sizeof(Any) << std::endl;
	std::cout << sizeof(X) << std::endl;
	std::cout << sizeof(Y) << std::endl;
}
