#ifndef NODE_H_
#define NODE_H_

#include <ostream>
#include <string>

class Node
{
public:
    
    virtual ~Node() = default;

    virtual double evaluate() const = 0;
    virtual void print(std::ostream& os) const = 0;
    virtual void print_tree(std::ostream& os, int depth = 0) const = 0;
    virtual Node* clone() const = 0;
    virtual Node* expand() = 0;
};

class Number : public Node
{
public:

    Number(double number);

    double evaluate() const override;
    void print(std::ostream& os) const override;
    void print_tree(std::ostream& os, int depth = 0) const override;
    Node* clone() const override;
    virtual Node* expand();

protected:

    double number;
};

class Operator : public Node
{
public:

    Operator(Node* lhs, Node* rhs);
    Operator(Operator const&) = delete;
    Operator(Operator&&) = delete;
    Operator& operator=(Operator const&) = delete;
    Operator& operator=(Operator&&) = delete;
    
    ~Operator();
    
    void print_tree(std::ostream& os, int depth = 0) const override;
    virtual Node* expand();
protected:

    virtual std::string glyph() const = 0;

    static Node* left_expand(Operator* root);
    static Node* right_expand(Operator* root);
    
    Node* lhs;
    Node* rhs;

};

class Addition : public Operator
{
public:

    using Operator::Operator;
    
    double evaluate() const override;
    void print(std::ostream& os) const override;
    Node* clone() const override;

protected:

    std::string glyph() const override;
};

class Subtraction : public Addition
{
public:

    using Addition::Addition;

    double evaluate() const override;
    Node* clone() const override;

protected:

    std::string glyph() const override;
};

class Multiplication : public Operator
{
public:

    using Operator::Operator;

    double evaluate() const override;
    void print(std::ostream& os) const override;
    Node* clone() const override;

protected:

    std::string glyph() const override;

private:

    // Helper function which print parenthesis around
    // 'expression' if it is an Addition,
    // otherwise it just prints the expression.
    void print(Node* expression, std::ostream& os) const;
};

class Division : public Multiplication
{
public:

    using Multiplication::Multiplication;

    double evaluate() const override;
    Node* clone() const override;

protected:

    std::string glyph() const override;
};

#endif//NODE_H_
