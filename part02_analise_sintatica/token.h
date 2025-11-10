using namespace std;

// Token types will be converted to integers later
enum TokenType 
{
    UNDEFINED,             // 0  - Undefined token
    ID,                    // 1  - Identifier (e.g., variable names)
    DIGIT,                 // 2  - Integer literal
    LETTER,                // 3  - Letter [a-zA-Z]
    INTEGER_CONSTANT,      // 4  - digit+
    CHAR_CONSTANT,         // 5  - 'ch' | '\n' | '\0', onde ch é qualquer caractere ASCII imprimível (como especificado pela função isprint(), exceto \ e aspas simples.
    STRING_CONSTANT,       // 6  - "(ch)*", onde ch é qualquer caractere ASCII imprimível (como especificado pela função isprint(), exceto aspas duplas e \n.

    RELATIONAL_OPERATOR,   // 7  - Relational operator
    EQUAL,                 // 8  - == (equal to)
    NOT_EQUAL,             // 9  - != (not equal to)
    GREATER_THAN,          // 10 - > (greater than)
    LESS_THAN,             // 11 - < (less than)
    GREATER_OR_EQUAL_THAN, // 12 - >= (greater or equal than)
    LESS_OR_EQUAL_THAN,    // 13 - <= (less or equal than)

    LOGICAL_OPERATOR,      // 14 - Logical operator
    AND_OPERATOR,          // 15 - && (logical AND)
    OR_OPERATOR,           // 16 - || (logical OR)
    NOT_OPERATOR,          // 17 - ! (logical NOT)

    ARITHMETIC_OPERATOR,   // 18 - Arithmetic operator
    PLUS_OPERATOR,         // 19 - + (addition)
    MINUS_OPERATOR,        // 20 - - (subtraction)
    MULTIPLY_OPERATOR,     // 21 - * (multiplication)
    DIVIDE_OPERATOR,       // 22 - / (division)

    SEPARATOR,             // 23 - Separator
    LEFT_BRACKET,          // 24 - ( (left parenthesis)
    RIGHT_BRACKET,         // 25 - ) (right parenthesis)
    LEFT_CURLY_BRACE,      // 26 - { (left curly brace)
    RIGHT_CURLY_BRACE,     // 27 - } (right curly brace)
    LEFT_SQUARE_BRACKET,   // 28 - [ (left square bracket)
    RIGHT_SQUARE_BRACKET,  // 29 - ] (right square bracket)
    SEMICOLON,             // 30 - ; (semicolon)
    COMMA,                 // 31 - , (comma)

    ASSIGNMENT,            // 32 - = (assignment)
    RESERVED_WORD,         // 33 - Reserved word (e.g., if, else)
    END_OF_FILE,           // 34 - End of file

    // Add reserved words here
    INT,                   // 35 - int
    CHAR,                  // 36 - char
    VOID,                  // 37 - void
    IF,                    // 38 - if
    ELSE,                  // 39 - else
    WHILE,                 // 40 - while
    FOR,                   // 41 - for
    RETURN                 // 42 - return
};

class Token 
{
    public: 
        int type;       // Token type
        int attribute;  // Attribute which can be empty
        string lexeme;  // Recognized text
    
        // Constructors for different types of tokens

        // Only type
        Token(int type) // Example: if, else.
        {
            this->type = type;
            attribute = UNDEFINED;
            lexeme = "";
        }

        // Type and lexeme
        Token(int type, string lexeme)
        {
            this->type = type;
            attribute = UNDEFINED;
            this->lexeme = lexeme;
        }
        
        // Type and attribute
        Token(int type, int attribute) 
        {
            this->type = type;
            this->attribute = attribute;
            lexeme = "";
        }

        // Static method to return the name of the token type
        static string getTokenTypeName(int type) {
            static string typeNames[] = {
                "UNDEFINED",
                "ID",
                "DIGIT",
                "LETTER",
                "INTEGER_CONSTANT",
                "CHAR_CONSTANT",
                "STRING_CONSTANT",
                "RELATIONAL_OPERATOR",
                "EQUAL",
                "NOT_EQUAL",
                "GREATER_THAN",
                "LESS_THAN",
                "GREATER_OR_EQUAL_THAN",
                "LESS_OR_EQUAL_THAN",
                "LOGICAL_OPERATOR",
                "AND_OPERATOR",
                "OR_OPERATOR",
                "NOT_OPERATOR",
                "ARITHMETIC_OPERATOR",
                "PLUS_OPERATOR",
                "MINUS_OPERATOR",
                "MULTIPLY_OPERATOR",
                "DIVIDE_OPERATOR",
                "SEPARATOR",
                "LEFT_BRACKET",
                "RIGHT_BRACKET",
                "LEFT_CURLY_BRACE",
                "RIGHT_CURLY_BRACE",
                "LEFT_SQUARE_BRACKET",
                "RIGHT_SQUARE_BRACKET",
                "SEMICOLON",
                "COMMA",
                "ASSIGNMENT",
                "RESERVED_WORD",
                "END_OF_FILE"
            };
            return typeNames[type];
        }    
};
