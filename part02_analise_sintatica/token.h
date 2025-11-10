using namespace std;

// Token types will be converted to integers later
enum TokenType 
{
    UNDEFINED,             // 0  - Undefined token
    ID,                    // 1  - Identifier (e.g., variable names)
    INTEGER_LITERAL,       // 2  - Integer literal (digit+)
    STRING_LITERAL,        // 3  - String literal delimited by " and "

    OP,                    // 4  - Operator
    LESS_THAN,             // 5  - < (less than)
    GREATER_THAN,          // 6  - > (greater than)
    LESS_OR_EQUAL_THAN,    // 7  - <= (less or equal than)
    GREATER_OR_EQUAL_THAN, // 8  - >= (greater or equal than)
    PLUS_OPERATOR,         // 9  - + (addition)
    MINUS_OPERATOR,        // 10 - - (subtraction)
    MULTIPLY_OPERATOR,     // 11 - * (multiplication)
    DIVIDE_OPERATOR,       // 12 - / (division)
    MODULO_OPERATOR,       // 13 - % (modulo)
    ASSIGNMENT,            // 14 - = (assignment)
    EQUAL,                 // 15 - == (equal to)
    NOT_EQUAL,             // 16 - != (not equal to)

    SEP,                   // 17 - Separator
    LEFT_BRACKET,          // 18 - ( (left parenthesis)
    RIGHT_BRACKET,         // 19 - ) (right parenthesis)
    LEFT_SQUARE_BRACKET,   // 20 - [ (left square bracket)
    RIGHT_SQUARE_BRACKET,  // 21 - ] (right square bracket)
    LEFT_CURLY_BRACE,      // 22 - { (left curly brace)
    RIGHT_CURLY_BRACE,     // 23 - } (right curly brace)
    SEMICOLON,             // 24 - ; (semicolon)
    DOT,                   // 25 - . (dot)
    COMMA,                 // 26 - , (comma)

    // Reserved words
    CLASS,                 // 27 - class
    EXTENDS,               // 28 - extends
    INT,                   // 29 - int
    STRING,                // 30 - string
    BREAK,                 // 31 - break
    PRINT,                 // 32 - print
    READ,                  // 33 - read
    RETURN,                // 34 - return
    SUPER,                 // 35 - super
    IF,                    // 36 - if
    ELSE,                  // 37 - else
    FOR,                   // 38 - for
    NEW,                   // 39 - new
    CONSTRUCTOR,           // 40 - constructor

    END_OF_FILE            // 41 - End of file
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
                "INTEGER_LITERAL",
                "STRING_LITERAL",
                "OP",
                "LESS_THAN",
                "GREATER_THAN",
                "LESS_OR_EQUAL_THAN",
                "GREATER_OR_EQUAL_THAN",
                "PLUS_OPERATOR",
                "MINUS_OPERATOR",
                "MULTIPLY_OPERATOR",
                "DIVIDE_OPERATOR",
                "MODULO_OPERATOR",
                "ASSIGNMENT",
                "EQUAL",
                "NOT_EQUAL",
                "SEP",
                "LEFT_BRACKET",
                "RIGHT_BRACKET",
                "LEFT_SQUARE_BRACKET",
                "RIGHT_SQUARE_BRACKET",
                "LEFT_CURLY_BRACE",
                "RIGHT_CURLY_BRACE",
                "SEMICOLON",
                "DOT",
                "COMMA",
                "CLASS",
                "EXTENDS",
                "INT",
                "STRING",
                "BREAK",
                "PRINT",
                "READ",
                "RETURN",
                "SUPER",
                "IF",
                "ELSE",
                "FOR",
                "NEW",
                "CONSTRUCTOR",
                "END_OF_FILE"
            };
            return typeNames[type];
        }    
};
