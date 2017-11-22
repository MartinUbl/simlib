/************************************************************
 * SimLib simulation library for event-based simulations    *
 * Author: Martin Ubl (A16N0026P)                           *
 *         ublm@students.zcu.cz                             *
 ************************************************************/

#pragma once

#include <random>

// global instance of true random device used across multiple generators
extern std::random_device _global_true_random_device;

/*
 * Base class for all random generators
 */
template<typename T>
class base_generator
{
    public:
        base_generator() : m_engine(_global_true_random_device()) { };

        // operator() is used for retrieving (pseudo)random numbers
        virtual T operator()() = 0;

    protected:
        // random engine instance; we go with platform/compiler defaults here (mt19937)
        std::default_random_engine m_engine;
};
