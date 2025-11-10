# Analisador Sintático para Linguagem X++

## Compilação e Execução

```bash
g++ -o xpp_compiler scanner.cpp parser.cpp principal_sin.cpp
.\xpp_compiler.exe arquivo.xpp
```

## Gramática Implementada

Este analisador sintático implementa a gramática completa da linguagem X++, uma linguagem orientada a objetos.

### Características Principais:

1. **Classes e Herança**: Suporte completo para declaração de classes e herança (`extends`)
2. **Construtores**: Declaração de construtores com chamadas `super()`
3. **Métodos**: Métodos podem retornar tipos primitivos (`int`, `string`) ou objetos
4. **Atributos**: Declaração de atributos de classe (variáveis e arrays)
5. **Operadores**: Relacionais (`<`, `>`, `<=`, `>=`, `==`, `!=`) e aritméticos (`+`, `-`, `*`, `/`, `%`)
6. **Comandos**: `if-else`, `for`, `break`, `return`, `print`, `read`
7. **Expressões**: Suporte para alocação de objetos (`new`) e arrays
8. **LValue**: Acesso a membros (`.`), arrays (`[]`) e chamadas de métodos

### Regras Importantes:

- Todas as declarações de variáveis devem vir ANTES dos construtores
- Todos os construtores devem vir ANTES dos métodos
- Não há suporte para `while` (apenas `for`)
- Não há operadores lógicos `&&` e `||` (apenas relacionais)
- Comandos `if` e `for` requerem blocos `{}` obrigatórios

## Exemplo de Código Válido

```cpp
class MinhaClasse {
    int x;
    string nome;
    
    constructor(int a, string n) {
        x = a;
        nome = n;
    }
    
    int soma(int a, int b) {
        return a + b;
    }
}
```
