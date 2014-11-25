#include "node.h"
#include "virtual_machine.h"
#include <cassert>

namespace BehaviorTree
{

E_State Action::self_update(VirtualMachine& vm, void* context, E_State ) {
    vm.increase_index();
    return update(context);
}

E_State Action::child_update(VirtualMachine& , void* , E_State ) {
    assert(false);
    return BH_RUNNING;
}

E_State Decorator::self_update(VirtualMachine& vm, void*, E_State) {
    vm.increase_index();
    return BH_RUNNING;
}

E_State Decorator::child_update(VirtualMachine& vm, void* context, E_State child_state) {
    NodeData self = vm.get_running_node();
    (void)self;
    assert(self.end - self.begin == 1); // decorator has and only has one child
    return update(context, child_state);
}

} /* BehaviorTree */ 