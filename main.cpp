#include <string.h>
#include <iostream>
#include <chrono>
#include <cassert>
using namespace std;
using namespace std::chrono;

enum class MEMCPY{
    NORMAL,
    MOVS,
    STANDARD,
};
extern "C" void memcpy_normal(char* a,char* b, int n);
extern "C" void memcpy_movs  (char* a,char* b, int n);

enum class STRCMP{
    NORMAL,
    CMPS,
    SSE42,
    STANDARD,
};
extern "C" int strcmp_normal(char* a,char* b, int n);
extern "C" int strcmp_cmps  (char* a,char* b, int n);
extern "C" int strcmp_sse42 (char* a,char* b, int n);


void testMemcpy(MEMCPY id) {
    const int SIZE = 1000'000'000/4;

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

void testStrcmp(STRCMP id) {
    const int SIZE = 1000'000'000;

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
    cout << "Testing MEMCPY-------------------" << endl;
    testMemcpy(MEMCPY::NORMAL);
    testMemcpy(MEMCPY::MOVS);
    testMemcpy(MEMCPY::STANDARD);

    cout << "Testing STRCMP-------------------" << endl;
    testStrcmp(STRCMP::NORMAL);
    testStrcmp(STRCMP::CMPS);
    testStrcmp(STRCMP::SSE42);
    testStrcmp(STRCMP::STANDARD);
    return 0;
}
