#include <assert.h>

#include <cstdlib>
#include <iostream>

#include "il.hpp"

class Tester {
 public:
  // testing for default constructor is done for you
  void testDefCtor() {
    IntList il;
    assert(il.head == NULL);
    assert(il.tail == NULL);
    assert(il.getSize() == 0);
  }
  // example of another method you might want to write
  void testAddFront() {
    // WRITE ME
    IntList il;
    il.addFront(1);
    assert(il.head != NULL && il.tail != NULL);
    assert(il.head->data == 1);
    assert(il.head == il.tail);
    assert(il.getSize() == 1);

    il.addFront(2);
    assert(il.head != NULL && il.head->data == 2);
    assert(il.head->next == il.tail && il.tail->prev == il.head);
    assert(il.getSize() == 2);
  }

  void testAddBack() {
    IntList il;
    il.addBack(1);
    assert(il.head != NULL && il.tail != NULL);
    assert(il.head == il.tail);
    assert(il.tail->data == 1);
    assert(il.getSize() == 1);

    il.addBack(2);
    assert(il.tail != NULL && il.tail->data == 2);
    assert(il.head->next == il.tail && il.tail->prev == il.head);
    assert(il.getSize() == 2);
  }

  void testCopyConstructor() {
    IntList il;
    il.addFront(1);
    il.addBack(2);

    IntList ilCopy(il);
    assert(ilCopy.getSize() == 2);
    assert(ilCopy[0] == 1 && ilCopy[1] == 2);
    // Further check for deep copy if necessary
    IntList *il2 = new IntList();
    il2->addFront(1);
    il2->remove(1);
    IntList *il3 = new IntList(*il2);
    delete il2;
    assert(il3->head == NULL && il3->tail == NULL && il3->getSize() == 0);
    delete il3;
    IntList * il4 = new IntList(il);
    assert(il4->head->data == 1 && il4->tail->data == 2 && il4->getSize() == 2);
    delete il4;
    
    il.addBack(3);
    assert(il.getSize() == 3);
    assert(ilCopy.getSize() == 2); 

  }

  void testAssignmentOperator() {
    IntList il;
    il.addFront(1);
    IntList ila;
    ila = il;
    assert(ila.getSize() == 1 && ila[0] == 1);
    IntList il1, il2, il3;
    il1.addFront(1);
    il2.addBack(2);

    // Assignment to a non-empty list
    il3 = il2;
    assert(il3.getSize() == 1 && il3[0] == 2);

    // Self-assignment
    il3 = il3;
    assert(il3.getSize() == 1 && il3[0] == 2);

    // Chain Assignment
    il1 = il2 = il3;
    assert(il1.getSize() == 1 && il1[0] == 2);
    assert(il2.getSize() == 1 && il2[0] == 2);
  }

  void testRemove() {
    IntList il1;
    assert(il1.remove(1) == false);
    assert(il1.head == NULL && il1.tail == NULL);

    IntList il2;
    il2.addFront(1);
    il2.addBack(2);
    assert(il2.remove(3) == false);
    assert(il2.getSize() == 2);

    IntList il3;
    il3.addFront(1);
    il3.addBack(2);
    assert(il3.remove(1) == true);
    assert(il3.head != NULL && il3.head->data == 2);

    IntList il4;
    il4.addFront(1);
    il4.addBack(2);
    assert(il4.remove(2) == true);
    assert(il4.tail != NULL && il4.tail->data == 1);

    IntList il5;
    il5.addFront(1);
    il5.addBack(2);
    il5.addBack(3);
    assert(il5.remove(2) == true);
    assert(il5.head->next == il5.tail && il5.tail->prev == il5.head);

    IntList il6;
    il6.addFront(1);
    assert(il6.remove(1) == true);
    assert(il6.head == NULL && il6.tail == NULL);

    IntList il7;
    il7.addFront(1);
    il7.addBack(2);
    il7.addBack(3);
    assert(il7.remove(1) == true);
    assert(il7.remove(2) == true);
    assert(il7.remove(3) == true);
    assert(il7.head == NULL && il7.tail == NULL);
    IntList il8;
    il8.addFront(1);
    il8.addBack(2);
    il8.addBack(3);
    assert(il8.remove(1) == true); 
    assert(il8.head->data == 2);
    assert(il8.remove(3) == true);  
    assert(il8.tail->data == 2);

    IntList il9;
    il9.addFront(1);
    il9.addBack(2);
    assert(il9.remove(1) == true);
    assert(il9.head == il9.tail && il9.head->data == 2);

    IntList il10;
    il10.addFront(1);
    il10.addBack(2);
    il10.addBack(3);
    assert(il10.remove(2) == true);
    assert(il10.remove(1) == true);
    assert(il10.remove(3) == true);
    assert(il10.head == NULL && il10.tail == NULL);

    IntList il11;
    il11.addFront(1);
    assert(il11.remove(1) == true);
    il11.addFront(2);
    assert(il11.head->data == 2 && il11.tail->data == 2);

    IntList il12;
    for (int i = 1; i <= 5; i++) {
        il12.addBack(i);
    }
    for (int i = 1; i <= 5; i++) {
        assert(il12.remove(i) == true);
    }
    assert(il12.head == NULL && il12.tail == NULL);
    IntList il13;
    il13.addFront(1);
    il13.addBack(2);
    il13.addBack(3);
    assert(il13.remove(1) == true);
    assert(il13.head != NULL && il13.head->data == 2);
    assert(il13.head->prev == NULL); 
    assert(il13.head->next != NULL && il13.head->next->data == 3);
    assert(il13.head->next->prev == il13.head);

    IntList il14;
    il14.addFront(1);
    il14.addBack(2);
    il14.addBack(3);
    assert(il14.remove(1) == true);
    assert(il14.head != NULL && il14.head->data == 2);
    assert(il14.head->prev == NULL);
    assert(il14.head->next != NULL && il14.head->next->data == 3);
    assert(il14.head->next->prev == il14.head);
  }
};

int main(void) {
  Tester t;
  t.testDefCtor();
  t.testAddFront();
  // write calls to your other test methods here
  t.testAddBack();
  t.testCopyConstructor();
  t.testAssignmentOperator();
  t.testRemove();
  return EXIT_SUCCESS;
}
