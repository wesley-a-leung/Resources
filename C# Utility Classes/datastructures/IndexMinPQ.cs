using System;
using System.Collections.Generic;
using System.Globalization;
using System.IO;
using System.Linq;
using System.Text;
using System.Threading;
using static System.Diagnostics.Debug;

namespace Datastructures {
    public class IndexMinPQ<Key> where Key : IComparable<Key> {
        private int maxN;
        private int n;
        private int[] pq;
        private int[] qp;
        private Key[] keys;
    
        public IndexMinPQ(int maxN) {
            if (maxN < 0) throw new Exception("maxN cannot be negative");
            this.maxN = maxN;
            n = 0;
            keys = new Key[maxN];
            pq = new int[maxN + 1];
            qp = new int[maxN];
            for (int i = 0; i < maxN; i++) qp[i] = -1;
        }
    
        public bool isEmpty() {
            return n == 0;
        }
    
        public bool contains(int i) {
            if (i < 0 || i >= maxN) throw new IndexOutOfRangeException();
            return qp[i] != -1;
        }
    
        public int size() {
            return n;
        }
        
        public void insert(int i, Key key) {
            if (i < 0 || i >= maxN) throw new IndexOutOfRangeException();
            if (contains(i)) throw new Exception("index is already in the priority queue");
            n++;
            qp[i] = n;
            pq[n] = i;
            keys[i] = key;
            swim(n);
        }
    
        public int minIndex() {
            if (n == 0) throw new Exception("Priority queue underflow");
            return pq[1];
        }
    
        public Key minKey() {
            if (n == 0) throw new Exception("Priority queue underflow");
            return keys[pq[1]];
        }
    
        public int delMin() {
            if (n == 0) throw new Exception("Priority queue underflow");
            int min = pq[1];
            exch(1, n--);
            sink(1);
            qp[min] = -1;
            pq[n + 1] = -1;
            return min;
        }
    
        public Key keyOf(int i) {
            if (i < 0 || i >= maxN) throw new IndexOutOfRangeException();
            if (!contains(i)) throw new Exception("index is not in the priority queue");
            else return keys[i];
        }
    
        public void changeKey(int i, Key key) {
            if (i < 0 || i >= maxN) throw new IndexOutOfRangeException();
            if (!contains(i)) throw new Exception("index is not in the priority queue");
            Key old = keys[i];
            keys[i] = key;
            if (old.CompareTo(key) < 0) swim(qp[i]);
            else if (old.CompareTo(key) > 0) sink(qp[i]);
        }
    
        public void delete(int i) {
            if (i < 0 || i >= maxN) throw new IndexOutOfRangeException();
            if (!contains(i)) throw new Exception("index is not in the priority queue");
            int index = qp[i];
            exch(index, n--);
            swim(index);
            sink(index);
            qp[i] = -1;
        }
    
        private bool greater(int i, int j) {
            return keys[pq[i]].CompareTo(keys[pq[j]]) > 0;
        }
    
        private void exch(int i, int j) {
            int swap = pq[i];
            pq[i] = pq[j];
            pq[j] = swap;
            qp[pq[i]] = i;
            qp[pq[j]] = j;
        }
    
        private void swim(int k) {
            while (k > 1 && greater(k / 2, k)) {
                exch(k, k / 2);
                k = k / 2;
            }
        }
    
        private void sink(int k) {
            while (2 * k <= n) {
                int j = 2 * k;
                if (j < n && greater(j, j + 1)) j++;
                if (!greater(k, j)) break;
                exch(k, j);
                k = j;
            }
        }
    }
}