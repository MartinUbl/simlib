/************************************************************
 * SimLib simulation library for event-based simulations    *
 * Author: Martin Ubl (A16N0026P)                           *
 *         ublm@students.zcu.cz                             *
 ************************************************************/

#pragma once

#include <iostream>
#include <fstream>

/*
 * Simulation logger class
 */
class Logger
{
    public:
        Logger(std::ostream& outFile);
        virtual ~Logger();

        // "transparent" logging function to pass inputs to output stream directly
        template<typename T>
        Logger& operator<<(T const& value)
        {
            m_outputFile << value;
            return *this;
        }

        // end of line
        static constexpr auto endl()
        {
            return '\n';
        }

    protected:
        // output file used
        std::ostream& m_outputFile;
};
