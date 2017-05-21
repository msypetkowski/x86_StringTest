#include <string.h>
#include <iostream>
#include <chrono>
using namespace std;
using namespace std::chrono;

extern "C" void calc    (char* a,char* b, int n);
extern "C" void calcFast(char* a,char* b, int n);

const int size = 1000'000'000;
//const int size = 500;

void test(int id) {
    char* a=new char[size+1];
    char* b=new char[size+1];
    memset(b, 0, size+1);
    memset(a, 0, size+1);

    for (int i=0;i<size;++i) {
        a[i] = 'a' + i%('z'-'a'+1);
    }

    high_resolution_clock::time_point t1;
    if (id==0) {
        cout << "Normal:" << endl;
        t1 = high_resolution_clock::now();
        calc(a, b, size);
    } else if (id==1) {
        cout << "Fast:" << endl;
        t1 = high_resolution_clock::now();
        calcFast(a, b, size);
    } else if (id==2) {
        cout << "Memcpy:" << endl;
        t1 = high_resolution_clock::now();
        memcpy(b, a, size);
    }
    high_resolution_clock::time_point t2 = high_resolution_clock::now();

    auto duration = duration_cast<microseconds>( t2 - t1 ).count();
    cout << "calc exec time: " << duration << endl;

    delete [] a;
    delete [] b;
}
int main() {
    test(0);
    test(1);
    test(2);
    return 0;
}
