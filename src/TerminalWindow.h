#pragma once
#include <string>

class TerminalWindow
{
    public:
        TerminalWindow();
        /** Print error on console in RED */
        static void error(std::string message);
        /** Print message on console in BOLD */
        static void message(std::string message);
        /** Get console width. Returns integer. */
        static int getConsoleWidth();
};
