#ifndef REQP
#define REQP

#include "token.h"

class RawEquisionParser {

	private:
	int iteration_positon;
	int rawSize;
	std::string RawData;

	public:
	RawEquisionParser(std::string rawString) {
		this->RawData = rawString;
		rawSize = rawString.size();
		iteration_positon = 0;
	}

	Token* nextToken() {

		string grabbedToken = "";
		char nextChar, currChar;
		int current_symbol_type = 0;

		if (iteration_positon < rawSize) {

			currChar = RawData.at(iteration_positon);
			iteration_positon++;
			grabbedToken += currChar;
			current_symbol_type = resolveSymbolType(currChar);

			// cout << current_symbol_type << endl;
			if (current_symbol_type) while(true) {
				if (iteration_positon >= rawSize)
					break;

				nextChar = RawData.at(iteration_positon);
				if (resolveSymbolType(nextChar) != current_symbol_type)
					break;

				currChar = nextChar;
				grabbedToken += currChar;
				iteration_positon++;
			}

		}

		Token* retVal;
		if (current_symbol_type == 0) retVal = new Token(Token::TYPE_UNDEFINED);
		else if (current_symbol_type == 1) retVal = new TokenNumber(grabbedToken);
		else if (current_symbol_type == 4) retVal = new TokenBracket(grabbedToken);
		else if (current_symbol_type == 3) retVal = new TokenOperator(grabbedToken);
		else if (current_symbol_type == 2) {
			if (isFunction(grabbedToken))
				retVal = new TokenFunction(grabbedToken);
			else retVal = new TokenVariable(grabbedToken);
		} else if (current_symbol_type == 5) retVal = new TokenSeparator();

		return retVal;
	}

	int resolveSymbolType(char s) {
		if (isdigit(s)) return SYM_NUMBER;
		string allowedSymbols = ALLOWED_SYMBOLS;
		if (allowedSymbols.find(s) !=  string::npos) return SYM_LETTER;
		if (
			s == '+' || 
			s == '-' || 
			s == '*' || 
			s == '/' ||
			s == '^'
		) return SYM_OPERATOR;
		if (isBracket(s)) return SYM_BRACKET;
		if (
			s == ' ' ||
			s == ','
		) return SYM_SEPARATOR;
		return SYM_UNDEFINED;
	}

};

#endif