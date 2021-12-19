#include <iostream>

class LinkedList
{
private:
    /* data */
    int value; 
    int index;
    LinkedList* next;
    void sort();
public:
    LinkedList(int value);
    ~LinkedList();
    void add(int value);
    void remove(int index);
    void print();
    int at(int index);

};

LinkedList::LinkedList(int value)
{
    this->value = value; 
    this->index = 0;
    this->next = nullptr;
}

LinkedList::~LinkedList()
{
     
}

void LinkedList::print(){
    std::cout << this->value << "-->";
    if (this->next != nullptr) this->next->print();
    else std::cout << "NULL" << std::endl;
}


int LinkedList::at(int index){
    if (this->index == index)return this->value;
    else if (this->next == nullptr) return -1; // don't know what we should reaturn if we index out of reange. NULL?
    else return this->next->at(index);
}

void LinkedList::add(int value){
    if (this->next != nullptr) this->next->add(value);
    this->next = new LinkedList(value);
    this->next->index = this->index++;
}

void LinkedList::remove(int index){

}



int main(){
    LinkedList linkedList = LinkedList(4711);
    linkedList.print();
    //std::cout << linkedList.at(0) << std::endl;
    linkedList.add(1234);
    linkedList.add(9999);
    linkedList.print();
    linkedList.remove(1);
    linkedList.print();
    return 0;
}