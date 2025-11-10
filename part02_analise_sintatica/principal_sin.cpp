#include "superheader.h"

// Função principal do programa
int main(int argc, char* argv[]) 
{
    // Verifica se o programa foi executado com o número correto de argumentos
    // Esta main espera receber o nome do arquivo a ser analisado na linha de comando.
    if (argc != 2) // Se o número de argumentos for diferente de 2
    {
        // Exibe mensagem de uso correto do programa
        cout << "Uso: ./compiler nome_arquivo.mj\n"; 
        return 1; // Retorna código de erro
    }

    // Cria uma instância do parser com o nome do arquivo passado como argumento
    Parser* parser = new Parser(argv[1]);

    // Executa o parser
    parser->run();

    return 0; // Retorna 0 indicando que o programa terminou com sucesso
}