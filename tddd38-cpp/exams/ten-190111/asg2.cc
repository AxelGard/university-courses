#include <string>
#include <vector>
#include <iostream>

using namespace std;


class Participant {
  public:

    Participant() = default;
    Participant(Participant const&) = delete;
    Participant& operator=(Participant const&) = delete;
    virtual ~Participant() = default;

    virtual bool defeats(Participant const* participants);

    virtual string get_name();

};


class Named : public Participant {
  protected: 
    string name; 

  public: 
    Named(string const& name)
      : name{name}
      { }

    string get_name() override {
      return this->name;
    }


};

class Child : public Named {
  public:
    using Named::Named;

    bool defeats(Participant const* participant) override;
};

class Naughty_Child : public Child {
  public:
    
    using Child::Child;

    bool defeats(Participant const* participant) override
    {
      return Child::defeats(participant) || typeid(*participant) == typeid(Child);
    }

};

class Santa : public Participant{
  public:
    bool defeats(Participant const* participant) override;
    
    string get_name() override {
      return "Santa";
    }
};

class Reindeer : public Named {
  private:
    bool glowing;

  public: 
    
    Reindeer(string const& name, bool glowing=false)
      : Named{name}, glowing{glowing}
      { }

    bool is_glowing() const{
      return this->glowing;
    }

    bool defeats(Participant const* participant) override {
      return dynamic_cast<Santa const*>(participant);
    }
};

bool Santa::defeats(Participant const* participant)
    {
      return dynamic_cast<Naughty_Child const*>(participant);
    }

bool Child::defeats(Participant const* participant)
    {
      if(auto p = dynamic_cast<Reindeer const*>(participant))
        return p->is_glowing();
      return false;
    }

/*
Correct output should be:

Wednesday Addams hits Kevin McCallister with a snowball!
Wednesday Addams hits Rudolf with a snowball!
Kevin McCallister hits Rudolf with a snowball!
Rudolf hits Santa with a snowball!
Cupid hits Santa with a snowball!
Santa hits Wednesday Addams with a snowball!
 */
int main()
{
  vector<Participant*> participants {
    new Naughty_Child{"Wednesday Addams"},
    new Child{"Kevin McCallister"},
    new Reindeer{"Rudolf", true},
    new Reindeer{"Cupid", false},
    new Santa{}
  };

  for (auto p1 : participants)
  {
    for (auto p2 : participants)
    {
      if (p1 != p2)
      {
        if (p1->defeats(p2))
        {
          cout << p1->get_name() << " hits " << p2->get_name() << " with a snowball!" << endl;
        }
      }
    }
  }

  for (Participant* p : participants)
  {
    delete p;
  }

}
