# Compilador X++ - Análise Léxica, Sintática e Semântica

## 👨‍💻 Projeto desenvolvido por: [Rafael Torres Nantes](https://github.com/rafael-torres-nantes) e [Sarah Baraldi](https://github.com/mbaraldi-sarah).

## Índice

* [📚 Sobre o Projeto](#-sobre-o-projeto)
* [🛠️ Tecnologias Utilizadas](#%EF%B8%8F-tecnologias-utilizadas)
* [📂 Estrutura do Projeto](#-estrutura-do-projeto)
* [🖥️ Funcionalidades Implementadas](#%EF%B8%8F-funcionalidades-implementadas)
   * [🔍 Etapa 1 - Análise Léxica](#-etapa-1---análise-léxica)
   * [📋 Etapa 2 - Análise Sintática](#-etapa-2---análise-sintática)
   * [🧮 Etapa 3 - Análise Semântica](#-etapa-3---análise-semântica)
* [📌 Como Executar](#-como-executar)
* [🧪 Testes](#-testes)
* [📝 Mensagens de Erro](#-mensagens-de-erro)

## 📚 Sobre o Projeto

Este projeto implementa um **compilador completo front-end** para a linguagem **X++**, uma linguagem orientada a objetos simplificada inspirada em Java. O compilador realiza:

- ✅ **Análise Léxica**: Reconhecimento de tokens (palavras reservadas, identificadores, operadores, etc.)
- ✅ **Análise Sintática**: Verificação da estrutura gramatical do código
- ✅ **Análise Semântica**: Validação de tipos, escopos, declarações e uso de identificadores
- ✅ **Tratamento de Erros**: Mensagens claras no estilo g++/gcc

O compilador foi desenvolvido em **C++** e processa arquivos `.xpp`, identificando e reportando erros léxicos, sintáticos e semânticos de forma clara e precisa.

## 🛠️ Tecnologias Utilizadas

- **Linguagem**: C++ (C++11 ou superior)
- **Compilador**: g++ (MinGW/GCC)
- **IDE**: Visual Studio Code
- **Versionamento**: Git/GitHub
- **Sistema Operacional**: Windows/Linux/Mac

## 📂 Estrutura do Projeto

```
part03_analise_semantica/
├── principal.cpp          # Arquivo principal (main)
├── scanner.h/cpp         # Analisador léxico
├── parser.h/cpp          # Analisador sintático
├── token.h               # Definição de tokens
├── stentry.h/cpp         # Entrada da tabela de símbolos
├── symboltable.h/cpp     # Tabela de símbolos
├── superheader.h         # Header unificado
├── run_tests.ps1         # Script de testes automatizados
└── tests/                # Casos de teste
    ├── test_success.xpp
    ├── test_erro_lexico.xpp
    ├── test_erro_sintaxe.xpp
    └── test_erro_semantico*.xpp
```


## 🖥️ Funcionalidades Implementadas

### 🔍 Etapa 1 - Análise Léxica

O analisador léxico (`scanner.cpp`) implementa uma **máquina de estados finita** que reconhece e classifica os tokens da linguagem X++.

#### Tokens Reconhecidos:

| Categoria | Tokens |
|-----------|--------|
| **Palavras Reservadas** | `class`, `extends`, `int`, `string`, `break`, `print`, `read`, `return`, `super`, `if`, `else`, `for`, `new`, `constructor` |
| **Identificadores** | Sequências de letras, dígitos e `_` (começando por letra ou `_`) |
| **Literais** | Inteiros: `123`, `0`, `999` <br> Strings: `"texto"` |
| **Operadores Aritméticos** | `+`, `-`, `*`, `/`, `%` |
| **Operadores Relacionais** | `==`, `!=`, `<`, `>`, `<=`, `>=` |
| **Operador de Atribuição** | `=` |
| **Separadores** | `(`, `)`, `{`, `}`, `[`, `]`, `,`, `;`, `.` |
| **Comentários** | Linha: `// comentário` <br> Bloco: `/* comentário */` |

#### Características:

- ✅ Ignora espaços em branco e tabulações
- ✅ Conta linhas corretamente para relatório de erros
- ✅ Detecta strings não terminadas
- ✅ Identifica caracteres inválidos
- ✅ Diferencia palavras reservadas de identificadores usando tabela de símbolos

### 📋 Etapa 2 - Análise Sintática

O analisador sintático (`parser.cpp`) implementa uma **análise descendente recursiva (top-down)** que verifica se o código segue a gramática da linguagem X++.

#### Gramática da Linguagem X++:

A gramática completa está implementada com **41 produções**, incluindo:

| Não-Terminal | Produções |
|--------------|-----------|
| **Program** | ClassList |
| **ClassList** | ClassDecl ClassList \| ClassDecl |
| **ClassDecl** | `class ID` ClassBody \| `class ID extends ID` ClassBody |
| **ClassBody** | `{` VarDeclListOpt ConstructDeclListOpt MethodDeclListOpt `}` |
| **VarDecl** | Type `ID` VarDeclOpt `;` \| Type `[]` `ID` VarDeclOpt `;` |
| **Type** | `int` \| `string` \| `ID` |
| **MethodDecl** | Type `ID` MethodBody \| Type `[]` `ID` MethodBody |
| **Statement** | VarDecl \| AtribStat `;` \| PrintStat `;` \| ReadStat `;` \| ReturnStat `;` \| SuperStat `;` \| IfStat \| ForStat \| `break ;` \| `;` |
| **Expression** | NumExpression \| NumExpression RelOp NumExpression |
| **LValue** | `ID` LValueComp |

#### Estruturas Suportadas:

- ✅ **Classes e Herança**: `class A extends B { ... }`
- ✅ **Construtores**: `constructor(params) { ... }` com suporte a `super()`
- ✅ **Métodos**: Retorno de tipos primitivos ou objetos, incluindo arrays
- ✅ **Atributos**: Variáveis e arrays como membros de classe
- ✅ **Comandos Condicionais**: `if (expr) { ... } else { ... }`
- ✅ **Laços**: `for (init; cond; incr) { ... }` com suporte a `break`
- ✅ **Alocação**: `new Classe(args)` e `Tipo[expr]`
- ✅ **Acesso a Membros**: `obj.atributo`, `obj.metodo()`, `array[i]`
- ✅ **Operadores**: Aritméticos (`+`, `-`, `*`, `/`, `%`) e relacionais (`==`, `!=`, `<`, `>`, `<=`, `>=`)

### 🧮 Etapa 3 - Análise Semântica

O analisador semântico implementa uma **tabela de símbolos hierárquica** com controle de escopos e realiza verificações semânticas completas.

#### Tabela de Símbolos:

A tabela de símbolos (`symboltable.cpp`) armazena:
- **Palavras reservadas** (inseridas na inicialização)
- **Classes declaradas** (escopo global)
- **Variáveis** (escopos aninhados)
- **Métodos e construtores**
- **Parâmetros**

Cada entrada (`STEntry`) contém:
```cpp
- Token* token        // Token associado
- SymbolKind kind     // KEYWORD, CLASS_NAME, VARIABLE, METHOD, PARAMETER
- string type         // Tipo (int, string, nome da classe)
- bool isArray        // Se é array
- int line            // Linha de declaração
- string parentClass  // Classe pai (para classes)
- bool reserved       // Se é palavra reservada
```

#### Gerenciamento de Escopos:

O compilador implementa **escopos hierárquicos**:
```
Global (Tabela Principal)
  ├─ Classe A
  │   ├─ Atributos de A
  │   ├─ Construtor
  │   │   └─ Parâmetros e variáveis locais
  │   └─ Métodos de A
  │       └─ Parâmetros e variáveis locais
  └─ Classe B
      └─ ...
```

- `enterScope()`: Cria novo escopo filho
- `exitScope()`: Retorna ao escopo pai
- `get(nome)`: Busca símbolo no escopo atual e pais (busca hierárquica)

#### Verificações Semânticas Implementadas:

| Verificação | Descrição |
|------------|-----------|
| ✅ **Declaração de Variáveis** | Verifica se variável foi declarada antes do uso |
| ✅ **Redeclaração** | Impede redeclaração no mesmo escopo |
| ✅ **Declaração de Classes** | Verifica se classe existe ao usar como tipo |
| ✅ **Herança Válida** | Verifica se classe pai existe no `extends` |
| ✅ **Palavras Reservadas** | Impede uso de palavras reservadas como identificadores |
| ✅ **Escopo de Variáveis** | Variáveis de blocos internos não vazam para fora |
| ✅ **Tipos** | Valida tipos em declarações de variáveis e métodos |

#### Exemplo de Análise Semântica:

**Código Válido:**
```cpp
class Pessoa {
    int idade;
    
    constructor(int i) {
        idade = i;  // ✓ 'idade' declarada na classe
    }
}
```

**Código com Erro:**
```cpp
class Teste {
    constructor() {
        resultado = 10;  // ✗ 'resultado' não foi declarado
    }
}
```
**Saída:**
```
erro: linha 3: 'resultado' nao foi declarado neste escopo
```

## 📌 Como Executar

### Pré-requisitos

- **g++** (MinGW no Windows, GCC no Linux/Mac)
- Sistema operacional: Windows, Linux ou Mac

### Compilação

Navegue até a pasta do projeto:
```bash
cd part03_analise_semantica/
```

Compile o compilador:
```bash
g++ -o xpp_compiler principal.cpp scanner.cpp parser.cpp stentry.cpp symboltable.cpp
```

### Execução

**Windows (PowerShell):**
```powershell
.\xpp_compiler.exe arquivo.xpp
```

**Linux/Mac:**
```bash
./xpp_compiler arquivo.xpp
```

### Exemplos de Uso

**Compilação bem-sucedida:**
```bash
.\xpp_compiler.exe tests\test_success.xpp
```
```
[SEMANTICO] Classe 'Pessoa' declarada na linha 4
[SEMANTICO] Variavel 'idade' do tipo 'int' declarada na linha 5
[SEMANTICO] Variavel 'nome' do tipo 'string' declarada na linha 6
...
Compilacao finalizada com sucesso.
```

**Arquivo com erro:**
```bash
.\xpp_compiler.exe tests\test_erro_semantico1.xpp
```
```
[SEMANTICO] Classe 'Teste' declarada na linha 3
[SEMANTICO] Variavel 'valor' do tipo 'int' declarada na linha 4
erro: linha 7: 'resultado' nao foi declarado neste escopo
```

## 🧪 Testes

### Suite de Testes Automatizada

Execute todos os testes de uma vez:
```powershell
.\run_tests.ps1
```

**Saída esperada:**
```
========================================
  TESTES DO COMPILADOR X++
========================================

----------------------------------------
TESTES DE SUCESSO
----------------------------------------
[1/12] Programa valido simples
  Arquivo: test_success.xpp
  PASSOU

[2/12] Programa valido complexo
  Arquivo: test_valid.xpp
  PASSOU

...

========================================
RESUMO DOS TESTES
========================================
Total de testes:  12
Testes passados:  12
Testes falhados:  0

========================================
  TODOS OS TESTES PASSARAM!
========================================
```

### Casos de Teste Disponíveis

| Arquivo | Descrição |
|---------|-----------|
| `test_success.xpp` | Programa válido simples |
| `test_valid.xpp` | Programa válido com múltiplas classes |
| `test_completo.xpp` | Teste completo da gramática |
| `test_simple.xpp` | Teste de escopos e variáveis locais |
| `test_erro_lexico.xpp` | Erro: caractere inválido `@` |
| `test_erro_sintaxe.xpp` | Erro: chave não fechada |
| `test_erro_semantico1.xpp` | Erro: variável não declarada |
| `test_erro_semantico2.xpp` | Erro: redeclaração de variável |
| `test_erro_semantico3.xpp` | Erro: classe não declarada |
| `test_erro_semantico4.xpp` | Erro: redeclaração de classe |
| `test_erro_semantico5.xpp` | Erro: herança inválida |

## 📝 Mensagens de Erro

O compilador emite mensagens de erro no estilo **g++/gcc**, claras e informativas:

### Erro Léxico
```
erro lexico: linha 7: caractere invalido '@'
```

### Erro Sintático
```
erro: linha 11: esperava 'SEMICOLON' mas encontrou 'LEFT_BRACKET'
```

### Erros Semânticos

**Variável não declarada:**
```
erro: linha 7: 'resultado' nao foi declarado neste escopo
```

**Redeclaração:**
```
erro: linha 5: redeclaracao de 'valor'
nota: declaracao anterior na linha 4
```

**Classe não declarada:**
```
erro: linha 10: 'MinhaClasse' nao foi declarado como uma classe
```

**Herança inválida:**
```
erro: linha 8: 'ClassePai' nao foi declarado
```

### Compilação Bem-Sucedida
```
Compilacao finalizada com sucesso.
```

---

## 🎯 Características Adicionais

- ✅ Mensagens de erro estilo g++
- ✅ Suporte completo a escopos aninhados
- ✅ Análise semântica completa (além do requisitado)
- ✅ Suite de testes automatizada
- ✅ Tratamento robusto de erros
- ✅ Código modular e bem documentado

---

## 📚 Referências

- Aho, A. V.; Lam, M. S.; Sethi, R.; Ullman, J. D. **Compilers: Principles, Techniques, and Tools** (2nd Edition)
- Appel, A. W. **Modern Compiler Implementation in C/Java/ML**
- Documentação do GCC/g++

---

**Desenvolvido como projeto acadêmico de Compiladores**
