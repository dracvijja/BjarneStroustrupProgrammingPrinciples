
/*

	Grammar used for input from cin:



Calculate:
    Quit
    Print
    Statement

Statement
    Declaration
    Expression

Declaration:
    "let" name "=" Expression


Print:
    "|"
Quit:
   "q"

Expression:
    Term
    Expression "+" Term
    Expression "-" Term

Term:
    Primary
    Term "*" Primary
    Term "/" Primary

Primary:
    Number
    "pow" "(" Expression "," Expression ")"
    "sqrt" "(" Expression ")"
    "+" Primary
    "-" Primary
    "(" Expression ")"


*/

#include "../std_lib_facilities.h"

class Token {
public:
	char kind;
	double value;
	string name;
	Token(char ch) :kind(ch), value(0) { }
	Token(char ch, double val) :kind(ch), value(val) { }
	Token(char ch, string n) :kind(ch), name(n){}
};

class Token_stream {
    // keep track of tokens
public:
	Token_stream() :full(0), buffer(0) { }

	Token get();
	void unget(Token t) { buffer=t; full=true; }

	void ignore(char);
private:
    bool full;
	Token buffer;
};

const char let = 'L';
const char quit = 'q';
const char print = '|';
const char number = '8';
const char name = 'a';
const char sqrtsymb = 'm';
const char powsymb = 'p';
const string powkey = "pow";
const string sqrtkey = "sqrt";
const string prompt = "> ";
const string result = "= ";
const string declkey = "let";

Token_stream ts;  // for get() and putback()

Token Token_stream::get()
{
    // reads characters from cin, returns them as tokens
    // if the token stream buffer is full, returns the token in the buffer.
	if (full) { full=false; return buffer; }
	char ch;
	cin >> ch;
	switch (ch) {
	case '(':
	case ')':
	case '+':
	case '-':
	case '*':
	case '/':
	case '%':
    case ',':
	case print:
    case quit:
	case '=':
		return Token(ch);
	case '.':
	case '0':
	case '1':
	case '2':
	case '3':
	case '4':
	case '5':
	case '6':
	case '7':
	case '8':
	case '9':
	{
	    cin.putback(ch);
		double val;
		cin >> val;
		return Token(number,val);

	}
	default:
        // if the cin reads a character (a-z) append to s,
        // get input with cin.ger(), if it is a char or a digit, append to s
        // return decleration key if s is "let"
            if (isalpha(ch)) {
			string s,lcase_s;
			s += ch;
			lcase_s += ch;
			while(cin.get(ch) && (isalpha(ch) || isdigit(ch))){ s+=ch; lcase_s+=ch;}
			cin.putback(ch);

			for(int i=0; i<lcase_s.size(); i++){
                lcase_s[i] = tolower(lcase_s[i]);

            }
			if (lcase_s == declkey) return Token(let);

			if (lcase_s == sqrtkey) return Token(sqrtsymb);

			if (lcase_s == powkey) return Token(powsymb);

			return Token(name,s);
		}
		error("Bad token");
	}
}

void Token_stream::ignore(char c)
{
	if (full && c==buffer.kind) {
		full = false;
		return;
	}
	full = false;

	char ch;
	while (cin>>ch)
		if (ch==c) return;
}

struct Variable {
	string name;
	double value;
	Variable(string n, double v) :name(n), value(v) { }
};

vector<Variable> names;

double get_value(string s)
    // look for s in the variable vector, return
{
	for (int i = 0; i<names.size(); ++i)
		if (names[i].name == s) return names[i].value;
	error("get: undefined name ",s);
}

void set_value(string s, double d)
    // assign d to s
{
	for (int i = 0; i<=names.size(); ++i)
		if (names[i].name == s) {
			names[i].value = d;
			return;
		}
	error("set: undefined name ",s);
}

bool is_declared(string s)
    // check if s is already declared in names vector
{
	for (int i = 0; i<names.size(); ++i)
		if (names[i].name == s) return true;
	return false;
}

double define_var(string s, double d){
        if(is_declared(s)) error("declared twice");
        names.push_back(Variable(s,d));
        return d;
}
double expression();
double get_square(){
    Token t = ts.get();
    if(t.kind != '(') error ("'(' expected");
    t=ts.get();
    if(t.kind == ')') error("Empty square root function!");
    ts.unget(t);
    double d = expression();
    if(d<0) error("Can\'t take square root of a negative!");
    t = ts.get();
    if(t.kind != ')') error("')' expected");
    return sqrt(d);
}
double get_pow(){
    double rval, lval;

    Token t = ts.get(); // get next token
    if(t.kind != '(') error ("'(' expected");

    t = ts.get();
    if(t.kind == ')'){ error("get_pow: empty function()");}
    ts.unget(t);
    rval = expression();

    t = ts.get();
    if(t.kind != ',') error ("',' expected");
    lval = expression();
    int intval = narrow_cast<int>(lval);

    t = ts.get();  // get next token
    if(t.kind != ')') error ("')' expected");
    return pow(rval, intval);

}
double primary()
{
	Token t = ts.get();
	switch (t.kind) {
	case '(':
	{	double d = expression();
		t = ts.get();
		if (t.kind != ')') error("'(' expected");
		return d;
	}
	case sqrtsymb:
        return get_square();
    case powsymb:
        return get_pow();
	case '-':
		return - primary();
    case name:
        return get_value(t.name);
    case '+':
        return primary();
	case number:
        return t.value;
	default:
		error("primary expected");
	}
}

double term() // handle * and /
{
	double left = primary();
	while(true) {
		Token t = ts.get(); // get next token
		switch(t.kind) {
		case '*':
			left *= primary();
			break;
		case '/':
		{	double d = primary();
			if (d == 0) error("divide by zero");
			left /= d;
			break;
		}

		default:
			ts.unget(t);
			return left;
		}
	}
}

double expression() // handle + and -
{
	double left = term();
	while(true) {
		Token t = ts.get();
		switch(t.kind) {
		case '+':
			left += term();
			break;
		case '-':
			left -= term();
			break;
		default:
			ts.unget(t);
			return left;
		}
	}
}
double declaration()
    //
{
	Token t = ts.get();
	if (t.kind != name) error ("name expected in declaration");
	string var_name = t.name;

	Token t2 = ts.get();
	if (t2.kind != '=') error("= missing in declaration of " ,var_name);
	double d = expression();

	define_var(var_name,d);
	return d;
}

double statement()
{
	Token t = ts.get();
	switch(t.kind) {
	case let:
		return declaration();
	default:
		ts.unget(t);
		return expression();
	}
}

void clean_up_mess()
{
	ts.ignore(print);
}


void calculate()
{
	while(true)
    try {
		cout << prompt;
		Token t = ts.get();
		while (t.kind == print) t=ts.get();
		if (t.kind == quit) return;
		ts.unget(t);
		cout << result << statement() << endl;
	}
	catch(runtime_error& e) {
		cerr << e.what() << endl;
		//cerr << "Enter \'" << print << "\' to try again\n";

		clean_up_mess();
		//cout << endl;

	}
}

int main()
try
{
    define_var("k", 1000);
    calculate();
    return 0;
}
	catch (exception& e) {
		cerr << "exception: " << e.what() << endl;
		char c;
		while (cin >>c&& c!=';') ;
		return 1;
	}
	catch (...) {
		cerr << "exception\n";
		char c;
		while (cin>>c && c!=';');
		return 2;
	}
