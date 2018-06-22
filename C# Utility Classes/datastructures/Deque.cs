using System;
using System.Collections;
using System.Collections.Generic;
using System.Globalization;
using System.IO;
using System.Linq;
using System.Text;
using System.Threading;
using static System.Diagnostics.Debug;

namespace Datastructures {
    public class Deque<Item> : IEnumerable<Item> {
        private class Node<I> {
            internal I item;
            internal Node<I> next;
            internal Node<I> prev;
        }

        private Node<Item> first;
        private Node<Item> last;
        private int n;
    
        public Deque() {
            first = null;
            last = null;
            n = 0;
        }

        public bool IsEmpty() {
            return n == 0;
        }

        public int Count() {
            return n;
        }

        public Item PeekFirst() {
            if (IsEmpty()) throw new Exception("deque underflow");
            return first.item;
        }

        public Item PeekLast() {
            if (IsEmpty()) throw new Exception("deque underflow");
            return last.item;
        }

        public void AddFirst(Item item) {
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

        public void AddLast(Item item) {
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

        public Item RemoveFirst() {
            if (IsEmpty()) throw new Exception("deque underflow");
            n--;
            Item item = first.item;
            first = first.next;
            if (first == null) last = null;
            else first.prev = null;
            return item;
        }

        public Item RemoveLast() {
            if (IsEmpty()) throw new Exception("deque underflow");
            n--;
            Item item = last.item;
            last = last.prev;
            if (last == null) first = null;
            else last.next = null;
            return item;
        }

        public IEnumerator<Item> GetEnumerator() {
            for (Node<Item> cur = first; cur != null; cur = cur.next) yield return cur.item;
        }

        IEnumerator IEnumerable.GetEnumerator() {
            return GetEnumerator();
        }
    }
}
