#include <vector>
#include <string>
#include <cassert>
#include <algorithm>
#include <numeric>
#include <iostream>


using namespace std;


int entry_score(string entry)
{
    // calculate the score of an entry
    vector<int> result{};
    
    transform(begin(entry), end(entry), begin(entry), 
        [](unsigned char c){ return tolower(c); });

    transform(begin(entry), end(entry), begin(entry), 
        [](char c){ return int(c); });
    
    adjacent_difference(begin(entry), end(entry), back_inserter(result));
    result.erase(begin(result));

    vector<double> weights(result.size());
    iota(begin(weights), end(weights), 1.0);
    
    transform(begin(weights), end(weights), begin(weights), 
        [size = weights.size()](double a){ return a / size; });
    return inner_product(begin(result), end(result), begin(weights), 0.0);
}

int calculate_score(vector<string> const& list)
{
    // calculate the score of a wishlist
    vector<double> scores(list.size());
    transform(begin(list), end(list), begin(scores), entry_score);
    return accumulate(begin(scores), end(scores), 0);
    
}

int main()
{
    {
        vector<string> list { "Fortnite", "Playstation 4", "Dog" };
        assert(calculate_score(list) == -62);
    }
    {
        vector<string> list { "Charcoal", "C++ Primer", "World Peace" };
        assert(calculate_score(list) == 35);
    }
    {
        vector<string> list { "Fortnite", "C++ Primer" };
        assert(calculate_score(list) == 20);
    }
    {
        vector<string> list { "Playstation 4", "Charcoal", "C++ Primer" };
        assert(calculate_score(list) == -17);
    }
}