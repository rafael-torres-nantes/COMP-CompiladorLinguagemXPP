#include "superheader.h"

/**********************************************************
*
*                       PARSER FUNCTIONS
*
***********************************************************/

// Construtor da classe Parser que recebe uma string de entrada.
Parser::Parser(string input, SymbolTable* table) : symbolTable(table) {
    scanner = new Scanner(input); // Cria um novo scanner para processar a entrada.
    advance(); // Obtem o primeiro token.
}

// Metodo principal que inicia o processo de análise sintática.
void Parser::run() {
    try {
        Program(); // Inicia a análise do programa.
        cout << "Compilation finished successfully." << endl; // Success.
    } catch (const runtime_error& e) {
        cout << e.what() << endl; // Exibe mensagem de erro se houver falha.
    }
}

// Avanca para o próximo token no scanner.
void Parser::advance() {
    lToken = scanner->nextToken(); // Atualiza o token atual.
}

// Verifica se o token atual corresponde ao esperado e avanca para o próximo.
void Parser::match(int t) {      
    if (lToken->type == t) {
        advance(); // Avanca se o token for o esperado.
    } 
    else { 
        error("Unexpected error in match()"); // Erro se o token nao for o esperado.
    }
}

/**********************************************************
*
*                       PROGRAM
*
***********************************************************/

// Producao principal que representa o programa inteiro.
// Continua analisando funcoes ate encontrar o fim do arquivo.
void Parser::Program() {
    while (lToken->type != END_OF_FILE) {
        Function(); // Análise de funcoes.
    }
    match(END_OF_FILE); // Espera que o arquivo termine corretamente.
}


/**********************************************************
*
*                       FUNCTION
*
***********************************************************/

// Producao que reconhece a definicao de uma funcao.
// Funcao pode comecar com "void", "int" ou "char".
void Parser::Function() {
    if (lToken->lexeme == "void" || lToken->lexeme == "int" || lToken->lexeme == "char") {
        advance(); // Avanca para o próximo token após o tipo.
        
        if (lToken->type == RESERVED_WORD) {
            error("Reserved word cannot be used as function name.");
        }
        match(ID); // Espera um identificador (nome da funcao).

        // Verifica se há parênteses para os parâmetros da funcao.
        match(LEFT_BRACKET);
        ParamTypes(); // Analisa os tipos dos parâmetros.
        match(RIGHT_BRACKET);
        match(LEFT_CURLY_BRACE); // Abre o bloco da funcao.

        // Declaracao de variáveis antes dos comandos.
        while (isType()) {
            advance(); // Avanca para o próximo token após o tipo.
            VarDeclaration(); // Análise de declaracao de variáveis.
            match(SEMICOLON); // Espera ponto e vírgula após a declaracao.
        }

        // Declaracao de comandos após as variáveis.
        while (isStatement()) {
            Statement(); // Análise de comandos.
        }

        match(RIGHT_CURLY_BRACE); // Fecha o bloco da funcao.
    } else {
        error("Function expected.");
    }
}

/**********************************************************
*
*                       VAR DECLARATION
*
***********************************************************/

// Producao que analisa a declaracao de variáveis.
void Parser::VarDeclaration() {
    
    checkDuplicatedDeclaration();

    match(ID); // Espera um identificador (nome da variável).
    if (lToken->type == LEFT_SQUARE_BRACKET) { // Verifica se e um array.
        advance();
        match(INTEGER_CONSTANT); // Espera o tamanho do array.
        match(RIGHT_SQUARE_BRACKET);
    }
}

/**********************************************************
*
*                       TYPE
*
***********************************************************/

// Producao que reconhece tipos ("int" ou "char").
void Parser::Type() {
    if (lToken->lexeme == "char" || lToken->lexeme == "int") {
        advance(); // Avanca se o tipo for válido.
    } else {
        error("Invalid type. Expected 'int' or 'char'.");
    }
}

/**********************************************************
*
*                       PARAM TYPES
*
***********************************************************/

// Producao que analisa os tipos dos parâmetros de uma funcao.
void Parser::ParamTypes() {
    if (lToken->lexeme == "void") {
        advance();
    } else {
        Type();
        checkDuplicatedDeclaration();
        match(ID); // Espera um identificador para o parâmetro.
        if (lToken->type == LEFT_SQUARE_BRACKET) { // Verifica se e um array.
            advance();
            match(RIGHT_SQUARE_BRACKET);
        }

        // Analisa múltiplos parâmetros separados por vírgula.
        while (lToken->type == COMMA) {
            advance();
            Type();
            checkDuplicatedDeclaration();
            match(ID);
            if (lToken->type == LEFT_SQUARE_BRACKET) {
                advance();
                match(RIGHT_SQUARE_BRACKET);
            }
        }
    }
}

/**********************************************************
*
*                       STATEMENT
*
***********************************************************/
// Producao que reconhece e analisa diferentes tipos de comandos.
void Parser::Statement() {
    if (lToken->lexeme == "if") {
        advance();
        match(LEFT_BRACKET);
        Expression(); // Análise da expressão condicional.
        match(RIGHT_BRACKET);
        Statement(); // Comando associado ao 'if'.
        if (lToken->lexeme == "else") {
            advance();
            Statement(); // Comando associado ao 'else'.
        }
    } 
    else if (lToken->lexeme == "while") {
        advance();
        match(LEFT_BRACKET);
        Expression(); // Expressão de controle do loop.
        match(RIGHT_BRACKET);
        Statement(); // Comando dentro do laço.
    } 
    else if (lToken->lexeme == "for") {
        advance();
        match(LEFT_BRACKET);
        // Verificação opcional para a inicialização
        if (lToken->type == ID) {
            isDeclared();
            Assign(); 
        }
        match(SEMICOLON);
        // Verificação opcional para a expressão condicional
        if (isExpression()) {
            Expression();
        }
        match(SEMICOLON);
        // Verificação opcional para a expressão de incremento
        if (lToken->type == ID) {
            isDeclared();
            Assign();
        }
        match(RIGHT_BRACKET);
        Statement();
    } 
    else if (lToken->lexeme == "return") {
        advance();
        if (isExpression()) {
            Expression();
        }
        match(SEMICOLON);
    } 
    else if (lToken->type == ID) {
        // Identificador encontrado, pode ser uma chamada de método, acesso a array, ou atribuição.
        string identifier = lToken->lexeme;
        isDeclared();
        advance();

        if (lToken->type == LEFT_BRACKET) { // Chamada de funcao
            match(LEFT_BRACKET);
            if (isExpression()) {
                Expression();
                while (lToken->type == COMMA) { // Verifica múltiplos argumentos
                    advance();
                    Expression();
                }
            }
            match(RIGHT_BRACKET);
            match(SEMICOLON); // Espera um ponto e vírgula após a chamada da funcao
        } 
        else if (lToken->type == LEFT_SQUARE_BRACKET) { // Acesso a array
            advance();
            Expression(); // Índice do array
            match(RIGHT_SQUARE_BRACKET);
            if (lToken->type == ASSIGNMENT) { // Atribuicao a um elemento do array
                Assign();
            }
            match(SEMICOLON); // Espera um ponto e vírgula após o acesso ao array
        } 
        else if (lToken->type == ASSIGNMENT) { // Atribuicao
            advance();
            Expression();
            match(SEMICOLON);
        } 
        else if (isExpression()) { // Caso seja uma expressao
            Expression();
            match(SEMICOLON);
        }
        else {
            error("Assignment, method call, or expression expected");
        }
    } 
    else if (lToken->type == LEFT_CURLY_BRACE) { // Bloco de código
        advance();
        while (isStatement()) {
            Statement();
        }
        match(RIGHT_CURLY_BRACE);
    } 
    else if (lToken->type == SEMICOLON) { // Declaracao vazia
        advance();
    } 
    else {
        error("Invalid Statement.");
    }
}



/**********************************************************
*
*                       ASSIGN
*
***********************************************************/
void Parser::Assign()
{   
    match(ID);
    if (lToken->type == LEFT_SQUARE_BRACKET) {
        advance();
        Expression();
        match(RIGHT_SQUARE_BRACKET);
    }
    match(ASSIGNMENT);
    Expression();

}


/**********************************************************
*
*                       EXPRESSION
*
***********************************************************/
// Producao que analisa expressoes aritmeticas.
void Parser::Expression() {
    if (lToken->lexeme == "-") {
        advance();
        Expression();
    } else if (lToken->lexeme == "!") {
        advance();
        Expression();
    } else {
        RelExpression();
        Expression_();
    }
}

/**********************************************************
*
*                       EXPRESSION
*
***********************************************************/
// Producao que analisa expressoes aritmeticas.
void Parser::Expression_()
{
    if (lToken->type == AND_OPERATOR || lToken->type == OR_OPERATOR) {
        advance();
        RelExpression();
        Expression_();
    }
}

/**********************************************************
* 
* RelExpression() = Analisa expressoes relacionais (==, !=, <, >, <=, >=).
*
***********************************************************/

// Producao que analisa expressoes relacionais.
void Parser::RelExpression()
{
    AddExpression();
    RelExpression_();
}

// Producao que analisa expressoes relacionais.
void Parser::RelExpression_()
{
    if (lToken->type == EQUAL || lToken->type == NOT_EQUAL || lToken->type == LESS_THAN || lToken->type == GREATER_THAN || lToken->type == LESS_OR_EQUAL_THAN || lToken->type == GREATER_OR_EQUAL_THAN) {
        advance();
        AddExpression();
        RelExpression_();
    }
}

/**********************************************************
* 
* AddExpression() = Analisa expressoes de adicao (+, -).
*
***********************************************************/
// Producao que analisa expressoes de adicao.
void Parser::AddExpression()
{
    MultExpression();
    AddExpression_();
}

// Producao que analisa expressoes de adicao.
void Parser::AddExpression_()
{
    if (lToken->type == PLUS_OPERATOR || lToken->type == MINUS_OPERATOR) {
        advance();
        MultExpression();
        AddExpression_();
    }
}

/**********************************************************
* 
* MultExpression() = Analisa expressoes de multiplicacao (*, /).
*
***********************************************************/

// Producao que analisa expressoes de multiplicacao.
void Parser::MultExpression()
{
    UnExpression();
    MultExpression_();
}


// Producao que analisa expressoes de multiplicacao.
void Parser::MultExpression_()
{
    if (lToken->type == MULTIPLY_OPERATOR || lToken->type == DIVIDE_OPERATOR) {
        advance();
        UnExpression();
        MultExpression_();
    }
}

/**********************************************************
*
* UnExpression() = Analisa expressoes unárias (-, !), 
*                  constantes (INTEGER_CONSTANT, CHAR_CONSTANT, STRING_CONSTANT), 
*                  identificadores e chamadas de funcao.
*
***********************************************************/
// Producao que analisa expressoes unárias.
void Parser::UnExpression()
{
    if (lToken->type == INTEGER_CONSTANT || lToken->type == CHAR_CONSTANT || lToken->type == STRING_CONSTANT) {
        advance();
    } else if (lToken->type == ID) {
        isDeclared();
        advance();
        if (lToken->type == LEFT_BRACKET) {
            match(RIGHT_BRACKET);
            while (lToken->type == COMMA) {
                advance();
                Expression();
            }
        } else if (lToken->type == LEFT_SQUARE_BRACKET) {
            advance();
            Expression();
            match(RIGHT_SQUARE_BRACKET);
        }
    } else if (lToken->lexeme == "-") {
        advance();
        UnExpression();
    } else if (lToken->lexeme == "!") {
        advance();
        UnExpression();
    } else if (lToken->type == LEFT_BRACKET) {
        advance();
        Expression();
        match(RIGHT_BRACKET);
    } else {
        error("Invalid expression.");
    }
}

// Metodo para verificar se o ID foi declarado.
void Parser::isDeclared()
{
    if (symbolTable->get(lToken->lexeme) == nullptr) {
        error("Not declaration of identifier: " + lToken->lexeme);
    }
}

// Metodo para verificar se houve uma declaracao duplicada.
void Parser::checkDuplicatedDeclaration() {

    if (symbolTable->get(lToken->lexeme) != nullptr && lToken->type == RESERVED_WORD) {
            error("Reserved word cannot be used as ID.");
    }

    if (symbolTable->get(lToken->lexeme) != nullptr) {
        error("Duplicated declaration of identifier: " + lToken->lexeme);
    } 
    
    symbolTable->add(new STEntry(new Token(ID, lToken->lexeme), false));
}   

// Metodo auxiliar para verificar se o token atual e um tipo.
bool Parser::isType()
{
    return (lToken->lexeme == "int" || lToken->lexeme == "char");
}

// Metodo auxiliar para verificar se o token atual e um comando.
bool Parser::isStatement()
{
    return lToken->lexeme == "if" || lToken->lexeme == "while" || lToken->lexeme == "for" || lToken->lexeme == "return" || lToken->type == ID || lToken->type == LEFT_CURLY_BRACE || lToken->type == SEMICOLON;
}

// Metodo auxiliar para verificar se o token atual e uma expressao.
bool Parser::isExpression()
{
    return lToken->type == ID || lToken->type == INTEGER_CONSTANT || lToken->type == CHAR_CONSTANT || lToken->type == STRING_CONSTANT || lToken->lexeme == "!" || lToken->lexeme == "-" || lToken->type == LEFT_BRACKET;
}

// Funcao para exibir mensagens de erro detalhadas.
void Parser::error(string str) {
    std::cout << "Line " << scanner->getLine() << ": " << str << std::endl;
    std::cout << "Erro no Token: " << lToken->getTokenTypeName(lToken->type) << " (" << lToken->lexeme << ")" << std::endl;
    exit(EXIT_FAILURE);
}
