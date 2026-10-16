// I WANT TO CREATE MY OWN STRING CLASS

// 1.  WE WANT IT TO BE STACK BASED SO WE WILL MAKE IT HEAP BASED
// 2.  char* ptr -> heap based char array
// 3.  WHENEVER U WRITE A STRING LITERal "gh474"  data type of it const char*
// 4. WE ARE NOT SURE IF COPY ASSIGNMENT WILL BE DEEP COPY OR SHallo copy
#include <cstdint>
#include <iostream>
namespace scaler {

class String {

private:
  char *mPtr{nullptr};
  uint mSize{0u};

public:
  String(){}; // DEFAULT 0 INIT
  String(const char *data) {
    uint size = 0u;
    while (data[size] != '\0') {
      size++;
    }
    mSize = size;
    mPtr = new char[mSize];
    for (size_t i = 0; i < mSize; i++) {
      mPtr[i] = data[i];
    }
    std::cout << "const* params constructor\n";
  }

  String(const String &other) {
    mSize = other.size();
    mPtr = new char[mSize];
    for (size_t i = 0; i < mSize; i++) {
      mPtr[i] = other.mPtr[i];
    }
    std::cout << "copy constructor\n";
  }

  String(String &&other) {
    std::cout << "move constructor\n";
    this->mSize = other.mSize;
    this->mPtr = other.mPtr;
    other->mSize = 0u;
    other->mPtr = nullptr;
  }
  // String operator=(const String& other){} // COPY ASSGINMENT

  ~String() {
    std::cout << "destructor\n";
    this->mSize = 0u;
    this->mPtr = nullptr;
  }

  uint size() const { return mSize; }

  void print() {
    uint i = 0;
    while (i < mSize)
      std::cout << mPtr[i++];
  }
};

String concat(String &a, String &b) {         // a copy const   b copt const
  return String("DWTTEWYFEWJYHFWQEYHWQEFYU"); // const * constructor
}
// MOVE SEMANTICS
} // namespace scaler
using namespace scaler;
int main() {

  String a{"BYYY"}; // const *
  String b{"CCC"};  // const *
  // temporrary object (a,b) no name of this temp
  // we copy from this un nameed object and write to c
  String c{concat(a, b)}; // move constructor
}
