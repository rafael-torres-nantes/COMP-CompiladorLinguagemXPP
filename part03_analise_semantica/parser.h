#include "superheader.h"

using namespace std;

class Parser {
public:
    // Constructor that initializes the scanner with the input file and symbol table
    Parser(string input, SymbolTable* st);

    // Method to start the parsing process
    void run();

private:
    Scanner* scanner;         // Scanner object to tokenize the input
    Token* lToken;            // Current token
    SymbolTable* symbolTable; // Symbol table for semantic analysis

    // Method to advance to the next token
    void advance();

    // Method to match the current token with the expected token type
    void match(int t);

    // Grammar production methods for X++ language
    void Program();              // Program → ClassList
    void ClassList();            // ClassList → ClassDecl ClassList | ClassDecl
    void ClassDecl();            // ClassDecl → class ID ClassBody | class ID extends ID ClassBody
    void ClassBody();            // ClassBody → { VarDeclListOpt ConstructDeclListOpt MethodDeclListOpt }
    void VarDeclListOpt();       // VarDeclListOpt → VarDeclList | ε
    void VarDecl();              // VarDecl → Type ID VarDeclOpt ; | Type [] ID VarDeclOpt ;
    void VarDeclOpt();           // VarDeclOpt → , ID VarDeclOpt | ε
    void Type();                 // Type → int | string | ID
    void ConstructDeclListOpt(); // ConstructDeclListOpt → ConstructDeclList | ε
    void ConstructDeclList();    // ConstructDeclList → ConstructDeclList ConstructDecl | ConstructDecl
    void ConstructDecl();        // ConstructDecl → constructor MethodBody
    void MethodDeclListOpt();    // MethodDeclListOpt → MethodDeclList | ε
    void MethodDeclList();       // MethodDeclList → MethodDeclList MethodDecl | MethodDecl
    void MethodDecl();           // MethodDecl → Type ID MethodBody | Type [] ID MethodBody
    void MethodBody();           // MethodBody → ( ParamListOpt ) { StatementsOpt }
    void ParamListOpt();         // ParamListOpt → ParamList | ε
    void ParamList();            // ParamList → ParamList , Param | Param
    void Param();                // Param → Type ID | Type [] ID
    void StatementsOpt();        // StatementsOpt → Statements | ε
    void Statements();           // Statements → Statements Statement | Statement
    void Statement();            // Statement → VarDeclList | AtribStat ; | PrintStat ; | ReadStat ; | ReturnStat ; | SuperStat ; | IfStat | ForStat | break ; | ;
    void AtribStat();            // AtribStat → LValue = Expression | LValue = AllocExpression
    void PrintStat();            // PrintStat → print Expression
    void ReadStat();             // ReadStat → read LValue
    void ReturnStat();           // ReturnStat → return Expression
    void SuperStat();            // SuperStat → super ( ArgListOpt )
    void IfStat();               // IfStat → if ( Expression ) { Statements } | if ( Expression ) { Statements } else { Statements }
    void ForStat();              // ForStat → for ( AtribStatOpt ; ExpressionOpt ; AtribStatOpt ) { Statements }
    void AtribStatOpt();         // AtribStatOpt → AtribStat | ε
    void ExpressionOpt();        // ExpressionOpt → Expression | ε
    void LValue();               // LValue → ID LValueComp
    void LValueComp();           // LValueComp → . ID LValueComp | . ID [ Expression ] LValueComp | . ID ( ArgListOpt ) LValueComp | [ Expression ] LValueComp | ε
    void Expression();           // Expression → NumExpression | NumExpression RelOp NumExpression
    void AllocExpression();      // AllocExpression → new ID ( ArgListOpt ) | Type [ Expression ]
    void NumExpression();        // NumExpression → Term + Term | Term - Term | Term
    void Term();                 // Term → UnaryExpression * UnaryExpression | UnaryExpression / UnaryExpression | UnaryExpression % UnaryExpression | UnaryExpression
    void UnaryExpression();      // UnaryExpression → + Factor | - Factor | Factor
    void Factor();               // Factor → INTEGER_LITERAL | STRING_LITERAL | LValue | ( Expression )
    void ArgListOpt();           // ArgListOpt → ArgList | ε
    void ArgList();              // ArgList → ArgList , Expression | Expression

    // Helper methods to check token types
    bool isType();               // Check if the current token is a type (int, string, ID)
    bool isStatement();          // Check if the current token starts a statement

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