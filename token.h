#ifndef TOKEN
#define TOKEN

#include <iostream>
#include <string>
#include <sstream>
#include <stack>
#include <math.h>

using namespace std;

class Token {

	public:
	static const int TYPE_UNDEFINED = 0;
	static const int TYPE_NUMBER = 1;
	static const int TYPE_OPERATOR = 2;
	static const int TYPE_BINARY_OPERATOR = 2;
	static const int TYPE_UNARY_OPERATOR = 3;
	static const int TYPE_LOGICAL_OPERATOR = 4;
	static const int TYPE_FUNCTION = 5;
	static const int TYPE_BRACKET = 6;
	static const int TYPE_VARIABLE = 7;
	static const int TYPE_SEPARATOR = 8;

	protected:
	int tokenType;
	int children;
	Token* left_;
	Token* right_;

	public:

	Token(int type) {
		tokenType = type;
		left_ = NULL;
		right_ = NULL;
	}

	Token* left() {
		return left_;
	}

	Token* right() {
		return right_;
	}

	void left(Token* left) {
		left_ = left;
	}

	void right(Token* right) {
		right_ = right;
	}

	bool push(stack<Token*> &expressionPolishNotation) {
		switch (getType())
			case Token::TYPE_NUMBER: case Token::TYPE_VARIABLE: return false;

		if (expressionPolishNotation.empty())
			return false;

		Token* tmp;

		if (getType() == Token::TYPE_FUNCTION) {
			if (getChildren() > 0) {
				if (left() == NULL) {
					if (expressionPolishNotation.empty())
						return false;
					tmp = expressionPolishNotation.top();
					expressionPolishNotation.pop();
					left(tmp);
					left()->push(expressionPolishNotation);
				}
			}
			if (getChildren() > 1) {
				if (right() == NULL) {
					if (expressionPolishNotation.empty())
						return false;
					tmp = expressionPolishNotation.top();
					expressionPolishNotation.pop();
					right(tmp);
					right()->push(expressionPolishNotation);
				}
			}
		} else {
			if (left() == NULL) {
				if (expressionPolishNotation.empty())
					return false;
				tmp = expressionPolishNotation.top();
				expressionPolishNotation.pop();
				left(tmp);
				left()->push(expressionPolishNotation);
			}
			if (right() == NULL) {
				if (expressionPolishNotation.empty())
					return false;
				tmp = expressionPolishNotation.top();
				expressionPolishNotation.pop();
				right(tmp);
				right()->push(expressionPolishNotation);
			}
		}

		return true;
	}

	void printNode(int level=0) {
		for(int i=0; i<level; i++)
			cout << "-- ";
		cout << "\"" << getValue() << "\""  << endl;
		if (right_ != NULL) right_->printNode(level+1);
		if (left_ != NULL) left_->printNode(level+1);
	}

	string getReadableType() {
		switch (tokenType) {
			case 0: return "TYPE_UNDEFINED";
			case 1: return "TYPE_NUMBER";
			case 2: return "TYPE_BINARY_OPERATOR";
			case 3: return "TYPE_UNARY_OPERATOR";
			case 4: return "TYPE_LOGICAL_OPERATOR";
			case 5: return "TYPE_FUNCTION";
			case 6: return "TYPE_BRACKET";
			case 7: return "TYPE_VARIABLE";
		}
	}

	virtual void getTokenInfo() {
		cout << "Token type is: " << getReadableType() << "\nToken value is " << getValue() << "\n";
	}

	virtual string getValue() {
		return "undefined";
	}

	virtual float calculate() {
		return 0;
	}

	int getType() {
		return tokenType;
	}

	int getChildren() { return children; }
};

class TokenNumber : public Token {
	private:
	float value;

	public:
	TokenNumber(string num) : Token(Token::TYPE_NUMBER) {
		stringstream ss(num);
		float convertedValue = 0;
		ss >> convertedValue;
		this->value = convertedValue;
	}

	string getValue() {
		return to_string(value);
	}

	float calculate() {
		return value;
	}
};

class TokenOperator : public Token {
	private:
	char value;
	int priority;

	public:

	TokenOperator(string initializer) : Token(Token::TYPE_OPERATOR){
		value = initializer.at(0);
		switch (value) {
			case '+': case '-':
				priority = 1;
				break;
			case '*': case '/':
				priority = 2;
				break;
			case '^':
				priority = 3;
				break;
		}
	}

	int comparePriority(TokenOperator *operator2) {
		return priority - operator2->getPriority(); 
	}

	int getPriority() {
		return priority;
	}

	string getValue() {
		string retVal = "";
		retVal += value;
		return retVal;
	}

	void getTokenInfo() {
		cout << "Token type is: " << getReadableType() << "\nToken value is " << getValue() << "\nOperator priority is " << getPriority() << endl;
	}

	float calculate() {
		switch (value){
			case '+': return right_->calculate() + left_->calculate();
			case '-': return right_->calculate() - left_->calculate();
			case '*': return right_->calculate() * left_->calculate();
			case '/': return right_->calculate() / left_->calculate();
			case '^': return pow(right_->calculate(), left_->calculate());
			default: return 0;
		}
	}
};

class TokenFunction : public Token {
	private:
	string value;

	public:
	TokenFunction(string initializer) : Token(Token::TYPE_FUNCTION) {
		value = initializer;
		if (
			initializer == "sin" ||
			initializer == "cos" ||
			initializer == "tg" ||
			initializer == "ctg" ||
			initializer == "sqrt"
		) this->children = 1;
		else if (
			initializer == "min" ||
			initializer == "max"
		) this->children = 2;
		else this->children = 0;
	}

	float calculate() {
		if (value == "sin") return sin(left_->calculate());
		if (value == "cos") return cos(left_->calculate());
		if (value == "tg") return tan(left_->calculate());
		if (value == "ctg") return 1/tan(left_->calculate());
		if (value == "sqrt") return sqrt(left_->calculate());
		if (value == "min") return min(left_->calculate(), right_->calculate());
		if (value == "max") return max(left_->calculate(), right_->calculate());
		else return 0;
	}

};

class TokenBracket : public Token {

	public:
	static const int TYPE_UNDEFINED = 0;
	static const int TYPE_ROUND = 1;
	static const int TYPE_SQUARE = 2;
	static const int TYPE_FIGURE = 3;

	static const int DIRECTION_UNDEFINED = 0;
	static const int DIRECTION_OPEN = 1;
	static const int DIRECTION_CLOSE = 2;

	private:
	char value;
	int type;
	int direction;

	public:
	TokenBracket(string initializer) : Token(Token::TYPE_BRACKET) {
		value = initializer.at(0);
		switch (value) {
			case '(':
				direction = DIRECTION_OPEN;
				type = TYPE_ROUND;
				break;
			case ')':
				direction = DIRECTION_CLOSE;
				type = TYPE_ROUND;
				break;
			case '[':
				direction = DIRECTION_OPEN;
				type = TYPE_SQUARE;
				break;
			case ']':
				direction = DIRECTION_CLOSE;
				type = TYPE_SQUARE;
				break;
			case '{':
				direction = DIRECTION_OPEN;
				type = TYPE_FIGURE;
				break;
			case '}':
				direction = DIRECTION_CLOSE;
				type = TYPE_FIGURE;
				break;
			default :
				direction = DIRECTION_UNDEFINED;
				type = TYPE_UNDEFINED;
				break;
		}
	}

	int getDirection() { return direction; }

	string getValue() {
		string retVal;
		return retVal+value;
	}
};

class TokenVariable : public Token {

	private:
	string title;

	public:
	TokenVariable(string varTitle) : Token(Token::TYPE_VARIABLE) {
		title = varTitle;
	}

};

class TokenSeparator : public Token {

	public:
	TokenSeparator() : Token(Token::TYPE_SEPARATOR) {}

	string getValue() {
		return "separator";
	}

};

class BinaryTree {
	private:
	Token *headNode_;

	public:
	BinaryTree(stack<Token*> expressionPolishNotation) {
		headNode_ = expressionPolishNotation.top();
		expressionPolishNotation.pop();
		headNode_->push(expressionPolishNotation);
	}

	void printTree() {
		headNode_->printNode();
	}

	void calculate() {
		cout << headNode_->calculate() << endl;
	}
};

#endif