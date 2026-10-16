#include <iostream>
#include <bits/stdc++.h>

/// [0,0,0,0,0]
//   1 2 3 
class Vector{
private:
	uint32_t size;
	int64_t  *arr;
	
	uint32_t index;

public:
	Vector(uint32_t sz){
		this->size = sz;
		this->arr = new int64_t[sz];
		this->index = 0;	
	}

	void push_back(int64_t value){
		
		this->arr[index++] = value;
	}
	void print() const{
		for(size_t i=0;i<index;i++){
			std::cout << this->arr[i] <<" ";
		}
		std::cout << std::endl;
	}
};
int main(){
	uint32_t N;
	std::cin >> N;
	Vector obj{N};
	for(int i=0;i<2000;i++){
		obj.push_back(12);
		obj.push_back(13);
		obj.push_back(15);
	}
	obj.print();
}
