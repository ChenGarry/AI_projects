#include <pch.h>
#include "D_RepeatUntilFail.h"

void D_RepeatUntilFail::on_update(float dt)
{
    BehaviorNode *child = children.front();

    child->tick(dt);

    if (child->succeeded() == true)
    {
        child->set_status(NodeStatus::READY);
    }
    else if (child->failed() == true)
    {
        on_failure();
    }

}

