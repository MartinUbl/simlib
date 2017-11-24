/************************************************************
 * SimLib simulation library for event-based simulations    *
 * Author: Martin Ubl (A16N0026P)                           *
 *         ublm@students.zcu.cz                             *
 ************************************************************/

#pragma once

#include <iostream>
#include <fstream>

#include "Types.h"

class Logger;

/*
 * Logger line guard created as "RAII" structure for putting endline after write end
 */
struct LoggerLineGuard
{
    LoggerLineGuard(Logger& logger);
    ~LoggerLineGuard();

    // "transparent" logging operator, pass directly to logger
    template<typename T>
    LoggerLineGuard& operator<<(T const& value)
    {
        m_logger.Write(value);
        return *this;
    }

    Logger& m_logger;
};

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
        LoggerLineGuard operator<<(T const& value)
        {
            m_outputFile << value;
            return LoggerLineGuard(*this);
        }

        // simulation time logger bridge
        LoggerLineGuard operator()(const simtime_t time)
        {
            m_outputFile << "[" << time << "] ";
            return LoggerLineGuard(*this);
        }

        // raw write to log
        template<typename T>
        void Write(T const& value)
        {
            m_outputFile << value;
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
