/************************************************************
 * SimLib simulation library for event-based simulations    *
 * Author: Martin Ubl (A16N0026P)                           *
 *         ublm@students.zcu.cz                             *
 ************************************************************/

#pragma once

#include "SimulationObject.h"

#include <vector>
#include <random>

/*
 * Selection criteria for event execution
 */
enum class ObjectSelectionCriteria
{
    GUID,       // select by object GUID
    TYPE,       // select by object type
    CLASS       // select by object class
};

/*
 * Selection mode for "secondary" selection
 */
enum class ObjectSelectionMode
{
    ALL,        // selects all, no additional condition
    ONE,        // selects one random element
    K_OF_N      // selects K of total of N elements; parameter: K
};

/*
 * Class representing a simulation event
 */
class SimEvent : public SimulationObject
{
    public:
        SimEvent(SimulationPtr simulation = nullptr, uint32_t objectClass = ObjectClass_NotSpecified);
        virtual ~SimEvent();

        // adds event criteria for affected elements
        template<typename T>
        void AddSelectionCriteria(ObjectSelectionCriteria crit, T critParam, ObjectSelectionMode mode, uint32_t modeParam = 0)
        {
            _AddSelectionCriteria(crit, (uint64_t)critParam, mode, modeParam);
        }

        // run event execution; select objects and execute on them
        void Run() override final;

        // filters objects before execution; empty implementation here, may be overriden
        virtual void FilterObjects(SimulationObjectList& objectList);
        // called before event execution on all selected objects
        virtual void BeforeExecute();
        // called after event execution on all selected objects
        virtual void AfterExecute();
        // execute event on specified object; called from Run method
        virtual void ExecuteOn(SimulationObjectPtr object);

    protected:
        /*
         * Structure of selection criteria used
         */
        struct SelectionCriteria
        {
            // criteria for selection
            ObjectSelectionCriteria criteria;

            // the parameter may have various types
            union
            {
                uint64_t asUInt64;
                int64_t asInt64;
                uint32_t asUInt32;
                int32_t asInt32;
                SimulationObjectType asSimType;
            } critParam;

            // selection mode for filtering
            ObjectSelectionMode mode;
            // parameter
            uint32_t modeParam;
        };

        // performs criteria selection (primary filter) on given list
        void PerformCriteriaSelection(SimulationObjectList& objList, SelectionCriteria& crit);
        // performs mode selection (secondary filter) on given list
        void PerformModeSelection(SimulationObjectList& objList, SelectionCriteria& crit);

        // internal method for adding selection criteria
        void _AddSelectionCriteria(ObjectSelectionCriteria crit, uint64_t critParam, ObjectSelectionMode mode, uint32_t modeParam = 0);

    private:
        // vector of criterias
        std::vector<SelectionCriteria> m_criterias;
        // instance of random engine used
        std::mt19937 m_randomEngine;
};
