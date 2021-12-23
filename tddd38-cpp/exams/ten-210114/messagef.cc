#include <algorithm>
#include <string>
#include <iostream>
#include <iterator>
#include <vector>
#include <sstream>
#include <utility>

class Message
{
public:
    virtual ~Message() = default;
    
    virtual int encode_type() const
    {
        return -1;
    }
    
    virtual void serialize(std::ostream& os) const
    {
        os << encode_type() << " ";
    }
};

class None : public Message
{
public:

    int encode_type() const override
    {
        return 0;
    }
};

class Text : public Message
{
public:

    Text(std::string const& text)
        : text { text }
    { }

    int encode_type() const override
    {
        return 1;
    }

    void serialize(std::ostream& os) const override
    {
        Message::serialize(os);
        os << text << "# ";
    }
    
private:
    std::string text;
};

template <typename T>
class Data : public Message
{
public:

    Data(T const& data)
        : data { data }
    { }

    int encode_type() const override
    {
        return 2;
    }

    void serialize(std::ostream& os) const override
    {
        Message::serialize(os);
        os << data << " ";
    }
    
private:

    T data;
};

class Composite : public Message
{
public:

    Composite(std::vector<Message*> const& messages)
        : messages { messages }
    { }

    ~Composite()
    {
        for (auto&& message : messages)
        {
            delete message;
        }
    }
    
    int encode_type() const override
    {
        return 4;
    }

    void serialize(std::ostream& os) const override
    {
        Message::serialize(os);
        os << messages.size() << " ";
        for (auto&& message : messages)
        {
            message->serialize(os);
        }
    }

private:

    std::vector<Message*> messages;
};

class Terminal
{
public:

    Terminal(std::string const& name, std::vector<Message*> const& messages = {})
        : name { name }, messages { messages }
    { }

    ~Terminal()
    {
        for (auto&& message : messages)
        {
            delete message;
        }
    }

    void add(Message* message)
    {
        messages.push_back(message);
    }

    void serialize(std::ostream& os) const
    {
        os << name << " " << messages.size() << " ";
        for (auto&& message : messages)
        {
            message->serialize(os);
        }
    }

    template <typename Comparator>
    std::vector<Message*> sort_messages(Comparator comparator)
    {
        std::vector<Message*> result { messages };
        std::sort(std::begin(result), std::end(result), comparator);
        return result;
    }

    template <typename Predicate>
    std::vector<Message*> filter_messages(Predicate predicate)
    {
        // alternative 1
        std::vector<Message*> result { };
        std::copy_if(std::begin(messages), std::end(messages), std::back_inserter(result),
                     [&predicate](auto&& a) { return !predicate(a); });
        return result;

        // alternative 2
        std::vector<Message*> result { messages };
        result.erase(std::remove_if(std::begin(result), std::end(result), predicate),
                      std::end(result));
        return result;
    }
    
private:

    std::string name;
    std::vector<Message*> messages { };
    
};

/*** TESTCASES ***/

bool comp(Message const* a, Message const* b)
{
    return a->encode_type() < b->encode_type();
}

bool pred(Message const* msg)
{
    int type { msg->encode_type() };
    return type == 2 || type == 3;
}

int main()
{
    Terminal terminal { "My Terminal" };

    // create some messages
    terminal.add(new None());
    terminal.add(new Text("This string is my message"));
    terminal.add(new Data<int>(12));
    terminal.add(new Data<double>(3.14));
    terminal.add(new None());
    terminal.add(new Text("More text"));

    // create a composite message
    std::vector<Message*> messages {
        new Text("1"),
        new Data<double>(2.3),
        new Data<int>(45)
    };
    terminal.add(new Composite(messages));

    // check if we can serialize all the messages
    terminal.serialize(std::cout);
    std::cout << std::endl;

    std::cout << "========" << std::endl;

    // test the sort_messages function. We sort on the type of message.

    // Would be nice if we could do this:
    // sort_messages(terminal, [](Message* a, Message* b) { return return encode_type(a) > encode_type(b); });
    std::vector<Message*> sorted { terminal.sort_messages(comp) };
    for (Message const* message : sorted)
    {
        message->serialize(std::cout);
        std::cout << std::endl;
    }

    std::cout << "========" << std::endl;

    // test the filter_messages function. We remove all Integer and
    // Floating messages.

    // Would be nice if we could do this:
    // filter_messages(terminal, [](Message* a) { encode_type(a) > 1; });
    std::vector<Message*> filtered { terminal.filter_messages(pred) };
    for (Message const* message : filtered)
    {
        message->serialize(std::cout);
        std::cout << std::endl;
    }
}