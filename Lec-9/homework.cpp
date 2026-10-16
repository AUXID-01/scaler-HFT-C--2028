///     []
///   MEMORY POOL
///   IT ALLOCATES A HUGE HEAP CHUNK AT STARTUP 
///  THEN U USE THIS MEMORY  TO CREATE OBJECTS ON IT 
///
///  TRICK
///  reinterpret_cast ORDER   PRICE 
//   struct C{  INT A; INT B;}
//   struct D{   long long int a;}
//   C*  c;
//   D*  d;
//   c =reinterpret_cast<C*>(d);
//    5 us 
//    new 1/2 micro 
//    price
//    quantoty
//    order
//  [{0   3}                           ]
//   void * ptr = 
//   new byte [1024 * 1024 * 1024]
