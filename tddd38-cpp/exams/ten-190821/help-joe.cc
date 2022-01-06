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
    public: 
        virtual ~Book() = default;
        string title; 
        virtual bool for_children() = 0;

};

class Fiction : public Book{

};




class Reader {
    string name; 
    
    Reader(string name)
    {
        this->name = name;
    }

    virtual void read(Book& book){
        cout << this->name << "recommends reading" << book.title << endl;
    }

};


int main()
{
  vector<Reader*> readers {
    new Reader{"Rhea Deer"},
    // Academic{"Professor McProfessorsen"},
    // Child{"Christopher Robin"},
    // Detective{"Sherlock Holmes"}
  };

  vector<Book *> books {
    // Fiction{"The Lord of the Rings"},
    // Fiction{"Winnie the Pooh", true},
    // Crime_Fiction{"The Da Vinci Code"},
    // Crime_Fiction{"Nancy Drew: The Secret of the Old Clock", true},
    // Non_Fiction{"Course Literature"}
  };

  for (auto reader : readers)
  {
    for (auto book : books)
    {
      // reader.read(book);
      // or
      // reader->read(book);
    }
  }
}


