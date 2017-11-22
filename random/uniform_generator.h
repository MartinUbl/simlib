/************************************************************
 * SimLib simulation library for event-based simulations    *
 * Author: Martin Ubl (A16N0026P)                           *
 *         ublm@students.zcu.cz                             *
 ************************************************************/

#pragma once

#include "base_generator.h"

/*
 * Uniform distribution generator (integers) with specified parameters
 */
template<typename T>
class uniform_int_generator : public base_generator<T>
{
    public:
        uniform_int_generator(T minval, T maxval) : base_generator(), m_dist(minval, maxval) { };

        virtual T operator()() override
        {
            return static_cast<T>(m_dist(m_engine));
        }

        // reinitializes the distribution with new parameters
        virtual void reinit(T minval, T maxval)
        {
            m_dist = std::uniform_int_distribution<T>(minval, maxval);
        }

    protected:
        // encapsulated standard library distribution generator
        std::uniform_int_distribution<T> m_dist;
};

/*
 * Uniform distribution generator (floats) with specified parameters
 */
template<typename T>
class uniform_real_generator : public base_generator<T>
{
    public:
        uniform_real_generator(T minval, T maxval) : base_generator(), m_dist(minval, maxval) { };

        virtual T operator()() override
        {
            return static_cast<T>(m_dist(m_engine));
        }

        // reinitializes the distribution with new parameters
        virtual void reinit(T minval, T maxval)
        {
            m_dist = std::uniform_real_distribution<T>(minval, maxval);
        }

    protected:
        // encapsulated standard library distribution generator
        std::uniform_real_distribution<T> m_dist;
};
