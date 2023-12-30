#pragma once
#include <string>

class TerminalWindow
{
    public:
        TerminalWindow();
        static void error(std::string message);
        static void message(std::string message);
        static int getConsoleWidth();
};
