#include <iostream>
using namespace std;

string Palindrome(string str) {
    string s = str;
    for(int i=0; i<s.size(); i++){
        if(isupper(s[i]))
            s[i]=tolower(s[i]);
        else
            s[i]=toupper(s[i]);
    }
    return s;


}

int main() {

  // keep this function call here
  string s = "aCv e _-DdW";
  cout << Palindrome(s);
  return 0;
}
