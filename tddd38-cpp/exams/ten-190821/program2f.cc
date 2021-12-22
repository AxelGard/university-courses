#include <string>
#include <iostream>
#include <vector>

using namespace std;

class Book
{
public:

  Book(string const& title)
    : title{title} { }

  Book(Book const&) = delete;
  
  virtual ~Book() = default;
  virtual bool for_children() const = 0;
  string get_title() const
  {
    return title;
  }
  
private:

  string const title{};
  
};

class Non_Fiction : public Book
{
public:

  using Book::Book;
  
  bool for_children() const override
  {
    return false;
  }
  
};

class Fiction : public Book
{
public:

  Fiction(string const& title, bool childrens_book = false)
    : Book{title}, childrens_book{childrens_book} { }
  
  bool for_children() const override
  {
    return childrens_book;
  }

private:

  bool const childrens_book{};
  
};

class Crime_Fiction : public Fiction
{
public:

  Crime_Fiction(string const& title, bool childrens_book = false)
    : Fiction{title + " (best seller)", childrens_book} { }
  
};

class Reader
{
public:

  Reader(string const& name)
    : name{name} { }

  virtual ~Reader() = default;
    
  Reader& Reader(Reader const&) = delete;
    
  virtual bool likes(Book const* book) const
  {
    return true;
  }

  void read(Book const* book) const
  {
    cout << name;
    if (likes(book))
    {
      cout << " recommends ";
    }
    else
    {
      cout << " does not recommend ";
    }
    cout << "reading \"" << book->get_title() << "\"." << endl;
  }

private:

  string const name;
  
};

class Academic : public Reader
{
public:
  using Reader::Reader;
  
  bool likes (Book const* book) const override
  {
    return dynamic_cast<Non_Fiction const*>(book);
  }
  
};

class Child : public Reader
{
public:
  using Reader::Reader;
  
  bool likes (Book const* book) const override
  {
    return book->for_children();
  }
};

class Detective : public Reader
{
  public:
  using Reader::Reader;
  
  bool likes (Book const* book) const override
  {
    return typeid(*book) == typeid(Fiction);
  }
};

int main()
{
  vector<Reader const*> readers {
    new Reader{"Rhea Deer"},
    new Academic{"Professor McProfessorsen"},
    new Child{"Christopher Robin"},
    new Detective{"Sherlock Holmes"}
  };

  vector<Book const*> books {
    new Fiction{"The Lord of the Rings"},
    new Fiction{"Winnie the Pooh", true},
    new Crime_Fiction{"The Da Vinci Code"},
    new Crime_Fiction{"Nancy Drew: The Secret of the Old Clock", true},
    new Non_Fiction{"Course Literature"}
  };

  for (auto reader : readers)
  {
    for (auto book : books)
    {
      reader->read(book);
    }
    delete reader;
  }

  for (auto book : books)
  {
    delete book;
  }
}