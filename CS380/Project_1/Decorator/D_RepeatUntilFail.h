#pragma once
#include "BehaviorNode.h"

class D_RepeatUntilFail : public BaseNode<D_RepeatUntilFail>
{
protected:
    virtual void on_update(float dt) override;
};