#pragma once
#include "BehaviorNode.h"
#include "Misc/NiceTypes.h"

class L_CheckCoolDown : public BaseNode<L_CheckCoolDown>
{
protected:
    virtual void on_enter() override;
    virtual void on_update(float dt) override;
private:
    float timer = 0.0f;
    float cooldown;
};