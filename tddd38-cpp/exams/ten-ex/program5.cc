#include <iostream>
#include <vector>
#include <numeric>


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
  for (int i{0}; i < k; ++i)
  {
    // move each element into position by swapping them to the end
    for (int j{0}; j + 1 < denominators.size() - i; ++j)
    {
      swap(denominators[j], denominators[j+1]);
    }
  }

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

    iota(begin(factors), end(factors), k + 1);

    iota(begin(denominators), end(denominators), 1);
    
    if (denominators.size() >= k){
        auto dist{rotate(denominators, k)};
        factors.erase(begin(factors), next(begin(factors), dist));
        denominators.erase(begin(denominators), next(begin(denominators), dist));
    }


    // fill factors with k+1, k+2, ..., n-1, n
    int counter{k + 1};
    for (int i{0}; i < factors.size(); ++i)
    {
        factors[i] = counter++;
    }

    // fill denominators with 1, 2, ..., n-k-1, n-k
    counter = 1;
    for (int i{0}; i < denominators.size(); ++i)
    {
        denominators[i] = counter++;
    }

    // if there is overlap between factors and denominators we remove it
    if (denominators.size() >= k)
    {
        // restructure denominators so that the overlap is in the
        // beginning of the vector and retrieve how many elements there
        // where in the overlap
        auto dist{rotate(denominators, k)};

        // remove the overlap
        for (int i{0}; i < dist; ++i)
        {
        factors.erase(factors.begin());
        denominators.erase(denominators.begin());
        }
    }

    // for each denominator we want to divided it with a factor in factors
    for (int i{0}; i < denominators.size(); ++i)
    {

        // find a factor that is divisible by the current denominator
        int j{0};
        for (; j < factors.size(); ++j)
        {
            if (factors[j] % denominators[i] == 0)
            {
                break;
            }
        }
        
        // divide the found factor
        factors[j] /= denominators[i];
        denominators[i] = 1;
    }

    // multiply all factors together
    int result{1};
    for (int i{0}; i < factors.size(); ++i)
    {
        result *= factors[i];
    }

    // print the result
    cout << result << endl;
}