#include <string.h>
#include <iostream>
using namespace std;

extern "C" void calc(char* a,char* b, int n);
extern "C" void calcFast(char* a,char* b, int n);

char b[200];
int main() {
    char* a=new char[20];
    strcpy(a,"qwexxxuuuaaau");

    //calc(a, b, strlen(a));
    calcFast(a, b, strlen(a));

    cout<<endl;
    cout<<"--------"<<endl;
    cout<<a<<endl;
    cout<<b<<endl;
    return 0;
}
