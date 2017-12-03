/************************************************************
 * SimLib simulation library for event-based simulations    *
 * Author: Martin Ubl (A16N0026P)                           *
 *         ublm@students.zcu.cz                             *
 ************************************************************/

#pragma once

#include <iostream>
#include <map>
#include <random>

#include "SimulationObject.h"
#include "Logger.h"
#include "Types.h"
#include "Calendar.h"

// exit code for successfull simulation
constexpr int64_t SimulationExitCode_OK = 0;
// exit code for errorneous simulation (terminated due to problems)
constexpr int64_t SimulationExitCode_Fail = -1;

/*
 * Supported simulation modes
 */
enum class SimulationMode
{
    CONTINUOUS,     // continue automatically
    STEPPED,        // request enter key press on every step
};

/*
 * Simulation class
 */
class Simulation : public std::enable_shared_from_this<Simulation>
{
    public:
        Simulation(std::ostream& logOutput = std::cout);
        virtual ~Simulation();

        // static factory method for creating simulation object pointer
        static SimulationPtr Create(std::ostream& logOutput = std::cout);

        // sets up a simulation using given main calendar
        void Setup(CalendarPtr mainCalendar);
        // adds next calendar to calendar list
        void AddCalendar(CalendarPtr nextCalendar);
        // retrieves main calendar
        CalendarPtr GetMainCalendar() const;

        // sets simulation mode
        void SetSimulationMode(SimulationMode mode);
        // retrieves current simulation mode
        SimulationMode GetSimulationMode() const;

        // runs simulation; Setup method must be called prior calling Run; returns simulation "exit code"
        int64_t Run();

        // is the simulation running?
        bool IsRunning() const;
        // terminates simulation with given exit code
        void Terminate(int64_t exitCode = SimulationExitCode_OK, SimulationObjectPtr initiator = nullptr);
        // retrieves object, that initiated simulation termination
        SimulationObjectPtr GetTerminateInitiator() const;

        // adds an object to simulation, assigns GUID and returns it
        uint64_t AddObject(SimulationObjectPtr object);

        // creates object using standard templated approach; the object needs to be child of SimulationObject
        template<typename T>
        typename std::enable_if<std::is_base_of<SimulationObject, T>::value, std::shared_ptr<T>>::type
        CreateObject(uint32_t objectClass = ObjectClass_NotSpecified)
        {
            SimulationPtr self = (SimulationPtr)shared_from_this();

            auto ptr = std::make_shared<T>();
            // override class setting only if the object didn't specify its own in constructor
            if (ptr->GetObjectClass() == ObjectClass_NotSpecified)
                ptr->SetObjectClass(objectClass);
            ptr->SetSimulation(self);
            AddObject(ptr);

            return ptr;
        }

        // removes object from simulation
        void RemoveObject(SimulationObjectPtr object);

        // retrieves object by its GUID
        SimulationObjectPtr GetObjectByGUID(uint64_t guid) const;
        // retrieves object list with given type
        SimulationObjectList GetObjectsByType(SimulationObjectType type) const;
        // retrieves object list with given class
        SimulationObjectList GetObjectsByClass(uint32_t objectClass) const;
        // retrieves list of all objects in simulation
        SimulationObjectList GetAllObjects() const;

        // retrieves simulation logger
        Logger& GetLogger();

        // retrieves a number from true random number device
        static unsigned int GetTrueRandomNumber();

        // retrieves current simulation time
        simtime_t GetSimulationTime() const;

    protected:
        // objects stored by their GUID
        std::map<uint64_t, SimulationObjectPtr> m_objectGuidMap;
        // objects stored in lists by their type
        std::map<SimulationObjectType, SimulationObjectList> m_objectTypeMap;
        // objects stored in lists by their class
        std::map<uint32_t, SimulationObjectList> m_objectClassMap;

        // removes specified object from given list
        void RemoveObjectFromList(SimulationObjectList& simList, SimulationObjectPtr object);

    private:
        // next GUID to be assigned
        uint64_t m_guidNext;
        // logger instance
        Logger m_logger;

        // current simulation time
        simtime_t m_simulationTime;
        // simulation mode used
        SimulationMode m_simMode;
        // all calendars in consideration
        CalendarList m_calendarList;

        // simulation "exit code"
        int64_t m_exitCode;
        // initiator of termination
        SimulationObjectPtr m_exitInitiator;
        // is the simulation still running?
        bool m_running;

        // static random device used for ocassional TRNG generations
        static std::random_device m_randDev;
};
