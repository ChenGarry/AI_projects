#pragma once
#include "BehaviorNode.h"
#include "Misc/NiceTypes.h"

class L_MoveUntillBorder : public BaseNode<L_MoveUntillBorder>
{
protected:
    virtual void on_enter() override;
    virtual void on_update(float dt) override;

private:
    float lane;
    int dir = 0;
};