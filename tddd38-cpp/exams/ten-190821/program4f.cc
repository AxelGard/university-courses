#include <algorithm>
#include <iostream>
#include <vector>
#include <string>
#include <map>
#include <fstream>
#include <iterator>

using namespace std;

int main()
{
  ifstream ifs{"SYNONYMS"};
  map<string, string> synonyms{};
  transform(istream_iterator<string>{ifs}, istream_iterator<string>{},
            inserter(synonyms, begin(synonyms)),
            [](string const& line)
            {
              auto it{find(begin(line), end(line), '=')};
              return make_pair(string{begin(line), it}, string{next(it), end(line)});
            });
  vector<string> words{};
  transform(istream_iterator<string>{cin}, istream_iterator<string>{},
            back_inserter(words), [&synonyms](string const& word)
            {
              auto it{synonyms.find(word)};
              if (it != end(synonyms))
                return it->second;
              return word;
            });
  copy(begin(words), end(words), ostream_iterator<string>{cout, " "});
  cout << endl;
}