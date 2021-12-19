#ifndef NODE_H_
#define NODE_H_

#include <ostream>
#include <string>

/*
  Implement the classes:
  - Node
  - Number
  - Operator
  - Addition
  - Subtraction
  - Multiplication
  - Division

  Node should have the following (pure virtual) members:
  - double evaluate()
    + Calculate the expression stored in 'this'
  - void print(std::ostream& os)
    + print the expression represented in 'this'
  - void print_tree(std::ostream& os, int depth)
    + print a textual representation of the expression tree
  - Node* clone()
    + return a a new instance of the expression stored in 'this'
 */
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
private:
  double number;
public:
  Number(double number);
  double evaluate() const override;
  void print(std::ostream& os) const override;
  void print_tree(std::ostream& os, int depth=0) const override;
  Node* clone() const override;
  virtual Node* expand();
  
};





#endif//NODE_H_
