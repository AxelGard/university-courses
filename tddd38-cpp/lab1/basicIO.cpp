#include <iostream>

void oneInt(){
    std::string userInput = {};
    std::cout << "Enter one integer: ";
    std::cin >> userInput;
    std::cout << "You entered the number:" << std::stoi(userInput) << std::endl;
}

void multiInt(){
    std::string userInput = {};
    std::cout << "Enter four integers: ";
    std::getline(std::cin >> std::ws, userInput);
    std::cout << "You entered the numbers: " << userInput << std::endl;
}

void oneIntOneReal(){
    std::string userInput = {};
    std::cout << "Enter one integer and one real number: ";
    std::getline(std::cin >> std::ws, userInput);
    std::cout << "The real is:  "<< userInput << std::endl;
    std::cout << "The integer is:  " << std::endl;
}
int main(){
    oneInt();
    multiInt();
    oneIntOneReal();
    return 0;
}
