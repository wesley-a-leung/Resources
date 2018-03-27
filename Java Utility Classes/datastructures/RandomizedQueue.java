package datastructures;

import java.util.Iterator;
import java.util.NoSuchElementException;

import utils.StdRandom;

/**
 *  The {@code RandomizedQueue} class represents a queue of generic items
 *  that randomly selects items.
 *  It supports the usual <em>enqueue</em> and <em>dequeue</em>
 *  operations, along with methods for sampling a random item, 
 *  testing if the queue is empty, and iterating through
 *  the items in a random order.
 *  <p>
 *  This implementation uses a singly-linked list with a static nested class for
 *  linked-list nodes. See {@link LinkedQueue} for the version from the
 *  textbook that uses a non-static nested class.
 *
 *  @author Wesley Leung
 *
 *  @param <Item> the generic type of an item in this queue
 */
public class RandomizedQueue<Item> implements Iterable<Item> {
    private Item[] q; // queue elements
    private int n; // number of elements on queue
    private int first; // index of first element of queue
    private int last; // index of next available slot

    /**
     * Initializes an empty queue.
     */
    public RandomizedQueue() {
        q = (Item[]) new Object[2];
        n = 0;
        first = 0;
        last = 0;
    }

    /**
     * Is this queue empty?
     * 
     * @return true if this queue is empty; false otherwise
     */
    public boolean isEmpty() {
        return n == 0;
    }

    /**
     * Returns the number of items in this queue.
     * 
     * @return the number of items in this queue
     */
    public int size() {
        return n;
    }

    // resize the underlying array
    private void resize(int capacity) {
        assert capacity >= n;
        Item[] temp = (Item[]) new Object[capacity];
        for (int i = 0; i < n; i++) {
            temp[i] = q[(first + i) % q.length];
        }
        q = temp;
        first = 0;
        last = n;
    }

    /**
     * Adds the item to this queue.
     * 
     * @param item
     *            the item to add
     */
    public void enqueue(Item item) {
        // double size of array if necessary and recopy to front of array
        if (n == q.length) resize(2 * q.length); // double size of array if necessary
        q[last++] = item; // add item
        if (last == q.length) last = 0; // wrap-around
        n++;
    }

    /**
     * Removes and returns a random item on this queue.
     * 
     * @return a random item on this queue
     * @throws java.util.NoSuchElementException
     *             if this queue is empty
     */
    public Item dequeue() {
        if (isEmpty()) throw new NoSuchElementException("Queue underflow");
        int rand = StdRandom.uniform(first, first + n) % q.length;
        Item item = q[rand];
        q[rand] = q[first];
        q[first] = null; // to avoid loitering
        n--;
        first++;
        if (first == q.length) first = 0; // wrap-around
        // shrink size of array if necessary
        if (n > 0 && n == q.length / 4) resize(q.length / 2);
        return item;
    }
    
    /**
     * Returns (but does not remove) a random item from the queue.
     * 
     * @return a random item from the queue
     * @throws java.util.NoSuchElementException
     *             if this queue is empty
     */
    public Item sample() {
        if (isEmpty()) throw new NoSuchElementException("Queue underflow");
        return q[StdRandom.uniform(first, first + n) % q.length];
    }

    /**
     * Returns an iterator that iterates over the items in this queue in FIFO order.
     * 
     * @return an iterator that iterates over the items in this queue in FIFO order
     */
    public Iterator<Item> iterator() {
        return new ArrayIterator();
    }

    // an iterator, doesn't implement remove() since it's optional
    private class ArrayIterator implements Iterator<Item> {
        private int i = 0;
        private int[] shuffle = new int[n];

        public boolean hasNext() {
            return i < n;
        }

        public void remove() {
            throw new UnsupportedOperationException();
        }

        public Item next() {
            if (!hasNext()) throw new NoSuchElementException();
            if (i == 0) {
                for (int j = 0; j < n; j++) {
                    shuffle[j] = j;
                }
                for (int j = 0; j < n; j++) {
                    int rand = StdRandom.uniform(j + 1);
                    int temp = shuffle[rand];
                    shuffle[rand] = shuffle[j];
                    shuffle[j] = temp;
                }
            }
            return q[(first + shuffle[i++]) % q.length];
        }
    }
}
