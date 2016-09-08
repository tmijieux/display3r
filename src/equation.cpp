#include "display3r/Equation.hpp"

// Equation::Equation(std::string eqpath_): eqpath(eqpath_)
// {
//     this->readEquation(eqpath_);
    
//     this->xeq = this->parseLine(xeql);
//     this->yeq = this->parseLine(yeql);
//     this->zeq = this->parseLine(zeql);
// }

// Tree Equation::parseLine(std::string line)
// {
//     std::vector<Equation::Token> toks =
// 	Equation::Token::tokenize(line);
//     Tree tree;
//     Tree::Node *n1(NULL), *n2(NULL), *n3(NULL);
	
//     for (auto& tok : toks)  {
// 	if (tok.isClosingParen()) {
// 	    Tree::Node *n1, *n2, *n3;
// 	    n1 = this->stack.pop();
// 	    n2 = this->stack.pop();
// 	    n3 = this->stack.pop();

// 	    n2->setRightChild(n1);
// 	    n2->setLeftChild(n3);
// 	    this->stack.push(n2);
// 	} else if (!tok.isOpeningParen()) {
// 	    Tree::Node *n = new Tree::Node(tok);
// 	    this->stack.push(n);
// 	}
//     }

//     return tree;
// }
