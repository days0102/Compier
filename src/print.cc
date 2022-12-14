/*
 * @Author: Outsider
 * @Date: 2022-11-19 14:35:45
 * @LastEditors: Outsider
 * @LastEditTime: 2022-12-15 13:39:16
 * @Description: 打印 AST
 * @FilePath: /compiler/src/print.cc
 */
#include "print.hh"
#include "tree.hh"

/*
    Node 有两个构造函数
    1. string 节点名
    2. string 节点名
       Nodes 子节点序列
*/

Node Program::getNode()
{
    Node n("program");
    Array_builder<Nodes, Node> b;
    if (this->prohead != nullptr)
        b.add(this->prohead->getNode());
    if (this->proclass != nullptr)
        b.add(this->proclass->getNode());
    n = {
        "program",
        b.build()};

    return n;
}

Node Prohead::getNode()
{
    Node n("using");
    n = {
        "using",
        Nodes::build_array(this->token->name)};

    return n;
}

Node Proclass::getNode()
{
    Array_builder<Nodes, Node> b;

    auto it = this->classes.begin();
    for (; it != this->classes.end(); ++it)
    {
        b.add((*it)->getNode());
    }

    // for (int i = 0; i < 3; i++)
    // {
    //     Array_builder<Nodes, Node> builder;
    //     builder.add("l1", "l2", "l3");
    //     b.add("name", Node("node", builder.build()));
    // }

    Node n("class");
    n = {
        "class",
        b.build()};

    return n;
}

Node Class::getNode()
{
    Node n(this->token->name);
    if (this->classbody == nullptr)
    {
        return n;
    }
    n = {
        this->token->name,
        this->classbody->getNodes()};

    return n;
}
Nodes Classbody::getNodes()
{
    Array_builder<Nodes, Node> b;
    b.add(this->explist->getNode());
    // b.add(Node("function"));

    return b.build();
}
Node Classbody::getNode() // unuse
{
    Array_builder<Nodes, Node> b;
    b.add(this->explist->getNode());
    b.add(Node("function"));

    Node n("body");
    n = {
        "",
        b.build()};
    return n;
}

Node Expressions::getNode()
{
    Array_builder<Nodes, Node> b;
    auto it = this->explist.begin();
    for (; it != this->explist.end(); ++it)
    {
        b.add((*it)->getNode());
    }
    Node n("expression");
    n = {
        "expression",
        b.build()};

    return n;
}

Node Evaluate::getNode()
{
    Array_builder<Nodes, Node> builder;
    builder.add(Node(this->left->name));
    builder.add(this->right->getNode());
    Node n("evaluate");
    n = {
        "=",
        builder.build()};
    return n;
}

Node Number::getNode()
{
    Node n(this->token->name);
    return n;
}

Node Object::getNode()
{
    Node n(this->token->name);
    return n;
}

Node Use::getNode()
{
    Array_builder<Nodes, Node> builder;
    builder.add(this->exp->getNode());
    Node n("use");
    n = {
        "use",
        builder.build()};
    return n;
}

Node Operation::getNode()
{
    Array_builder<Nodes, Node> b;
    b.add(this->left->getNode());
    b.add(this->right->getNode());

    Node n(std::string(1, this->op), b.build());
    // n = {
    //     std::string(1, this->op),
    //     b.build()};
    return n;
}

Node Parameter::getNode()
{
    Node n("parameter");
    n = {
        "parameter",
        Nodes::build_array("...")};
    return n;
}

Node Parameters::getNode()
{
    Node n("parameters");
    n = {
        "parameters",
        Nodes::build_array("...")};
    return n;
}

Node Function::getNode()
{
    Node n("funtion");
    n = {
        "funtion",
        Nodes::build_array("...")};
    return n;
}

Node Call::getNode()
{
    Node n("call");
    n = {
        "call",
        Nodes::build_array(this->token->name)};
    return n;
}