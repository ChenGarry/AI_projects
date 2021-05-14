#pragma once
#include "BehaviorNode.h"
#include "Misc/NiceTypes.h"

class L_Die : public BaseNode<L_Die>
{
protected:
    virtual void on_enter() override;
    virtual void on_update(float dt) override;
    virtual void on_exit() override;

private:
    float timer;
    float rateTimer;
};