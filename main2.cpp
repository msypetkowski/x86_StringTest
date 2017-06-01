#include <string.h>
#include <iostream>
#include <chrono>
#include <cassert>
using namespace std;
using namespace std::chrono;

enum STRCMP{
    NORMAL,
    CMPS,
    SSE42,
    STANDARD,
};

extern "C" int strcmp_normal(char* a,char* b, int n);
extern "C" int strcmp_cmps  (char* a,char* b, int n);
//extern "C" int strcmp_cmps  (char* a,char* b);
extern "C" int strcmp_sse42 (char* a,char* b, int n);
//extern "C" int strcmp_sse42 (char* a,char* b);

const int SIZE = 1000'000'000;
//const int SIZE = 16*6;

void test(int id) {
    char* a=new char[SIZE+1];
    char* b=new char[SIZE+1];
    memset(a, 0, SIZE+1);
    memset(b, 0, SIZE+1);

    for (int i=0;i<SIZE;++i) {
        a[i] = 'a' + i%('z'-'a'+1);
        b[i] = 'a' + i%('z'-'a'+1);
    }

    b[SIZE/6]+=3;

    high_resolution_clock::time_point t1;

    int result=-1;
    switch(id) {
        case STRCMP::NORMAL:
            cout << "normal: ";
            t1 = high_resolution_clock::now();
            result = strcmp_normal(a, b, SIZE);
            break;

        case STRCMP::CMPS:
            cout << "cmps  : ";
            t1 = high_resolution_clock::now();
            result = strcmp_cmps(a, b, SIZE);
            break;

        case STRCMP::SSE42:
            cout << "sse42 : ";
            t1 = high_resolution_clock::now();
            result = strcmp_sse42(a, b, SIZE);
            break;

        case STRCMP::STANDARD:
            cout << "std   : ";
            t1 = high_resolution_clock::now();
            result = strcmp(a, b);
            break;

        default:
            assert(0);
    }
    high_resolution_clock::time_point t2 = high_resolution_clock::now();
    auto duration = duration_cast<microseconds>( t2 - t1 ).count();
    cout << "exec_time=" << duration << " ";
    cout << "Result=" << result << " ";
    cout << endl;

    delete [] a;
    delete [] b;
}

int main() {
    test(STRCMP::NORMAL);
    test(STRCMP::CMPS);
    test(STRCMP::SSE42);
    test(STRCMP::STANDARD);
    return 0;
}
