#include "superheader.h"

using namespace std;

class Parser {
public:
    // Construtor que inicializa o scanner com o arquivo de entrada e a tabela de símbolos
    Parser(string input, SymbolTable* st);

    // Método para iniciar o processo de parsing
    void run();

private:
    Scanner* scanner;         // Objeto Scanner para tokenizar a entrada
    Token* lToken;            // Token atual
    SymbolTable* symbolTable; // Tabela de símbolos para análise semântica
    SymbolTable* currentScope; // Escopo atual (para escopos aninhados)
    string currentClass;      // Nome da classe atual sendo processada
    string currentType;       // Tipo atual sendo processado
    bool currentIsArray;      // Se o tipo atual é array

    // Avança para o próximo token
    void advance();

    // Verifica se o token atual corresponde ao tipo esperado e avança
    void match(int t);

    // Métodos das produções gramaticais para a linguagem X++
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

    // Semantic analysis helper methods
    void enterScope();           // Cria um novo escopo (tabela filha)
    void exitScope();            // Retorna ao escopo pai
    void declareClass(string className, string parentClass = ""); // Declara uma classe
    void declareVariable(string varName, string varType, bool isArray); // Declara uma variável
    void declareMethod(string methodName, string returnType, bool isArray); // Declara um método
    void checkVariableDeclared(string varName); // Verifica se variável foi declarada
    void checkClassDeclared(string className);  // Verifica se classe foi declarada
    void semanticError(string message); // Lança erro semântico

    // Method to throw a syntax error with a message
    void error(string str);
};
// Comentários:
// - A classe Parser é responsável por analisar (parsear) a string de entrada de acordo com a gramática especificada.
// - O método run() inicia o processo de parsing chamando o método Program().
// - O método advance() avança para o próximo token usando o scanner.
// - O método match() verifica se o token atual corresponde ao tipo de token e, quando aplicável, ao lexema esperado, avançando em caso positivo.
// - Os métodos das produções gramaticais (Program, Function, VarDeclaration, etc.) implementam as regras de parsing para cada não-terminal da gramática.
// - Os métodos auxiliares (isType, isStatement, isExpression) verificam se o token atual atende a critérios específicos.
// - O método error() lança um runtime_error com uma mensagem de erro de sintaxe, incluindo o número da linha.
