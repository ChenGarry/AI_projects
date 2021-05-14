#pragma once
#include "BehaviorNode.h"
#include "Misc/NiceTypes.h"

class L_Sprint : public BaseNode<L_Sprint>
{
protected:
    virtual void on_enter() override;
    virtual void on_update(float dt) override;
private:
    float timer = 1.0f;
};