# COMP-CompiladorLinguagemXPP

## 👨‍💻 Projeto desenvolvido por:
[Rafael Torres Nantes](https://github.com/rafael-torres-nantes)

## Índice

* [📚 Contextualização do projeto](#-contextualização-do-projeto)
* [🛠️ Tecnologias/Ferramentas utilizadas](#%EF%B8%8F-tecnologiasferramentas-utilizadas)
* [📁 Estrutura do projeto](#estrutura-do-projeto)
* [📌 Como executar o projeto](#como-executar-o-projeto)

## 📚 Contextualização do projeto

Implementação completa de um compilador desenvolvido em C++ para a linguagem fictícia X++. O projeto integra as fases de compilação essenciais e foi desenvolvido como parte prática da disciplina de Compiladores (UFMS), abordando desde varreduras léxicas até semântica com tabelas de símbolos.

## 🛠️ Tecnologias/Ferramentas utilizadas

* C++
* Powershell script (`.ps1`) para automação de testes
* Arquivos de código teste em X++ e C-- (`.cmm`, `.xpp`)

## 📁 Estrutura do projeto

```
part01_analise_lexica/
  ├── scanner.cpp
  ├── principal_lex.cpp
  └── test_completo.cmm
part02_analise_sintatica/
  ├── scanner.cpp
  ├── parser.cpp
  ├── principal_sin.cpp
  └── sint_test.cmm
part03_analise_semantica/
  ├── scanner.cpp
  ├── parser.cpp
  ├── stentry.cpp
  ├── symboltable.cpp
  ├── principal.cpp
  ├── run_tests.ps1
  └── tests/
Esqueleto em C++/
Trabalho Antigo/
Trabalho Atual/
```

## 📌 Como executar o projeto

Você precisará do compilador `g++` instalado no sistema. Entre na pasta correspondente à etapa que deseja compilar e digite os comandos, por exemplo: `g++ -o xpp_compiler *.cpp` ou especificando os arquivos centrais como `g++ -o xpp_compiler scanner.cpp parser.cpp principal_sin.cpp`. Em seguida, chame o executável gerado informando o arquivo `.cmm` ou `.xpp` a ser analisado.
