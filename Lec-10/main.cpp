// 1. market listener and sends data to strategy 
//   thread just listening data 
// 2. strategy listener .... 
// 3. market sender .. io bounc 
// HOW FUTURES REALL WORK IN JAVA 
//
///    1 2 3 4 5 6 7 8 
///    [1 2 3 4 5]
// start  = 0 
// end    = 0
// push_back()
//    LOCK = TRUE 
//  if(end - start +1 == size)
//  LOCK = FALSE 
//  arr[end] = data  T/// 
//  LOCK 
//  end++
//  UNLOCK
//  end 3 ...  4 
//  end 
//
// pop_front()
//  while(lcok == true){}
//  // LOCK FALSE 
//  LOCK = FALSE 
//  if(start == end)
//    return 
//   UNLOCK 
//  temp = arr[start]
//   LOCK 
//  start++;
//  UNLOCK 
//  return temp 
//
MULTITHREADING
thread 1 
Event arr[N];
      pushes data at frony      pop back 
   // [[             ]]
// producer thread 
void marketListener(){
  // wait for marker events 
  Event event = socket.listen();
  send(event);
  push_front(arr);
  //start end 
  //start++
}
// consumee thread 
thread 2 
void strategy(){
  pop_back(arr);
  // end--
  if(arr.ize()>0){

  }
  // listen to events from MARKET LISTENER THREAD 
  //
  // ....
  //
  //
  tell thread 3 to send response 
}
thread 3 
void sendResponse(){
  while(1)
    sendResponse();
}
// single double ended queue 
// MARKET DATA THING 
// /FORM NETWORK SOCKET EVENTS
//  IMAGINE 
//  CLIENT 
//[A]            [B]
// 3000          8080
//              removes ip geader 
//              removed tcp headers 
//              see the real data 
//
//5 bytes 1000 bytes
//  [{  "a": true , [IP _HEADER] , [TCP HEswea]}]
//   IP HEADERS DESTINATION PORT SOURCE PORT CHECLKSUM 
//   TCP HEADER -> SYN ACK ... MEATDATA 
//   __
//
//
//    127.0.0.1
//    tcp loopback 
//  PORT 3000 -> REACT APP
//  ..... PORT 300O TO PORT 8080 GET REQUEST
//  PORT 8080 -> SPRING -> CONNECTING DB  
//  PORT 3000
//  RESPONSE 200 , 400
//
// 1. U LISTEN MARKET EVENT PRICE OF SOMETHING BID ASK  FRONTEND 
// 2/. STRATEGY 50 45  BACKEDN 
// 3 SEND OFF A TRADE FRONTEND 
//
// ONE PROCESS THREE DIFFERENT 
// WHY NOT THREE DIFFERENT PROCESSES 
// 1.  LISTENER MD 
