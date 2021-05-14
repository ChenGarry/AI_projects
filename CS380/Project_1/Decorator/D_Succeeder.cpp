#include <pch.h>
#include "D_Succeeder.h"

void D_Succeeder::on_update(float dt)
{
    BehaviorNode *child = children.front();

    child->tick(dt);

    // return success after child finished
    if (child->succeeded() == true || child->failed() == true)
    {
         on_success();
    }
}

