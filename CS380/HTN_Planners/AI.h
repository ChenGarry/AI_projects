#ifndef AI
#define AI
#include <string>
#include <queue>
#include "planner.h"

class HTNAgent {
        friend class HTNPlanner;
    public:
        HTNAgent(std::string name, int stateCount) : _name(name), _aiWordState(stateCount,0){}
        void tick();
        void set_health(int health);
        void set_pos(int pos);
        virtual ~HTNAgent();
    protected:
        virtual void sense();
        int runPlan();
        void findPlan();
        std::string _name;
        std::vector<char> _aiWordState;
        std::queue<Tasks> _plan;
        Tasks _root;
        int _pos;
        int _health;
};

class Dog : public HTNAgent {
    public:
        Dog(std::string name) : HTNAgent(name, 5) {}
    protected:
        void sense();
};

class Human: public HTNAgent {
    public:
        Human(std::string name) : HTNAgent(name, 5) {}
    protected:
        void sense();
        int ammo;
        int gun;
};

#endif
