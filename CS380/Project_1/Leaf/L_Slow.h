#pragma once
#include "BehaviorNode.h"
#include "Misc/NiceTypes.h"

class L_Slow : public BaseNode<L_Slow>
{
protected:
    virtual void on_enter() override;
    virtual void on_update(float dt) override;
private:
    float timer = 1.0f;
};