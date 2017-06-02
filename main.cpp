#include <string.h>
#include <iostream>
#include <chrono>
#include <cassert>
#include <vector>
#include <map>
using namespace std;
using namespace std::chrono;

const int TEST_ITERATIONS = 4;

const int MEMCPY_TESTS_COUNT = 3;
enum class MEMCPY : int{
    NORMAL,
    MOVS,
    STANDARD,
};
extern "C" void memcpy_normal(char* a,char* b, int n);
extern "C" void memcpy_movs  (char* a,char* b, int n);

const int STRCMP_TESTS_COUNT = 3;
enum class STRCMP : int{
    NORMAL,
    CMPS,
    SSE42,
    STANDARD,
};
extern "C" int strcmp_normal(char* a,char* b, int n);
extern "C" int strcmp_cmps  (char* a,char* b, int n);
extern "C" int strcmp_sse42 (char* a,char* b, int n);

map<MEMCPY,vector<int>> memcpyResults;
map<STRCMP,vector<int>> strcmpResults;
map<MEMCPY, string> memcpyNames = {
    {MEMCPY::NORMAL, "NORMAL"},
    {MEMCPY::MOVS, "MOVS"},
    {MEMCPY::STANDARD, "STANDARD"},
};
map<STRCMP, string> strcmpNames = {
    {STRCMP::NORMAL, "NORMAL"},
    {STRCMP::CMPS, "CMPS"},
    {STRCMP::SSE42, "SSE42"},
    {STRCMP::STANDARD, "STANDARD"},
};

void testMemcpy(MEMCPY id, int size) {
    char* a=new char[size+1];
    char* b=new char[size+1];
    memset(b, 0, size+1);
    memset(a, 0, size+1);
    for (int i=0;i<size;++i) {
        a[i] = 'a' + i%('z'-'a'+1);
    }

    high_resolution_clock::time_point t1;
    switch(id) {
        case MEMCPY::NORMAL:
            t1 = high_resolution_clock::now();
            memcpy_normal(a, b, size);
            break;
        case MEMCPY::MOVS:
            t1 = high_resolution_clock::now();
            memcpy_movs(a, b, size);
            break;
        case MEMCPY::STANDARD:
            t1 = high_resolution_clock::now();
            memcpy(b, a, size);
            break;
        default:
            assert(0);
    }
    high_resolution_clock::time_point t2 = high_resolution_clock::now();
    assert(strcmp(a,b) == 0);
    auto duration = duration_cast<microseconds>( t2 - t1 ).count();
    memcpyResults[id].push_back(duration);

    delete [] a;
    delete [] b;
}

void testStrcmp(STRCMP id, int size) {
    char* a=new char[size+1];
    char* b=new char[size+1];
    memset(a, 0, size+1);
    memset(b, 0, size+1);
    for (int i=0;i<size;++i) {
        a[i] = 'a' + i%('z'-'a'+1);
        b[i] = 'a' + i%('z'-'a'+1);
    }
    b[size/6]+=3;
    high_resolution_clock::time_point t1;

    int result=-1;
    switch(id) {
        case STRCMP::NORMAL:
            t1 = high_resolution_clock::now();
            result = strcmp_normal(a, b, size);
            break;
        case STRCMP::CMPS:
            t1 = high_resolution_clock::now();
            result = strcmp_cmps(a, b, size);
            break;
        case STRCMP::SSE42:
            t1 = high_resolution_clock::now();
            result = strcmp_sse42(a, b, size);
            break;
        case STRCMP::STANDARD:
            t1 = high_resolution_clock::now();
            result = strcmp(a, b);
            break;
        default:
            assert(0);
    }
    high_resolution_clock::time_point t2 = high_resolution_clock::now();
    auto duration = duration_cast<microseconds>( t2 - t1 ).count();
    assert(result == size/6 || result == -3);
    strcmpResults[id].push_back(duration);

    delete [] a;
    delete [] b;
}

int main() {
    for (int q=0 ; q < TEST_ITERATIONS ; ++q) {
        int size = 16 * 1'000'000 ;
        cerr << "Testing MEMCPY" << endl;
        testMemcpy(MEMCPY::NORMAL, size);
        testMemcpy(MEMCPY::MOVS, size);
        testMemcpy(MEMCPY::STANDARD, size);

        cerr << "Testing STRCMP" << endl;
        testStrcmp(STRCMP::NORMAL, size);
        testStrcmp(STRCMP::CMPS, size);
        testStrcmp(STRCMP::SSE42, size);
        testStrcmp(STRCMP::STANDARD, size);
    }

    cout << "MEMCPY " << MEMCPY_TESTS_COUNT << endl;
    for(auto a : memcpyResults ) {
        cout << memcpyNames[a.first] << " ";
        for(int res : a.second)
            cout << res << " ";
        cout << endl;
    }

    cout << "STRCMP " << STRCMP_TESTS_COUNT << endl;
    for(auto a : strcmpResults ) {
        cout << strcmpNames[a.first] << " ";
        for(int res : a.second)
            cout << res << " ";
        cout << endl;
    }
    return 0;
}
