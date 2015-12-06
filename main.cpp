#define SYM_UNDEFINED 0;
#define SYM_NUMBER 1;
#define SYM_LETTER 2;
#define SYM_OPERATOR 3;
#define SYM_BRACKET 4;
#define SYM_SEPARATOR 5;
#define ALLOWED_SYMBOLS "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ"

#include <iostream>
#include <string>
#include <stack>
#include "functions.h"
#include "token.h"
#include "RawEquisionParser.h"

using namespace std;

int main(int argc, char const *argv[]) {

	RawEquisionParser rep("sin(64-2)^(3*4)+1");

	stack<Token*> operatorS;
	stack<Token*> resultS;
	Token* temporaryToken = rep.nextToken();

	// equision to polish notation transformation
	{
		while (temporaryToken->getType() != Token::TYPE_UNDEFINED) {
	
			if (temporaryToken->getType() == Token::TYPE_NUMBER) {
				resultS.push(temporaryToken);
			} else if (temporaryToken->getType() == Token::TYPE_FUNCTION) {
				operatorS.push(temporaryToken);
			} else if (temporaryToken->getType() == Token::TYPE_SEPARATOR) {
				while (!operatorS.empty()) {
					Token* topStack = operatorS.top();
					if (
						topStack->getType() == Token::TYPE_BRACKET && 
						((TokenBracket*)topStack)->getDirection() == TokenBracket::DIRECTION_OPEN
					) {
						operatorS.pop();
						break;
					}
					resultS.push(topStack);
					operatorS.pop();
				}
			} else if (temporaryToken->getType() == Token::TYPE_BRACKET) {
				if (((TokenBracket*)temporaryToken)->getDirection() == TokenBracket::DIRECTION_OPEN) {
					operatorS.push(temporaryToken);
				}
				else {
					while (!operatorS.empty()) {
						Token* topStack = operatorS.top();
						if (
							topStack->getType() == Token::TYPE_BRACKET && 
							((TokenBracket*)topStack)->getDirection() == TokenBracket::DIRECTION_OPEN
						) {
							operatorS.pop();
							break;
						}
						resultS.push(topStack);
						operatorS.pop();
					}
				}
			} else if (temporaryToken->getType() == Token::TYPE_OPERATOR) {
				if (!operatorS.empty()) {
					TokenOperator* topStackOperator = (TokenOperator*)operatorS.top();
					int priorityDifference = ((TokenOperator*)temporaryToken)->comparePriority(topStackOperator);
					if (priorityDifference < 0) {
						resultS.push(topStackOperator);
						operatorS.pop();
					}
				}
				operatorS.push(temporaryToken);
			} 
			temporaryToken = rep.nextToken();
		}
		
		// dropping all the rest operators from stack to the polish notation
		while (!operatorS.empty()) {
			resultS.push(operatorS.top());
			operatorS.pop();
		}
	}

	// dump polish notation to console
	stack<Token*> newOne = resultS;
	while (!newOne.empty()) {
		cout << newOne.top()->getValue() << " ";
		newOne.pop();
	}

	// evaluating formula
	cout << endl << endl;
	BinaryTree tree(resultS);
	tree.printTree();
	cout << endl << endl;
	tree.calculate();
	cout << endl << endl;

	return 0;
}