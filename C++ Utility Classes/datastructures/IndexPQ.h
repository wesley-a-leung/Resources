#ifndef DATASTRUCTURES_INDEXPQ_H_
#define DATASTRUCTURES_INDEXPQ_H_

#include <bits/stdc++.h>

using namespace std;

class no_such_element_exception: public runtime_error {
public:
    no_such_element_exception(): runtime_error("No such element exists"){}
    no_such_element_exception(string message): runtime_error(message){}
};

class illegal_argument_exception: public runtime_error {
public:
    illegal_argument_exception(): runtime_error("No such element exists"){}
    illegal_argument_exception(string message): runtime_error(message){}
};

/**
 *  The {@code IndexPQ} class represents an indexed priority queue of
 *  generic keys.
 *  It supports the usual <em>insert</em> and <em>pop</em>
 *  operations, along with <em>remove</em> and <em>change-the-key</em>
 *  methods. In order to let the client refer to items on the priority queue,
 *  an index of any type is associated with each key.
 *  The client uses this type to specify which key to delete or change.
 *  It also supports methods for peeking at the top key, testing if the
 *  priority queue is empty, and iterating through the keys.
 *  <p>
 *  This implementation uses a binary heap along with an array to associate
 *  keys with integers in the given range.
 *  The <em>insert</em>, <em>pop</em>, <em>remove</em>,
 *  <em>change-key</em>, <em>decrease-key</em>, and <em>increase-key</em>
 *  operations take logarithmic time.
 *  The <em>is-empty</em>, <em>size</em>, <em>top</em>,
 *  and <em>key-of</em> operations take constant time.
 *  Construction takes time proportional to the specified capacity.
 */
template <typename Index, typename Key, typename Comparator = less<Key>>
struct IndexPQ {
private:
    Comparator cmp;

    const int INIT_CAPACITY = 3;            // default capacity
    int n;                                  // number of elements on PQ
    int curCapacity;                        // current capacity of the priority queue
    Index *pq;                              // binary heap using 1-based indexing
    unordered_map<Index, int> qp;           // inverse of pq; qp[i] = heap location of the key at index i; qp[pq[i]] = pq[qp[i]] = i
    unordered_map<Index, Key> keys;         // keys[i] = priority of index i

    // helper function to double the size of the heap array
    void resize(int capacity) {
        // assert capacity > n;
        Index *newpq = new Index[capacity + 1];
        for (int i = 1; i <= n; i++) {
            newpq[i] = pq[i];
        }
        swap(newpq, pq);
        delete[](newpq);
        curCapacity = capacity;
    }

    void exch(int i, int j) {
        swap(pq[i], pq[j]);
        qp[pq[i]] = i;
        qp[pq[j]] = j;
    }

    void swim(int k) {
        while (k > 1 && cmp(keys[pq[k/2]], keys[pq[k]])) {
            exch(k, k/2);
            k = k/2;
        }
    }

    void sink(int k) {
        while (2*k <= n) {
            int j = 2*k;
            if (j < n && cmp(keys[pq[j]], keys[pq[j+1]])) j++;
            if (!cmp(keys[pq[k]], keys[pq[j]])) break;
            exch(k, j);
            k = j;
        }
    }

public:
    /**
     * Initializes an empty indexed priority queue.
     */
    IndexPQ() {
        n = 0;
        curCapacity = INIT_CAPACITY;
        pq = new Index[INIT_CAPACITY + 1];
    }

    /**
     * Initializes an empty indexed priority queue with an initial capacity.
     *
     * @param N the initial capacity of the priority queue
     */
    IndexPQ(int N) {
        n = 0;
        curCapacity = N;
        pq = new Index[N + 1];
    }

    /**
     * Returns true if this priority queue is empty.
     *
     * @return {@code true} if this priority queue is empty;
     *         {@code false} otherwise
     */
    bool isEmpty() {
        return n == 0;
    }

    /**
     * Is {@code i} an index on this priority queue?
     *
     * @param  i an index
     * @return {@code true} if {@code i} is an index on this priority queue;
     *         {@code false} otherwise
     */
    bool contains(Index i) {
        return qp.count(i);
    }

    /**
     * Returns the number of keys on this priority queue.
     *
     * @return the number of keys on this priority queue
     */
    int size() {
        return n;
    }

    /**
      * Associate key with index i.
      *
      * @param  i an index
      * @param  key the key to associate with index {@code i}
      * @throws illegal_argument_exception if there already is an item
      *         associated with index {@code i}
      */
     void insert(Index i, Key key) {
         if (contains(i)) throw illegal_argument_exception("index is already in the priority queue");
         if (n >= curCapacity) resize(2 * curCapacity + 1);
         n++;
         qp[i] = n;
         pq[n] = i;
         keys[i] = key;
         swim(n);
     }

     /**
      * Returns a pair containing the index and key at the top of the heap.
      *
      * @return a pair containing the index and key at the top of the heap
      * @throws no_such_element_exception if this priority queue is empty
      */
     pair<Index, Key> top() {
         if (n == 0) throw no_such_element_exception("Priority queue underflow");
         return make_pair(pq[1], keys[pq[1]]);
     }

     /**
      * Removes the top key and returns a pair containing its associated index
      * and the key removed.
      *
      * @return a pair containing the index and key at the top of the heap
      * @throws no_such_element_exception if this priority queue is empty
      */
     pair<Index, Key> pop() {
         if (n == 0) throw no_such_element_exception("Priority queue underflow");
         Index minInd = pq[1];
         Key minKey = keys[minInd];
         exch(1, n--);
         sink(1);
         // assert pq[n+1] == min;
         qp.erase(minInd);        // delete
         keys.erase(minInd);
         pq[n+1] = -1;        // not needed
         if ((n > 0) && (n == (curCapacity - 1) / 4)) resize((curCapacity + 1) / 2);
         return make_pair(minInd, minKey);
     }

     /**
      * Returns the key associated with index {@code i}.
      *
      * @param  i the index of the key to return
      * @return the key associated with index {@code i}
      * @throws no_such_element_exception no key is associated with index {@code i}
      */
     Key keyOf(Index i) {
         if (!contains(i)) throw no_such_element_exception("index is not in the priority queue");
         else return keys[i];
     }

     /**
      * Change the key associated with index {@code i} to the specified value.
      *
      * @param  i the index of the key to change
      * @param  key change the key associated with index {@code i} to this key
      * @throws no_such_element_exception no key is associated with index {@code i}
      */
     void changeKey(Index i, Key key) {
         if (!contains(i)) throw no_such_element_exception("index is not in the priority queue");
         Key old = keys[i];
         keys[i] = key;
         if (cmp(old, key)) swim(qp[i]);
         else if (cmp(key, old)) sink(qp[i]);
     }

     /**
      * Removes the key on the priority queue associated with index {@code i}.
      *
      * @param  i the index of the key to remove
      * @throws no_such_element_exception no key is associated with index {@code i}
      */
     void remove(Index i) {
         if (!contains(i)) throw no_such_element_exception("index is not in the priority queue");
         int index = qp[i];
         exch(index, n--);
         swim(index);
         sink(index);
         qp.erase(i);
         keys.erase(i);
         if ((n > 0) && (n == (curCapacity - 1) / 4)) resize((curCapacity + 1) / 2);
     }
};

#endif /* DATASTRUCTURES_INDEXPQ_H_ */
