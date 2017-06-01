#include <string.h>
#include <iostream>
#include <chrono>
#include <cassert>
using namespace std;
using namespace std::chrono;

enum MEMCPY{
    NORMAL,
    MOVS,
    STANDARD
};

extern "C" void memcpy_normal(char* a,char* b, int n);
extern "C" void memcpy_movs  (char* a,char* b, int n);

const int SIZE = 1000'000'000/4;
//const int SIZE = 500;

void test(int id) {
    char* a=new char[SIZE+1];
    char* b=new char[SIZE+1];
    memset(b, 0, SIZE+1);
    memset(a, 0, SIZE+1);

    for (int i=0;i<SIZE;++i) {
        a[i] = 'a' + i%('z'-'a'+1);
    }

    high_resolution_clock::time_point t1;
    switch(id) {
        case MEMCPY::NORMAL:
            cout << "Normal:" << endl;
            t1 = high_resolution_clock::now();
            memcpy_normal(a, b, SIZE);
            break;
        case MEMCPY::MOVS:
            cout << "Fast:" << endl;
            t1 = high_resolution_clock::now();
            memcpy_movs(a, b, SIZE);
            break;
        case MEMCPY::STANDARD:
            cout << "Memcpy:" << endl;
            t1 = high_resolution_clock::now();
            memcpy(b, a, SIZE);
            break;
        default:
            assert(0);
    }
    high_resolution_clock::time_point t2 = high_resolution_clock::now();
    assert(strcmp(a,b) == 0);

    auto duration = duration_cast<microseconds>( t2 - t1 ).count();
    cout << "calc exec time: " << duration << endl;

    delete [] a;
    delete [] b;
}
int main() {
    test(MEMCPY::NORMAL);
    test(MEMCPY::MOVS);
    test(MEMCPY::STANDARD);
    return 0;
}
