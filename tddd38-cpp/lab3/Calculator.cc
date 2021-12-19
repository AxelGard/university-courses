#include "Parser.h"

#include <stdexcept>
#include <iomanip>
#include <iostream>
#include <string>
#include <sstream>
#include <vector>
#include <utility>

using namespace std;

/* vector with each flag and their corresponding description */
vector<pair<string, string>> const options{
    {"-h, --help", "Show this message."},
    {"-t, --print-tree", "Print the expression tree."},
    {"-p, --print-expression", "Print the inline expression."},
    {"-e, --expand", "Expand all parenthesised operands\n"
                     "in multiplication statements."}       
};

void show_help(string const& program)
{
    cout << "Usage: " << program << " [OPTIONS]\n"
         << "A simple terminal based floating point calculator.\n"
         << "can handle +,-,* and /.\n"
         << "OPTIONS:\n";
    for (auto const& p : options)
    {
        /* print flags */
        cout << "  " << left << setw(24) << p.first << " ";

        /* print the description, handle line by line
         * to make sure that the indentation is correct. */
        istringstream iss{p.second};
        string line;

        /* simply print the first line */
        getline(iss, line);
        cout << line << endl;

        /* all successive lines should be indented
         * to make up for the flags column */
        while (getline(iss, line))
        {
            cout << "  " << setw(24) << "" << " " << line << endl;
        }
    }
    
}

int main(int argc, char* argv[]) try
{
    /* all possible flags */
    bool print_expression{false};
    bool print_tree{false};
    bool expand{false};

    /* parse arguments */
    for (int i{1}; i < argc; ++i)
    {
        string arg{argv[i]};
        if (arg == "--help" || arg == "-h")
        {
            show_help(argv[0]);
            return 1;
        }
        else if (arg == "--print-expression" || arg == "-p")
        {
            print_expression = true;
        }
        else if (arg == "--print-tree" || arg == "-t")
        {
            print_tree = true;
        }
        else if (arg == "--expand" || arg == "-e")
        {
            expand = true;
        }
        else
        {
            throw invalid_argument{
                "Unknown argument '" + arg + "', try --help."};
        }
    }

    /* get expression from user */
    string input;
    getline(cin, input);

    /* parse the expression */
    Expression result{parse(input)};
    
    if (expand)
    {
        result.expand();
    }

    /* let the arguments decide what should be printed */
    if (print_tree)
    {
        result.print_tree(cout);
    }
    else if (print_expression)
    {
        result.print(cout);
    }
    else
    {
        cout << result.evaluate();
    }
    cout << endl;
}
catch (invalid_argument& e)
{
    cerr << "Error: " << e.what() << endl;
    return 1;
}
catch (parser_error& e)
{
    cerr << "Error during parse: " << e.what() << endl;
    return 1;
}
catch (...)
{
    cerr << "Unkown error occured." << endl;
    return 1;
}
