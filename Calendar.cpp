/************************************************************
 * SimLib simulation library for event-based simulations    *
 * Author: Martin Ubl (A16N0026P)                           *
 *         ublm@students.zcu.cz                             *
 ************************************************************/

#include "SimulationObject.h"
#include "Calendar.h"

bool TimePriorityCmp::operator()(SimulationObjectPtr const& a, SimulationObjectPtr const& b)
{
    return a->GetNextSimTime() > b->GetNextSimTime();
}

CalendarPtr Calendar::Create()
{
    auto ptr = std::make_shared<Calendar>();
    return ptr;
}

bool Calendar::remove(const SimulationObjectPtr obj)
{
    // find iterator containing specified object
    auto it = c.begin();
    for (; it != c.end(); it++)
    {
        if ((*it)->GetGUID() == obj->GetGUID())
            break;
    }

    // if found...
    if (it != c.end())
    {
        // erase from heap and rebuild it
        c.erase(it);
        std::make_heap(c.begin(), c.end(), comp);
        return true;
    }

    return false;
}

SimulationObjectPtr CalendarList::top() const
{
    // select first non-empty calendar
    auto itr = begin();
    while (itr != end() && (*itr)->empty())
        ++itr;

    if (itr == end())
        return nullptr;

    // current top is "the best" for now
    SimulationObjectPtr curTop = (*itr)->top();
    ++itr;

    // go through the rest of calendars and find better
    while (itr != end())
    {
        if ((*itr)->empty())
            continue;

        if ((*itr)->top()->GetNextSimTime() < curTop->GetNextSimTime())
            curTop = (*itr)->top();

        ++itr;
    }

    return curTop;
}

void CalendarList::pop()
{
    // select first non-empty calendar
    auto itr = begin();
    while (itr != end() && (*itr)->empty())
        ++itr;

    if (itr == end())
        return;

    // current top is "the best" calendar for now
    CalendarPtr curTop = (*itr);
    ++itr;

    while (itr != end())
    {
        if ((*itr)->empty())
            continue;

        if ((*itr)->top()->GetNextSimTime() < curTop->top()->GetNextSimTime())
            curTop = (*itr);

        ++itr;
    }

    curTop->pop();
}

bool CalendarList::all_empty() const
{
    auto itr = begin();
    while (itr != end() && (*itr)->empty())
        ++itr;

    return itr == end();
}
