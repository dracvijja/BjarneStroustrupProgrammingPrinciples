

#include "../std_lib_facilities.h"

int main()
try
{
       char a = '1233';
       if(isdigit(a)) cout <
       else cout << "little lamb sucks dicks";

}
catch (exception& e) {
    cerr << "error: " << e.what() << '\n';
    //keep_window_open("~~");
    return 1;
}
catch (...) {
    cerr << "Oops: unknown exception!\n";
    keep_window_open("~~");
    return 2;
}

