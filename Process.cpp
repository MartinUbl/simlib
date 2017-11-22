/************************************************************
 * SimLib simulation library for event-based simulations    *
 * Author: Martin Ubl (A16N0026P)                           *
 *         ublm@students.zcu.cz                             *
 ************************************************************/

#include "Process.h"
#include "Simulation.h"

SimProcess::SimProcess(SimulationPtr simulation, uint32_t objectClass)
    : SimulationObject(SimulationObjectType::PROCESS, simulation, objectClass)
{
    //
}

SimProcess::~SimProcess()
{
    //
}

void SimProcess::Run()
{
    //
}

void SimProcess::ReceiveEvent(SimEvent& ev)
{
    //
}
