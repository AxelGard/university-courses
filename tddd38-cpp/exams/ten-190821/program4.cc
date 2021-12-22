#include <iostream>
#include <fstream>
#include <map>
#include <regex>
using namespace std;
/* Testcase:

achieve my advise -> put my tell
my extraordinary walk is gradual -> my amazing move is slow
the evil diagram -> the ugly plan
I consider a microscopic cloak to be irritable -> I think a little hide to be moody

 */

int main()
{
    // 1. open SYNONYMS
    map<string, string> synonyms{};
    string text{""};
    regex re("\\w+=");
    ifstream file("./SYNONYMS");
    
    while (getline(file,text))
    {
        
    }
    file.close();

    // 2. read the synonyms into the synonyms map
    // 3. read text from cin into the vector words, replacing each
    //    possible word, leaving the rest as-is
    // 4. print the text stored in words
}