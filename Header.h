#include<string>
#include<iostream>
#include<vector>
using namespace std;
#ifndef Header_h
#define Header_h
class CPerson {
private:
    string name ;
    int highscore;
public:
    void setname(string N);
    string getname();
    void sethighscore(int H);
    int gethighscore();
};
#endif // !Header_h

