#include <iostream>
#include <fstream>
#include <vector>
#include <sstream>

std::vector<std::string> wordsToVec(std::string text){
    const char delim = ' ';
    std::vector<std::string> words = {};
    std::stringstream ss(text);
    std::string s;
    while (std::getline(ss,s, delim)){
        words.push_back(s);
    }
    return words;
}

float avgLenOfWords(std::vector<std::string> words){
    float totLen = 0.0;
    for (std::string word: words){
        totLen += word.size();
    }
   return totLen / words.size(); 
}

std::string fileToString(std::string path){
    std::string text = "";
    std::string line;
    std::ifstream rfile;
    rfile.open(path);
    if (rfile.is_open()){
        while (std::getline(rfile,line)){
            text += line;
        }
    } 
    
    return text;
}

int main(){
    
    std::cout << wordsToVec(fileToString("./ww2.txt")).size() << std::endl;    
    std::cout << avgLenOfWords(wordsToVec(fileToString("./ww2.txt"))) << std::endl;

    return 0;
}