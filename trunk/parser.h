#ifndef _PARSER_H_
#define _PARSER_H_

#include "Singleton.h"
#include "lexer.h"
#include "tree.h"

class Parser : public Singleton<Parser>
{
public:
	Parser();
	virtual ~Parser();

	void parse();
	TreeNode<Token>* parse_global_scope();
	TreeNode<Token>* parse_declaration_block();
	TreeNode<Token>* parse_declaration();

	Tree<Token>* ast;
};

#endif