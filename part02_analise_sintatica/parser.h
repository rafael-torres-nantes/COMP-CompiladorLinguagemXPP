#include "superheader.h"

using namespace std;

class Parser {
public:
    // Constructor that initializes the scanner with the input string
    Parser(string input);

    // Method to start the parsing process
    void run();

private:
    Scanner* scanner; // Scanner object to tokenize the input
    Token* lToken;    // Current token

    // Method to advance to the next token
    void advance();

    // Method to match the current token with the expected token type and lexeme
    void match(int t);

    // Grammar production methods
    void Program();          // Program → (Function)∗ EOF
    void Function();         // Function → Type ID( ParamTypes ){ ... } | void ID( ParamTypes ){ ... }
    void VarDeclaration();   // VarDeclaration → ID ([integerconstant] )?
    void Type();             // Type → char | int
    void ParamTypes();       // ParamTypes → void | Type ID([])?(,Type ID([])?)∗
    void Statement();        // Statement → if ... | while ... | for ... | return ... | Assign ; | ID( ... ) | { ... } | ;
    void Assign();           // Assign → ID ([ Expression ])? = Expression
    void Expression();       // Expression → - Expression | ! Expression | Expression BinOp Expression | ...
    void Expression_();      // Expression' → && RelExpression Expression' | || RelExpression Expression'
    void RelExpression();    // RelExpression → AddExpression RelExpression'
    void RelExpression_();   // RelExpression' → < AddExpression RelExpression' | > AddExpression RelExpression' | ...
    void AddExpression();    // AddExpression → MultExpression AddExpression'
    void AddExpression_();   // AddExpression' → + MultExpression AddExpression' | - MultExpression AddExpression'
    void MultExpression();   // MultExpression → UnExpression MultExpression'
    void MultExpression_();  // MultExpression' → * UnExpression MultExpression' | / UnExpression MultExpression'
    void UnExpression();     // UnExpression → ! UnExpression | - UnExpression | INTEGER_LITERAL | ...

    // Helper methods to check token types
    bool isType();           // Check if the current token is a type
    bool isStatement();      // Check if the current token is a statement
    bool isExpression();     // Check if the current token is an expression

    // Method to throw a syntax error with a message
    void error(string str);
};

// Comments:
// - The Parser class is responsible for parsing the input string according to the specified grammar.
// - The run() method starts the parsing process by calling the Program() method.
// - The advance() method advances to the next token using the scanner.
// - The match() method checks if the current token matches the expected token type and lexeme, and advances if it does.
// - The grammar production methods (Program, Function, VarDeclaration, etc.) implement the parsing rules for each non-terminal in the grammar.
// - The helper methods (isType, isStatement, isExpression) check if the current token matches specific criteria.
// - The error() method throws a runtime error with a syntax error message, including the line number.