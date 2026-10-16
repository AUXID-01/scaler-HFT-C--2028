#include <iostream>
struct Person{
	int id;
	int age;
	int marks;
	Person (int a,int b,int c): 
		id(a),age(b), marks(c){
	}
	~Person(){
		std::cout << id <<" Person delete\n";
	}
};
void fKartik(){
	Person person{1,12,0};
}

int main(){
	fKartik();
	return 0;
}
