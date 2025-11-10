#include "parser.h"

Parser::Parser(string input)
{
	scanner = new Scanner(input);
}

void
Parser::advance()
{
	lToken = scanner->nextToken();
}

void
Parser::match(int t)
{
	if (lToken->name == t || lToken->attribute == t)
		advance();
	else
		error("Erro inesperado");
}

void
Parser::run()
{
	advance();	

	program();
	
	cout << "Compilação encerrada com sucesso!\n";
}

void
Parser::program()
{
    while (lToken->name != END_OF_FILE)
        function();

    match(END_OF_FILE);
}

void
Parser::function()
{
    type();
    match(ID);
    match(LPARENTHESE);
    paramTypes();
    match(RPARENTHESE);
    match(LBRACE);
    //TODO
}

//TODO

void
Parser::error(string str)
{
	cout << "Linha " << scanner->getLine() << ": " << str << endl;

	exit(EXIT_FAILURE);
}
