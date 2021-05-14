#ifndef TASK
#define TASK
#include <string>
#include <vector>
#include <stack>

enum Tasks {
    T_NAVIGATE,
    T_ATTACK,
    T_PATROL,
    T_HIDE,
    T_RETREAT,
    T_COUNT
};

enum TaskType {
    PRIMITIVE,
    COMPOUND
};

template <typename T>
T& Singleton() {
    static T single;
    return single;
}

class Method {
        std::vector<Tasks> _subTasks;
    public:
        void InsertTop(std::stack<Tasks>& plan);
};

class Task {
    public:
        Task(std::string name, TaskType type) : _name(name), _type(type){}
        virtual bool run(){return true;};
        virtual const Method *findSatisfiedMethod(const std::vector<char>& aiWordState){aiWordState.empty();return nullptr;}
        virtual void simulate(std::vector<char>& aiWordState){aiWordState.empty();}
        virtual ~Task(){}
        TaskType type();
    protected:
        std::string _name;
        TaskType _type;
};

class Navigate : public Task {
    public:
        Navigate() : Task("Navigate", PRIMITIVE){}
        //~Task1(){}
        bool run();
};

class Task2: public Task {
    public:
        Task2() : Task("Task2", COMPOUND){}
        //~Task2(){}
        bool run();
        //bool findSatisfiedMethod(const std::vector<char>& aiWordState);
};

#endif
