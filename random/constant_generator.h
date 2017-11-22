/************************************************************
 * SimLib simulation library for event-based simulations    *
 * Author: Martin Ubl (A16N0026P)                           *
 *         ublm@students.zcu.cz                             *
 ************************************************************/

#pragma once

#include "base_generator.h"

/*
 * Generator of constant number line; just for genericity
 */
template<typename T>
class constant_generator : public base_generator<T>
{
    public:
        constant_generator(T val) : base_generator(), m_value(val) { };

        virtual T operator()() override
        {
            return static_cast<T>(m_value);
        }

        // reinitializes generator with a new value
        virtual void reinit(T val)
        {
            m_value = val;
        }

    protected:
        T m_value;
};
