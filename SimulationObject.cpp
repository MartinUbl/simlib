/************************************************************
 * SimLib simulation library for event-based simulations    *
 * Author: Martin Ubl (A16N0026P)                           *
 *         ublm@students.zcu.cz                             *
 ************************************************************/

#include "SimulationObject.h"
#include "Simulation.h"
#include "Calendar.h"

#include "Process.h"
#include "Event.h"

SimulationObject::SimulationObject(SimulationObjectType type, SimulationPtr simulation, uint32_t objectClass)
    : m_type(type), m_objectClass(objectClass), m_simulation(simulation)
{
    m_guid = 0;
}

uint64_t SimulationObject::GetGUID() const
{
    return m_guid;
}

SimulationObjectType SimulationObject::GetType() const
{
    return m_type;
}

void SimulationObject::SetGUID(uint64_t guid)
{
    m_guid = guid;
}

void SimulationObject::SetObjectClass(uint32_t objectClass)
{
    m_objectClass = objectClass;
}

void SimulationObject::SetSimulation(SimulationPtr simulation)
{
    m_simulation = simulation;
}

uint32_t SimulationObject::GetObjectClass() const
{
    return m_objectClass;
}

SimulationPtr SimulationObject::GetSimulation() const
{
    return m_simulation.lock();
}

simtime_t SimulationObject::GetNextSimTime() const
{
    return m_simTimeNext;
}

CalendarPtr SimulationObject::GetCurrentCalendar() const
{
    return m_currCalendar;
}

void SimulationObject::Schedule(CalendarPtr calendar, simtime_t scheduleTime, bool relative)
{
    auto self = shared_from_this();

    // remove from calendar, if present
    if (m_currCalendar)
        m_currCalendar->remove(self);

    // relative time needs to have base schedule time retrieved from simulation object
    if (relative)
    {
        SimulationPtr simulation = GetSimulation();
        if (simulation)
            m_simTimeNext = simulation->GetSimulationTime() + scheduleTime;
        else
            return;
    }
    else
        m_simTimeNext = scheduleTime;

    m_currCalendar = calendar;
    m_currCalendar->push(self);
}

void SimulationObject::ClearScheduleInfo()
{
    m_simTimeNext = 0;
    m_currCalendar = nullptr;
}

SimProcess* SimulationObject::ToProcess()
{
    return dynamic_cast<SimProcess*>(this);
}

SimEvent* SimulationObject::ToEvent()
{
    return dynamic_cast<SimEvent*>(this);
}

void SimulationObject::Terminate()
{
    auto simulation = GetSimulation();
    if (!simulation)
        return;

    auto self = (SimulationObjectPtr)shared_from_this();

    // at first, remove from calendar
    if (m_currCalendar)
    {
        m_currCalendar->remove(self);
        m_currCalendar = nullptr;
    }
    m_simTimeNext = 0;

    // remove from simulation
    simulation->RemoveObject(self);

    m_simulation.reset();
}

bool SimulationObject::HasPeriodicSchedule() const
{
    return (m_scheduleGenerator != nullptr);
}

void SimulationObject::NextPeriodicSchedule(CalendarPtr calendar)
{
    if (!m_scheduleGenerator)
        return;

    Schedule(calendar, (*m_scheduleGenerator)(), true);
}

void SimulationObject::CancelPeriodicSchedule(bool removeFromCalendar)
{
    if (!m_scheduleGenerator)
        return;

    m_scheduleGenerator.reset();

    if (removeFromCalendar)
    {
        auto self = (SimulationObjectPtr)shared_from_this();

        // at first, remove from calendar
        if (m_currCalendar)
        {
            m_currCalendar->remove(self);
            m_currCalendar = nullptr;
        }
        // then reset time
        m_simTimeNext = 0;
    }
}
