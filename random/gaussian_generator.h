/************************************************************
 * SimLib simulation library for event-based simulations    *
 * Author: Martin Ubl (A16N0026P)                           *
 *         ublm@students.zcu.cz                             *
 ************************************************************/

#pragma once

#include "base_generator.h"

/*
 * Normal (gaussian) distribution generator using specified parameters
 */
template<typename T>
class gaussian_generator : public base_generator<T>
{
    public:
        gaussian_generator(double mean, double deviation) : base_generator(), m_dist(mean, deviation) { };

        virtual T operator()() override
        {
            return static_cast<T>(m_dist(m_engine));
        }

        // reinitializes distribution with new parameters
        virtual void reinit(double mean, double deviation)
        {
            m_dist = std::normal_distribution<double>(mean, deviation);
        }

    protected:
        // encapsulated standard library distribution generator
        std::normal_distribution<double> m_dist;
};
