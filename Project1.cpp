//COP3530 Programming Project 1
//Prakash Pudhucode
//UFID 4156-0131
//COP3530 Section 13A8
#include <iostream>
#include <string>
#include <stack>
#include <algorithm>
#include <iostream>
#include <cctype>
#include <vector>
#include <unordered_map>
#include <cmath>
using namespace std;
const vector<string> operators = { "+", "-", "*", "/", "^", "(", ")", "=" };
vector<string> keywords = { "let","sin","cos","log","pi","e" };
unordered_map<string, double> map;

bool isOperator(char c) {
	if (c == '*' || c == '+' || c == '-' || c == '/' || c == '^' || c == '(' || c == ')' || c == '=')
		return true;

	return false;
}

bool isOperator(string s) {
	bool x = false;
	for (int i = 0; i < operators.size(); i++) {
		if (s.compare(operators[i]) == 0) {
			x = true;
		}
	}
	return x;
}

bool isOperand(char c) {
	if (c >= '0' && c <= '9')
		return true;
	if (c == '.')
		return true;

	return false;
}

bool isKeyword(string s) {
	bool x = false;
	for (int i = 0; i < keywords.size(); i++) {
		if (keywords[i].compare(s) == 0) {
			x = true;;
		}
	}
	return x;
}

bool isVariable(string s) {
	bool x = false;
		if (map.find(s) != map.end()) {
			x = true;
	}
	return x;
}

int precedent(string op)
{
	int weight = -1;
	if (op == "+" || op == "-")
		weight = 1;
	if (op == "*" || op == "/")
		weight = 2;
	if (op == "^")
		weight = 3;
	return weight;
}

vector<string> postfix(vector <string> x) {
	vector<string> output;
	stack<string> opt;
	for (int i = 0; i < x.size(); i++) {
		if (isOperator(x[i])) {
			if (x[i] != ")") {
			if(x[i] == "(")
				opt.push(x[i]);
			else if (opt.empty())
				opt.push(x[i]);
			else if (opt.top() == "(")
				opt.push(x[i]);
			else if (x[i] != ")" && precedent(opt.top()) < precedent(x[i]))
				opt.push(x[i]);
			else if (x[i] != ")" && precedent(opt.top()) >= precedent(x[i])) {
				if (x[i] == "^" && opt.top() == "^")
					opt.push(x[i]);
				else {
					while (!opt.empty() && x[i] != ")" && precedent(opt.top()) >= precedent(x[i])) {
						output.push_back(opt.top());
						opt.pop();
					}
					opt.push(x[i]);
				}
			}
			}
			else {//if it is a right parentheses
				while (opt.top() != "(") {
					output.push_back(opt.top());
					opt.pop();
				}
				opt.pop();
				if (opt.top() == "sin" || opt.top() == "cos" || opt.top() == "log") {
					output.push_back(opt.top());
					opt.pop();
				}
			}
		}
		else {
			if (isKeyword(x[i])) {
				if (x[i] == "pi" || x[i] == "e") {
					output.push_back(to_string(map[x[i]]));
				}
				else {
					opt.push(x[i]);
				}
				//output.push_back(x[i]);
			}
			else {
				output.push_back(x[i]);
			}
		}
	}
	while (!opt.empty()) {
		output.push_back(opt.top());
		opt.pop();
	}
	return output;
}

double evaluate(string op, double val1, double val2) {
	double result = 0;
			if(op== "+"){
				result = val1 + val2;}
			if(op== "-"){
				result = val1 - val2;}
			if(op== "*"){
				result = val1 * val2;}
			if(op== "/"){
				if (val2 == 0)
					result = string::npos;
				else{
				result = val1 / val2;}
				}
			if (op == "^") {
				result = pow(val1, val2);
			}
			return result;
}

vector<string> toinfix(string s) {
	vector<string> operand;
	string num = "";
	bool b = false;
	string x = "";
	for (int i = 0; i < s.length(); i++) {
		//cout << s[i]<< endl;
		if (isOperator(s[i])) {
			if (num != "") {
				operand.push_back(num);
				num = "";
			}
			x = s.substr(0, 1);
			operand.push_back(x);
			x = "";
			b = false;
			i = 0;
		}
		else if (true) {
			if (b) {
				num += s[i];
			}
			else {
				num = "";
				num += s[i];
				b = true;
				//substring 
			}
		}
		s.erase(0, 1);
		i = -1;
		//cout << num<<endl;
		//cout << s<<endl;
	}
	if (num != "")
		operand.push_back(num);
	return operand;
}

double kwevaluate(string op, double val1) {
	double result = 0;
	if (op == "sin") {
		result = sin(val1);
	}
	else if (op == "cos") {
		result = cos(val1);
	}
	else
		result = log(val1);

	return result;
}

double evalpostfix(vector<string> operand) {
	stack<double> values;
	double notinlist = 0;
	double result = 0;
	for (int i = 0; i < operand.size(); i++) {
		if (isdigit(operand[i][0])) {
			values.push(stod(operand[i]));
		}
		else if (isOperator(operand[i])) {
			double val2 = values.top();
			values.pop();
			double val1 = values.top();
			values.pop();
			double x = evaluate(operand[i], val1, val2);
			if (x == string::npos) {
				return string::npos;
				break;
			}
			else {
				values.push(x);
			}
		}
		else {						/*if (!isdigit(operand[i][0]) || !isOperator(operand[i]))*/
			if (!isKeyword(operand[i])) {
				if (!isVariable(operand[i])) {
					notinlist = -1;
					break;
				}
				else {
					operand[i] = to_string(map[operand[i]]);
					values.push(stod(operand[i]));
				}
			}
			else {
				if (operand[i] == "sin" || operand[i] == "cos" || operand[i] == "log") {
					double val1 = values.top();
					values.pop();
					values.push(kwevaluate(operand[i], val1));
				}
				else if (operand[i] == "pi" || operand[i] == "e") {
					operand[i] = to_string(map[operand[i]]);
					values.push(stod(operand[i]));
				}
			}
		}
	}
	if (!values.empty()) {
		result = values.top();
		values.pop();
	}
	if (notinlist == -1)
		result = string::npos - 10000;
	return result;
}
vector<string> lettoinfix(string s) {
	string num = "";
	bool b = false;
	string x = "";
	vector<string> operand;
	if (s.substr(0, 4).compare("let ") == 0 || s.substr(0, 4).compare("LET ") == 0) {
		operand.push_back("let");
		s = s.substr(4, s.length() - 1);
	}
	s.erase(remove(s.begin(), s.end(), ' '),
		s.end());
	for (int i = 0; i < s.length(); i++) {
		//cout << s[i]<< endl;
		if (isOperator(s[i])) {
			if (num != "") {
				operand.push_back(num);
				num = "";
			}
			x = s.substr(0, 1);
			operand.push_back(x);
			x = "";
			b = false;
			i = 0;
		}
		else if (true) {
			if (b) {
				num += s[i];
			}
			else {
				num = "";
				num += s[i];
				b = true;
				//substring 
			}
		}
		s.erase(0, 1);
		i = -1;
		//cout << num<<endl;
		//cout << s<<endl;
	}
	if (num != "")
		operand.push_back(num);
	return operand;
}

int main() {
	string s;
	map.emplace("e", 2.718);
	map.emplace("pi", 3.14169);
	stack<char> opt;
	stack<double> values;
	vector<string> operand;
	do {
		getline(cin, s);
		if (s.substr(0, 4).compare("let ") == 0 || s.substr(0, 4).compare("LET ") == 0) {
			operand = lettoinfix(s);
		}
		else {
			s.erase(remove(s.begin(), s.end(), ' '), s.end()); //remove spaces
			operand = toinfix(s);
		}

		bool notinlist = false;
		if (operand[0] == "quit")
			break;
		if (operand[0] == "let") {//if first index contains let
			//convert and evaluate everything after equal sign to postfix 
			vector<string> declare;
			for (int i = 0; i < operand.size()-3; i++) {
				string j = operand[i + 3];
				declare.push_back(j);
			}
			declare = postfix(declare);
			double result = evalpostfix(declare);
			if (isVariable(operand[1])) {
				unordered_map<string, double>::iterator it = map.find(operand[1]);
				if (it!= map.end())
					it->second = result;
			}
			else {
				map.emplace(operand[1], result);
			}
			//assign value to variable name
		}
		else { // just an expression
			operand = postfix(operand);
			//for (int i = 0; i < operand.size(); i++)
				//cout << operand[i] << " ";
			double result = evalpostfix(operand);
			if (result == string::npos)
				cout << "Divide-By-Zero" << endl;
			else if (result == string::npos-10000)
				cout << "Undeclared-Variable" << endl;
			else
				cout << result<< endl;

		}
	} while (operand[0] != "quit");
}




