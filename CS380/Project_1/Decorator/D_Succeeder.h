#pragma once
#include "BehaviorNode.h"

class D_Succeeder : public BaseNode<D_Succeeder>
{
protected:
    virtual void on_update(float dt) override;
};