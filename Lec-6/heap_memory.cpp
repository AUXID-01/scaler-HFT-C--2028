#include <thread>

void func(){
        int *arr = new int[1024*2024];
	delete arr;
}

int main(){
        while(1){
                func();
                std::this_thread::sleep_for(std::chrono::seconds(1));
        }
        return 0;
}

