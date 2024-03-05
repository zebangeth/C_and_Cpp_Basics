#include "bstmap.h"
#include <iostream>
#include <cassert>

void testAddAndLookup() {
    BstMap<int, int> map;
    map.add(1, 100);
    map.add(2, 200);
    map.add(3, 300);

    assert(map.lookup(1) == 100);
    assert(map.lookup(2) == 200);
    assert(map.lookup(3) == 300);

    std::cout << "Test Add and Lookup Passed." << std::endl;
}

void testReplaceValue() {
    BstMap<int, int> map;
    map.add(1, 100);
    map.add(1, 101);

    assert(map.lookup(1) == 101);

    std::cout << "Test Replace Value Passed." << std::endl;
}

void testRemove() {
    BstMap<int, int> map;
    map.add(1, 100);
    map.add(2, 200);
    map.add(3, 300);

    map.remove(2);
    try {
        map.lookup(2);
        assert(false); 
    } catch (const std::invalid_argument&) {
        
    }

    std::cout << "Test Remove Passed." << std::endl;
}

void testCopyConstructor() {
    BstMap<int, int> map;
    map.add(1, 100);
    map.add(2, 200);

    BstMap<int, int> mapCopy = map;
    assert(mapCopy.lookup(1) == 100);
    assert(mapCopy.lookup(2) == 200);

    std::cout << "Test Copy Constructor Passed." << std::endl;
}

void testAssignmentOperator() {
    BstMap<int, int> map;
    map.add(1, 100);
    map.add(2, 200);

    BstMap<int, int> mapAssign;
    mapAssign = map;
    assert(mapAssign.lookup(1) == 100);
    assert(mapAssign.lookup(2) == 200);

    std::cout << "Test Assignment Operator Passed." << std::endl;
}

int main() {
    testAddAndLookup();
    testReplaceValue();
    testRemove();
    testCopyConstructor();
    testAssignmentOperator();

    std::cout << "All tests passed." << std::endl;
    return 0;
}
