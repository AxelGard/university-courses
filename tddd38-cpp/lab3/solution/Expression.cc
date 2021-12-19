#include "Expression.h"

#include <utility>

using namespace std;

Expression::Expression(Node* root)
    : root{root}
{
}

Expression::Expression(Expression const& other)
    : root{other.root->clone()}
{
}

Expression::Expression(Expression&& other)
    : root{other.root}
{
    other.root = nullptr;
}

Expression::~Expression()
{
    delete root;
}

Expression& Expression::operator=(Expression const& rhs)
{
    Expression tmp{rhs};
    return *this = move(tmp);
}

Expression& Expression::operator=(Expression&& rhs)
{
    swap(root, rhs.root);
    return *this;
}

double Expression::evaluate() const
{
    return root->evaluate();
}

void Expression::print(ostream& os) const
{
    root->print(os);
}

void Expression::print_tree(ostream& os) const
{
    root->print_tree(os);
}

void Expression::expand()
{
    root = root->expand();
}
