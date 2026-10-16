// 1. MetaProgramming
// 2. Smart Pointers

template<typename T>
class Stack{
  public:
    Stack(size_t s): mSize(s){}
    // EVERY THREAD CAN PUSH AND EVERY THREAD CAN POP 
    bool push(T val){
      auto idx = pushIdx;
      if(idx == pushIdx){
        std::cout <<"HELLO";
      }
      if(mPushIdx == mSize)
        return false;
      arr[mPushIdx] = val;
      mPushIdx++;
    }
    bool pop(T& val){}
    bool empty(){

    }
  private:
    size_t mSize{0u};
    T* arr;
    atomic<size_t> pushIdx;
};
