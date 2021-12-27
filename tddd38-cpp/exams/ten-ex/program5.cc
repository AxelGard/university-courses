#include <iostream>
#include <vector>
#include <numeric>
#include <algorithm>


using namespace std;

/*
  Some test data:

  input:  | output:
  5 0     | 1
  5 1     | 5
  5 2     | 10
  5 3     | 10
  5 4     | 5
  5 5     | 1

  input:  | output:
  6 0     | 1
  6 1     | 6
  6 2     | 15
  6 3     | 20
  6 4     | 15
  6 5     | 6
  6 6     | 1

  input:  | output:
  10 0    | 1
  10 1    | 10
  10 2    | 45
  10 3    | 120
  10 4    | 210
  10 5    | 252
  10 6    | 210
  10 7    | 120
  10 8    | 45
  10 9    | 10
  10 10   | 1
  
 */

int rotate(vector<int> & denominators, int k)
{
  // move all elements in the range [0, k] to the end of the vector
// move each element into position by swapping them to the end
  rotate(denominators, k);

  // return how far in the overlap ends
  return denominators.size() - k;
}

int main()
{
  int n, k;
  std::cin >> n >> k;

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
    auto dist{rotate(denominators, k)};

    // remove the overlap
    factors.erase(begin(factors), next(begin(factors), dist));
    denominators.erase(begin(denominators), next(begin(denominators), dist));
  }

  // for each denominator we want to divided it with a factor in factors
  transform(begin(denominators), end(denominators), begin(denominators), 
    [&factors](int den){
      // find a factor that is divisible by the current denominator
      auto it{find_if(begin(factors), end(factors), 
          [den](int num){return num % den == 0;}
        )};
      *it /= den;  // divide the found factor
      return 1;
  });

  // multiply all factors together
  int result{accumulate(begin(factors), end(factors),1, std::multiplies<int>{})};

  // print the result
  cout << result << endl;
}