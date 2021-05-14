#pragma once
#include "BehaviorNode.h"

class C_WeightedRandomSelector : public BaseNode<C_WeightedRandomSelector>
{
public:
    C_WeightedRandomSelector();
protected:
    size_t randomIndex;
    size_t weights[3];

    virtual void on_enter() override;
    virtual void on_update(float dt) override;
    
    void choose_random_node();
    bool check_for_all_failed() const;
    
};