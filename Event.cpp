/************************************************************
 * SimLib simulation library for event-based simulations    *
 * Author: Martin Ubl (A16N0026P)                           *
 *         ublm@students.zcu.cz                             *
 ************************************************************/

#include "Event.h"
#include "Process.h"
#include "Simulation.h"

#include <random>
#include <algorithm>

SimEvent::SimEvent(SimulationPtr simulation, uint32_t objectClass)
    : SimulationObject(SimulationObjectType::EVENT, simulation, objectClass),
      m_randomEngine(simulation->GetTrueRandomNumber())
{
    //
}

SimEvent::~SimEvent()
{
    //
}

void SimEvent::_AddSelectionCriteria(ObjectSelectionCriteria crit, uint64_t critParam, ObjectSelectionMode mode, uint32_t modeParam)
{
    m_criterias.resize(m_criterias.size() + 1);

    size_t i = m_criterias.size() - 1;

    m_criterias[i].criteria = crit;
    m_criterias[i].critParam.asUInt64 = critParam;
    m_criterias[i].mode = mode;
    m_criterias[i].modeParam = modeParam;
}

void SimEvent::Run()
{
    // valid simulation object is required
    SimulationPtr simulation = GetSimulation();
    if (!simulation)
        return;

    SimulationObjectList objlist;

    // not empty criterias - filter
    if (!m_criterias.empty())
    {
        size_t i = 0;

        SelectionCriteria& crit = m_criterias[i];

        // the first filter is processed separatelly due to need of calling specialized version of "Get" method
        // in fact this would not be needed, but instead of copying objects, this is slightly closer to being "optimal"

        switch (crit.criteria)
        {
            case ObjectSelectionCriteria::GUID:
            {
                auto ptr = simulation->GetObjectByGUID(crit.critParam.asUInt64);
                if (ptr)
                    objlist.push_back(ptr);
                break;
            }
            case ObjectSelectionCriteria::TYPE:
            {
                objlist = simulation->GetObjectsByType(crit.critParam.asSimType);
                break;
            }
            case ObjectSelectionCriteria::CLASS:
            {
                objlist = simulation->GetObjectsByClass(crit.critParam.asUInt32);
                break;
            }
        }

        // also perform separate mode selection
        PerformModeSelection(objlist, crit);

        // now apply all criterias separatelly in a generic way
        for (; i < m_criterias.size(); i++)
        {
            crit = m_criterias[i];

            PerformCriteriaSelection(objlist, crit);
            PerformModeSelection(objlist, crit);

            // no need to apply more criterias, if the list is empty
            if (objlist.empty())
                break;
        }
    }
    else // if no criterias at all - select all objects
        objlist = simulation->GetAllObjects();

    // call user-defined filters
    FilterObjects(objlist);

    // now we are ready!

    BeforeExecute();

    // iterate through all objects in list and execute event on them
    for (auto obj : objlist)
    {
        ExecuteOn(obj);

        if (obj->GetType() == SimulationObjectType::PROCESS)
            obj->ToProcess()->ReceiveEvent(*this);
    }

    AfterExecute();

    // destroy event object, if not rescheduled (or not scheduled periodically)
    if (!HasPeriodicSchedule() && !GetCurrentCalendar())
        Terminate();
}

void SimEvent::PerformCriteriaSelection(SimulationObjectList& objList, SelectionCriteria& crit)
{
    auto itr = objList.begin();

    switch (crit.criteria)
    {
        case ObjectSelectionCriteria::GUID:
            itr = std::remove_if(objList.begin(), objList.end(), [=](SimulationObjectPtr obj) { return obj->GetGUID() != crit.critParam.asUInt64; });
            break;
        case ObjectSelectionCriteria::TYPE:
            itr = std::remove_if(objList.begin(), objList.end(), [=](SimulationObjectPtr obj) { return obj->GetType() != crit.critParam.asSimType; });
            break;
        case ObjectSelectionCriteria::CLASS:
            itr = std::remove_if(objList.begin(), objList.end(), [=](SimulationObjectPtr obj) { return obj->GetObjectClass() != crit.critParam.asUInt32; });
            break;
    }

    objList.erase(itr, objList.end());
}

void SimEvent::PerformModeSelection(SimulationObjectList& objList, SelectionCriteria& crit)
{
    switch (crit.mode)
    {
        case ObjectSelectionMode::ALL:
            break;
        case ObjectSelectionMode::ONE:
        {
            if (objList.size() <= 1)
                return;

            // generate random index (will be used)
            std::uniform_int_distribution<size_t> indexRandDist(0, objList.size() - 1);
            size_t i = indexRandDist(m_randomEngine);

            // move iterator to that "index"
            auto itr = objList.begin();
            std::advance(itr, i);

            // get pointer, erase list and put pointer back in
            SimulationObjectPtr obj = *itr;
            objList.clear();
            objList.push_back(obj);

            break;
        }
        case ObjectSelectionMode::K_OF_N:
        {
            // no need to perform "0 of N"
            if (crit.modeParam == 0)
            {
                objList.clear();
                return;
            }

            // ..or "N of N" selection
            if (crit.modeParam >= objList.size())
                return;

            // shuffle the list
            std::vector<SimulationObjectPtr> tmpVec(objList.begin(), objList.end());
            std::shuffle(tmpVec.begin(), tmpVec.end(), m_randomEngine);
            objList.assign(tmpVec.begin(), tmpVec.end());

            // leave just first K elements
            auto itr = objList.begin();
            std::advance(itr, crit.modeParam);

            // the rest is erased
            while (itr != objList.end())
                itr = objList.erase(itr);

            break;
        }
    }
}

void SimEvent::FilterObjects(SimulationObjectList& objectList)
{
    //
}

void SimEvent::BeforeExecute()
{
    //
}

void SimEvent::AfterExecute()
{
    //
}

void SimEvent::ExecuteOn(SimulationObjectPtr object)
{
    //
}
