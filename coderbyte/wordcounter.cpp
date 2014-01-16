//#include "../std_lib_facilities.h"
#include <iostream>
using namespace std;

int word_counter(const string& str){

    int word_count = 0;
    for(int i=0; i<str.size(); i++){
        while(isspace(str[i])) i++;
        if(!isspace(str[i])){
            word_count++;
            i++;
        }
        while(!isspace(str[i]))
            i++;


    }
    return word_count;
}

int main(){

    string s = "hw ew";
    cout << word_counter(s);

}
