#include "TerminalWindow.h"
#include <iostream>
#include <sys/ioctl.h>
#include <unistd.h>

#define RESET   "\033[0m"
#define RED     "\033[31m"
#define BOLD    "\033[1m"

TerminalWindow::TerminalWindow() 
{
}

void TerminalWindow::error(std::string message) 
{
    std::cout << RED << message << RESET << std::endl;
}

void TerminalWindow::message(std::string message) 
{
    std::cout << BOLD << message << RESET << std::endl;
}

int TerminalWindow::getConsoleWidth() {
    struct winsize size;
    ioctl(STDOUT_FILENO, TIOCGWINSZ, &size);
    return size.ws_col;
}