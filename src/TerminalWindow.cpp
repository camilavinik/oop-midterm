#include "TerminalWindow.h"
#include <iostream>
#include <sys/ioctl.h>
#include <unistd.h>

// terminal output colors and styles
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

// used reference from: https://stackoverflow.com/questions/1022957/getting-terminal-width-in-c
int TerminalWindow::getConsoleWidth() {
    struct winsize size;
    ioctl(STDOUT_FILENO, TIOCGWINSZ, &size);
    return size.ws_col;
}