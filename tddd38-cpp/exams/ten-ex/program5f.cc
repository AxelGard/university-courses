#include <algorithm>
#include <iostream>
#include <iterator>
#include <numeric>
#include <vector>

using namespace std;

int rotate(vector<int> & denominators, int k)
{
  // move all elements in the range [0, k] to the end of the vector
  auto it{rotate(begin(denominators),
                 next(begin(denominators), k),
                 end(denominators))};

  // return how far in the overlap ends
  return distance(begin(denominators), it);
}

int main()
{

  int n, k;
  cin >> n >> k;
  
  // store a vector of all factors in the numerator
  vector<int> factors(n - k, 0);

  // store a vector of all divisors in the denominator
  vector<int> denominators(n - k, 0);

  // fill factors with k+1, k+2, ..., n-1, n
  iota(begin(factors), end(factors), k + 1);

  // fill denominators with 1, 2, ..., n-k-1, n-k
  iota(begin(denominators), end(denominators), 1);

  // if there is overlap between factors and denominators we remove it
  if (denominators.size() >= k)
  {
    // restructure denominators so that the overlap is in the
    // beginning of the vector and retrieve how many elements there
    // where in the overlap
    auto dist {rotate(denominators, k)};

    // remove the overlap
    factors.erase(begin(factors), next(begin(factors), dist));
    denominators.erase(begin(denominators), next(begin(denominators), dist));
  }

  // for each denominator we want to divided it with a factor in factors
  transform(begin(denominators), end(denominators), begin(denominators),
            [&factors](int den)
            {
              // find a factor that is divisible by the current denominator
              auto it {find_if(begin(factors), end(factors),
                               [den](int num) { return num % den == 0; })};
              // divide the found factor
              *it /= den;
              return 1;
            });

  // multiply all factors together
  int result{accumulate(begin(factors), end(factors), 1, multiplies<int>{})};

  // print the result
  cout << result << endl;
  
}