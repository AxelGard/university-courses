#include <algorithm>
#include <iostream>
#include <iterator>
#include <string>
#include <vector>

using namespace std;

// List of all "synonyms"
vector<string> words {
    "back", "ball", "band", "bare", "bass", "beads", "bean", "belt", "berry",
    "bids", "bite", "bold", "bone", "bones", "book", "cafe", "caps", "card",
    "cash", "cell", "cent", "chan", "code", "cool", "cork", "cost", "cure",
    "dale", "days", "dead", "dish", "dive", "dome", "ears", "east", "eight",
    "face", "fail", "fear", "feed", "file", "files", "find", "fired", "food",
    "form", "full", "gage", "grad", "hang", "hart", "hate", "heat", "hill",
    "hole", "jake", "jose", "king", "lane", "leading", "lets", "life",
    "lines", "list", "logs", "long", "lover", "mark", "math", "mens", "mice",
    "mild", "mint", "miss", "modes", "nest", "nick", "nike", "paid", "pine",
    "poll", "pope", "port", "real", "ride", "ross", "same", "sans", "seas",
    "seeds", "seek", "shake", "shot", "slow", "span", "takes", "then",
    "tire", "tons", "track", "wales", "walt", "warm", "wave", "wise",
    "word", "wound"
};

bool is_synonyms(string const& word, string const& other)
{
    
    
    // 1. If word and other don't have the same size, return false
    if (word.size() == other.size()){
        // 2. Find the first position where word and other have different characters
        auto it = mismatch(begin(word), end(word), begin(other));
        
        // 3. If such a position is found, search for the next position where they differ
        if (it.first != end(word)){
            ++it.first;
            ++it.second;
            it = mismatch(it.first, end(word), it.second);
            // 4. If no other difference is found, return true
            return it.first == end(word); 
        }
        // 5. return false in every other case
    }
    return false;
}

string get_synonym(string const& word)
{
    // 1. Try to find a synonym to word in the words vector; use
    //    the is_synonym function
    auto it {find_if(begin(words), end(words), [&word](string other){
        return is_synonyms(word, other);
    })};
    // 2. If a synonym was found, return it
    if (it == end(words))
        return word;
    // 3. If no synonym was found, return word
    return *it;
}

int main()
{
    // 1. Read all words from cin to a container named text
    vector<string> txt {
        istream_iterator<string>{cin},
        istream_iterator<string>{}
    };
    cout << endl;
    // 2. Replace each word in text with a synonym; use get_synonym
    transform(begin(txt),end(txt), begin(txt), get_synonym);
    // 3. Print the new text
    copy(begin(txt), end(txt), ostream_iterator<string>{cout, " "});
    cout << endl;
}