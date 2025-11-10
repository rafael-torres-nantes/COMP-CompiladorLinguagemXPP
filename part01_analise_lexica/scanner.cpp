#include "superheader.h"

// Construtor que recebe uma string com o nome do arquivo de entrada e preenche input com seu conteúdo.
Scanner::Scanner(string fileName)
{
    pos = 0;
    line = 1;

    ifstream inputFile(fileName, ios::in); // Verifica se o arquivo está aberto
    string fileLine;

    if (inputFile.is_open())
    {
        while (getline(inputFile, fileLine)) // Se estiver, preenche o input
        {
            this->input.append(fileLine + '\n');
        }
        inputFile.close();
    }
    else // Se não estiver
        cout << "Unable to open file\n";
}

// Getter que retorna a linha atual do arquivo
int Scanner::getLine()
{
    return line;
}

// Método que retorna o próximo token da entrada
Token* Scanner::nextToken()
{
    Token* token;
    int state = 0;
    string lexeme;

    while (true)
    {
        switch (state)
        {
        case 0: // Verifica os caracteres iniciais para determinar o tipo de token
            if (input[pos] == '\0')
            {
                token = new Token(END_OF_FILE);
                return token;
            }
            else if (input[pos] == '<')
                state = 5;
            else if (input[pos] == '=')
                state = 13;
            else if (input[pos] == '>')
                state = 6;
            else if (input[pos] == '*')
                state = 7;
            else if (input[pos] == '-')
                state = 8;
            else if (input[pos] == '+')
                state = 9;
            else if (input[pos] == '/')
                state = 10;
            else if (input[pos] == '&')
                state = 11;
            else if (input[pos] == '!')
                state = 16;
            else if (input[pos] == ')')
                state = 19;
            else if (input[pos] == '(')
                state = 20;
            else if (input[pos] == '{')
                state = 22;
            else if (input[pos] == '}')
                state = 21;
            else if (input[pos] == ']')
                state = 24;
            else if (input[pos] == '[')
                state = 23;
            else if (input[pos] == ',')
                state = 26;
            else if (input[pos] == ';')
                state = 27;
            else if (input[pos] == '|')
                state = 37;
            else if (input[pos] == '\'')
                state = 43;
            else if (input[pos] == '\"')
                state = 45;
            else if (isalpha(input[pos]))
            {
                state = 1;
                lexeme += input[pos]; // Palavra reservada
            }
            else if (isdigit(input[pos]))
                state = 3;
            else if (isspace(input[pos]))
            {
                if (input[pos] == '\n')
                    line++;
                state = 28;
            }
            else
                lexicalError();

            pos++;
            break;

        case 1: // Identificador e palavras reservadas
            if (!isalnum(input[pos]) && input[pos] != '_')
            {
                state = 2;
            }
            else
            {
                lexeme += input[pos]; // Palavra reservada
            }
            pos++;
            break;

        case 2: // Identificador e palavras reservadas
            pos--;
            if (lexeme == "char" || lexeme == "int" || lexeme == "else" ||
                lexeme == "return" || lexeme == "void" || lexeme == "if" ||
                lexeme == "while" || lexeme == "for")
            {
                state = 36; // Palavra reservada
            }
            else
            {
                state = 34; // Identificador normal
            }
            pos++;
            break;

        case 3: // Dígito
            if (!isdigit(input[pos]))
                state = 4;
            else
                lexeme += input[pos];

            pos++;
            break;

        case 4: // Integer Constant
            if (isalpha(input[pos])) {
                lexicalError();
            }
            token = new Token(INTEGER_CONSTANT, lexeme);
            pos--;
            return token;


        case 5: // <
            if (input[pos] == '=')
                state = 39;
            else
                state = 40;
            pos++;
            break;

        case 6: // >
            if (input[pos] == '=')
                state = 41;
            else
                state = 42;
            pos++;
            break;

        case 7: // *
            token = new Token(MULTIPLY_OPERATOR);
            return token;

        case 8: // -
            token = new Token(MINUS_OPERATOR);
            return token;

        case 9: // +
            token = new Token(PLUS_OPERATOR);
            return token;

        case 10: // / ou comentário
            if (input[pos] == '/')
            {
                state = 30;
            }
            else if (input[pos] == '*')
            { // Comentário */
                state = 32;
            }
            else
            {
                state = 31;
                pos++;
            }
            break;

        case 11: // &&
            if (input[pos] == '&')
                state = 12;
            else
                lexicalError();

            pos++;
            break;

        case 12: // &&
            token = new Token(AND_OPERATOR);
            return token;

        case 13: // == ou =
            if (input[pos] == '=') // == 
            {
                state = 14;
            }
            else
            { // = 
                state = 15;
            }

            pos++;
            break;

        case 14: // == 
            token = new Token(EQUAL);
            return token;

        case 15: // =
            token = new Token(ASSIGNMENT);
            pos--;
            return token;

        case 16: // != ou !
            if (input[pos] == '=')
            {
                state = 17;
            }
            else
            { // !
                state = 18;
            }
            break;

        case 17: // !=
            token = new Token(NOT_EQUAL);
            pos++;
            return token;

        case 18: // !
            token = new Token(NOT_OPERATOR);
            return token;

        case 19: // )
            token = new Token(RIGHT_BRACKET);
            return token;

        case 20: // (
            token = new Token(LEFT_BRACKET);
            return token;

        case 21: // }
            token = new Token(RIGHT_CURLY_BRACE);
            return token;

        case 22: // {
            token = new Token(LEFT_CURLY_BRACE);
            return token;

        case 23: // [
            token = new Token(LEFT_SQUARE_BRACKET);
            return token;

        case 24: // ]
            token = new Token(RIGHT_SQUARE_BRACKET);
            return token;

        case 26: // ,
            token = new Token(COMMA);
            return token;

        case 27: // ;
            token = new Token(SEMICOLON);
            return token;

        case 28: // Espaços em branco
            state = 0; // Retorna ao estado inicial
            break;

        case 30: // Passa para o próximo enquanto não houver quebra de linha
            while (input[pos] != '\n' && input[pos] != '\0')
            {
                pos++;
            }
            state = 0;
            break;

        case 31: // Divide
            token = new Token(DIVIDE_OPERATOR);
            pos--;
            return token;

        case 32: // Comentário */
            if (input[pos] == '*')
                state = 33;
            if (input[pos] == '\n')
                line++;

            pos++;
            break;

        case 33: // Comentário continua
            if (input[pos] == '/')
                state = 0;
            else
                state = 32;

            pos++;
            break;

        case 34: // Identificador normal
            token = new Token(ID, lexeme);
            pos--;
            return token;

        case 36: // Palavra reservada
            token = new Token(RESERVED_WORD, lexeme);
            pos--;
            return token;

        case 37: // ||
            if (input[pos] == '|')
                state = 38;
            else
                lexicalError();

            pos++;
            break;

        case 38: // ||
            token = new Token(OR_OPERATOR);
            return token;

        case 39: // <=
            token = new Token(LESS_OR_EQUAL_THAN);
            return token;

        case 40: // <
            token = new Token(LESS_THAN);
            pos--;
            return token;

        case 41: // >=
            token = new Token(GREATER_OR_EQUAL_THAN);
            return token;

        case 42: // >
            token = new Token(GREATER_THAN);
            pos--;
            return token;

        case 43: // Char literal '
            pos++; // Avança para o próximo caractere após o apóstrofo inicial
            if (input[pos] == '\\') {
                // Verifica sequência de escape
                pos++;
                if (input[pos] == 'n' || input[pos] == '0') { // '\n' ou '\0'
                    lexeme = string("\\") + input[pos];
                } else {
                    lexicalError(); // Qualquer outra sequência é inválida
                }
            } else if (isprint(input[pos]) && input[pos] != '\'' && input[pos] != '\\') {
                // Verifica se é um caractere ASCII imprimível válido (exceto aspas simples e barra invertida)
                lexeme = input[pos];
            } else {
                lexicalError();
            }
            pos++;
            
            // Verifica se há uma aspa simples de fechamento
            if (input[pos] != '\'') {
                lexicalError(); // Falta a aspa simples de fechamento
            }
            pos++;
            token = new Token(CHAR_CONSTANT, lexeme);
            return token;


        case 45: // String literal "
            pos++; // Avança para o próximo caractere após a aspa dupla inicial
            lexeme.clear();
            
            while (input[pos] != '\"') {
                if (input[pos] == '\0') {
                    lexicalError(); // Fim inesperado da entrada
                }
                if (input[pos] == '\n') {
                    lexicalError(); // Quebra de linha não permitida em uma string
                }
                if (input[pos] == '\\') {
                    // Ignorar a sequência de escape válida na string
                    pos++;
                    if (input[pos] != '\\' && input[pos] != '\"') {
                        lexicalError(); // Qualquer outra sequência de escape é inválida
                    }
                } else if (!isprint(input[pos])) {
                    lexicalError(); // Caracteres não imprimíveis são inválidos em uma string
                }
                lexeme += input[pos];
                pos++;
            }
            
            pos++; // Avança para além da aspa dupla de fechamento
            token = new Token(STRING_CONSTANT, lexeme);
            return token;


        default:
            lexicalError();
        }
    }
}

// Função de erro léxico
void Scanner::lexicalError()
{
    cout << "Lexical error at line " << line << endl;
    exit(1); // Finaliza o programa
}
