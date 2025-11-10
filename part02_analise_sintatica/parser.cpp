#include "superheader.h"

/**********************************************************
*
*                       PARSER FUNCTIONS
*
***********************************************************/

// Construtor da classe Parser que recebe uma string de entrada.
Parser::Parser(string input) {
    scanner = new Scanner(input); // Cria um novo scanner para processar a entrada.
    advance(); // Obtem o primeiro token.
}

// Metodo principal que inicia o processo de análise sintática.
void Parser::run() {
    try {
        Program(); // Inicia a análise do programa.
        cout << "Compilacao finalizada com sucesso." << endl; // Sucesso.
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
    } else {
        cout << "Esperava: " << Token::getTokenTypeName(t) << endl;
        cout << "Encontrou: " << Token::getTokenTypeName(lToken->type) << " ('" << lToken->lexeme << "')" << endl;
        error("Token inesperado em match()"); // Erro se o token nao for o esperado.
    }
}

/**********************************************************
*
*                       PROGRAM
*
***********************************************************/

// Producao principal: Program → ClassList
// Representa o programa completo composto por uma lista de classes.
void Parser::Program() {
    ClassList(); // Analisa a lista de classes.
    match(END_OF_FILE); // Espera que o arquivo termine corretamente.
}

/**********************************************************
*
*                       CLASS LIST
*
***********************************************************/

// Producao: ClassList → ClassDecl ClassList | ClassDecl
// Analisa uma ou mais declaracoes de classes.
void Parser::ClassList() {
    ClassDecl(); // Analisa uma declaracao de classe.
    if (lToken->type == CLASS) {
        ClassList(); // Se houver mais classes, analisa recursivamente.
    }
}

/**********************************************************
*
*                       CLASS DECL
*
***********************************************************/

// Producao: ClassDecl → class ID ClassBody | class ID extends ID ClassBody
// Reconhece a definicao de uma classe, com ou sem heranca.
void Parser::ClassDecl() {
    match(CLASS); // Espera a palavra reservada 'class'.
    match(ID); // Espera o identificador (nome da classe).
    
    if (lToken->type == EXTENDS) {
        advance(); // Se houver 'extends', avanca.
        match(ID); // Espera o identificador da classe pai.
    }
    
    ClassBody(); // Analisa o corpo da classe.
}

/**********************************************************
*
*                       CLASS BODY
*
***********************************************************/

// Producao: ClassBody → { VarDeclListOpt ConstructDeclListOpt MethodDeclListOpt }
// Analisa o corpo da classe contendo variaveis, construtores e metodos.
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

// Producao: VarDeclListOpt → VarDeclList | ε
// Analisa zero ou mais declaracoes de variaveis.
// IMPORTANTE: Todas as variaveis devem ser declaradas ANTES dos metodos.
void Parser::VarDeclListOpt() {
    // Processa todas as declaracoes de variaveis com tipos primitivos.
    while (lToken->type == INT || lToken->type == STRING) {
        VarDecl(); // Analisa cada declaracao de variavel.
    }
    // Nota: Variaveis com tipo ID (classe) não são processadas aqui
    // para evitar ambiguidade com metodos que retornam objetos.
}

// Producao: VarDecl → Type ID VarDeclOpt ; | Type [] ID VarDeclOpt ;
// Analisa a declaracao de uma ou mais variaveis do mesmo tipo.
void Parser::VarDecl() {
    Type(); // Analisa o tipo da variavel.
    
    // Verifica se é um array.
    if (lToken->type == LEFT_SQUARE_BRACKET) {
        advance();
        match(RIGHT_SQUARE_BRACKET);
    }
    
    // Espera o identificador da variavel.
    if (lToken->type != ID) {
        error("ID esperado na declaracao");
    }
    
    advance(); // Consome o ID.
    
    // Verifica se não é um metodo (metodos tem "(" apos o ID).
    if (lToken->type == LEFT_BRACKET) {
        error("Declaracao de metodo encontrada onde se esperava variavel. Declare todas as variaveis antes dos metodos.");
    }
    
    VarDeclOpt(); // Verifica se ha mais variaveis separadas por virgula.
    match(SEMICOLON); // Espera ponto e virgula ao final.
}

// Producao: VarDeclOpt → , ID VarDeclOpt | ε
// Analisa declaracoes adicionais de variaveis separadas por virgula.
void Parser::VarDeclOpt() {
    if (lToken->type == COMMA) {
        advance(); // Consome a virgula.
        match(ID); // Espera o proximo identificador.
        VarDeclOpt(); // Verifica se ha mais variaveis.
    }
}

// Producao: Type → int | string | ID
// Reconhece tipos primitivos ou classes personalizadas.
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

// Producao: ConstructDeclListOpt → ConstructDeclList | ε
// Analisa zero ou mais declaracoes de construtores.
void Parser::ConstructDeclListOpt() {
    if (lToken->type == CONSTRUCTOR) {
        ConstructDeclList(); // Se houver construtor, analisa a lista.
    }
}

// Producao: ConstructDeclList → ConstructDeclList ConstructDecl | ConstructDecl
// Analisa um ou mais construtores da classe.
void Parser::ConstructDeclList() {
    ConstructDecl(); // Analisa um construtor.
    if (lToken->type == CONSTRUCTOR) {
        ConstructDeclList(); // Se houver mais construtores, analisa recursivamente.
    }
}

// Producao: ConstructDecl → constructor MethodBody
// Analisa a declaracao de um construtor.
void Parser::ConstructDecl() {
    match(CONSTRUCTOR); // Espera a palavra reservada 'constructor'.
    MethodBody(); // Analisa o corpo do construtor.
}

/**********************************************************
*
*                   METHOD DECLARATIONS
*
***********************************************************/

// Producao: MethodDeclListOpt → MethodDeclList | ε
// Analisa zero ou mais declaracoes de metodos.
void Parser::MethodDeclListOpt() {
    if (isType()) {
        MethodDeclList(); // Se houver tipo, analisa a lista de metodos.
    }
}

// Producao: MethodDeclList → MethodDeclList MethodDecl | MethodDecl
// Analisa um ou mais metodos da classe.
void Parser::MethodDeclList() {
    MethodDecl(); // Analisa um metodo.
    if (isType()) {
        MethodDeclList(); // Se houver mais metodos, analisa recursivamente.
    }
}

// Producao: MethodDecl → Type ID MethodBody | Type [] ID MethodBody
// Analisa a declaracao de um metodo com seu tipo de retorno.
void Parser::MethodDecl() {
    Type(); // Analisa o tipo de retorno.
    
    // Verifica se retorna um array.
    if (lToken->type == LEFT_SQUARE_BRACKET) {
        advance();
        match(RIGHT_SQUARE_BRACKET);
    }
    
    match(ID); // Espera o identificador (nome do metodo).
    MethodBody(); // Analisa o corpo do metodo.
}

// Producao: MethodBody → ( ParamListOpt ) { StatementsOpt }
// Analisa o corpo de um metodo ou construtor.
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

// Producao: ParamListOpt → ParamList | ε
// Analisa zero ou mais parametros de um metodo ou construtor.
void Parser::ParamListOpt() {
    if (isType()) {
        ParamList(); // Se houver tipo, analisa a lista de parametros.
    }
}

// Producao: ParamList → ParamList , Param | Param
// Analisa um ou mais parametros separados por virgula.
void Parser::ParamList() {
    Param(); // Analisa o primeiro parametro.
    while (lToken->type == COMMA) {
        advance(); // Consome a virgula.
        Param(); // Analisa o proximo parametro.
    }
}

// Producao: Param → Type ID | Type [] ID
// Analisa um parametro com seu tipo e identificador.
void Parser::Param() {
    Type(); // Analisa o tipo do parametro.
    
    // Verifica se é um array.
    if (lToken->type == LEFT_SQUARE_BRACKET) {
        advance();
        match(RIGHT_SQUARE_BRACKET);
    }
    
    match(ID); // Espera o identificador do parametro.
}

/**********************************************************
*
*                   STATEMENTS
*
***********************************************************/

// Producao: StatementsOpt → Statements | ε
// Analisa zero ou mais comandos dentro de um bloco.
void Parser::StatementsOpt() {
    if (isStatement()) {
        Statements(); // Se houver comando, analisa a lista de comandos.
    }
}

// Producao: Statements → Statements Statement | Statement
// Analisa uma sequencia de comandos.
void Parser::Statements() {
    while (isStatement()) {
        Statement(); // Analisa cada comando da sequencia.
    }
}

// Producao: Statement → VarDeclList | AtribStat ; | PrintStat ; | ReadStat ; 
//                     | ReturnStat ; | SuperStat ; | IfStat | ForStat | break ; | ;
// Reconhece e analisa diferentes tipos de comandos.
void Parser::Statement() {
    // Verifica se é uma declaracao de variavel (tipos primitivos).
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

// Producao: AtribStat → LValue = Expression | LValue = AllocExpression
// Analisa comandos de atribuicao.
void Parser::AtribStat() {
    LValue(); // Lado esquerdo da atribuicao (variavel, array, ou membro).
    match(ASSIGNMENT); // Espera o operador de atribuicao '='.
    
    // Verifica se é uma alocacao (new ou array).
    if (lToken->type == NEW || lToken->type == INT || lToken->type == STRING) {
        AllocExpression(); // Alocacao de objeto ou array.
    } else {
        Expression(); // Expressao comum.
    }
}

// Producao: PrintStat → print Expression
// Analisa comando de impressao.
void Parser::PrintStat() {
    match(PRINT); // Espera a palavra reservada 'print'.
    Expression(); // Expressao a ser impressa.
}

// Producao: ReadStat → read LValue
// Analisa comando de leitura.
void Parser::ReadStat() {
    match(READ); // Espera a palavra reservada 'read'.
    LValue(); // Variavel onde o valor sera armazenado.
}

// Producao: ReturnStat → return Expression
// Analisa comando de retorno de funcao.
void Parser::ReturnStat() {
    match(RETURN); // Espera a palavra reservada 'return'.
    Expression(); // Expressao a ser retornada.
}

// Producao: SuperStat → super ( ArgListOpt )
// Analisa chamada ao construtor da superclasse.
void Parser::SuperStat() {
    match(SUPER); // Espera a palavra reservada 'super'.
    match(LEFT_BRACKET); // Abre lista de argumentos.
    ArgListOpt(); // Argumentos para o construtor pai (opcional).
    match(RIGHT_BRACKET); // Fecha lista de argumentos.
}

// Producao: IfStat → if ( Expression ) { Statements } 
//                  | if ( Expression ) { Statements } else { Statements }
// Analisa comando condicional if-else.
void Parser::IfStat() {
    match(IF); // Espera a palavra reservada 'if'.
    match(LEFT_BRACKET); // Abre expressao condicional.
    Expression(); // Condicao do if.
    match(RIGHT_BRACKET); // Fecha expressao condicional.
    match(LEFT_CURLY_BRACE); // Abre bloco do if.
    Statements(); // Comandos executados se condicao for verdadeira.
    match(RIGHT_CURLY_BRACE); // Fecha bloco do if.
    
    // Verifica se ha clausula else.
    if (lToken->type == ELSE) {
        advance(); // Consome 'else'.
        match(LEFT_CURLY_BRACE); // Abre bloco do else.
        Statements(); // Comandos executados se condicao for falsa.
        match(RIGHT_CURLY_BRACE); // Fecha bloco do else.
    }
}

// Producao: ForStat → for ( AtribStatOpt ; ExpressionOpt ; AtribStatOpt ) { Statements }
// Analisa comando de repeticao for.
void Parser::ForStat() {
    match(FOR); // Espera a palavra reservada 'for'.
    match(LEFT_BRACKET); // Abre estrutura do for.
    AtribStatOpt(); // Inicializacao (opcional).
    match(SEMICOLON);
    ExpressionOpt(); // Condicao de parada (opcional).
    match(SEMICOLON);
    AtribStatOpt(); // Incremento (opcional).
    match(RIGHT_BRACKET); // Fecha estrutura do for.
    match(LEFT_CURLY_BRACE); // Abre bloco do for.
    Statements(); // Comandos executados em cada iteracao.
    match(RIGHT_CURLY_BRACE); // Fecha bloco do for.
}

// Producao: AtribStatOpt → AtribStat | ε
// Analisa atribuicao opcional (usada no for).
void Parser::AtribStatOpt() {
    if (lToken->type == ID) {
        AtribStat(); // Atribuicao presente.
    }
}

// Producao: ExpressionOpt → Expression | ε
// Analisa expressao opcional (usada no for).
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

// Producao: LValue → ID LValueComp
// Analisa o lado esquerdo de uma atribuicao ou acesso.
void Parser::LValue() {
    match(ID); // Identificador inicial.
    LValueComp(); // Complemento opcional (acesso a membro, array, ou metodo).
}

// Producao: LValueComp → . ID LValueComp 
//                      | . ID [ Expression ] LValueComp 
//                      | . ID ( ArgListOpt ) LValueComp 
//                      | [ Expression ] LValueComp 
//                      | ε
// Analisa complementos de LValue (acesso a membros, arrays e chamadas).
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

// Producao: Expression → NumExpression | NumExpression RelOp NumExpression
// Analisa expressoes aritmeticas e relacionais.
void Parser::Expression() {
    NumExpression(); // Primeira expressao numerica.
    
    // Verifica se ha operador relacional (<, >, <=, >=, ==, !=).
    if (lToken->type == EQUAL || lToken->type == NOT_EQUAL || 
        lToken->type == LESS_THAN || lToken->type == GREATER_THAN || 
        lToken->type == LESS_OR_EQUAL_THAN || lToken->type == GREATER_OR_EQUAL_THAN) {
        advance(); // Consome o operador relacional.
        NumExpression(); // Segunda expressao numerica.
    }
}

// Producao: AllocExpression → new ID ( ArgListOpt ) | Type [ Expression ]
// Analisa alocacao de objetos ou arrays.
void Parser::AllocExpression() {
    if (lToken->type == NEW) {
        // Alocacao de objeto: new ID(args)
        advance(); // Consome 'new'.
        match(ID); // Nome da classe.
        match(LEFT_BRACKET); // Abre argumentos do construtor.
        ArgListOpt(); // Argumentos (opcional).
        match(RIGHT_BRACKET); // Fecha argumentos do construtor.
    }
    else if (lToken->type == INT || lToken->type == STRING || lToken->type == ID) {
        // Alocacao de array: Type[expr]
        Type(); // Tipo dos elementos.
        match(LEFT_SQUARE_BRACKET); // Abre tamanho do array.
        Expression(); // Tamanho do array.
        match(RIGHT_SQUARE_BRACKET); // Fecha tamanho do array.
    }
    else {
        error("AllocExpression esperada (new ID(...) ou Type[...])");
    }
}

// Producao: NumExpression → Term + Term | Term - Term | Term
// Analisa expressoes de adicao e subtracao.
void Parser::NumExpression() {
    Term(); // Primeiro termo.
    
    // Permite multiplas operacoes: a + b - c + d
    while (lToken->type == PLUS_OPERATOR || lToken->type == MINUS_OPERATOR) {
        advance(); // Consome operador + ou -.
        Term(); // Proximo termo.
    }
}

// Producao: Term → UnaryExpression * UnaryExpression 
//                | UnaryExpression / UnaryExpression 
//                | UnaryExpression % UnaryExpression 
//                | UnaryExpression
// Analisa expressoes de multiplicacao, divisao e modulo.
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

// Producao: UnaryExpression → + Factor | - Factor | Factor
// Analisa expressoes unarias (com operador + ou -).
void Parser::UnaryExpression() {
    if (lToken->type == PLUS_OPERATOR || lToken->type == MINUS_OPERATOR) {
        advance(); // Consome operador unario + ou -.
    }
    Factor(); // Fator (literal, variavel ou expressao entre parenteses).
}

// Producao: Factor → INTEGER_LITERAL | STRING_LITERAL | LValue | ( Expression )
// Analisa fatores: literais, variaveis ou expressoes entre parenteses.
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

// Producao: ArgListOpt → ArgList | ε
// Analisa zero ou mais argumentos em uma chamada de funcao.
void Parser::ArgListOpt() {
    // Verifica se ha algum argumento (comeca com expressao).
    if (lToken->type == ID || lToken->type == INTEGER_LITERAL || 
        lToken->type == STRING_LITERAL || lToken->type == PLUS_OPERATOR || 
        lToken->type == MINUS_OPERATOR || lToken->type == LEFT_BRACKET) {
        ArgList(); // Se houver, analisa a lista de argumentos.
    }
}

// Producao: ArgList → ArgList , Expression | Expression
// Analisa um ou mais argumentos separados por virgula.
void Parser::ArgList() {
    Expression(); // Primeiro argumento.
    
    // Analisa argumentos adicionais separados por virgula.
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
    cout << "Linha " << scanner->getLine() << ": " << str << endl;
    cout << "Token atual: " << Token::getTokenTypeName(lToken->type) << " ('" << lToken->lexeme << "')" << endl;
    exit(EXIT_FAILURE);
}
