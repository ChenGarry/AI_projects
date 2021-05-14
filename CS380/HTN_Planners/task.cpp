#include "task.h"
#include <iostream>

bool Navigate::run() 
{
    std::cout << _name << std::endl;
    return true;
}

bool Task2::run() 
{
    std::cout << _name << std::endl;
    return true;
}