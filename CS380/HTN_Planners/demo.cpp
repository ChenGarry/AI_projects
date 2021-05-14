#include "planner.h"
#include <iostream>
#include <vector>
#include <stdlib.h>

void test0(unsigned frame) {
    std::cout << "test0 :" << frame << std::endl;
    Singleton<Navigate>().run();
    Singleton<Task2>().run();
}

void test1(unsigned frame) {
    std::cout << "test1 :" << frame << std::endl;
    ChangeWord();
    for(auto w: currentWordState) {
        std::cout << w << " ";
    }
    std::cout << currentWordState[EWS_1] << std::endl;
}

void (*pState[])(unsigned) = { 
    test0,test1 };

int main() {
    bool run = true;
    unsigned frame = 0;
    int state = 0;
    while(run)
    {
        system("cls");
        int choice;
        std::cout << "time: "<< frame++ << std::endl;
        pState[state](frame);
        std::cin >> choice; 
        switch (choice)
        {
        case -1:
            run = false;
            break;
        case 0:
        case 1:
            state = choice;
            break;
        default:
            break;
        }
    }
}
