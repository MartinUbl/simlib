/************************************************************
 * SimLib simulation library for event-based simulations    *
 * Author: Martin Ubl (A16N0026P)                           *
 *         ublm@students.zcu.cz                             *
 ************************************************************/

#pragma once

#include "base_generator.h"
#include <cmath>

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
            return static_cast<T>(round(m_dist(m_engine)));
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


// explicitly specializate generators for double and float, to not round the result

template <>
double exponential_generator<double>::operator()()
{
    return static_cast<double>(m_dist(m_engine));
}

template <>
float exponential_generator<float>::operator()()
{
    return static_cast<float>(m_dist(m_engine));
}
