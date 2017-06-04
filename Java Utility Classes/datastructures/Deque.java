package datastructures;

/******************************************************************************
 *  Compilation:  javac deque.java
 *  Execution:    java deque < input.txt
 *  Dependencies: StdIn.java StdOut.java
 *  Data files:   http://algs4.cs.princeton.edu/13stacks/tobe.txt  
 *
 *  A generic deque, implemented using a linked list.
 *
 *  % java deque < tobe.txt 
 *  to be or not to be (2 left on deque)
 *
 ******************************************************************************/

import java.util.Iterator;
import java.util.NoSuchElementException;

/**
 *  Deque is a double-ended deque supporting adding and removing at both
 *  the first and last element
 *
 *  @author Wesley Leung
 *
 *  @param <Item> the generic type of an item in this deque
 */
public class Deque<Item> implements Iterable<Item> {
    private Node<Item> first;    // beginning of deque
    private Node<Item> last;     // end of deque
    private int n;               // number of elements on deque

    // helper linked list class
    private class Node<Item> {
        private Item item;
        private Node<Item> next;
        private Node<Item> prev;
    }

    /**
     * Initializes an empty deque.
     */
    public Deque() {
        first = null;
        last  = null;
        n = 0;
    }

    /**
     * Returns true if this deque is empty.
     *
     * @return {@code true} if this deque is empty; {@code false} otherwise
     */
    public boolean isEmpty() {
        return n == 0;
    }

    /**
     * Returns the number of items in this deque.
     *
     * @return the number of items in this deque
     */
    public int size() {
        return n;
    }

    /**
     * Returns the item at the front of the deque.
     *
     * @return the item at the front of the deque
     * @throws NoSuchElementException if this deque is empty
     */
    public Item peekFirst() {
        if (isEmpty()) throw new NoSuchElementException("deque underflow");
        return first.item;
    }
    
    /**
     * Returns the item at the end of the deque.
     *
     * @return the item at the end of the deque
     * @throws NoSuchElementException if this deque is empty
     */
    public Item peekLast() {
        if (isEmpty()) throw new NoSuchElementException("deque underflow");
        return last.item;
    }
    
    /**
     * Adds the item to the front of the deque.
     *
     * @param  item the item to add
     */
    public void addFirst(Item item) {
        n++;
        if (first == null) {
        	Node<Item> newFirst = new Node<Item>();
        	newFirst.item = item;
        	first = newFirst;
        	last = newFirst;
        } else {
        	Node<Item> newFirst = new Node<Item>();
        	newFirst.item = item;
        	newFirst.next = first;
        	first.prev = newFirst;
        	first = newFirst;
        }
    }

    /**
     * Adds the item to the end of the deque.
     *
     * @param  item the item to add
     */
    public void addLast(Item item) {
    	n++;
    	if (last == null) {
    		Node<Item> newLast = new Node<Item>();
    		newLast.item = item;
    		first = newLast;
    		last = newLast;
    	} else {
    		Node<Item> newLast = new Node<Item>();
    		newLast.item = item;
    		newLast.prev = last;
    		last.next = newLast;
    		last = newLast;
    	}
    }

    /**
     * Removes and returns the item at the front of the deque
     *
     * @return the item on at the front of the deque
     * @throws NoSuchElementException if this deque is empty
     */
    public Item removeFirst() {
        if (isEmpty()) throw new NoSuchElementException("deque underflow");
        n--;
        Item item = first.item;
        first = first.next;
        if (first == null) last = null;
        else first.prev = null;
        return item;
    }
    
    /**
     * Removes and returns the item at the end of the deque
     *
     * @return the item on at the end of the deque
     * @throws NoSuchElementException if this deque is empty
     */
    public Item removeLast() {
        if (isEmpty()) throw new NoSuchElementException("deque underflow");
        n--;
        Item item = last.item;
        last = last.prev;
        if (last == null) first = null;
        else last.next = null;
        return item;
    }

    /**
     * Returns a string representation of this deque.
     *
     * @return the sequence of items from from to back
     */
    public String toString() {
        StringBuilder s = new StringBuilder();
        for (Item item : this) {
            s.append(item);
            s.append(' ');
        }
        return s.toString();
    } 

    /**
     * Returns an iterator that iterates over the items in this deque from front to back
     *
     * @return an iterator that iterates over the items in this deque from front to back
     */
    public Iterator<Item> iterator()  {
        return new ListIterator<Item>(first);  
    }

    // an iterator, doesn't implement remove() since it's optional
    private class ListIterator<Item> implements Iterator<Item> {
        private Node<Item> current;

        public ListIterator(Node<Item> first) {
            current = first;
        }

        public boolean hasNext()  { return current != null;                     }
        public void remove()      { throw new UnsupportedOperationException();  }

        public Item next() {
            if (!hasNext()) throw new NoSuchElementException();
            Item item = current.item;
            current = current.next; 
            return item;
        }
    }
}