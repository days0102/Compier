/*
 * @Author: Outsider
 * @Date: 2022-10-23 20:53:44
 * @LastEditors: Outsider
 * @LastEditTime: 2022-11-02 16:41:18
 * @Description: In User Settings Edit
 * @FilePath: /compiler/src/tree.cc
 */
#include "tree.hh"
#include "codegen.hh"

tnode::tnode(int line) : line(line) {}
tnode::~tnode() {}
int tnode::getline() { return line; }

Expression::Expression(int line) : tnode(line) {}
void Expression::print(int level) {}
Expression::~Expression() {}

Prohead::Prohead(int line, Token *using_name) : Expression(line), name(using_name) {}
Prohead::Prohead(Token *using_name) : name(using_name) {}
llvm::Value *Prohead::CodeGen()
{
    return nullptr;
}
void Prohead::print(int level)
{
    std::string str = std::string(level, ' ');
    cout << str;
    cout << "USING " << this->name->str << endl;
}
Prohead::~Prohead() {}

Expressions::Expressions(int line, Expression *exp) : Expression(line)
{
    explist.push_back(exp);
}
Expressions *Expressions::add(Expression *exp)
{
    explist.push_back(exp);
    return this;
}
llvm::Value *Expressions::CodeGen()
{
    return nullptr;
}
void Expressions::print(int level)
{
    std::string str = std::string(level, ' ');
    cout << str;
    auto it = this->explist.begin();
    for (; it != this->explist.end(); ++it)
    {
        (*it)->print(level + 2);
    }
}
Expressions::~Expressions() {}

Classbody::Classbody(int line, Expressions *explist) : Expression(line), explist(explist) {}
llvm::Value *Classbody::CodeGen()
{
    return nullptr;
}
void Classbody::print(int level)
{
    std::string str = std::string(level, ' ');
    cout << str;
    this->explist->print(level);
}
Classbody::~Classbody() {}

Class ::Class(int line, Token *token) : Expression(line), name(token) {}
Class ::Class(int line, Token *token, Classbody *body)
    : Expression(line), name(token), classbody(body) {}
llvm::Value *Class::CodeGen()
{
    return nullptr;
}
void Class::print(int level)
{
    std::string str = std::string(level, ' ');
    cout << str;
    cout << "CLASS " << this->name->str << endl;
    if (this->classbody != nullptr)
        this->classbody->print(level);
}
Class *Class::add(Expression *exp)
{
    // body.push_back(exp);
    return this;
}
Class ::~Class() {}

Proclass::Proclass(int line) : Expression(line) {}
Proclass::Proclass(int line, Class *a) : Expression(line)
{
    classes.push_back(a);
}
Proclass *Proclass::add(Class *a)
{
    classes.push_back(a);
    return this;
}
llvm::Value *Proclass::CodeGen()
{
    return nullptr;
}
void Proclass::print(int level)
{
    std::string str = std::string(level, ' ');
    cout << str;
    auto it = this->classes.begin();
    for (; it != this->classes.end(); ++it)
    {
        (*it)->print(level);
    }
}
Proclass::~Proclass() {}

Program::Program(int line, Prohead *head, Proclass *proclass)
    : tnode(line), prohead(head), proclass(proclass) {}
void Program::print(int level)
{
    std::string str = std::string(level, ' ');
    cout << str;
    this->prohead->print(level);
    this->proclass->print(level);
}
Program::~Program() {}

Evaluate::Evaluate(int line, Token *left, Expression *right)
    : Expression(line), left(left), right(right) {}
llvm::Value *Evaluate::CodeGen()
{
    return nullptr;
}
void Evaluate::print(int level)
{
    std::string str = std::string(level, ' ');
    cout << str;
    cout << "EVALUATE " << this->left->str << endl;
    this->right->print(level);
}
Evaluate::~Evaluate() {}

Number::Number(int line, Token *token) : token(token), Expression(line)
{
    this->val = stold(this->token->str);
}

llvm::Value *Number::CodeGen()
{
    return llvm::ConstantFP::get(TheContext, llvm::APFloat(this->val));
}
void Number::print(int level)
{
    std::string str = std::string(level, ' ');
    cout << str;
    cout << "NUMBER " << this->token->str << endl;
}
Number::~Number() {}

Object::Object(int line, Token *token) : token(token), Expression(line) {}
llvm::Value *Object::CodeGen()
{
    return nullptr;
}
void Object::print(int level)
{
    std::string str = std::string(level, ' ');
    cout << str;
    cout << "OBJECT " << this->token->str << endl;
}
Object::~Object() {}

Use::Use(int line, Expression *exp) : Expression(line), exp(exp) {}
llvm::Value *Use::CodeGen()
{
    return nullptr;
}
void Use::print(int level)
{
    std::string str = std::string(level, ' ');
    cout << str;
    cout << "USE " << endl;
    this->exp->print(level + 2);
}
Use::~Use() {}

Operation::Operation(int line, Expression *left, char op, Expression *right)
    : Expression(line), left(left), op(op), right(right) {}
llvm::Value *Operation::CodeGen()
{
    llvm::Value *lv = this->left->CodeGen();
    llvm::Value *rv = this->right->CodeGen();

    switch (this->op)
    {
    case '+':
        return Builder.CreateFAdd(lv, rv, "addtmp");
    case '-':
        return Builder.CreateFSub(lv, rv, "subtmp");
    case '*':
        return Builder.CreateFMul(lv, rv, "multmp");
    case '/':
        return Builder.CreateFDiv(lv, rv, "divtmp");
    default:
        return LogErrorV("invalid binary operator");
    }
}
void Operation::print(int level)
{
    std::string str = std::string(level, ' ');
    cout << str;
    cout << "OPERATION " << this->op << endl;
    this->left->print(level + 2);
    this->right->print(level + 2);
}
Operation::~Operation() {}

Parameter::Parameter() {}
llvm::Value *Parameter::CodeGen()
{
    return nullptr;
}
void Parameter::print(int level)
{
    std::string str = std::string(level, ' ');
    cout << str;
}
Parameter::~Parameter() {}

Parameters::Parameters() {}
llvm::Value *Parameters::CodeGen()
{
    return nullptr;
}
void Parameters::print(int level)
{
    std::string str = std::string(level, ' ');
    cout << str;
}
Parameters::~Parameters() {}

Function::Function() {}
llvm::Value *Function::CodeGen()
{
    return nullptr;
}
void Function::print(int level)
{
    std::string str = std::string(level, ' ');
    cout << str;
}
Function::~Function() {}