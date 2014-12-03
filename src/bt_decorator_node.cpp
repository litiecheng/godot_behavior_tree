#include "bt_decorator_node.h"
#include "bt_string_names.h"

BTDecoratorNode::BTDecoratorNode()
    : delegate(*this)
{
}

void BTDecoratorNode::_bind_methods() {
	BIND_VMETHOD( MethodInfo(BTStringNames::get_singleton()->_continue, PropertyInfo(Variant::INT,"index"), PropertyInfo(Variant::NIL,"context")) );
	BIND_VMETHOD( MethodInfo(BTStringNames::get_singleton()->_prepare, PropertyInfo(Variant::INT,"index"), PropertyInfo(Variant::NIL,"context")) );
	BIND_VMETHOD( MethodInfo(BTStringNames::get_singleton()->_pre_update, PropertyInfo(Variant::INT,"index"), PropertyInfo(Variant::NIL,"context")) );
	BIND_VMETHOD( MethodInfo(BTStringNames::get_singleton()->_post_update, PropertyInfo(Variant::INT,"index"), PropertyInfo(Variant::NIL,"context"), PropertyInfo(Variant::INT,"child_state")) );
	BIND_VMETHOD( MethodInfo(BTStringNames::get_singleton()->_abort, PropertyInfo(Variant::INT,"index"), PropertyInfo(Variant::NIL,"context")) );
}

void BTDecoratorNode::add_child_node(BTNode& child, Vector<BehaviorTree::Node*>& node_hierarchy) {
    BTNode* p_parent = get_parent() ? get_parent()->cast_to<BTNode>() : NULL;
	ERR_EXPLAIN("Parent node is not a BTNode.");
	ERR_FAIL_NULL(p_parent);
	ERR_EXPLAIN("Decorator node only allowed one child.");
	ERR_FAIL_COND(get_child_count() > 1);
    if (p_parent && get_child_count() <= 1) {
        node_hierarchy.push_back(get_behavior_node());
        p_parent->add_child_node(child, node_hierarchy);
    }
}

void BTDecoratorNode::remove_child_node(BTNode& child, Vector<BehaviorTree::Node*>& node_hierarchy) {
    BTNode* p_parent = get_parent() ? get_parent()->cast_to<BTNode>() : NULL;
	//ERR_EXPLAIN("Parent node is not a BTNode.");
	//ERR_FAIL_NULL(p_parent);
    if (p_parent) {
        node_hierarchy.push_back(get_behavior_node());
        p_parent->remove_child_node(child, node_hierarchy);
    }
}

void BTDecoratorNode::Delegate::restore_running(
        BehaviorTree::VirtualMachine& vm,
        BehaviorTree::IndexType index,
        void* context) {
    super::restore_running(vm, index, context);
    script_call(BTStringNames::get_singleton()->_continue, index, context);
}

void BTDecoratorNode::Delegate::prepare(
        BehaviorTree::VirtualMachine& vm,
        BehaviorTree::IndexType index,
        void* context) {
    super::prepare(vm, index, context);
    script_call(BTStringNames::get_singleton()->_prepare, index, context);
}

BehaviorTree::E_State BTDecoratorNode::Delegate::pre_update(BehaviorTree::IndexType index, void* context) {
    Variant result_state = script_call(BTStringNames::get_singleton()->_pre_update, index, context);
    ERR_EXPLAIN("Variant type is not int.");
    ERR_FAIL_COND_V( result_state.get_type() != Variant::INT, BehaviorTree::BH_ERROR );
    return static_cast<BehaviorTree::E_State>(static_cast<int>(result_state));
}

BehaviorTree::E_State BTDecoratorNode::Delegate::post_update(
        BehaviorTree::IndexType index,
        void* context,
        BehaviorTree::E_State child_state) {
    Variant result_state = script_call(BTStringNames::get_singleton()->_post_update, index, context, child_state);
    ERR_EXPLAIN("Variant type is not int.");
    ERR_FAIL_COND_V( result_state.get_type() != Variant::INT, BehaviorTree::BH_ERROR );
    return static_cast<BehaviorTree::E_State>(static_cast<int>(result_state));
}

void BTDecoratorNode::Delegate::abort(BehaviorTree::VirtualMachine& vm, BehaviorTree::IndexType index, void* context) {
    super::abort(vm, index, context);
    script_call(BTStringNames::get_singleton()->_abort, index, context);
}
