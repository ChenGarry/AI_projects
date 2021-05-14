#pragma once
#include "BehaviorNode.h"
#include "Misc/NiceTypes.h"

class L_JitterAroundCenter : public BaseNode<L_JitterAroundCenter>
{
protected:
    virtual void on_enter() override;
    virtual void on_update(float dt) override;

private:
    float radius = 2.0f;
	float facing;
    Vec3 targetPoint;
};