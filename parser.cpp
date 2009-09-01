#include "parser.h"
#include "lexer.h"
#include "tree.h"

Parser::Parser() {
	ast = new Tree<Token>;
}

Parser::~Parser() {
}

void Parser::parse() {
	TreeNode<Token>* root = ast->AddNode(NODE_ROOT);
	TreeNode<Token>* child;

	Lexer::getInstance()->getToken();

	while(Lexer::getInstance()->getCurrentToken() != TOK_EOF)
	{
		child = parse_global_scope();
		if(child != NULL)
			ast->AddChild(root, child);
	}

	dl_iterator<Token> it = root->m_children.getIterator();

	for(it.begin(); it.valid(); it.forth())
		std::cout << it.item().type;
}

TreeNode<Token>* Parser::parse_global_scope() {
	
	TreeNode<Token>* node = ast->AddNode(NODE_GLOBAL);

	if(Lexer::getInstance()->getCurrentToken() == TOK_INT)
	{
		std::cout << "Parsing Declaration Block" << std::endl;
		ast->AddChild(node, parse_declaration_block());
	}

	return node;
}

TreeNode<Token>* Parser::parse_declaration_block() {

	TreeNode<Token>* node = NULL;

	if( Lexer::getInstance()->getCurrentToken() == TOK_INT)
	{
		TreeNode<Token>* node = ast->AddNode(NODE_DECLARATION_BLOCK);

		bool loop = false;
		do {
			ast->AddChild( node, parse_declaration( ) );

			loop = ( Lexer::getInstance()->getCurrentToken() == TOK_COMMA );

			if(loop) 
				Lexer::getInstance()->getToken();
		} while (loop);

		if(Lexer::getInstance()->getCurrentToken() != TOK_SEMICOLON)
			std::cout << "Expecting ;" << std::endl; exit(0);
	}

	Lexer::getInstance()->getToken();

	return node;
}

TreeNode<Token>* Parser::parse_declaration() {
	std::cout << "Parsing declaration" << std::endl;
	
	//dummy
	Lexer::getInstance()->getToken();
	Lexer::getInstance()->getToken();
	Lexer::getInstance()->getToken();
	Lexer::getInstance()->getToken();

	return ast->AddNode(NODE_DECLARATION);
}