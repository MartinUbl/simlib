/************************************************************
 * SimLib simulation library for event-based simulations    *
 * Author: Martin Ubl (A16N0026P)                           *
 *         ublm@students.zcu.cz                             *
 ************************************************************/

#include <iostream>
#include "Simulation.h"

std::random_device Simulation::m_randDev;

Simulation::Simulation(std::ostream& logOutput)
    : m_guidNext(1), m_logger(logOutput), m_simMode(SimulationMode::CONTINUOUS), m_simulationTime(0)
{
    //
}

Simulation::~Simulation()
{
    //
}

SimulationPtr Simulation::Create(std::ostream& logOutput)
{
    auto ptr = std::make_shared<Simulation>(logOutput);

    return ptr;
}

void Simulation::SetSimulationMode(SimulationMode mode)
{
    m_simMode = mode;
}

SimulationMode Simulation::GetSimulationMode() const
{
    return m_simMode;
}

Logger& Simulation::GetLogger()
{
    return m_logger;
}

unsigned int Simulation::GetTrueRandomNumber()
{
    return m_randDev();
}

void Simulation::Setup(CalendarPtr mainCalendar)
{
    m_calendarList.push_back(mainCalendar);

    m_logger << "Setting up simulation" << Logger::endl();
}

void Simulation::AddCalendar(CalendarPtr nextCalendar)
{
    m_calendarList.push_back(nextCalendar);
}

CalendarPtr Simulation::GetMainCalendar() const
{
    if (m_calendarList.empty())
        return nullptr;

    return *(m_calendarList.begin());
}

simtime_t Simulation::GetSimulationTime() const
{
    return m_simulationTime;
}

bool Simulation::IsRunning() const
{
    return m_running;
}

void Simulation::Terminate(int64_t exitCode)
{
    if (!IsRunning())
        return;

    m_running = false;
    m_exitCode = exitCode;
}

int64_t Simulation::Run()
{
    // step counter
    size_t step = 0;

    // by default, assume that everything's ok
    m_exitCode = SimulationExitCode_OK;

    m_running = true;

    while (!m_calendarList.all_empty() && m_running)
    {
        step++;

        // if stepped simulation mode is selected, wait for key press every step
        if (m_simMode == SimulationMode::STEPPED)
        {
            m_logger << "Step: " << step << Logger::endl();
            std::cin.get();
        }

        // retrieve head of calendar queues (all of calendars)
        auto obj = m_calendarList.top();
        m_calendarList.pop();

        // set current simulation time
        m_simulationTime = obj->GetNextSimTime();

        m_logger << "Object " << obj->GetGUID() << " fired at " << obj->GetNextSimTime() << Logger::endl();

        // run scope
        {
            CalendarPtr cal = obj->GetCurrentCalendar();

            // at first, clear schedule info from object, so the object is no longer considered "scheduled"
            obj->ClearScheduleInfo();
            // fire Run method
            obj->Run();

            // if the object have periodic schedule plan, perform planning
            if (obj->HasPeriodicSchedule())
                obj->NextPeriodicSchedule(cal);
        }
    }

    m_running = false;

    return m_exitCode;
}

uint64_t Simulation::AddObject(SimulationObjectPtr object)
{
    // assign GUID
    object->SetGUID(m_guidNext);

    // add object to all maps; from this point, the GUID, type and class are fixed and should not be changed!
    m_objectGuidMap[m_guidNext] = object;
    m_objectTypeMap[object->GetType()].push_back(object);
    m_objectClassMap[object->GetObjectClass()].push_back(object);

    m_logger << "Adding object (GUID: " << m_guidNext << ", type: " << (int)object->GetType() << ", class: " << object->GetObjectClass() << ")" << Logger::endl();

    return m_guidNext++;
}

void Simulation::RemoveObjectFromList(SimulationObjectList& simList, SimulationObjectPtr object)
{
    for (auto itr = simList.begin(); itr != simList.end(); )
    {
        if ((*itr)->GetGUID() == object->GetGUID())
            itr = simList.erase(itr);
        else
            ++itr;
    }
}

void Simulation::RemoveObject(SimulationObjectPtr object)
{
    if (m_objectGuidMap.find(object->GetGUID()) != m_objectGuidMap.end())
        m_objectGuidMap.erase(object->GetGUID());

    auto itr = m_objectTypeMap.find(object->GetType());
    if (itr != m_objectTypeMap.end())
        RemoveObjectFromList(itr->second, object);

    auto itr2 = m_objectClassMap.find(object->GetObjectClass());
    if (itr2 != m_objectClassMap.end())
        RemoveObjectFromList(itr2->second, object);
}

SimulationObjectPtr Simulation::GetObjectByGUID(uint64_t guid) const
{
    auto itr = m_objectGuidMap.find(guid);
    if (itr != m_objectGuidMap.end())
        return itr->second;

    return nullptr;
}

SimulationObjectList Simulation::GetObjectsByType(SimulationObjectType type) const
{
    auto itr = m_objectTypeMap.find(type);
    if (itr != m_objectTypeMap.end())
        return itr->second;

    return SimulationObjectList();
}

SimulationObjectList Simulation::GetObjectsByClass(uint32_t objectClass) const
{
    auto itr = m_objectClassMap.find(objectClass);
    if (itr != m_objectClassMap.end())
        return itr->second;

    return SimulationObjectList();
}

SimulationObjectList Simulation::GetAllObjects() const
{
    SimulationObjectList lst;

    for (auto& pr : m_objectGuidMap)
        lst.push_back(pr.second);

    return lst;
}
