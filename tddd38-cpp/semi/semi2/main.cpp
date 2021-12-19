#include <iostream>
#include <vector>

// references  

int addOne (int& x){
    return ++x;
}

int addOne (int const& x){
    return x + 1;
}

class cls{
    public:
        int magic(int x){
            return ++x;
        }

        float magic(float x){
            return --x;
        }
        void set(int x){
            this->x = x;
        }
        int get(){
            return this->x;
        }
    private:
        int x;
};


int main(){
    int num = 3;
    int& refnum = num;
    std::cout << refnum << std::endl;
    std::cout << addOne(refnum) << std::endl;
    std::cout << num << std::endl;

    std::cout << "\n"<< std::endl;


    cls m = cls{};
    float flo = 3.0;
    std::cout << m.magic(num) << std::endl;
    std::cout << m.magic(flo) << std::endl;

    return 0;
}