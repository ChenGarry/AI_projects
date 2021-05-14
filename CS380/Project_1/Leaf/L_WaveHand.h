#pragma once
#include "BehaviorNode.h"
#include "Misc/NiceTypes.h"

class L_WaveHand : public BaseNode<L_WaveHand>
{
protected:
    virtual void on_enter() override;
    virtual void on_update(float dt) override;
    virtual void on_exit() override;

private:
    int count;
    int dir = 1;
    //float timer;
    float waveSpeed = 90.0f;
};