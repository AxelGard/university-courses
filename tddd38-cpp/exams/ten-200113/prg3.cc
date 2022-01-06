#include <cmath>
#include <iostream>
#include <vector>
#include <algorithm>
#include <iterator>
#include <numeric> 

using namespace std;

int main()
{
    double const length {10.0};
    vector<double> road {1.2,3.4,4.5,14.4};

    /*double tmp{};
    while (cin >> tmp)
    {
        road.push_back(tmp);
    }*/

    vector<double> result(road);

    /*for (unsigned i{1}; i < road.size(); ++i)
    {
        swap(road[i-1], road[i]);
    }*/

    rotate(begin(road), next(begin(road)),  end(road));

    std::transform(begin(road), end(road), begin(result), end(result),
    [length](double a, double b){
        return sqrt((a - b) * (a - b) + length * length);
    });

    /*
    for (unsigned i{0}; i < road.size(); ++i)
    {
        double a {road[i]};
        double b {result[i]};
        result[i] = sqrt((a - b) * (a - b) + length * length);
    }*/

    copy(begin(result), end(result), ostream_iterator<double>(cout, " m\n"));

    /*
    for (unsigned i{0}; i < result.size(); ++i)
    {
        cout << result[i] << " m" << endl;
    }*/




    cout << "Total distance: " << std::accumulate(begin(result), end(result), 0.0)
     << " m" << endl;
     /*
    for (unsigned i{0}; i < result.size(); ++i)
    {
        total += result[i];
    }

    cout << "Total distance: " << total << " m" << endl;
    */
}
