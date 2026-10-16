#include <iostream>
/// int arr[5]
/// [1 2 3 4 5]
///
/// vectror<int>(5)
/// [size =5 PTR->]                    []
// vector<int> x;
//
//  0 ->
//           [ [1].[2].[3].[4]]
//  x.push_back(1)
//  x.push_back(2)
//  x.push_back(3)
template <typename T> class vector {
public:
  vector() {
    mSize = 0;
    mCapacity = 4;
    mData = new T[mCapacity];
  }


  // VECTOR 1     VECTOR 2 
  //  Mdata ->     mData
  //    |             |
  //    |            |
  //  1 2 3 4 5      [1, 2 3 4 5]
  vector(const vector<int>& other){
    std::cout <<"COPY CONSTRUCTOR VERY COSTLY\n";
    mData = new T[other.mSize];
    for(size_t i=0;i<mSize;i++){
      mData[i] = other.mData[i];
    }
    mSize = other.mSize;
    mCapacity = other.mSize;
  }
  // other 
  // 0 
  // [mData]      nullptr 
  // Vector A = std::move(other);
  // A 
  // 5  
  // [mData]->       -> 0x100[ 1 2 3 4 5 ]
  vector(vector<int>&& other){
    std::cout <<"SHALLOW COPY CONSUTRUCTION VERY CHEAP\n";
    mData = other.mData;
    mSize = other.mSize;
    mCapacity = other.mCapacity;

    other.mData = nullptr;
    other.mSize  = 0;
    other.mCapacity = 0;
  }
  vector(size_t size) {
    mSize = 0;
    mCapacity = size;
    mData = new T[mCapacity];
  }

  void push_back(T val) {
    if (mSize == mCapacity) {
      std::cout << "I HAVE TO GET 2X MEMORY FROM HEAP AND COPY EVERYTYING "
                   "COSTLY OPERATION\n";
      return;
      // DOUBLE THE SIZE IOF UR HEAP ARRAY
      // COPY EVERYTYING
      // DELETE EXISTIG HEAP ARRAT
    }
    std::cout <<"PUSH\n";
    mData[mSize] = val;
    mSize++;
  }
  void pop_back() {
    if (mSize == 0)
      throw;
    /// [obj1][obj2][obj5][]
    mData[mSize].~T();
    mSize--;
  }
  size_t size() { return mSize; }

private:
  size_t mSize{0u};     // number of elements pushed
  size_t mCapacity{0u}; // number of elements u can push
  T *mData{nullptr};
};

vector<int> makeVector(){
  vector<int> a;
  a.push_back(12);
  a.push_back(13);
  a.push_back(3);
  return a;
}

template<typename T>
void func(T&& temp){
  vector<int> b(std::forward<T>(temp));
}
// EITHER U MAKE A DEEP COPY 

// OR YOU TELL UR COMPILER THAT I Wanna make a shallow COPY
int main() {
  vector<int> a(makeVector());
  std::cout << a.size() ;
}
