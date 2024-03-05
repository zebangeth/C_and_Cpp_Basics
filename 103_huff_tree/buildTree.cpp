#include "node.h"

Node *  buildTree(uint64_t * counts) {
    if (!counts) {
        return NULL;
    }
    priority_queue_t pq;
    for (int i = 0; i < 257; i++) {
        if (counts[i] > 0) {
            pq.push(new Node(i, counts[i]));
        }
    }
    while (pq.size() != 1) {
        Node * l = pq.top();
        pq.pop();
        Node * r = pq.top();
        pq.pop();
        Node * parent = new Node(l, r);
        pq.push(parent);
    }
    return pq.top();
}
