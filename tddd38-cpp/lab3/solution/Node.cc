#include "Node.h"

#include <iomanip>

using namespace std;

/**** Number ****/

Number::Number(double number)
    : number{number}
{
}

double Number::evaluate() const
{
    return number;
}

void Number::print(ostream& os) const
{
    os << evaluate();
}

void Number::print_tree(ostream& os, int depth) const
{
    os << setw(depth) << "";
    print(os);
    os << endl;
}

Node* Number::clone() const
{
    return new Number{number};
}

Node* Number::expand()
{
    return this;
}

/**** Operator ****/

Operator::Operator(Node* lhs, Node* rhs)
    : lhs{lhs}, rhs{rhs}
{
}

Operator::~Operator()
{
    delete lhs;
    delete rhs;
}

void Operator::print_tree(ostream& os, int depth) const
{
    lhs->print_tree(os, depth + 3);
    os << setw(depth) << "" << " / \n"
       << setw(depth) << "" << glyph() << "\n"
       << setw(depth) << "" << " \\ \n";
    rhs->print_tree(os, depth + 3);
}

Node* Operator::left_expand(Operator* root)
{
    auto child {dynamic_cast<Operator*>(root->lhs)};
    root->lhs = child->lhs;
    child->lhs = root;
    child->rhs = new Multiplication{child->rhs, root->rhs->clone()};
    return child->expand();
}

Node* Operator::right_expand(Operator* root)
{
    auto child {dynamic_cast<Operator*>(root->rhs)};
    root->rhs = child->lhs;
    child->lhs = root;
    child->rhs = new Multiplication{root->lhs->clone(), child->rhs};
    return child->expand();
}

Node* Operator::expand()
{
    lhs = lhs->expand();
    rhs = rhs->expand();

    if (typeid(*this) == typeid(Multiplication))
    {
        if (dynamic_cast<Addition*>(lhs))
        {
            return left_expand(this);
        }
        else if (dynamic_cast<Addition*>(rhs))
        {
            return right_expand(this);
        }
    }
    return this;
}

/**** Addition ****/

double Addition::evaluate() const
{
    return lhs->evaluate() + rhs->evaluate();
}

void Addition::print(ostream& os) const
{
    lhs->print(os);
    os << glyph();
    rhs->print(os);
}

Node* Addition::clone() const
{
    return new Addition{lhs->clone(), rhs->clone()};
}

string Addition::glyph() const
{
    return "+";
}

/**** Subtraction ****/

double Subtraction::evaluate() const
{
    return lhs->evaluate() - rhs->evaluate();
}

Node* Subtraction::clone() const
{
    return new Subtraction{lhs->clone(), rhs->clone()};
}

string Subtraction::glyph() const
{
    return "-";
}

/**** Multiplication ****/

double Multiplication::evaluate() const
{
    return lhs->evaluate() * rhs->evaluate();
}

void Multiplication::print(ostream& os) const
{
    print(lhs, os);
    os << glyph();
    print(rhs, os);
}

Node* Multiplication::clone() const
{
    return new Multiplication{lhs->clone(), rhs->clone()};
}

string Multiplication::glyph() const
{
    return "*";
}

void Multiplication::print(Node* expression, ostream& os) const
{
    if (dynamic_cast<Addition*>(expression))
    {
        os << '(';
        expression->print(os);
        os << ')';
    }
    else
    {
        expression->print(os);
    }
}

/**** Division ****/

double Division::evaluate() const
{
    return lhs->evaluate() / rhs->evaluate();
}

Node* Division::clone() const
{
    return new Division{lhs->clone(), rhs->clone()};
}

string Division::glyph() const
{
    return "/";
}
