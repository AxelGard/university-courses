#include <cmath>
#include <iostream>
#include <vector>

using namespace std;

int main()
{
    double const length {10.0};
    vector<double> road {};

    double tmp{};
    while (cin >> tmp)
    {
        road.push_back(tmp);
    }

    vector<double> result(road);

    for (unsigned i{1}; i < road.size(); ++i)
    {
        std::swap(road[i-1], road[i]);
    }

    for (unsigned i{0}; i < road.size(); ++i)
    {
        double a {road[i]};
        double b {result[i]};
        result[i] = sqrt((a - b) * (a - b) + length * length);
    }

    for (unsigned i{0}; i < result.size(); ++i)
    {
        cout << result[i] << " m" << endl;
    }

    double total {0.0};
    for (unsigned i{0}; i < result.size(); ++i)
    {
        total += result[i];
    }

    cout << "Total distance: " << total << " m" << endl;
}
