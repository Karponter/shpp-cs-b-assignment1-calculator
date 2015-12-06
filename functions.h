#ifndef FUNC
#define FUNC

#include <string>

using namespace std;

bool isOperator(string token) {
	if (
		token == "+" ||
		token == "-" ||
		token == "*" ||
		token == "/" ||
		token == "^"
	) return true;
	return false;
}

int isBracket(char s) {
	if (
		s == ')' ||
		s == ']' ||
		s == '}'
	) return -1;
	if (
		s == '(' ||
		s == '[' ||
		s == '{'
	) return 1;	
	return 0;
}

bool isSeparator(string token) {
	if (token == "," || token == " ") return 1;
	return 0;
}

int comparePriority(string oper1, string oper2) {
	int prio1 = 0, prio2 = 0;
	
	if (oper1 == "+" || oper1 == "-") prio1 = 1;
	else if (oper1 == "*" || oper1 == "/") prio1 = 2;
	else if (oper1 == "^") prio1 = 3;

	if (oper2 == "+" || oper2 == "-") prio2 = 1;
	else if (oper2 == "*" || oper2 == "/") prio2 = 2;
	else if (oper2 == "^") prio2 = 3;
		
	return prio1 - prio2;
}

bool isFunction(string token) {
	return (
		token == "sin" ||
		token == "cos" ||
		token == "tg" ||
		token == "ctg" ||
		token == "min" ||
		token == "max" ||
		token == "floor" ||
		token == "round" ||
		token == "ceil" ||
		token == "sqrt"
	) ? true : false;
}

#endif