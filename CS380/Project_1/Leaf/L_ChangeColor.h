#pragma once
#include "BehaviorNode.h"
#include "Misc/NiceTypes.h"

class L_ChangeColor : public BaseNode<L_ChangeColor>
{
protected:
    virtual void on_enter() override;
    virtual void on_update(float dt) override;
private:
    int colorIndex = 1;
    Vec3 Color;
};