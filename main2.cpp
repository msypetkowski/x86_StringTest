#include <string.h>
#include <iostream>
#include <chrono>
using namespace std;
using namespace std::chrono;

extern "C" int strcmp_normal(char* a,char* b, int n);
extern "C" int strcmp_cmps  (char* a,char* b, int n);
//extern "C" int strcmp_cmps  (char* a,char* b);
extern "C" int strcmp_sse42 (char* a,char* b, int n);
//extern "C" int strcmp_sse42 (char* a,char* b);

const int SIZE = 1000'000'000;
//const int SIZE = 16*6;
//const int SIZE = 33;

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
    if (id==0) {
        cout << "normal: ";
        t1 = high_resolution_clock::now();
        cout << strcmp_normal(a, b, SIZE) << endl;
    }else if (id==1) {
        cout << "cmps  : ";
        t1 = high_resolution_clock::now();
        cout << strcmp_cmps(a, b, SIZE) << endl;
        //cout << strcmp_cmps(a, b) << endl;
    }else if (id==2) {
        cout << "sse42 : ";
        t1 = high_resolution_clock::now();
        cout << strcmp_sse42(a, b, SIZE) << endl;
        //cout << strcmp_sse42(a, b) << endl;
    } else if (id==3) {
        cout << "standard strcmp : ";
        t1 = high_resolution_clock::now();
        cout << strcmp(a, b) << endl;
        //cout << strcmp_sse42(a, b) << endl;
    }
    high_resolution_clock::time_point t2 = high_resolution_clock::now();

    auto duration = duration_cast<microseconds>( t2 - t1 ).count();
    cout << "exec time: " << duration << endl;

    delete [] a;
    delete [] b;
}

int main() {
    test(0);
    test(1);
    test(2);
    test(3);
    return 0;
}
