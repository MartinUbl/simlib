/************************************************************
 * SimLib simulation library for event-based simulations    *
 * Author: Martin Ubl (A16N0026P)                           *
 *         ublm@students.zcu.cz                             *
 ************************************************************/

#include "Logger.h"

LoggerLineGuard::LoggerLineGuard(Logger& logger)
    : m_logger(logger)
{
    //
}

LoggerLineGuard::~LoggerLineGuard()
{
    // write endline at the end of writing
    m_logger.Write(Logger::endl());
}

Logger::Logger(std::ostream& outFile)
    : m_outputFile(outFile)
{
    //
}

Logger::~Logger()
{
    //
}
