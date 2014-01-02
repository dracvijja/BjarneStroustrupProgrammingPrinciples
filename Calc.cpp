
// Revised for ch7 by Dragan Cvijanovic 1/1/2014
/*
GRAMMAR:

Expression:
    Term;
    Expression "+" Term;
    Expression "-" Term;
Term:
    Primary;
    Term "*" Primary;
    Term "/" Primary;
Primary:
    Number;
    Number "!";
    "(" Expression ")";
    "{" Expression "}";


*/
// This is example code from Chapter 6.7 "Trying the second version" of
// "Programming -- Principles and Practice Using C++" by Bjarne Stroustrup
//

#include "std_lib_facilities.h"



//------------------------------------------------------------------------------
const char number = '8';
const char print = '|';
const char quit = 'q';
const char prompt = '>';
const char result = '=';
//------------------------------------------------------------------------------
class Variable{
public:
    string name;
    double value;
    Variable(string n, double v)
        : name(n), value(v) {}
}
//--------------------------------------------------------------------------------
vector<Variable>var_table;
//--------------------------------------------------------------------------------

class Token {
public:
    char kind;        // what kind of token
    double value;     // for numbers: a value
    Token(char ch)    // make a Token from a char
        :kind(ch), value(0) { }
    Token(char ch, double val)     // make a Token from a char and a double
        :kind(ch), value(val) { }
};

//------------------------------------------------------------------------------

class Token_stream {
public:
    Token_stream();   // make a Token_stream that reads from cin
    Token get();      // get a Token (get() is defined elsewhere)
    void putback(Token t);    // put a Token back
    void ignore(char c);
private:
    bool full;        // is there a Token in the buffer?
    Token buffer;     // here is where we keep a Token put back using putback()
};

//------------------------------------------------------------------------------

// The constructor just s full to indicate that the buffer is empty:
Token_stream::Token_stream()
:full(false), buffer(0)    // no Token in buffer
{
}

//--------------------------------------------------------------------------------

// The putback() member function puts its argument back into the Token_stream's buffer:
void Token_stream::putback(Token t)
{
    if (full) error("putback() into a full buffer");
    buffer = t;       // copy t to buffer
    full = true;      // buffer is now full
}

//------------------------------------------------------------------------------

Token Token_stream::get()
{
    if (full) {       // do we already have a Token ready?
        // remove token from buffer
        full=false;
        return buffer;
    }

    char ch;
    cin >> ch;    // note that >> skips whitespace (space, newline, tab, etc.)

    switch (ch) {
    case '|':    // for "print"
    case 'q':    // for "quit"
    case '{':
    case '}':
    case '(':
    case ')':
    case '+':
    case '-':
    case '*':
    case '/':
    case '!':   // for factorial
        return Token(ch);        // let each character represent itself
    case '.':
    case '0': case '1': case '2': case '3': case '4':
    case '5': case '6': case '7': case '8': case '9':
        {
            cin.putback(ch);         // put digit back into the input stream
            double val;
            cin >> val;              // read a floating-point number
            return Token('8',val);   // let '8' represent "a number"
        }
    default:
        error("Bad token");
    }
}
void Token_stream::ignore(char c){
    if(full && c==buffer.kind){
        full = false;
        return;
    }
    full = false;

    char ch = 0;
    while(cin>>ch)
        if(ch==c) return;
}
//-----------------------------------------------------------------------------
int get_factorial(int x){
    if (x < 0) error ("Positive integer expected");
    else if (x == 0) return 1;
    int fact = 1;
    for(int i = 2; i<=x; i++){
        fact *= i;
    }
    return fact;
}

//------------------------------------------------------------------------------

Token_stream ts;        // provides get() and putback()

//------------------------------------------------------------------------------

double expression();    // declaration so that primary() can call expression()

//------------------------------------------------------------------------------

// deal with numbers and parentheses
double primary()
{
    Token t = ts.get();
    switch (t.kind) {
    case '{':
        {
            double d = expression();
            t = ts.get();
            if (t.kind != '}') error("'}' expected");
            return d;
        }
    case '(':    // handle '(' expression ')'
        {
            double d = expression();
            t = ts.get();
            if (t.kind != ')') error("')' expected");
            return d;
        }

  case '8':           // we use '8' to represent a number
        {
            double tempval = t.value;
            t = ts.get();
            if (t.kind == '!')
                return get_factorial(tempval);
            else
                ts.putback(t);
                return tempval;  // return the number's value
        }
    case '-':
        return -primary();
    case '+':
        return primary();
    default:
        error("primary expected");
    }
}

//------------------------------------------------------------------------------

// deal with *, /, and %
double term()
{
    double left = primary();
    Token t = ts.get();        // get the next token from token stream

    while(true) {
        switch (t.kind) {
        case '*':
            left *= primary();
            t = ts.get();
            break;
        case '/':
            {
                double d = primary();
                if (d == 0) error("divide by zero");
                left /= d;
                t = ts.get();
                break;
            }
        case '%':
        {
            int i1 = narrow_cast<int>(left);
            int i2 = narrow_cast<int>(term());
            if(i2==0) error ("%: divide by zero");
            left = i1%i2;
            t = ts.get();
            break;
        }

        default:
            ts.putback(t);     // put t back into the token stream
            return left;
        }
    }
}

//------------------------------------------------------------------------------

// deal with + and -
double expression()
{
    double left = term();      // read and evaluate a Term
    Token t = ts.get();        // get the next token from token stream

    while(true) {
        switch(t.kind) {
        case '+':
            left += term();    // evaluate Term and add
            t = ts.get();
            break;
        case '-':
            left -= term();    // evaluate Term and subtract
            t = ts.get();
            break;
        default:
            ts.putback(t);     // put t back into the token stream
            return left;       // finally: no more + or -: return the answer
        }
    }
}
//-----------------------------------------------------------------------------
double get_value(string s){
    for(int i=0; i<var_table; i++)
        if(var_table[i].name==s) return var_table[i].value
    error("get: undefines variable ", s);
}

void set_value(string s, double d){
    for(int i=0; i<var_table.size(); i++){
        if(var_table[i].name == s){}
            var_table[i].value = d;
            return;
        }
        error("set: undefined variable ", s);
        error("set: undefined variable ", s);
        error("set: undefined variable ", s);
        error("set: undefined variable ", s);
        error("set: undefined variable ", s);
        error("set: undefined variable ", s);
        error("set: undefined variable ", s);
        error("set: undefined variable ", s);
        error("set: undefined variable ", s);
        error("set: undefined variable ", s);
        error("set: undefined variable ", s);
        error("set: undefined variable ", s);
        error("set: undefined variable ", s);
        error("set: undefined variable ", s);
        error("set: undefined variable ", s);
    }
}
//------------------------------------------------------------------------------
void clean_up_mess(){
    ts.ignore(print);
}
//------------------------------------------------------------------------------
void calculate(){

    while (cin) {
        try{
            cout << "> ";
            Token t = ts.get();

            while(t.kind==print) t=ts.get();
            if (t.kind == 'quit') return;
            ts.putback(t);
            cout << result << expression() << endl;
        } catch(exception &e){
            cerr << e.what() <<"\nEnter \'" << print << "\' and try again.";
            clean_up_mess();
            cout << "\nTry again:\n";
    }

}
}
//------------------------------------------------------------------------------
int main()
try
{
    calculate();
    keep_window_open();
    return 0;
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

//------------------------------------------------------------------------------

