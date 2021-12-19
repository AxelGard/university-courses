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
 * EBNF Grammar (https://en.wikipedia.org/wiki/Extended_Backus%E2%80%93Naur_form):
 *
 * <expression> ::= <term> {("+" | "-") <term>}*
 * <term>       ::= <factor> {("*" | "/") <factor>}*
 * <factor>     ::= "(" <expression> ")" |
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
        Node* result{parse_term(is)};
        while (peek_token(is) == '+' || peek_token(is) == '-')
        {
            char c {parse_token(is)};
            Node* rhs{parse_term(is)};
            if (c == '+')
                result = new Addition{result, rhs};
            else if (c == '-')
                result = new Subtraction{result, rhs};
        }
        return result;
    }

    Node* parse_term(istream& is)
    {
        Node* result{parse_factor(is)};
        while (peek_token(is) == '*' || peek_token(is) == '/')
        {
            char c {parse_token(is)};
            Node* rhs{parse_factor(is)};
            if (c == '*')
                result = new Multiplication{result, rhs};
            else if (c == '/')
                result = new Division{result, rhs};
        }
        return result;
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
