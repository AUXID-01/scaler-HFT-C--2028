#include<iostream>
struct X{
  size_t x;
  X(size_t x){
    this->x = x;
    std::cout << "CONSTRUCTOR\n";
  }
};

int main(){
  X *arr = (X*)(::operator new (10* sizeof(X)));
  arr[0] = X{1};
}
