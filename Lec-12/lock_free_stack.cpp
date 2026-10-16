struct Node{
  int val;
  Node* next;
};
// WE WILL ADD LAST AT FIRST 
std::atomic<Node*> head = nullptr;
// TWO THREADS ARE ... 
// MULTIPLE THREADS CAN PUSH AS WELL 
// MULTIPLE THREADS CAN POP AS WELL 
template<typename T=int>
// LOCK FREE VERSION 
// WAIT FREE VERION -------- 
//  [ 1 2 3 3 4 ]
//  STACK 
//  ONE THREAD 
class stack{
  public:
    bool push(T val){
      Node* newNode = new Node(val, head);
      while(head.compare_exchange_weak(newNode->next,  newNode)){};
      // if(head == nextNode->next){
      // head = newNode   
      // }
      // else{
      //   nexNode->next = head 
      // }
      // A.compare(B,C)
      // if(A== B){
      //    A = C  
      // }
      // else{
      //    B = A
      // }
    }
    // 4 -5 - 6
    // h 
    // t1         t2 
    // h =4       h = 4
    bool pop(T& val){
      auto current = head;
      while(head.compare_exchange_weak(current, head>next)){}
      current.~T()
    }
};
int main(){

}
