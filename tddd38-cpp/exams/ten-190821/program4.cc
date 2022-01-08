#include <algorithm>
#include <iostream>
#include <vector>
#include <string>
#include <map>
#include <fstream>
#include <iterator>


using namespace std;
/* Testcase:

achieve my advise -> put my tell
my extraordinary walk is gradual -> my amazing move is slow
the evil diagram -> the ugly plan
I consider a microscopic cloak to be irritable -> I think a little hide to be moody

 */

int main() {
    // 1. open SYNONYMS
    ifstream ifs {"SYNONYMS.txt"};
    map<string, string> synonyms;
    
    // 2. read the synonyms into the synonyms map
    transform(istream_iterator<string>{ifs}, istream_iterator<string>{}, 
        inserter(synonyms, begin(synonyms)), 
        [](string const& line){
            auto it{find(begin(line), end(line), '=')};
            return make_pair(string{begin(line), it}, string{next(it), end(line)});
        });
    
    // 3. read text from cin into the vector words, replacing each
    //    possible word, leaving the rest as-is
    vector<string> words{};
    transform(istream_iterator<string>{cin}, istream_iterator<string>{},
        back_inserter(words), [&synonyms](string const& word){
            auto it{synonyms.find(word)};
            if (it != end(synonyms))
                return it->second;
            return word;
        });

    // 4. print the text stored in words
    copy(begin(words), end(words), ostream_iterator<string>{cout, " "});
    cout << endl;

}