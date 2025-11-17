#include "superheader.h"

/**********************************************************
*
*                       PARSER FUNCTIONS
*
***********************************************************/

Parser::Parser(string input, SymbolTable* st) {
    symbolTable = st;
    currentScope = st;
    currentClass = "";
    currentType = "";
    currentIsArray = false;
    scanner = new Scanner(input, symbolTable);
    advance();
}

void Parser::run() {
    try {
        Program();
        cout << "\n[SUCESSO] Compilacao finalizada com sucesso." << endl;
    } catch (const runtime_error& e) {
        cout << e.what() << endl;
    }
}

void Parser::advance() {
    lToken = scanner->nextToken();
}

void Parser::match(int t) {   
    if (lToken->type == t) {
        advance();
    } else {
        cout << "\n[ERRO SINTATICO] Linha " << scanner->getLine() << ": esperava '" 
             << Token::getTokenTypeName(t) << "' mas encontrou '" 
             << Token::getTokenTypeName(lToken->type) << "'" << endl;
        exit(EXIT_FAILURE);
    }
}

/**********************************************************
*
*                       PROGRAM
*
***********************************************************/

// Regra 1: Program → ClassList
void Parser::Program() {
    ClassList();
    match(END_OF_FILE);
}

/**********************************************************
*
*                       CLASS LIST
*
***********************************************************/

// Regra 2: ClassList → ClassDecl ClassList | ClassDecl
void Parser::ClassList() {
    ClassDecl();
    if (lToken->type == CLASS) {
        ClassList();
    }
}

/**********************************************************
*
*                       CLASS DECL
*
***********************************************************/

// Regra 3: ClassDecl → class ID ClassBody | class ID extends ID ClassBody
void Parser::ClassDecl() {
    match(CLASS);
    
    if (lToken->type != ID) {
        error("Nome da classe esperado");
    }
    string className = lToken->lexeme;
    currentClass = className;
    match(ID);
    
    string parentClass = "";
    if (lToken->type == EXTENDS) {
        advance();
        if (lToken->type != ID) {
            error("Nome da classe pai esperado");
        }
        parentClass = lToken->lexeme;
        match(ID); // Espera o identificador da classe pai.
    }
    
    // ANÁLISE SEMÂNTICA: Declara a classe na tabela de símbolos.
    declareClass(className, parentClass);
    
    enterScope();
    
    ClassBody(); // Analisa o corpo da classe.
    
    exitScope();
    currentClass = "";
}

/**********************************************************
*
*                       CLASS BODY
*
***********************************************************/

// Regra ClassBody → { VarDeclListOpt ConstructDeclListOpt MethodDeclListOpt }
void Parser::ClassBody() {
    match(LEFT_CURLY_BRACE); // Abre o corpo da classe.
    VarDeclListOpt(); // Analisa declaracoes de variaveis (opcional).
    ConstructDeclListOpt(); // Analisa declaracoes de construtores (opcional).
    MethodDeclListOpt(); // Analisa declaracoes de metodos (opcional).
    match(RIGHT_CURLY_BRACE); // Fecha o corpo da classe.
}


/**********************************************************
*
*                   VARIABLE DECLARATIONS
*
***********************************************************/

// Regra VarDeclListOpt → VarDeclList | ε
// IMPORTANTE: Todas as variaveis devem ser declaradas ANTES dos metodos.
void Parser::VarDeclListOpt() {
    while (lToken->type == INT || lToken->type == STRING) {
        VarDecl();
    }
}

// Regra VarDecl → Type ID VarDeclOpt ; | Type [] ID VarDeclOpt ;
void Parser::VarDecl() {
    currentType = lToken->lexeme;
    Type(); // Analisa o tipo da variavel.
    
    currentIsArray = false;
    if (lToken->type == LEFT_SQUARE_BRACKET) {
        currentIsArray = true;
        advance();
        match(RIGHT_SQUARE_BRACKET);
    }
    
    if (lToken->type != ID) {
        error("ID esperado na declaracao");
    }
    
    // ANÁLISE SEMÂNTICA: Declara a primeira variável.
    string varName = lToken->lexeme;
    declareVariable(varName, currentType, currentIsArray);
    
    advance(); // Consome o ID.
    
    VarDeclOpt(); // Verifica se ha mais variaveis separadas por virgula.
    match(SEMICOLON); // Espera ponto e virgula ao final.
}

// Regra VarDeclOpt → , ID VarDeclOpt | ε
void Parser::VarDeclOpt() {
    if (lToken->type == COMMA) {
        advance(); // Consome a virgula.
        
        if (lToken->type != ID) {
            error("ID esperado apos virgula na declaracao de variaveis");
        }
        
        // ANÁLISE SEMÂNTICA: Declara variável adicional com o mesmo tipo.
        string varName = lToken->lexeme;
        declareVariable(varName, currentType, currentIsArray);
        
        match(ID); // Espera o proximo identificador.
        VarDeclOpt(); // Verifica se ha mais variaveis.
    }
}

// Regra Type → int | string | ID
void Parser::Type() {
    if (lToken->type == INT || lToken->type == STRING || lToken->type == ID) {
        advance(); // Avanca se o tipo for valido.
    } else {
        error("Tipo esperado (int, string ou ID)");
    }
}

/**********************************************************
*
*                   CONSTRUCTOR DECLARATIONS
*
***********************************************************/

// Regra ConstructDeclListOpt → ConstructDeclList | ε
void Parser::ConstructDeclListOpt() {
    if (lToken->type == CONSTRUCTOR) {
        ConstructDeclList(); // Se houver construtor, analisa a lista.
    }
}

// Regra ConstructDeclList → ConstructDeclList ConstructDecl | ConstructDecl
void Parser::ConstructDeclList() {
    ConstructDecl(); // Analisa um construtor.
    if (lToken->type == CONSTRUCTOR) {
        ConstructDeclList(); // Se houver mais construtores, analisa recursivamente.
    }
}

// Regra ConstructDecl → constructor MethodBody
void Parser::ConstructDecl() {
    match(CONSTRUCTOR); // Espera a palavra reservada 'constructor'.
    
    // cout << "[SEMANTICO] Construtor declarado na classe '" << currentClass 
    //     << "' na linha " << scanner->getLine() << endl;
    
    // Cria novo escopo para o construtor.
    enterScope();
    MethodBody(); // Analisa o corpo do construtor.
    exitScope();
}

/**********************************************************
*
*                   METHOD DECLARATIONS
*
***********************************************************/

// Regra MethodDeclListOpt → MethodDeclList | ε
void Parser::MethodDeclListOpt() {
    if (isType()) {
        MethodDeclList(); // Se houver tipo, analisa a lista de metodos.
    }
}

// Regra MethodDeclList → MethodDeclList MethodDecl | MethodDecl
void Parser::MethodDeclList() {
    MethodDecl(); // Analisa um metodo.
    if (isType()) {
        MethodDeclList(); // Se houver mais metodos, analisa recursivamente.
    }
}

// Regra MethodDecl → Type ID MethodBody | Type [] ID MethodBody
void Parser::MethodDecl() {
    currentType = lToken->lexeme;
    Type(); // Analisa o tipo de retorno.
    
    currentIsArray = false;
    if (lToken->type == LEFT_SQUARE_BRACKET) {
        currentIsArray = true;
        advance();
        match(RIGHT_SQUARE_BRACKET);
    }
    
    if (lToken->type != ID) {
        error("Nome do metodo esperado");
    }
    string methodName = lToken->lexeme;
    
    // ANÁLISE SEMÂNTICA: Declara o método.
    declareMethod(methodName, currentType, currentIsArray);
    
    match(ID); // Espera o identificador (nome do metodo).
    
    // Cria novo escopo para o corpo do método.
    enterScope();
    MethodBody(); // Analisa o corpo do metodo.
    exitScope();
}

// Regra MethodBody → ( ParamListOpt ) { StatementsOpt }
void Parser::MethodBody() {
    match(LEFT_BRACKET); // Abre lista de parametros.
    ParamListOpt(); // Analisa parametros (opcional).
    match(RIGHT_BRACKET); // Fecha lista de parametros.
    match(LEFT_CURLY_BRACE); // Abre corpo do metodo.
    StatementsOpt(); // Analisa comandos (opcional).
    match(RIGHT_CURLY_BRACE); // Fecha corpo do metodo.
}

/**********************************************************
*
*                   PARAMETER LIST
*
***********************************************************/

// Regra ParamListOpt → ParamList | ε
void Parser::ParamListOpt() {
    if (isType()) {
        ParamList(); // Se houver tipo, analisa a lista de parametros.
    }
}

// Regra ParamList → ParamList , Param | Param
void Parser::ParamList() {
    Param(); // Analisa o primeiro parametro.
    while (lToken->type == COMMA) {
        advance(); // Consome a virgula.
        Param(); // Analisa o proximo parametro.
    }
}

// Regra Param → Type ID | Type [] ID
void Parser::Param() {
    currentType = lToken->lexeme;
    Type(); // Analisa o tipo do parametro.
    
    currentIsArray = false;
    if (lToken->type == LEFT_SQUARE_BRACKET) {
        currentIsArray = true;
        advance();
        match(RIGHT_SQUARE_BRACKET);
    }
    
    if (lToken->type != ID) {
        error("Nome do parametro esperado");
    }
    string paramName = lToken->lexeme;
    
    // ANÁLISE SEMÂNTICA: Declara o parâmetro como variável no escopo do método.
    Token* paramToken = new Token(ID, paramName);
    STEntry* paramEntry = new STEntry(paramToken, PARAMETER, currentType, currentIsArray, scanner->getLine());
    
    if (!currentScope->add(paramEntry)) {
        semanticError("Parametro '" + paramName + "' ja foi declarado");
    }
    
    // cout << "[SEMANTICO] Parametro '" << paramName << "' do tipo '" << currentType;
    // if (currentIsArray) cout << "[]";
    // cout << "' declarado na linha " << scanner->getLine() << endl;
    
    match(ID); // Espera o identificador do parametro.
}

/**********************************************************
*
*                   STATEMENTS
*
***********************************************************/

// Regra StatementsOpt → Statements | ε
void Parser::StatementsOpt() {
    if (isStatement()) {
        Statements(); // Se houver comando, analisa a lista de comandos.
    }
}

// Regra Statements → Statements Statement | Statement
void Parser::Statements() {
    while (isStatement()) {
        Statement(); // Analisa cada comando da sequencia.
    }
}

// Regra Statement → VarDeclList | AtribStat ; | PrintStat ; | ReadStat ; 
//                     | ReturnStat ; | SuperStat ; | IfStat | ForStat | break ; | ;
void Parser::Statement() {
    if (lToken->type == INT || lToken->type == STRING) {
        VarDecl(); // Declaracao de variavel dentro de metodo.
    }
    else if (lToken->type == ID) {
        // Atribuicao: ID.member = expr ou ID[i] = expr ou ID = expr
        AtribStat();
        match(SEMICOLON);
    }
    else if (lToken->type == PRINT) {
        PrintStat(); // Comando print.
        match(SEMICOLON);
    }
    else if (lToken->type == READ) {
        ReadStat(); // Comando read.
        match(SEMICOLON);
    }
    else if (lToken->type == RETURN) {
        ReturnStat(); // Comando return.
        match(SEMICOLON);
    }
    else if (lToken->type == SUPER) {
        SuperStat(); // Chamada ao construtor da superclasse.
        match(SEMICOLON);
    }
    else if (lToken->type == IF) {
        IfStat(); // Comando condicional if-else.
    }
    else if (lToken->type == FOR) {
        ForStat(); // Comando de repeticao for.
    }
    else if (lToken->type == BREAK) {
        advance(); // Comando break (saida de loop).
        match(SEMICOLON);
    }
    else if (lToken->type == SEMICOLON) {
        advance(); // Comando vazio.
    }
    else {
        error("Statement esperado");
    }
}

/**********************************************************
*
*                   STATEMENT TYPES
*
***********************************************************/

// Regra AtribStat → LValue = Expression | LValue = AllocExpression
void Parser::AtribStat() {
    LValue(); // Lado esquerdo da atribuicao (variavel, array, ou membro).
    match(ASSIGNMENT); // Espera o operador de atribuicao '='.
    
    if (lToken->type == NEW || lToken->type == INT || lToken->type == STRING) {
        AllocExpression(); // Alocacao de objeto ou array.
    } else {
        Expression(); // Expressao comum.
    }
}

// Regra PrintStat → print Expression
void Parser::PrintStat() {
    match(PRINT); // Espera a palavra reservada 'print'.
    Expression(); // Expressao a ser impressa.
}

// Regra ReadStat → read LValue
void Parser::ReadStat() {
    match(READ); // Espera a palavra reservada 'read'.
    LValue(); // Variavel onde o valor sera armazenado.
}

// Regra ReturnStat → return Expression
void Parser::ReturnStat() {
    match(RETURN); // Espera a palavra reservada 'return'.
    Expression(); // Expressao a ser retornada.
}

// Regra SuperStat → super ( ArgListOpt )
void Parser::SuperStat() {
    match(SUPER); // Espera a palavra reservada 'super'.
    match(LEFT_BRACKET); // Abre lista de argumentos.
    ArgListOpt(); // Argumentos para o construtor pai (opcional).
    match(RIGHT_BRACKET); // Fecha lista de argumentos.
}

// Regra IfStat → if ( Expression ) { Statements } 
//                  | if ( Expression ) { Statements } else { Statements }
void Parser::IfStat() {
    match(IF); // Espera a palavra reservada 'if'.
    match(LEFT_BRACKET); // Abre expressao condicional.
    Expression(); // Condicao do if.
    match(RIGHT_BRACKET); // Fecha expressao condicional.
    match(LEFT_CURLY_BRACE); // Abre bloco do if.
    
    // Cria escopo para o bloco if.
    enterScope();
    Statements(); // Comandos executados se condicao for verdadeira.
    exitScope();
    
    match(RIGHT_CURLY_BRACE); // Fecha bloco do if.
    
    if (lToken->type == ELSE) {
        advance(); // Consome 'else'.
        match(LEFT_CURLY_BRACE); // Abre bloco do else.
        
        // Cria escopo para o bloco else.
        enterScope();
        Statements(); // Comandos executados se condicao for falsa.
        exitScope();
        
        match(RIGHT_CURLY_BRACE); // Fecha bloco do else.
    }
}

// Regra ForStat → for ( AtribStatOpt ; ExpressionOpt ; AtribStatOpt ) { Statements }
void Parser::ForStat() {
    match(FOR); // Espera a palavra reservada 'for'.
    match(LEFT_BRACKET); // Abre estrutura do for.
    
    // Cria escopo para o for (inclui variáveis da inicialização).
    enterScope();
    
    AtribStatOpt(); // Inicializacao (opcional).
    match(SEMICOLON);
    ExpressionOpt(); // Condicao de parada (opcional).
    match(SEMICOLON);
    AtribStatOpt(); // Incremento (opcional).
    match(RIGHT_BRACKET); // Fecha estrutura do for.
    match(LEFT_CURLY_BRACE); // Abre bloco do for.
    Statements(); // Comandos executados em cada iteracao.
    match(RIGHT_CURLY_BRACE); // Fecha bloco do for.
    
    exitScope(); // Sai do escopo do for.
}

// Regra AtribStatOpt → AtribStat | ε
// Também pode ser uma declaração de variável (int i = 0)
void Parser::AtribStatOpt() {
    if (lToken->type == INT || lToken->type == STRING) {
        // Declaração de variável no for
        VarDecl();
    }
    else if (lToken->type == ID) {
        AtribStat(); // Atribuicao presente.
    }
}

// Regra ExpressionOpt → Expression | ε
void Parser::ExpressionOpt() {
    if (lToken->type == ID || lToken->type == INTEGER_LITERAL || 
        lToken->type == STRING_LITERAL || lToken->type == PLUS_OPERATOR || 
        lToken->type == MINUS_OPERATOR || lToken->type == LEFT_BRACKET) {
        Expression(); // Expressao presente.
    }
}


/**********************************************************
*
*                   LVALUE
*
***********************************************************/

// Regra LValue → ID LValueComp
void Parser::LValue() {
    if (lToken->type != ID) {
        error("Identificador esperado");
    }
    
    string varName = lToken->lexeme;
    
    // ANÁLISE SEMÂNTICA: Verifica se a variável foi declarada.
    checkVariableDeclared(varName);
    
    match(ID); // Identificador inicial.
    LValueComp(); // Complemento opcional (acesso a membro, array, ou metodo).
}

// Regra LValueComp → . ID LValueComp 
//                      | . ID [ Expression ] LValueComp 
//                      | . ID ( ArgListOpt ) LValueComp 
//                      | [ Expression ] LValueComp 
//                      | ε
void Parser::LValueComp() {
    if (lToken->type == DOT) {
        advance(); // Consome o ponto (acesso a membro).
        match(ID); // Identificador do membro.
        
        if (lToken->type == LEFT_SQUARE_BRACKET) {
            // Acesso a array: .ID[expr]
            advance();
            Expression(); // Indice do array.
            match(RIGHT_SQUARE_BRACKET);
        } else if (lToken->type == LEFT_BRACKET) {
            // Chamada de metodo: .ID(args)
            advance();
            ArgListOpt(); // Argumentos (opcional).
            match(RIGHT_BRACKET);
        }
        
        LValueComp(); // Permite encadeamento: obj.member.method()
    }
    else if (lToken->type == LEFT_SQUARE_BRACKET) {
        // Acesso a array: [expr]
        advance();
        Expression(); // Indice do array.
        match(RIGHT_SQUARE_BRACKET);
        LValueComp(); // Permite encadeamento: array[i].member
    }
    // ε (epsilon - nada a fazer, fim do LValue)
}

/**********************************************************
*
*                   EXPRESSIONS
*
***********************************************************/

// Regra Expression → NumExpression | NumExpression RelOp NumExpression
void Parser::Expression() {
    NumExpression(); // Primeira expressao numerica.
    
    if (lToken->type == EQUAL || lToken->type == NOT_EQUAL || 
        lToken->type == LESS_THAN || lToken->type == GREATER_THAN || 
        lToken->type == LESS_OR_EQUAL_THAN || lToken->type == GREATER_OR_EQUAL_THAN) {
        advance(); // Consome o operador relacional.
        NumExpression(); // Segunda expressao numerica.
    }
}

// Regra AllocExpression → new ID ( ArgListOpt ) | Type [ Expression ]
void Parser::AllocExpression() {
    if (lToken->type == NEW) {
        // Alocacao de objeto: new ID(args)
        advance(); // Consome 'new'.
        
        if (lToken->type != ID) {
            error("Nome da classe esperado apos 'new'");
        }
        
        string className = lToken->lexeme;
        
        // ANÁLISE SEMÂNTICA: Verifica se a classe foi declarada.
        checkClassDeclared(className);
        
        // cout << "[SEMANTICO] Alocacao de objeto da classe '" << className 
        //     << "' na linha " << scanner->getLine() << endl;
        
        match(ID); // Nome da classe.
        match(LEFT_BRACKET); // Abre argumentos do construtor.
        ArgListOpt(); // Argumentos (opcional).
        match(RIGHT_BRACKET); // Fecha argumentos do construtor.
    }
    else if (lToken->type == INT || lToken->type == STRING || lToken->type == ID) {
        // Alocacao de array: Type[expr]
        string arrayType = lToken->lexeme;
        
        // Se for tipo classe, verifica se existe.
        if (lToken->type == ID) {
            checkClassDeclared(arrayType);
        }
        
        Type(); // Tipo dos elementos.
        match(LEFT_SQUARE_BRACKET); // Abre tamanho do array.
        Expression(); // Tamanho do array.
        match(RIGHT_SQUARE_BRACKET); // Fecha tamanho do array.
        
        // cout << "[SEMANTICO] Alocacao de array do tipo '" << arrayType 
        //     << "' na linha " << scanner->getLine() << endl;
    }
    else {
        error("AllocExpression esperada (new ID(...) ou Type[...])");
    }
}

// Regra NumExpression → Term + Term | Term - Term | Term
void Parser::NumExpression() {
    Term(); // Primeiro termo.
    
    // Permite multiplas operacoes: a + b - c + d
    while (lToken->type == PLUS_OPERATOR || lToken->type == MINUS_OPERATOR) {
        advance(); // Consome operador + ou -.
        Term(); // Proximo termo.
    }
}

// Regra Term → UnaryExpression * UnaryExpression 
//                | UnaryExpression / UnaryExpression 
//                | UnaryExpression % UnaryExpression 
//                | UnaryExpression
void Parser::Term() {
    UnaryExpression(); // Primeira expressao unaria.
    
    // Permite multiplas operacoes: a * b / c % d
    while (lToken->type == MULTIPLY_OPERATOR || 
           lToken->type == DIVIDE_OPERATOR || 
           lToken->type == MODULO_OPERATOR) {
        advance(); // Consome operador *, / ou %.
        UnaryExpression(); // Proxima expressao unaria.
    }
}

// Regra UnaryExpression → + Factor | - Factor | Factor
void Parser::UnaryExpression() {
    if (lToken->type == PLUS_OPERATOR || lToken->type == MINUS_OPERATOR) {
        advance(); // Consome operador unario + ou -.
    }
    Factor(); // Fator (literal, variavel ou expressao entre parenteses).
}

// Regra Factor → INTEGER_LITERAL | STRING_LITERAL | LValue | ( Expression )
void Parser::Factor() {
    if (lToken->type == INTEGER_LITERAL) {
        advance(); // Literal inteiro.
    }
    else if (lToken->type == STRING_LITERAL) {
        advance(); // Literal string.
    }
    else if (lToken->type == ID) {
        LValue(); // Variavel, acesso a membro, array ou chamada de metodo.
    }
    else if (lToken->type == LEFT_BRACKET) {
        advance(); // Abre expressao entre parenteses.
        Expression(); // Expressao interna.
        match(RIGHT_BRACKET); // Fecha expressao entre parenteses.
    }
    else {
        error("Factor esperado (literal, LValue ou expressao entre parenteses)");
    }
}

/**********************************************************
*
*                   ARGUMENT LIST
*
***********************************************************/

// Regra ArgListOpt → ArgList | ε
void Parser::ArgListOpt() {
    if (lToken->type == ID || lToken->type == INTEGER_LITERAL || 
        lToken->type == STRING_LITERAL || lToken->type == PLUS_OPERATOR || 
        lToken->type == MINUS_OPERATOR || lToken->type == LEFT_BRACKET) {
        ArgList(); // Se houver, analisa a lista de argumentos.
    }
}

// Regra ArgList → ArgList , Expression | Expression
void Parser::ArgList() {
    Expression(); // Primeiro argumento.
    
    while (lToken->type == COMMA) {
        advance(); // Consome a virgula.
        Expression(); // Proximo argumento.
    }
}

/**********************************************************
*
*                   HELPER METHODS
*
***********************************************************/

// Metodo auxiliar para verificar se o token atual e um tipo.
bool Parser::isType() {
    return (lToken->type == INT || lToken->type == STRING || lToken->type == ID);
}

// Metodo auxiliar para verificar se o token atual inicia um statement.
bool Parser::isStatement() {
    return (lToken->type == INT || 
            lToken->type == STRING || 
            lToken->type == ID || 
            lToken->type == PRINT || 
            lToken->type == READ || 
            lToken->type == RETURN || 
            lToken->type == SUPER || 
            lToken->type == IF || 
            lToken->type == FOR || 
            lToken->type == BREAK || 
            lToken->type == SEMICOLON);
}

// Funcao para exibir mensagens de erro detalhadas.
void Parser::error(string str) {
    cout << "\n[ERRO SINTATICO] Linha " << scanner->getLine() << ": " << str << endl;
    exit(EXIT_FAILURE);
}

/**********************************************************
*
*                   SEMANTIC ANALYSIS METHODS
*
***********************************************************/

void Parser::enterScope() {
    currentScope = new SymbolTable(currentScope);
}

void Parser::exitScope() {
    if (currentScope->getParent() != nullptr) {
        currentScope = currentScope->getParent();
    }
}

// Declara uma classe na tabela de símbolos.
void Parser::declareClass(string className, string parentClass) {
    STEntry* existing = symbolTable->get(className);
    
    // Verifica se já existe uma classe com esse nome.
    if (existing != nullptr && existing->kind == CLASS_NAME) {
        semanticError("Classe '" + className + "' ja foi declarada na linha " + to_string(existing->line));
    }
    
    // Se há classe pai, verifica se ela existe.
    if (!parentClass.empty()) {
        STEntry* parent = symbolTable->get(parentClass);
        if (parent == nullptr || parent->kind != CLASS_NAME) {
            semanticError("Classe pai '" + parentClass + "' nao foi declarada");
        }
    }
    
    // Cria entrada para a classe.
    Token* classToken = new Token(ID, className);
    STEntry* classEntry = new STEntry(classToken, CLASS_NAME, "class", false, scanner->getLine());
    classEntry->parentClass = parentClass;
    
    if (!symbolTable->add(classEntry)) {
        semanticError("Erro ao adicionar classe '" + className + "' na tabela de simbolos");
    }
    
    // cout << "[SEMANTICO] Classe '" << className << "' declarada";
    // if (!parentClass.empty()) {
    //    cout << " (herda de '" << parentClass << "')";
    // }
    // cout << " na linha " << scanner->getLine() << endl;
}

// Declara uma variável na tabela de símbolos do escopo atual.
void Parser::declareVariable(string varName, string varType, bool isArray) {
    
    // Verifica se já existe no escopo ATUAL (não nos pais).
    if (currentScope->symbols.find(varName) != currentScope->symbols.end()) {
        STEntry* existing = currentScope->symbols[varName];
        semanticError("Variavel '" + varName + "' ja foi declarada na linha " + to_string(existing->line));
    }
    
    // Se o tipo é uma classe (ID), verifica se a classe existe.
    if (varType != "int" && varType != "string") {
        checkClassDeclared(varType);
    }
    
    // Cria entrada para a variável.
    Token* varToken = new Token(ID, varName);
    STEntry* varEntry = new STEntry(varToken, VARIABLE, varType, isArray, scanner->getLine());
    
    if (!currentScope->add(varEntry)) {
        semanticError("Erro ao adicionar variavel '" + varName + "' na tabela de simbolos");
    }
    
    // cout << "[SEMANTICO] Variavel '" << varName << "' do tipo '" << varType;
    // if (isArray) cout << "[]";
    // cout << "' declarada na linha " << scanner->getLine() << endl;
}

// Declara um método na tabela de símbolos.
void Parser::declareMethod(string methodName, string returnType, bool isArray) {
    if (currentScope->symbols.find(methodName) != currentScope->symbols.end()) {
        STEntry* existing = currentScope->symbols[methodName];
        semanticError("Metodo '" + methodName + "' ja foi declarado na linha " + to_string(existing->line));
    }
    
    // Se o tipo de retorno é uma classe, verifica se existe.
    if (returnType != "int" && returnType != "string" && returnType != "void") {
        checkClassDeclared(returnType);
    }
    
    // Cria entrada para o método.
    Token* methodToken = new Token(ID, methodName);
    STEntry* methodEntry = new STEntry(methodToken, METHOD, returnType, isArray, scanner->getLine());
    
    if (!currentScope->add(methodEntry)) {
        semanticError("Erro ao adicionar metodo '" + methodName + "' na tabela de simbolos");
    }
    
    // cout << "[SEMANTICO] Metodo '" << methodName << "' com retorno '" << returnType;
    // if (isArray) cout << "[]";
    // cout << "' declarado na linha " << scanner->getLine() << endl;
}

void Parser::checkVariableDeclared(string varName) {
    STEntry* entry = currentScope->get(varName);
    
    if (entry == nullptr) {
        semanticError("Variavel '" + varName + "' nao foi declarada");
    }
    
    if (entry->reserved) {
        semanticError("'" + varName + "' e uma palavra reservada e nao pode ser usada como variavel");
    }
    
    // cout << "[SEMANTICO] Variavel '" << varName << "' usada na linha " << scanner->getLine() 
    //     << " (declarada na linha " << entry->line << ")" << endl;
}

void Parser::checkClassDeclared(string className) {
    STEntry* entry = symbolTable->get(className);
    
    if (entry == nullptr || entry->kind != CLASS_NAME) {
        semanticError("Classe '" + className + "' nao foi declarada");
    }
}

// Lança um erro semântico com mensagem detalhada.
void Parser::semanticError(string message) {
    cout << "\n[ERRO SEMANTICO] Linha " << scanner->getLine() << ": " << message << endl;
    exit(EXIT_FAILURE);
}
