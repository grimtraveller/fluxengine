#include "lexer.h"
#include "parser.h"
#include "tree.h"
#include <fstream>
#include <iostream>
#include <vector>

Lexer*	lexer	= new Lexer();
Parser*	parser	= new Parser();
Tree<Token>*  ast;

int main() {
	std::string source_data, t_src;
	std::ifstream sourcefile ("example.txt");

	//open the source file
	if ( sourcefile.is_open() ) {
		while ( !sourcefile.eof() ) {
			getline (sourcefile, t_src);
			source_data += t_src;
		}
		sourcefile.close();
	}

	//call the lexer
	Lexer::getInstance()->init(source_data);
	Parser::getInstance()->parse();

	delete lexer;
	delete parser;

	return 0;
}