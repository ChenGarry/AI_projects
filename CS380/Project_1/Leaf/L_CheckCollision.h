#pragma once
#include "BehaviorNode.h"
#include "Misc/NiceTypes.h"

class L_CheckCollision : public BaseNode<L_CheckCollision>
{
protected:
    virtual void on_enter() override;
    virtual void on_update(float dt) override;

private:
    bool result;
};