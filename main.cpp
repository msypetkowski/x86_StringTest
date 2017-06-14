#include <string.h>
#include <iostream>
#include <chrono>
#include <cassert>
#include <vector>
#include <map>
#include <fstream>
using namespace std;
using namespace std::chrono;

const int TEST_ITERATIONS = 100000;

enum class MEMCPY : int{
    NORMAL,
    MOVSB,
    MOVSQ,
    MOVDQU,
    STANDARD,
};
extern "C" void memcpy_normal(char* a,char* b, int n);
extern "C" void memcpy_movsb (char* a,char* b, int n);
extern "C" void memcpy_movsq (char* a,char* b, int n);
extern "C" void memcpy_movdqu(char* a,char* b, int n);

enum class STRCMP : int{
    NORMAL,
    CMPS,
    SSE42,
    PCMP,
    STANDARD,
};
extern "C" int strcmp_normal(char* a,char* b, int n);
extern "C" int strcmp_cmps  (char* a,char* b, int n);
extern "C" int strcmp_pcmp  (char* a,char* b, int n);
extern "C" int strcmp_sse42 (char* a,char* b, int n);

enum class STRLEN : int{
    NORMAL,
    SCAS,
    SSE,
    SSE42,
    STANDARD,
};
extern "C" int strlen_normal(const char* a);
extern "C" int strlen_scas  (const char* a);
extern "C" int strlen_sse(const char* a);
extern "C" int strlen_sse42 (const char* a);

enum class STRSTR : int{
    NORMAL,
    CMPS,
    SSE42,
    STANDARD,
};
extern "C" char* strstr_normal(char* a, char* b);
extern "C" char* strstr_xxx   (char* a, char* b);
extern "C" char* strstr_sse42 (char* a, char* b);

map<MEMCPY,vector<int>> memcpyResults;
map<STRCMP,vector<int>> strcmpResults;
map<STRLEN,vector<int>> strlenResults;
map<STRSTR,vector<int>> strstrResults;
map<MEMCPY, string> memcpyNames = {
    {MEMCPY::NORMAL, "NORMAL"},
    {MEMCPY::MOVSB, "MOVSB"},
    {MEMCPY::MOVSQ, "MOVSQ"},
    {MEMCPY::MOVDQU, "MOVDQU"},
    {MEMCPY::STANDARD, "STANDARD"},
};
map<STRCMP, string> strcmpNames = {
    {STRCMP::NORMAL, "NORMAL"},
    {STRCMP::CMPS, "CMPS"},
    {STRCMP::PCMP, "PCMPEQB"},
    {STRCMP::SSE42, "SSE42"},
    {STRCMP::STANDARD, "STANDARD"},
};
map<STRLEN, string> strlenNames = {
    {STRLEN::NORMAL, "NORMAL"},
    {STRLEN::SCAS, "SCAS"},
    {STRLEN::SSE, "SSE"},
    {STRLEN::SSE42, "SSE42"},
    {STRLEN::STANDARD, "STANDARD"},
};
map<STRSTR, string> strstrNames = {
    {STRSTR::NORMAL, "NORMAL"},
    {STRSTR::CMPS, "CMPS"},
    {STRSTR::SSE42, "SSE42"},
    {STRSTR::STANDARD, "STANDARD"},
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
        case MEMCPY::MOVSB:
            t1 = high_resolution_clock::now();
            memcpy_movsb(a, b, size);
            break;
        case MEMCPY::MOVSQ:
            t1 = high_resolution_clock::now();
            memcpy_movsq(a, b, size);
            break;
        case MEMCPY::MOVDQU:
            t1 = high_resolution_clock::now();
            memcpy_movdqu(a, b, size);
            break;
        case MEMCPY::STANDARD:
            t1 = high_resolution_clock::now();
            memcpy(b, a, size);
            break;
        default:
            assert(0);
    }
    high_resolution_clock::time_point t2 = high_resolution_clock::now();
    // cout << "------------------" <<endl;
    // cout << a <<endl;
    // cout << b <<endl;
    assert(strcmp(a,b) == 0);
    auto duration = duration_cast<nanoseconds>( t2 - t1 ).count();
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
    int changedIndex = size - size/9;
    b[changedIndex]+=3;
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
        case STRCMP::PCMP:
            t1 = high_resolution_clock::now();
            result = strcmp_pcmp(a, b, size);
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
    auto duration = duration_cast<nanoseconds>( t2 - t1 ).count();
    // cout << result << " " << changedIndex << endl;
    assert(result == changedIndex || result == -3);
    strcmpResults[id].push_back(duration);

    delete [] a;
    delete [] b;
}

void testStrlen(STRLEN id, int size) {
    char* a=new char[size+1];
    char* b=new char[size+1];
    memset(a, 0, size+1);
    memset(b, 0, size+1);
    for (int i=0;i<size;++i) {
        a[i] = 'a' + i%('z'-'a'+1);
        b[i] = 'a' + i%('z'-'a'+1);
    }
    high_resolution_clock::time_point t1;

    int result=-1;
    switch(id) {
        case STRLEN::NORMAL:
            t1 = high_resolution_clock::now();
            result = strlen_normal(a);
            break;
        case STRLEN::SCAS:
            t1 = high_resolution_clock::now();
            result = strlen_scas(a);
            break;
        case STRLEN::SSE:
            t1 = high_resolution_clock::now();
            result = strlen_sse(a);
            break;
        case STRLEN::SSE42:
            t1 = high_resolution_clock::now();
            result = strlen_sse42(a);
            break;
        case STRLEN::STANDARD:
            t1 = high_resolution_clock::now();
            result = strlen(a);
            break;
        default:
            assert(0);
    }
    high_resolution_clock::time_point t2 = high_resolution_clock::now();
    auto duration = duration_cast<nanoseconds>( t2 - t1 ).count();
    //cout << result << " " << size << endl;
    assert(result == size);
    strlenResults[id].push_back(duration);

    delete [] a;
    delete [] b;
}

void testStrstr(STRSTR id) {
    ifstream t("text.txt");
    string text((std::istreambuf_iterator<char>(t)),
                    std::istreambuf_iterator<char>());
    char key[] = "This is text that we need";
    high_resolution_clock::time_point t1;
    char* result = nullptr;
    switch(id) {
        case STRSTR::NORMAL:
            t1 = high_resolution_clock::now();
            result = strstr_normal(&text[0], key);
            break;
        case STRSTR::CMPS:
            t1 = high_resolution_clock::now();
            assert(id == STRSTR::CMPS);
            result = strstr_xxx(&text[0], key);
            assert(id == STRSTR::CMPS);
            //result = strstr(&text[0], key);
            break;
        case STRSTR::SSE42:
            t1 = high_resolution_clock::now();
            result = strstr_sse42(&text[0], key);
            //result = strstr(&text[0], key);
            break;
        case STRSTR::STANDARD:
            t1 = high_resolution_clock::now();
            result = strstr(&text[0], key);
            break;
        default:
            assert(0);
    }
    high_resolution_clock::time_point t2 = high_resolution_clock::now();
    auto duration = duration_cast<nanoseconds>( t2 - t1 ).count();
    char expected[] = "This is text that we needThis is text suffix\n";
    // cout << result << endl;
    // cout << expected << endl;
    // cout << strlen(result)<< endl;
    // cout << strlen(expected) << endl;
    assert(strlen(result) == strlen(expected));
    assert(strcmp(result, expected)==0);
    //strstrResults[STRSTR::CMPS].push_back(duration);
    strstrResults[id].push_back(duration);
}

int main() {
    for (int q=0 ; q < TEST_ITERATIONS ; ++q) {
        // int size = 7;
        int size = 8011;
		//int size = 3200;
		// int size = 16 + 5;
        //int size = 160 +7;
        cerr << "Testing MEMCPY" << endl;
        testMemcpy(MEMCPY::NORMAL, size);
        testMemcpy(MEMCPY::MOVSB, size);
        testMemcpy(MEMCPY::MOVSQ, size);
        testMemcpy(MEMCPY::MOVDQU, size);
        testMemcpy(MEMCPY::STANDARD, size);

        cerr << "Testing STRCMP" << endl;
        testStrcmp(STRCMP::NORMAL, size);
        testStrcmp(STRCMP::CMPS, size);
        testStrcmp(STRCMP::PCMP, size);
        testStrcmp(STRCMP::SSE42, size);
        testStrcmp(STRCMP::STANDARD, size);

        cerr << "Testing STRLEN" << endl;
        testStrlen(STRLEN::NORMAL, size);
        testStrlen(STRLEN::SCAS, size);
        testStrlen(STRLEN::SSE, size);
        testStrlen(STRLEN::SSE42, size);
        testStrlen(STRLEN::STANDARD, size);

        cerr << "Testing STRSTR" << endl;
        testStrstr(STRSTR::NORMAL);
        testStrstr(STRSTR::CMPS);
        testStrstr(STRSTR::SSE42);
        testStrstr(STRSTR::STANDARD);
    }

    cout << "MEMCPY " << memcpyResults.size() << endl;
    for(auto a : memcpyResults ) {
        cout << memcpyNames[a.first] << " ";
        for(int res : a.second)
            cout << res << " ";
        cout << endl;
    }

    cout << "STRCMP " << strcmpResults.size() << endl;
    for(auto a : strcmpResults ) {
        cout << strcmpNames[a.first] << " ";
        for(int res : a.second)
            cout << res << " ";
        cout << endl;
    }

    cout << "STRLEN " << strlenResults.size() << endl;
    for(auto a : strlenResults ) {
        cout << strlenNames[a.first] << " ";
        for(int res : a.second)
            cout << res << " ";
        cout << endl;
    }


    cout << "STRSTR " << strstrResults.size() << endl;
    for(auto a : strstrResults ) {
        cout << strstrNames[a.first] << " ";
        for(int res : a.second)
            cout << res << " ";
        cout << endl;
    }
    return 0;
}
