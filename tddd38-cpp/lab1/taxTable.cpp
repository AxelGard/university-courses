#include <iostream>
#include <iomanip> 
#include <map>
#include <math.h>

int main(){

    std::cout << "INPUT PART\n==========" << std::endl;

    float firstPrice; 
    do {

        std::cout << "Enter first price: ";
        std::cin >> firstPrice;
        if (firstPrice < 0)
            std::cout << "ERROR: First price must be at least 0 (zero) SEK" << std::endl;
    } while (firstPrice > 0);

    float lastPrice; 
    do {
        std::cout << "Enter last price: ";
        std::cin >> lastPrice;
        if (lastPrice < 0)
        std::cout << "ERROR: Last price must be at least 0 (zero) SEK" << std::endl;
    } while (lastPrice > 0);

    float stride; 
    do {
        std::cout << "Enter stride: ";
        std::cin >> stride;
        if (stride < 0)
            std::cout << "ERROR: Stride must be at least 0.01" << std::endl;
    } while (stride >= 0.01);

    float tax; 
    do {
        std::cout << "Enter tax percent: ";
        std::cin >> tax;
        if (lastPrice < 0)
            std::cout << "ERROR: Tax must be at least 0%" << std::endl;
    } while (lastPrice > 0);

    std::cout << "TAX TABLE\n=========\n\tPrice\tTax\tPrice with tax\n-------------------------------------------------" << std::endl;
    float curPrice = firstPrice;
    while (curPrice != lastPrice){
        curPrice += stride;

        float taxOnPrice = curPrice * (tax/100);


        std::cout << "\t" +  std::to_string(roundf(curPrice * 100) / 100) + "\t" + 
            std::to_string(roundf(taxOnPrice * 100) / 100) + "\t" + std::to_string(roundf((curPrice + taxOnPrice) * 100) / 100) << std::endl;
    } 

    return 0;
}