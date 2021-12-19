#ifndef EXPRESSION_H_
#define EXPRESSION_H_

#include "Node.h"

#include <ostream>

class Expression
{
public:

    Expression(Node* root);
    Expression(Expression const& other);
    Expression(Expression&& other);
    ~Expression();

    Expression& operator=(Expression const& rhs);
    Expression& operator=(Expression&& rhs);

    double evaluate() const;
    void print(std::ostream& os) const;
    void print_tree(std::ostream& os) const;

    void expand();
    
private:

    Node* root;
};

#endif//EXPRESSION_H_
