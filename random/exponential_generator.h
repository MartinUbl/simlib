/************************************************************
 * SimLib simulation library for event-based simulations    *
 * Author: Martin Ubl (A16N0026P)                           *
 *         ublm@students.zcu.cz                             *
 ************************************************************/

#pragma once

#include "base_generator.h"

/*
 * Generator of exponential distribution with specified parameters
 */
template<typename T>
class exponential_generator : public base_generator<T>
{
    public:
        exponential_generator(double lambda) : base_generator(), m_dist(lambda) { };

        virtual T operator()() override
        {
            return static_cast<T>(m_dist(m_engine));
        }

        // reinitializes generator with a new lambda
        virtual void reinit(double lambda)
        {
            m_dist = std::exponential_distribution<double>(lambda);
        }

    protected:
        // encapsulated standard library distribution generator
        std::exponential_distribution<double> m_dist;
};
