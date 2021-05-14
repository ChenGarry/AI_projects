#pragma once
#include "BehaviorNode.h"
#include "Misc/NiceTypes.h"

class L_Flocking : public BaseNode<L_Flocking>
{
protected:
    virtual void on_enter() override;
    virtual void on_update(float dt) override;

private:
    float rotation;
    bool result;
};