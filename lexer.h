#ifndef _LEXER_H_
#define _LEXER_H_

#include <string>
#include <iostream>
#include <cctype>
#include "Singleton.h"

enum TOKENS {
	TOK_EOF = 0,
	TOK_UNKNOWN = 1,
	TOK_IDENTIFIER,
	TOK_COMMA,
	TOK_SEMICOLON,
	TOK_DIGIT,
	TOK_LBRACE,
	TOK_RBRACE,
	TOK_INT,
	TOK_OP_ASSIGN
};



struct Token {
	int				type;
	int				intval;
	bool			boolval;
	std::string		identifier;
};

class Lexer : public Singleton<Lexer> {
public:
	Lexer();
	virtual ~Lexer();

	bool	init(std::string _source);
	int		proto_getToken();
	int		getToken();
	int		getCurrentToken();
	int		getChar();
	int		back();

	int				m_currentToken;
private:
	std::string		m_source;
	std::string		m_currentIdentifier;
	int				m_currentCharPos;
	
	
};

#endif