/*
 * @Author: Outsider
 * @Date: 2022-10-05 20:08:20
 * @LastEditors: Outsider
 * @LastEditTime: 2022-11-02 16:15:31
 * @Description: In User Settings Edit
 * @FilePath: /compiler/src/main.cc
 */
#include <iostream>
#include "tree.hh"
#include "tokens.hh"
// #include "bison.tab.hh"
#include "llvm/Support/raw_ostream.h"

using std::cout;
using std::endl;
using std::ostream;

extern FILE *yyin;
extern int yylex();
extern int yyparse();
extern int yylineno;

extern Program *ast_root;

extern Tokentable tokentable;
extern int displaytoken(int);

int line = 1, word = 0;
/*
void initkeyword()
{
    tokentable.add("using");
    tokentable.add("class");
    tokentable.add("str");
    tokentable.add("int");
    tokentable.add("use");
    tokentable.add("ftn");
}
*/

int main(int argc, char **argv)
{
    if (argc <= 1)
    {
        cout << "usage read filename" << endl;
        return 0;
    }
    if ((yyin = fopen(argv[1], "r")) == nullptr)
    {
        cout << "fopen" << argv[1] << "error" << endl;
        return 0;
    }
    while (displaytoken(yylex()) != 0)
        ;
    // initkeyword();

    cout << "-------------------" << endl;
    yylineno = 1;
    fseek(yyin, 0, SEEK_SET);
    yyparse();
    cout << endl;

    ast_root->print(0);
    llvm::Value *v = ast_root->proclass->classes.front()->classbody->explist->explist.front()->CodeGen();
    v->print(llvm::outs());

    cout << endl;
    cout << "------------" << endl;
    cout << "line: " << line << endl
         << "word: " << word << endl;
}