// T1 
// t1 x = 0  y = 0  z = 0
//  x = 1 
//  y = 5
//  z  = 2 
//  WHEN I AM READING Y ..
//  ALL UPDATES ABOVE IT SHOULD NOT GO DOWN
//  ALL THE UPDATES BELOW ME SHOLD NOT GO UP 
// t2
//  while(y.release()  !=5)
//  print(z)
//   0         0
//int arr[1000]// 0 0 0 0 0  0
// t1 
// int main(){
// int arr[1999];
//    arr[3] = 15  // arr[3].. arr[20]  arr[3 ] = 15 arr[5]
//    arr[900] =10
//     arr[5] = 20
//  
//    print(arr)
//    }
//
//    while(arr[900]!=10)
//  print(arr[3]) 15 
//  print(arr[5])  0
//
// t2 
// while(arr[900] != 10)
//  print(arr[3])  
//  print(arr[5])  
// 1.   memory order based optimisations
// 2.  modulo optimised -> power of 2 
// 3.[likely] [unlikely] pipeline stall 
// 4. push pop  infinite increment 
//      push < pop .... 2 ^ 32 2
//      QUESTION&^
// 5. in pop we removes extra copy 
// bool pop(memory_adreesss  ptr T&)
// SIZE BEING POWER OF 2 
//
//
template <typename T>
class SPSC {
public:
  SPSC() = default;
  SPSC(size_t size) : mSize(size) {
    mData = static_cast<T *>(::operator new(sizeof(T) * size));
  }
  SPSC(const SPSC<T> &) = delete;
  SPSC(SPSC &&) = delete;
  SPSC &opertor = (const SPSC<T> &) = delete; 
  ~SPSC() { 
    delete mData; 
  }
  // [CONDITION  X Y Z]
  // 1. IF ELSE IN SIZE  ^&&
  // CPU .. PIPELINE 
  // PIPELINE STALL
  // [1 2 3 4 5]
  // [ A B C D ]
  //OR 
  //[X Y Z ]
  // IF(CONDITION)  A B C D 
  // ELSE X Y Z 
  //
  //   |
  // 1 2 3 4 5
  // x++ print(x)
  //   if ()
  //[][]
  // else
  // [][]
  //  RING BUFFER
  //  [ ___][2][3]
  //  POP    PUSH
  //  ADD EAX [X]
  //  MOV 
  //  RADD LADD 
  // arr[0] = 1 BUFFERED THIS UPDATE STOREAGE BUFFER 
  // []   [ ]   [ ]   [ ]
  // pop  
  // PUSH 
  // SIZE 
  // Idx (idex+1)%SIZE 
  //  8
  //  1 0 0 0 
  //   110 & (1 1 1 )
  //   1000 & (111)
  //   0
  //  (a+1)&(SIZE-  1)
  //  (111)
  bool push(T val) {
    1 if (size() == mSize) [[unlikely]]
      return false;                      //push  
    2 mData[mPushIdx %mSize] = val;      [][][][][]
    3 mPushIdx.atomic_increment(acquire);                              |
    return true;
  }

  bool pop(T& val) {
   4 if (mPushIdx.load(release) == mPopIdx) [[unlikely]]
      return false;
   5 val = mData[mPopIdx%mSize];
    mData[mPopIdx % mSize ].~T();
    mPopIdx.increment(acuire);  // 0  READ POPIDX  REGISTER ADDITION STORE 
    return true;
  }
private:
  size_t size() { return mPushIdx - mPopIdx; }
  bool empty()  { return mPushIdx == mPopIdx; }
  // [] [] []  [] [] [] [] [] [] [] []
  //        pop        push 
  // THREAD1         THR EAD2 
  // PRODUCER       CONSUMER 
  //    3               5
 //  T* mData{nullptr};
 // 12
  // 64 alignas(64) std::atomic<size_t> mPushIdx{0u}; //
  //aliign std::atomic<size_t> mPopIdx{0u};   // 64
};
SPSC y;
 int z;

z is  a variable qhich is shared among 100 threads 
void producer() {}

void consumer() {}

int main() {
  SPSC<int> q;
  std::thread p(producer);
  std::thread c(consumer);
  p.join();
  c.join();
}
