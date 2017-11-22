/************************************************************
 * SimLib simulation library for event-based simulations    *
 * Author: Martin Ubl (A16N0026P)                           *
 *         ublm@students.zcu.cz                             *
 ************************************************************/

#pragma once

#include <memory>
#include <cstdint>
#include <list>
#include <limits>

#include "Types.h"
#include "random/base_generator.h"

/*
 * Type of object in simulation
 */
enum class SimulationObjectType
{
    PROCESS,
    EVENT
};

// class is not specified (default)
constexpr uint32_t ObjectClass_NotSpecified = 0;

/*
 * Class representing a single simulation object
 */
class SimulationObject : public std::enable_shared_from_this<SimulationObject>
{
    friend class Simulation;
    public:
        SimulationObject(SimulationObjectType type, SimulationPtr simulation, uint32_t objectClass = ObjectClass_NotSpecified);

        // retrieves object GUID (unique within single simulation)
        uint64_t GetGUID() const;
        // retrieves object type
        SimulationObjectType GetType() const;
        // retrieves object class
        uint32_t GetObjectClass() const;

        // retrieves simulation pointer
        SimulationPtr GetSimulation() const;

        // retrieves a time this object is scheduled onto (0 if not scheduled)
        simtime_t GetNextSimTime() const;
        // retrieves calendar, in which is this object scheduled (empty pointer if not scheduled)
        CalendarPtr GetCurrentCalendar() const;
        // schedules this object to given calendar
        void Schedule(CalendarPtr calendar, simtime_t scheduleTime, bool relative = false);

        // schedules this object using (pseudo-)random generator with some distribution, periodically
        template<typename T, typename ...Args>
        typename std::enable_if<std::is_base_of<base_generator<simtime_t>, T>::value, void>::type
            SchedulePeriodic(CalendarPtr calendar, bool initialFire, Args... args)
        {
            m_scheduleGenerator = std::make_unique<T>(args...);

            Schedule(calendar, initialFire ? 0 : (*m_scheduleGenerator)(), true);
        }

        // does this object have periodic schedule
        bool HasPeriodicSchedule() const;
        // schedules next tick using number from schedule generator
        void NextPeriodicSchedule(CalendarPtr calendar);
        // cancels periodic schedule if set up previously
        void CancelPeriodicSchedule(bool removeFromCalendar = false);

        // called when scheduled object is fired
        virtual void Run() = 0;

        // casts this object to process
        SimProcess* ToProcess();
        // casts this object to event
        SimEvent* ToEvent();

        // terminates this object within current simulation
        void Terminate();

    protected:
        // sets GUID (used from within Simulation object only); DO NOT call after adding to simulation
        void SetGUID(uint64_t guid);
        // sets object class; DO NOT call after adding to simulation
        void SetObjectClass(uint32_t objectClass);
        // sets simulation which contains this object; DO NOT call after adding to simulation
        void SetSimulation(SimulationPtr simulation);

    private:
        // object GUID (assigned by simulation)
        uint64_t m_guid;
        // object type
        SimulationObjectType m_type;
        // object custom class
        uint32_t m_objectClass;

        // simulation, where this object belongs
        SimulationWeakPtr m_simulation;

        // random 
        std::unique_ptr<base_generator<simtime_t>> m_scheduleGenerator;

        // scheduled time (0 if not scheduled)
        simtime_t m_simTimeNext;
        // scheduled calendar (empty ptr if not scheduled)
        CalendarPtr m_currCalendar;

        // clears schedule info
        void ClearScheduleInfo();
};
