#include "parser.h"

int main(int argc, char* argv[]) 
{
    string input;
    
    getline(cin, input);

    //Scanner* scanner = new Scanner(input);
    

    //Token* t;

    Parser* parser = new Parser(input);
    
    parser->run();

    /*do
    {
        t = scanner->nextToken();
        
        cout << t->name << " ";
    }while (t->name != END_OF_FILE);
    */

    delete parser;

    return 0;
}   