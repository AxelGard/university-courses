// Suggestions and hints:

// - Read the 'main' function to understand how the code is used.

// - Don't worry too much about rewriting everything. Find a few
//   places where you can demonstrate your knowledge and only change
//   those places.

// - The wishlists are just hints/suggestions. You don't have to
//   follow them if you don't want to.

// - It is more important to understand how the code is used rather
//   than what it does.

// - To help you understand the code it might be helpful to make small
//   changes in the code and then see what effect it has on the
//   output.

// - It is ok if the behaviour of the code changes, as long as it
//   stays approximately the same. Use your own judgement.

// - There are a lot of comments in this file, read them.

#include <string>
#include <iostream>
#include <vector>
#include <sstream>
#include <utility>

// This enum represents the different types of messages that can be
// handled in the messaging system. These are:

// - None: An empty message, doesn't contain any data.

// - Text: A message which contains a string (without the '#'
//         character).

// - Integer: A message which contains a single integer value.

// - Floating: A message which contains a single floating-point
//   (double) value.

// - Composite: A message which consists of several submessages.

// This struct represents a message that can be sent or
// recieved. There are four types of messages (see above) determined
// by 'type'.

// 'text' is used for Text messages. It contains the actual string
// (text) content.

// 'integer' is used for Integer messages. It contains the integer
// value.

// 'floating' is used for Floating messages. It contains the
// floating-point value.

// 'submessages' is a vector of messages that are used in Composite
// messages.

// Wishlist:

// - It would be a good idea to represent the 'type' in some other way.

// - It should be easy to add other types of messages.

// - Would be nice if 'integer' could be other types as well, for
//   example float, double, bool and other types that has operator<<.

// - Can we store only the relevant data member for the corresponding
//   type somehow?
/*
struct Message
{
    Message_Type type { None };
    
    std::string text;
    int integer;
    double floating;
    std::vector<Message*> submessages;
};
*/
class Message {
    public: 
        virtual ~Message() = default;

        virtual int encode_type() const 
        {
            return -1;
        }

        virtual void serialize(std::ostream& os) const 
        {
            os << this->encode_type() << " ";
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
    private:
        std::string text;
    public: 
        Text(std::string const& text)
            : text{text}
        { }

        int encode_type() const override 
        {
            return 1;
        }

        void serialize(std::ostream& os) const override 
        {
            Message::serialize(os);
            os << this->text << "# ";
        }
};

template<typename T>
class Data : public Message
{
    private:
        T data;

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
            Message::serialize(os)
            os << this->data << " ";
        }
};

class Composite : public Message
{
    private:
        std::vector<Message*> messages;

    public:
        Composite(std::vector<Message*> const& messages)
            : messages { messages }
            { }
        ~Composite(){
            for (auto&& message : messages){
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
};


class Terminal
{
    private:
        std::string name;
        std::vector<Message*> messages {};

    public:
        Terminal(std::string const& name, std::vector<Message*> const& messages = {})
            : name { name }, messages {messages}
            { }

        ~Terminal(){
            for (auto&& message : this->messages){
                delete message;
            }
        }

        void add(Message* message)
        {
            this->messages.push_back(message);
        }

        void serialize(std::ostream& os) const 
        {
            os << name << " " << messages.size() << " ";
            for (auto&& message : this->messages)
            {
                message->serialize(os);
            }
        }

        template <typename Comparator>
        std::vector<Message*> sort_messages(Comparator comarator)
        {
            std::vector<Message*> result {messages};
            std::sort(std::begin(result), std::end(result), comparator);
            return result;
        }

        template <typename Predicate>
        std::vector<Message*> filter_messages(Predicate predicate){
            std::copy_if(std::begin(this->messages), std::end(this->messages), std::back_inserter(result)
                    [&predicate](auto&& a){return !predicate(a); });
        }

};


bool comp(Message const* a, Message const* b){
    return a->encode_type() < b->encode_type();
}

bool pred(Message const* msg){
    int type{msg->encode_type()};
    return type==2 || type == 3;
}


int main(){
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