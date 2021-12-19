#include "Parser.h"

#include <cctype>
#include <istream>
#include <sstream>

using namespace std;

/* Simple recursive descent parser 
 * (https://en.wikipedia.org/wiki/Recursive_descent_parser)
 *
 * Can handle all parenthesized expressions dealing with +, -, * or /
 *
 * Uses the Node class hierarchy to represent the expression tree.
 *
 * BNF Grammar (https://en.wikipedia.org/wiki/Backus%E2%80%93Naur_form):
 *
 * <expression> ::= <term> "+" <expression> |
 *                  <term> "-" <expression> |
 *                  <term>
 * <term>       ::= <factor> "*" <term>     |
 *                  <factor> "/" <term>     |
 *                  <factor>
 * <factor>     ::= "(" <expression> ")"    |
 *                  <number>
 */

/* Use anonymous namespace to prevent the compiler from generating
 * bindings for these functions.
 *
 * An anonymous namespace will guarantee that all functions defined
 * inside of it will only exist inside this translation-unit.
 */
namespace
{
    /**** Forward declaration of the Non-Terminal parsers ****/
    
    Node* parse_expression(istream& is);
    Node* parse_term(istream& is);
    Node* parse_factor(istream& is);
    Node* parse_number(istream& is);

    /**** Token parser ****/
    
    void ignore_whitespace(istream& is)
    {
        while (isspace(is.peek()))
            is.ignore();
    }
    
    char peek_token(istream& is)
    {
        ignore_whitespace(is);
        return is.peek();
    }
    
    char parse_token(istream& is)
    {
        ignore_whitespace(is);
        return is.get();
    }
    
    /**** Non-Terminal parsers ****/

    Node* parse_expression(istream& is)
    {
        Node* lhs{parse_term(is)};
        char c{peek_token(is)};
        if (c == '+' || c == '-')
        {
            parse_token(is);
            Node* rhs{parse_expression(is)};

            if (c == '+')
                return new Addition{lhs, rhs};
            else if (c == '-')
                return new Subtraction{lhs, rhs};
        }
        return lhs;
    }

    Node* parse_term(istream& is)
    {
        Node* lhs{parse_factor(is)};
        char c{peek_token(is)};
        if (c == '*' || c == '/')
        {
            parse_token(is);
            Node* rhs{parse_term(is)};
            if (c == '*')
                return new Multiplication{lhs, rhs};
            else if (c == '/')
                return new Division{lhs, rhs};
            delete rhs;
        }
        return lhs;
    }

    Node* parse_factor(istream& is)
    {
        if (peek_token(is) == '(')
        {
            parse_token(is);
            auto lhs{parse_expression(is)};
            if (parse_token(is) != ')')
            {
                delete lhs;
                throw parser_error{"Unmatched '(' found"};
            }
            return lhs;
        }
        return parse_number(is);
    }
    
    /**** Terminal parsers ****/

    Node* parse_number(istream& is)
    {
        double number;
        if (is >> number)
            return new Number{number};
        throw parser_error{"Expected a number"};
    }
}

/**** Parser entry ****/

Expression parse(string const& expression)
{
    istringstream iss{expression};
    Expression result{parse_expression(iss)};
    char c;
    /* "..."s denotes a std::string literal rather than a char const*
     * literal. Quite handy since we can use + on them. */
    if (iss >> c)
        throw parser_error{"Unexpected token '"s + c + "' found"};
    return result;
}
