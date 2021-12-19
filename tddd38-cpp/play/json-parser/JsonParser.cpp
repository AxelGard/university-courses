#include <iostream>
#include <fstream>
#include <vector>
#include <sstream>
#include <map>

class JsonParser
{
private:
    /* data */
    std::string filePath;
    std::string fileToString();
public:
    JsonParser(std::string filePath);
    ~JsonParser();
    std::map<std::string, std::string> Parser();
};

JsonParser::JsonParser(std::string filePath)
{
    this->filePath = filePath;

}

JsonParser::~JsonParser()
{
}

std::map<std::string, std::string> JsonParser::Parser(){

}

std::string JsonParser::fileToString(){
    std::string text = "";
    std::string line;
    std::ifstream rfile;
    rfile.open(this->filePath);
    if (rfile.is_open()){
        while (std::getline(rfile,line)){
            text += line;
        }
    } 
    return text;
}




int main(){
    JsonParser jsonParser = JsonParser("./example_all.json");

    return 0;
}
