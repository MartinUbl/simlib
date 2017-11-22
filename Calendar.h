/************************************************************
 * SimLib simulation library for event-based simulations    *
 * Author: Martin Ubl (A16N0026P)                           *
 *         ublm@students.zcu.cz                             *
 ************************************************************/

#pragma once

#include "Types.h"

#include <queue>
#include <list>
#include <memory>

/*
 * Comparator functor for time-ordered priority queue
 */
struct TimePriorityCmp
{
    bool operator()(SimulationObjectPtr const& a, SimulationObjectPtr const& b);
};

/*
 * Calendar class used for scheduling simulation events
 */
class Calendar : public std::priority_queue<SimulationObjectPtr, std::vector<SimulationObjectPtr>, TimePriorityCmp>
{
    public:
        // removes specified simulation object from calendar (using its GUID)
        bool remove(const SimulationObjectPtr obj);

        // creates new calendar instance
        static CalendarPtr Create();
};

/*
 * Class representing a list of calendars with joined top and pop methods
 */
class CalendarList : public std::list<CalendarPtr>
{
    public:
        // get simulation object on top of dominant calendar
        SimulationObjectPtr top() const;
        // pops element from top
        void pop();
        // are all calendars empty?
        bool all_empty() const;
};
