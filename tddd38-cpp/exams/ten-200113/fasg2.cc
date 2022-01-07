#include <vector>
#include <string>
#include <sstream>
#include <iostream>

class Option;

class Parser
{
public:

    ~Parser();
    Parser() = default;
    Parser(Parser const&) = delete;
    Parser& operator=(Parser const&) = delete;
    
    void add(Option* arg);
    bool parse(std::vector<std::string> const& args);
    void print(std::ostream& os, std::string const& program) const;
    
private:
    std::vector<Option*> options {};
    
};

class Option
{
public:
    Option(std::string const& name)
        : name{name} { }

    Option(Option const&) = delete;
    Option& operator=(Option const&) = delete;
    
    virtual ~Option() = default;
    virtual bool parse(std::string const& arg) = 0;
    virtual void print(std::ostream& os) const
    {
        os << name;
    }
protected:
    std::string const name;
    
};

class Flag : public Option
{
public:
    Flag(std::string const& name, bool& target)
        : Option{name}, target{target}
    {
        target = false;
    }

    bool parse(std::string const& arg) override
    {
        if (arg == name)
        {
            target = true;
            return true;
        }
        return false;
    }

    void print(std::ostream& os) const override
    {
        os << '[' << name << ']';
    }
    
private:
    bool& target;
};

template <typename T>
class Argument : public Option
{
public:

    Argument(std::string const& name, T& target)
        : Option{name}, target{target}
    { }
    
    bool parse(std::string const& arg) override
    {
        std::istringstream iss{arg};
        if (iss >> target)
        {
            return true;
        }
        return false;
    }
    
private:
    T& target;
};

Parser::~Parser()
{
    for (Option* arg : options)
    {
        delete arg;
    }
}

void Parser::add(Option* arg)
{
    options.push_back(arg);
}

bool Parser::parse(std::vector<std::string> const& args)
{
    if (args.empty())
    {
        std::cerr << "No arguments given." << std::endl;
        return false;
    }

    int i{0};
    for (Option* option : options)
    {
        bool is_flag {dynamic_cast<Flag*>(option)};
        if (i < args.size())
        {
            if (option->parse(args[i]))
            {
                ++i;
            }
            else if (!is_flag)
            {
                std::cerr << "Unknown argument: " << args[i] << std::endl;
                return false;
            }
        }
        else if (!is_flag)
        {
            std::cerr << "Too few arguments" << std::endl;
            return false;
        }
        
    }

    if (i < args.size())
    {
        std::cerr << "Too many arguments." << std::endl;
        return false;
    }
    
    return true;
}

void Parser::print(std::ostream& os, std::string const& program) const
{
    os << "Usage: " << program;
    for (Option* option : options)
    {
        os << " ";
        option->print(os);
    }
    os << std::endl;
}

int main(int argc, char** argv)
{
    std::vector<std::string> args {argv + 1, argv + argc};
    Parser p{};

    bool a{};
    p.add(new Flag{"-a", a});

    bool b{};
    p.add(new Flag{"--b-flag", b});

    int num{};
    p.add(new Argument<int>{"int", num});

    std::string str{};
    p.add(new Argument<std::string>{"string", str});

    if (!p.parse(args))
    {
        p.print(std::cerr, argv[0]);
    }
    else
    {
        std::cout << "a == " << a << std::endl
                  << "b == " << b << std::endl
                  << "num == " << num << std::endl
                  << "str == " << str << std::endl;
    }
}