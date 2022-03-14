#include <algorithm>
#include <iostream>
#include <iterator>
#include <string>
#include <vector>
#include <fstream>
#include <tuple>
#include <iomanip>

using Event = std::tuple<int, int, std::string>;

namespace std
{
    std::istream &operator>>(std::istream &is, Event &event)
    {
        is >> std::get<0>(event) >> std::get<1>(event);
        std::getline(is, std::get<2>(event));
        return is;
    }

    std::ostream &operator<<(std::ostream &os, Event const &event)
    {
        os << std::setfill('0')
           << std::setw(2) << std::get<0>(event)
           << " to "
           << std::setw(2) << std::get<1>(event)
           << ": "
           << std::get<2>(event);
        return os;
    }
}

int main()
{
    std::ifstream first_ifs{"first.txt"};
    std::vector<Event> first_schedule{
        std::istream_iterator<Event>{first_ifs},
        std::istream_iterator<Event>{}};

    std::sort(first_schedule.begin(), first_schedule.end());

    std::ifstream second_ifs{"second.txt"};
    std::vector<Event> second_schedule{
        std::istream_iterator<Event>{second_ifs},
        std::istream_iterator<Event>{}};

    std::sort(second_schedule.begin(), second_schedule.end());

    std::vector<Event> combined_schedule{};

    std::merge(first_schedule.begin(), first_schedule.end(),
               second_schedule.begin(), second_schedule.end(),
               std::back_inserter(combined_schedule));

    combined_schedule.erase(std::unique(combined_schedule.begin(),
                                        combined_schedule.end()),
                            combined_schedule.end());

    std::copy(combined_schedule.begin(), combined_schedule.end(),
              std::ostream_iterator<Event>{std::cout, "\n"});
}