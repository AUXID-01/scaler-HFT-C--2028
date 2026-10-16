#include <thread>

void func(){

	int arr[1024*1024];
}

int main(){
	while(1){
		func();
		std::this_thread::sleep_for(std::chrono::seconds(1));		
	}
	return 0;
}
