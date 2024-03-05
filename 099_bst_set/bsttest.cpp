#include "bstset.h"
#include <iostream>
#include <cassert>

void testAddAndContains() {
    BstSet<int> set;
    set.add(1);
    set.add(2);
    set.add(3);

    assert(set.contains(1));
    assert(set.contains(2));
    assert(set.contains(3));
    assert(!set.contains(4));

    std::cout << "Test Add and Contains Passed." << std::endl;
}

void testDuplicateAdd() {
    BstSet<int> set;
    set.add(1);
    set.add(1);

    assert(set.contains(1));

    std::cout << "Test Duplicate Add Passed." << std::endl;
}

void testRemove() {
    BstSet<int> set;
    set.add(1);
    set.add(2);
    set.add(3);

    set.remove(2);
    assert(!set.contains(2));

    std::cout << "Test Remove Passed." << std::endl;
}

void testRemoveNonExistent() {
    BstSet<int> set;
    set.add(1);
    set.add(2);

    set.remove(3);

    std::cout << "Test Remove Non-existent Passed." << std::endl;
}

int main() {
    testAddAndContains();
    testDuplicateAdd();
    testRemove();
    testRemoveNonExistent();

    std::cout << "All tests passed." << std::endl;
    return 0;
}
