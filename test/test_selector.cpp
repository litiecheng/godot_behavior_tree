#include "catch.hpp"
#include "utils.h"

TEST_CASE( "Behavior Tree Selector", "[bt_seq]" ) {
    VirtualMachine vm;
    MockSelector selector;
    SECTION( "single" ) {
        to_vm(vm, selector.inner_node);
        vm.tick(nullptr);
        REQUIRE(selector.counter.prepare == 1);
        REQUIRE(selector.counter.abort == 0);
        REQUIRE(selector.counter.self_update == 1);
        REQUIRE(selector.counter.child_update == 0);
    }

    MockAction action_foo;
    selector.inner_node.children.push_back(action_foo.inner_node);
    SECTION( "[S]" ) {
        action_foo.update_result = BH_SUCCESS;
        to_vm(vm, selector.inner_node);
        vm.tick(nullptr);
        REQUIRE(selector.child_update_result == BH_SUCCESS);
        REQUIRE(selector.counter.prepare == 1);
        REQUIRE(selector.counter.abort == 0);
        REQUIRE(selector.counter.self_update == 1);
        REQUIRE(selector.counter.child_update == 1);
        REQUIRE(action_foo.counter.prepare == 1);
        REQUIRE(action_foo.counter.abort == 0);
        REQUIRE(action_foo.counter.self_update == 1);
        REQUIRE(action_foo.counter.child_update == 0);
        vm.tick(nullptr);
        REQUIRE(selector.child_update_result == BH_SUCCESS);
        REQUIRE(selector.counter.prepare == 2);
        REQUIRE(selector.counter.abort == 0);
        REQUIRE(selector.counter.self_update == 2);
        REQUIRE(selector.counter.child_update == 2);
        REQUIRE(action_foo.counter.prepare == 2);
        REQUIRE(action_foo.counter.abort == 0);
        REQUIRE(action_foo.counter.self_update == 2);
        REQUIRE(action_foo.counter.child_update == 0);
    }

    SECTION( "[F]" ) {
        action_foo.update_result = BH_FAILURE;
        to_vm(vm, selector.inner_node);
        vm.tick(nullptr);
        REQUIRE(selector.child_update_result == BH_FAILURE);
        REQUIRE(selector.counter.prepare == 1);
        REQUIRE(selector.counter.abort == 0);
        REQUIRE(selector.counter.self_update == 1);
        REQUIRE(selector.counter.child_update == 1);
        REQUIRE(action_foo.counter.prepare == 1);
        REQUIRE(action_foo.counter.abort == 0);
        REQUIRE(action_foo.counter.self_update == 1);
        REQUIRE(action_foo.counter.child_update == 0);
        vm.tick(nullptr);
        REQUIRE(selector.child_update_result == BH_FAILURE);
        REQUIRE(selector.counter.prepare == 2);
        REQUIRE(selector.counter.abort == 0);
        REQUIRE(selector.counter.self_update == 2);
        REQUIRE(selector.counter.child_update == 2);
        REQUIRE(action_foo.counter.prepare == 2);
        REQUIRE(action_foo.counter.abort == 0);
        REQUIRE(action_foo.counter.self_update == 2);
        REQUIRE(action_foo.counter.child_update == 0);
    }

    SECTION( "[R]" ) {
        action_foo.update_result = BH_RUNNING;
        to_vm(vm, selector.inner_node);
        vm.tick(nullptr);
        REQUIRE(selector.child_update_result == BH_RUNNING);
        REQUIRE(selector.counter.prepare == 1);
        REQUIRE(selector.counter.abort == 0);
        REQUIRE(selector.counter.self_update == 1);
        REQUIRE(selector.counter.child_update == 1);
        REQUIRE(action_foo.counter.prepare == 1);
        REQUIRE(action_foo.counter.abort == 0);
        REQUIRE(action_foo.counter.self_update == 1);
        REQUIRE(action_foo.counter.child_update == 0);
        vm.tick(nullptr);
        REQUIRE(selector.child_update_result == BH_RUNNING);
        REQUIRE(selector.counter.prepare == 1);
        REQUIRE(selector.counter.abort == 0);
        REQUIRE(selector.counter.self_update == 2);
        REQUIRE(selector.counter.child_update == 2);
        REQUIRE(action_foo.counter.prepare == 1);
        REQUIRE(action_foo.counter.abort == 0);
        REQUIRE(action_foo.counter.self_update == 2);
        REQUIRE(action_foo.counter.child_update == 0);
    }

    MockAction action_bar;
    selector.inner_node.children.push_back(action_bar.inner_node);
    SECTION( "[F,F]" ) {
        action_foo.update_result = BH_FAILURE;
        action_bar.update_result = BH_FAILURE;
        to_vm(vm, selector.inner_node);
        vm.tick(nullptr);
        REQUIRE(selector.child_update_result == BH_FAILURE);
        REQUIRE(selector.counter.prepare == 1);
        REQUIRE(selector.counter.abort == 0);
        REQUIRE(selector.counter.self_update == 1);
        REQUIRE(selector.counter.child_update == 2);
        REQUIRE(action_foo.counter.prepare == 1);
        REQUIRE(action_foo.counter.abort == 0);
        REQUIRE(action_foo.counter.self_update == 1);
        REQUIRE(action_foo.counter.child_update == 0);
        REQUIRE(action_bar.counter.prepare == 1);
        REQUIRE(action_bar.counter.abort == 0);
        REQUIRE(action_bar.counter.self_update == 1);
        REQUIRE(action_bar.counter.child_update == 0);
        vm.tick(nullptr);
        REQUIRE(selector.child_update_result == BH_FAILURE);
        REQUIRE(selector.counter.prepare == 2);
        REQUIRE(selector.counter.abort == 0);
        REQUIRE(selector.counter.self_update == 2);
        REQUIRE(selector.counter.child_update == 4);
        REQUIRE(action_foo.counter.prepare == 2);
        REQUIRE(action_foo.counter.abort == 0);
        REQUIRE(action_foo.counter.self_update == 2);
        REQUIRE(action_foo.counter.child_update == 0);
        REQUIRE(action_bar.counter.prepare == 2);
        REQUIRE(action_bar.counter.abort == 0);
        REQUIRE(action_bar.counter.self_update == 2);
        REQUIRE(action_bar.counter.child_update == 0);
    }

    SECTION( "[S,F]" ) {
        action_foo.update_result = BH_SUCCESS;
        action_bar.update_result = BH_FAILURE;
        to_vm(vm, selector.inner_node);
        vm.tick(nullptr);
        REQUIRE(selector.child_update_result == BH_SUCCESS);
        REQUIRE(selector.counter.prepare == 1);
        REQUIRE(selector.counter.abort == 0);
        REQUIRE(selector.counter.self_update == 1);
        REQUIRE(selector.counter.child_update == 1);
        REQUIRE(action_foo.counter.prepare == 1);
        REQUIRE(action_foo.counter.abort == 0);
        REQUIRE(action_foo.counter.self_update == 1);
        REQUIRE(action_foo.counter.child_update == 0);
        REQUIRE(action_bar.counter.prepare == 0);
        REQUIRE(action_bar.counter.abort == 0);
        REQUIRE(action_bar.counter.self_update == 0);
        REQUIRE(action_bar.counter.child_update == 0);
        vm.tick(nullptr);
        REQUIRE(selector.child_update_result == BH_SUCCESS);
        REQUIRE(selector.counter.prepare == 2);
        REQUIRE(selector.counter.abort == 0);
        REQUIRE(selector.counter.self_update == 2);
        REQUIRE(selector.counter.child_update == 2);
        REQUIRE(action_foo.counter.prepare == 2);
        REQUIRE(action_foo.counter.abort == 0);
        REQUIRE(action_foo.counter.self_update == 2);
        REQUIRE(action_foo.counter.child_update == 0);
        REQUIRE(action_bar.counter.prepare == 0);
        REQUIRE(action_bar.counter.abort == 0);
        REQUIRE(action_bar.counter.self_update == 0);
        REQUIRE(action_bar.counter.child_update == 0);
    }

    SECTION( "[F,S]" ) {
        action_foo.update_result = BH_FAILURE;
        action_bar.update_result = BH_SUCCESS;
        to_vm(vm, selector.inner_node);
        vm.tick(nullptr);
        REQUIRE(selector.child_update_result == BH_SUCCESS);
        REQUIRE(selector.counter.prepare == 1);
        REQUIRE(selector.counter.abort == 0);
        REQUIRE(selector.counter.self_update == 1);
        REQUIRE(selector.counter.child_update == 2);
        REQUIRE(action_foo.counter.prepare == 1);
        REQUIRE(action_foo.counter.abort == 0);
        REQUIRE(action_foo.counter.self_update == 1);
        REQUIRE(action_foo.counter.child_update == 0);
        REQUIRE(action_bar.counter.prepare == 1);
        REQUIRE(action_bar.counter.abort == 0);
        REQUIRE(action_bar.counter.self_update == 1);
        REQUIRE(action_bar.counter.child_update == 0);
        vm.tick(nullptr);
        REQUIRE(selector.child_update_result == BH_SUCCESS);
        REQUIRE(selector.counter.prepare == 2);
        REQUIRE(selector.counter.abort == 0);
        REQUIRE(selector.counter.self_update == 2);
        REQUIRE(selector.counter.child_update == 4);
        REQUIRE(action_foo.counter.prepare == 2);
        REQUIRE(action_foo.counter.abort == 0);
        REQUIRE(action_foo.counter.self_update == 2);
        REQUIRE(action_foo.counter.child_update == 0);
        REQUIRE(action_bar.counter.prepare == 2);
        REQUIRE(action_bar.counter.abort == 0);
        REQUIRE(action_bar.counter.self_update == 2);
        REQUIRE(action_bar.counter.child_update == 0);
    }

    SECTION( "[F,R]" ) {
        action_foo.update_result = BH_FAILURE;
        action_bar.update_result = BH_RUNNING;
        to_vm(vm, selector.inner_node);
        vm.tick(nullptr);
        REQUIRE(selector.child_update_result == BH_RUNNING);
        REQUIRE(selector.counter.prepare == 1);
        REQUIRE(selector.counter.abort == 0);
        REQUIRE(selector.counter.self_update == 1);
        REQUIRE(selector.counter.child_update == 2);
        REQUIRE(action_foo.counter.prepare == 1);
        REQUIRE(action_foo.counter.abort == 0);
        REQUIRE(action_foo.counter.self_update == 1);
        REQUIRE(action_foo.counter.child_update == 0);
        REQUIRE(action_bar.counter.prepare == 1);
        REQUIRE(action_bar.counter.abort == 0);
        REQUIRE(action_bar.counter.self_update == 1);
        REQUIRE(action_bar.counter.child_update == 0);
        vm.tick(nullptr);
        REQUIRE(selector.child_update_result == BH_RUNNING);
        REQUIRE(selector.counter.prepare == 1);
        REQUIRE(selector.counter.abort == 0);
        REQUIRE(selector.counter.self_update == 2);
        REQUIRE(selector.counter.child_update == 4);
        REQUIRE(action_foo.counter.prepare == 2);
        REQUIRE(action_foo.counter.abort == 0);
        REQUIRE(action_foo.counter.self_update == 2);
        REQUIRE(action_foo.counter.child_update == 0);
        REQUIRE(action_bar.counter.prepare == 1);
        REQUIRE(action_bar.counter.abort == 0);
        REQUIRE(action_bar.counter.self_update == 2);
        REQUIRE(action_bar.counter.child_update == 0);
    }

    SECTION( "[F,R]->[S,R]" ) {
        action_foo.update_result = BH_FAILURE;
        action_bar.update_result = BH_RUNNING;
        to_vm(vm, selector.inner_node);
        vm.tick(nullptr);
        REQUIRE(selector.child_update_result == BH_RUNNING);
        REQUIRE(selector.counter.prepare == 1);
        REQUIRE(selector.counter.abort == 0);
        REQUIRE(selector.counter.self_update == 1);
        REQUIRE(selector.counter.child_update == 2);
        REQUIRE(action_foo.counter.prepare == 1);
        REQUIRE(action_foo.counter.abort == 0);
        REQUIRE(action_foo.counter.self_update == 1);
        REQUIRE(action_foo.counter.child_update == 0);
        REQUIRE(action_bar.counter.prepare == 1);
        REQUIRE(action_bar.counter.abort == 0);
        REQUIRE(action_bar.counter.self_update == 1);
        REQUIRE(action_bar.counter.child_update == 0);
        action_foo.update_result = BH_SUCCESS;
        vm.tick(nullptr);
        REQUIRE(selector.child_update_result == BH_SUCCESS);
        REQUIRE(selector.counter.prepare == 1);
        REQUIRE(selector.counter.abort == 0);
        REQUIRE(selector.counter.self_update == 2);
        REQUIRE(selector.counter.child_update == 3);
        REQUIRE(action_foo.counter.prepare == 2);
        REQUIRE(action_foo.counter.abort == 0);
        REQUIRE(action_foo.counter.self_update == 2);
        REQUIRE(action_foo.counter.child_update == 0);
        REQUIRE(action_bar.counter.prepare == 1);
        REQUIRE(action_bar.counter.abort == 1);
        REQUIRE(action_bar.counter.self_update == 1);
        REQUIRE(action_bar.counter.child_update == 0);
    }

    SECTION( "[R,F]->[F,F]" ) {
        action_foo.update_result = BH_RUNNING;
        action_bar.update_result = BH_FAILURE;
        to_vm(vm, selector.inner_node);
        vm.tick(nullptr);
        REQUIRE(selector.child_update_result == BH_RUNNING);
        REQUIRE(selector.counter.prepare == 1);
        REQUIRE(selector.counter.abort == 0);
        REQUIRE(selector.counter.self_update == 1);
        REQUIRE(selector.counter.child_update == 1);
        REQUIRE(action_foo.counter.prepare == 1);
        REQUIRE(action_foo.counter.abort == 0);
        REQUIRE(action_foo.counter.self_update == 1);
        REQUIRE(action_foo.counter.child_update == 0);
        REQUIRE(action_bar.counter.prepare == 0);
        REQUIRE(action_bar.counter.abort == 0);
        REQUIRE(action_bar.counter.self_update == 0);
        REQUIRE(action_bar.counter.child_update == 0);
        action_foo.update_result = BH_FAILURE;
        vm.tick(nullptr);
        REQUIRE(selector.child_update_result == BH_FAILURE);
        REQUIRE(selector.counter.prepare == 1);
        REQUIRE(selector.counter.abort == 0);
        REQUIRE(selector.counter.self_update == 2);
        REQUIRE(selector.counter.child_update == 3);
        REQUIRE(action_foo.counter.prepare == 1);
        REQUIRE(action_foo.counter.abort == 0);
        REQUIRE(action_foo.counter.self_update == 2);
        REQUIRE(action_foo.counter.child_update == 0);
        REQUIRE(action_bar.counter.prepare == 1);
        REQUIRE(action_bar.counter.abort == 0);
        REQUIRE(action_bar.counter.self_update == 1);
        REQUIRE(action_bar.counter.child_update == 0);
    }
}
