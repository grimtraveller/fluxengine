#include "lexer.h"
#include <algorithm>

Lexer::Lexer() {
}

Lexer::~Lexer() {
}

bool Lexer::init(std::string _source) {
	std::transform(_source.begin(), _source.end(), _source.begin(), std::toupper);
	m_source			= _source;
	m_currentCharPos	= 0;
	m_currentIdentifier = "";

	return true;
}

int Lexer::getToken() {
	m_currentToken = proto_getToken();
	return m_currentToken;
}

int Lexer::proto_getToken() {
	int		current_char = ' ';
	Token	currentToken;

	if(m_currentCharPos >= m_source.length())
		return TOK_EOF;

	//skip whitespace
	while( std::isspace(current_char) )
		current_char = getChar();

	//check for identifiers
	if( std::isalpha(current_char) )
	{
		m_currentIdentifier = current_char;

		while( std::isalnum((current_char = getChar())) )
			m_currentIdentifier += current_char;

		currentToken.identifier = m_currentIdentifier;

		if(currentToken.identifier == "INT") return TOK_INT;

		return TOK_IDENTIFIER;
	}

	if( std::isdigit(current_char) ) {
		std::string t_digit;

		do {
			t_digit += current_char;
			current_char = getChar();
			
		} while( std::isdigit(current_char));
		current_char = back();

		currentToken.intval = strtod(t_digit.c_str(), 0);

		return TOK_DIGIT;
	}

	switch(	current_char ) {
		case ',': return TOK_COMMA; break;
		case ';': return TOK_SEMICOLON; break;
		case '=': return TOK_OP_ASSIGN; break;
		default: return TOK_UNKNOWN; break;
	}

	
}

int Lexer::getCurrentToken() {
	return m_currentToken;
}

int Lexer::getChar() {
		return m_source[m_currentCharPos++];
}

int Lexer::back() {
		return m_source[m_currentCharPos--];
}