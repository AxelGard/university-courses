#include <algorithm>
#include <iostream>
#include <iterator>
#include <string>
#include <vector>
#include <fstream>
#include <tuple>
#include <iomanip>

using namespace std;
using Event = std::tuple<int, int, std::string>;

namespace std
{
    std::istream& operator >> (std::istream &is, Event &event)
    {
        is >> std::get<0>(event) >> std::get<1>(event);
        std::getline(is, std::get<2>(event));
        return is;
    }

    std::ostream& operator << (ostream& os, Event const& event)
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

/*
  Expected output:

07 to 08:  Eat breakfast
08 to 10:  Go to morning lecture
10 to 12:  Study
12 to 13:  Eat lunch
13 to 15:  Afternoon lecture
15 to 17:  Lab work
17 to 18:  Go Home
18 to 19:  Eat dinner
18 to 22:  Relax
22 to 07:  Sleep

 */

int main()
{
    // Events are sorted with respect to start hour. If two start
    // hours are equal then they are sorted by end hour in second
    // hand. If both start and end hour are equal then they are sorted
    // by description.
    
 

    // 1. Read all events from "first.txt" into a vector of Event
    //    called first_schedule.
    std::ifstream first_ifs{"first.txt"};
    std::vector<Event> first_schedule{
        std::istream_iterator<Event>{first_ifs},
        std::istream_iterator<Event>{}};
    
    // 2. Sort first_schedule.
    std::sort(first_schedule.begin(), first_schedule.end());
    
    // 3. Read all events from "second.txt" into a vector of Event
    //    called second_schedule.
    std::ifstream second_ifs{"second.txt"};
    std::vector<Event> second_schedule{
        std::istream_iterator<Event>{second_ifs},
        std::istream_iterator<Event>{}};
    
    // 4. Sort second_schedule.
    std::sort(second_schedule.begin(), second_schedule.end());
    
    // 5. Create a new vector of Event called combined_schedule.
    std::vector<Event> combined_schedule;
        
    // 6. Copy first_schedule and second_schedule into
    //    combined_schedule in such a way that the events are sorted.
    std::merge(first_schedule.begin(), first_schedule.end(),
          second_schedule.begin(), second_schedule.end(),
          back_inserter(combined_schedule));
    
    // 7. Remove any duplicates from combined_schedule.
    auto last = unique(combined_schedule.begin(), combined_schedule.end());
    combined_schedule.erase(last, combined_schedule.end());
    
    // 8. Print each event in combined_schedule, one line per event.
    std::copy(combined_schedule.begin(), combined_schedule.end(),
              std::ostream_iterator<Event>{std::cout, "\n"});
}