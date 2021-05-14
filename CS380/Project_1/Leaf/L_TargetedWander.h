#pragma once
#include "BehaviorNode.h"
#include "Misc/NiceTypes.h"

class L_TargetedWander : public BaseNode<L_TargetedWander>
{
protected:
    virtual void on_enter() override;
    virtual void on_update(float dt) override;

private:
    float rotation;
	float timer;
    Vec3 targetPoint;
	Vec3 movingPoint;
};