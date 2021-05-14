#pragma once
#include "BehaviorNode.h"
#include "Misc/NiceTypes.h"

class L_StandUp : public BaseNode<L_StandUp>
{
protected:
    virtual void on_enter() override;
    virtual void on_update(float dt) override;
    virtual void on_exit() override;

private:
    bool standing;
    float scale = 1.0f;
    Vec3 o_scales;
};