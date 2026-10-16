
int *arr = 1GB OF HEAP ;
shared_ptr<int> ptr1(arr); // 1 
shared_ptr<int> ptr2(ptr1);// 2 
// 

void f(shared_ptr<int> a){
  g(a); // 3
}
int main(){
  int * arr = new int[1GB];

  shared_ptr<int> a(arr);  // 1 
  f(a); // 2 
  {
    shared_ptr<int> b(a);  //  4
  }
  // 3
  return 0;
}
template<typename T>
struct shared_ptr{
  shared_ptr() = delete;
  // copy constructor 
  shared_ptr(const shared_ptr&other){
    ptr = other.ptr; // we both shared same 1 GB RESOURCE 
    // ref count of POINTER ARR (1gb resource)++ 
  }
  ~shared_ptr(){
    // refcount of resource -- 
    if(refcount == 0){
      delete ptr;
    }
  }
T* ptr;
};

// UNIQUE POINTER 
//  A POINTER WHICH IS OWNED BY ONE RESOURCE
//
//  SHARED POINTER 
//   SHARED  
//
//
//   T* ptr = new int[1gb];
//
//   shared_ptr<T> x (1 gb ptr);
//   shared_ptr<T> y  ( 1gb ptr);
//   when both x and y are deleted i want to clean up 1gb
template<typename T>
class SmartPointer{
  public:
    SmartPointer(T* p): ptr(p){}
    ~SmartPointer(){
      delete p;
    }
T* ptr;
};
void f(){
  //int* arrr = new int[1000];
  SmartPointer obj(new int [1000]);
  // obj.~SmartPointer()
}

int main(){
  f();
}
