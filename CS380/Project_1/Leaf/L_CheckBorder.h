#pragma once
#include "BehaviorNode.h"
#include "Misc/NiceTypes.h"

class L_CheckBorder : public BaseNode<L_CheckBorder>
{
protected:
    virtual void on_enter() override;
    virtual void on_update(float dt) override;

private:
    bool result;
};