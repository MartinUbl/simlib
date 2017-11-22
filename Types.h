/************************************************************
 * SimLib simulation library for event-based simulations    *
 * Author: Martin Ubl (A16N0026P)                           *
 *         ublm@students.zcu.cz                             *
 ************************************************************/

#pragma once

#include <memory>

// general

using simtime_t = unsigned long long;

// Simulation class

class Simulation;

using SimulationPtr = std::shared_ptr<Simulation>;
using SimulationWeakPtr = std::weak_ptr<Simulation>;

// Calendar class

class Calendar;

using CalendarPtr = std::shared_ptr<Calendar>;

// SimulationObject class and children

class SimulationObject;

using SimulationObjectPtr = std::shared_ptr<SimulationObject>;
using SimulationObjectList = std::list<SimulationObjectPtr>;

class SimProcess;
class SimEvent;
