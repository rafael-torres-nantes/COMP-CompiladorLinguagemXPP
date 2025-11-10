#include "superheader.h"

// Função principal do programa
int main(int argc, char* argv[]) 
{
    // Verifica se o programa foi executado com o número correto de argumentos
    // Esta main espera receber o nome do arquivo a ser analisado na linha de comando.
    if (argc != 2) // Se não foram passados 2 argumentos
    {
        cout << "Uso: ./compiler nome_arquivo.mj\n"; // Exibe mensagem de uso correto
        return 1; // Retorna código de erro
    }

    // Cria o objeto scanner com o arquivo passado como argumento
    Scanner* scanner = new Scanner(argv[1]);
    
    Token* t;
    // Repete a leitura de tokens até o final do arquivo
    do
    {
        t = scanner->nextToken();
        // Caso o token tenha um atributo definido, imprime o atributo
        if (t->attribute != UNDEFINED) {
            cout << Token::getTokenTypeName(t->attribute) << " ";
        } else {
            // Caso contrário, imprime o nome do token
            cout << Token::getTokenTypeName(t->type) << " ";
        }
    } while (t->type != END_OF_FILE); // Continua até encontrar o token de fim de arquivo

    delete scanner; // Libera a memória alocada para o scanner
}