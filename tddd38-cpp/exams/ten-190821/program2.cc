#include <vector>
#include <string>
#include <iostream>


using namespace std;

/* if implemented correctly this program
   should give the following output:

Rhea Deer recommends reading "The Lord of the Rings".
Rhea Deer recommends reading "Winnie the Pooh".
Rhea Deer recommends reading "The Da Vinci Code (best seller)".
Rhea Deer recommends reading "Nancy Drew: The Secret of the Old Clock (best seller)".
Rhea Deer recommends reading "Course Literature".
Professor McProfessorsen does not recommend reading "The Lord of the Rings".
Professor McProfessorsen does not recommend reading "Winnie the Pooh".
Professor McProfessorsen does not recommend reading "The Da Vinci Code (best seller)".
Professor McProfessorsen does not recommend reading "Nancy Drew: The Secret of the Old Clock (best seller)".
Professor McProfessorsen recommends reading "Course Literature".
Christopher Robin does not recommend reading "The Lord of the Rings".
Christopher Robin recommends reading "Winnie the Pooh".
Christopher Robin does not recommend reading "The Da Vinci Code (best seller)".
Christopher Robin recommends reading "Nancy Drew: The Secret of the Old Clock (best seller)".
Christopher Robin does not recommend reading "Course Literature".
Sherlock Holmes recommends reading "The Lord of the Rings".
Sherlock Holmes recommends reading "Winnie the Pooh".
Sherlock Holmes does not recommend reading "The Da Vinci Code (best seller)".
Sherlock Holmes does not recommend reading "Nancy Drew: The Secret of the Old Clock (best seller)".
Sherlock Holmes does not recommend reading "Course Literature".
*/

class Book {
    protected: 
        bool child_book;

    public:
        Book() = default;
        Book(string const&& title, bool child_book = false)
            : title{title}, child_book{child_book}
            { }

        string title;
        
        virtual bool for_children() = 0;

        virtual ~Book() = default;

};


class Non_Fiction : public Book {
    
    public: 
        using Book::Book;
        bool for_children() override{
            return false;
        }

};

class Fiction : public Book {
    public: 
        using Book::Book;
        bool for_children() override{
            return this->child_book;
        } 
    
};

class Crime_Fiction: public Book{
    public:
        Crime_Fiction(string const&& title, bool child_book = false){
            this->title = title + " (best seller)";
            this->child_book = child_book;
        }
          
        
        bool for_children() override{
            return this->child_book;
        } 
};


class Reader {
    protected:
        string name;
        
    public:
        Reader(string const& name)
            : name{name}
            { }

        virtual bool likes(Book& book) {
            return true; 
        }

        void read(Book& book){
             if (this->likes(book)) { 
                cout << this->name << " recommends reading \"" << book.title << "\"." <<endl;
            }
            else {
                cout << this->name << " does not recommend reading \"" << book.title << "\"." << endl;
            }
        }

};

class Academic: public Reader {   
    public: 
        using Reader::Reader;
        bool likes(Book& book) override {
           return dynamic_cast<Non_Fiction*>(&book);
        }        
};

class Child : public Reader {
    public: 
        using Reader::Reader;
        bool likes(Book& book) override {
            return dynamic_cast<Non_Fiction*>(&book) && book.for_children();
        }  
};

class Detective : public Reader {
    public: 
        using Reader::Reader;
        bool likes(Book& book) override {
           return dynamic_cast<Fiction*>(&book);
        }   
};





int main()
{
    vector<Reader*> readers {
        new Reader{"Rhea Deer"},
        new Academic{"Professor McProfessorsen"},
        new Child{"Christopher Robin"},
        new Detective{"Sherlock Holmes"}
    };

    vector<Book*> books {
        new Fiction{"The Lord of the Rings"},
        new Fiction{"Winnie the Pooh", true},
        new Crime_Fiction{"The Da Vinci Code"},
        new Crime_Fiction{"Nancy Drew: The Secret of the Old Clock", true},
        new Non_Fiction{"Course Literature"}
    };

    for (auto reader : readers){
        for (auto book : books)
        {
            reader->read(*book);
        }
    }

    for (auto book : books)
    {
        delete book;
    }


  
}
