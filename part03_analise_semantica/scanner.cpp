#include "superheader.h"

Scanner::Scanner(string fileName, SymbolTable* st)
{
    pos = 0;
    line = 1;
    symbolTable = st; // Armazena referencia para a tabela de simbolos.

    ifstream inputFile(fileName, ios::in); // Verifica se o arquivo esta aberto
    string fileLine;

    if (inputFile.is_open())
    {
        while (getline(inputFile, fileLine)) // Se estiver, preenche o input
        {
            this->input.append(fileLine + '\n');
        }
        inputFile.close();
    }
    else // Se nao estiver
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
            else if (input[pos] == '%')
                state = 50;
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
            else if (input[pos] == '.')
                state = 51;
            else if (input[pos] == '\"')
                state = 45;
            else if (isalpha(input[pos]) || input[pos] == '_')
            {
                state = 1;
                lexeme += input[pos];
            }
            else if (isdigit(input[pos]))
            {
                state = 3;
                lexeme += input[pos];
            }
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

        case 1: // Identificador
            if (isalnum(input[pos]) || input[pos] == '_')
            {
                lexeme += input[pos];
                pos++;
            }
            else
            {
                state = 2;
            }
            break;

        case 2: // Retorna identificador ou palavra reservada usando a tabela de simbolos
        {
            // Consulta a tabela de simbolos para verificar se o lexema e uma palavra reservada.
            STEntry* entry = symbolTable->get(lexeme);
            
            if (entry != nullptr && entry->reserved) {
                // E uma palavra reservada: retorna o token correspondente da tabela.
                token = new Token(entry->token->type, lexeme);
            } else {
                // E um identificador normal: cria um novo token ID.
                token = new Token(ID, lexeme);
            }
            
            return token;
        }

        case 3: // Dígito
            if (isdigit(input[pos]))
            {
                lexeme += input[pos];
                pos++;
            }
            else
                state = 4;
            break;

        case 4: // Integer Literal
            if (isalpha(input[pos]) || input[pos] == '_') {
                lexicalError();
            }
            token = new Token(INTEGER_LITERAL, lexeme);
            return token;


        case 5: // <
            if (input[pos] == '=')
            {
                pos++;
                token = new Token(LESS_OR_EQUAL_THAN);
                return token;
            }
            else
            {
                token = new Token(LESS_THAN);
                return token;
            }

        case 6: // >
            if (input[pos] == '=')
            {
                pos++;
                token = new Token(GREATER_OR_EQUAL_THAN);
                return token;
            }
            else
            {
                token = new Token(GREATER_THAN);
                return token;
            }

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
                state = 30; // Comentário de linha
                pos++;
            }
            else if (input[pos] == '*')
            {
                state = 32; // Comentário de bloco
                pos++;
            }
            else
            {
                token = new Token(DIVIDE_OPERATOR);
                return token;
            }
            break;

        case 13: // == ou =
            if (input[pos] == '=')
            {
                pos++;
                token = new Token(EQUAL);
                return token;
            }
            else
            {
                token = new Token(ASSIGNMENT);
                return token;
            }

        case 16: // !=
            if (input[pos] == '=')
            {
                pos++;
                token = new Token(NOT_EQUAL);
                return token;
            }
            else
            {
                lexicalError(); // ! sozinho não é um operador válido
            }
            break;

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

        case 30: // Comentário de linha - passa até o final da linha
            while (input[pos] != '\n' && input[pos] != '\0')
            {
                pos++;
            }
            state = 0;
            break;

        case 32: // Comentário de bloco - procura por */
            if (input[pos] == '*')
                state = 33;
            if (input[pos] == '\n')
                line++;
            if (input[pos] == '\0')
                lexicalError(); // Comentário não fechado

            pos++;
            break;

        case 33: // Continuação do comentário de bloco
            if (input[pos] == '/')
            {
                state = 0;
                pos++;
            }
            else
                state = 32;
            break;

        case 45: // String literal "
            lexeme.clear();
            
            while (input[pos] != '\"')
            {
                if (input[pos] == '\0')
                {
                    lexicalError(); // Fim inesperado da entrada
                }
                if (input[pos] == '\n')
                {
                    lexicalError(); // Quebra de linha não permitida em string
                }
                lexeme += input[pos];
                pos++;
            }
            
            pos++; // Avança para além da aspa dupla de fechamento
            token = new Token(STRING_LITERAL, lexeme);
            return token;

        case 50: // %
            token = new Token(MODULO_OPERATOR);
            return token;

        case 51: // .
            token = new Token(DOT);
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
