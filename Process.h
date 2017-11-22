/************************************************************
 * SimLib simulation library for event-based simulations    *
 * Author: Martin Ubl (A16N0026P)                           *
 *         ublm@students.zcu.cz                             *
 ************************************************************/

#pragma once

#include "SimulationObject.h"

#include "Types.h"

/*
 * Class representing a single process in simulation
 */
class SimProcess : public SimulationObject
{
    public:
        SimProcess(SimulationPtr simulation = nullptr, uint32_t objectClass = ObjectClass_NotSpecified);
        virtual ~SimProcess();

        // run method - executed after being scheduled
        virtual void Run() override;
        // method called upon receiving event
        virtual void ReceiveEvent(SimEvent& ev);

    protected:
        //

    private:
        //
};
