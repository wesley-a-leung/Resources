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
    public class IndexMaxPQ<Key> where Key : IComparable<Key> {
        private int maxN;
        private int n;
        private int[] pq;
        private int[] qp;
        private Key[] keys;
    
        public IndexMaxPQ(int maxN) {
            if (maxN < 0) throw new Exception("maxN cannot be negative");
            this.maxN = maxN;
            n = 0;
            keys = new Key[maxN];
            pq = new int[maxN + 1];
            qp = new int[maxN];
            for (int i = 0; i < maxN; i++) qp[i] = -1;
        }
    
        public bool IsEmpty() {
            return n == 0;
        }
    
        public bool Contains(int i) {
            if (i < 0 || i >= maxN) throw new IndexOutOfRangeException();
            return qp[i] != -1;
        }
    
        public int Count() {
            return n;
        }
        
        public void Insert(int i, Key key) {
            if (i < 0 || i >= maxN) throw new IndexOutOfRangeException();
            if (Contains(i)) throw new Exception("index is already in the priority queue");
            n++;
            qp[i] = n;
            pq[n] = i;
            keys[i] = key;
            Swim(n);
        }
    
        public int MaxIndex() {
            if (n == 0) throw new Exception("Priority queue underflow");
            return pq[1];
        }
    
        public Key MaxKey() {
            if (n == 0) throw new Exception("Priority queue underflow");
            return keys[pq[1]];
        }
    
        public int DeleteMax() {
            if (n == 0) throw new Exception("Priority queue underflow");
            int max = pq[1];
            Exch(1, n--);
            Sink(1);
            qp[max] = -1;
            pq[n + 1] = -1;
            return max;
        }
    
        public Key KeyOf(int i) {
            if (i < 0 || i >= maxN) throw new IndexOutOfRangeException();
            if (!Contains(i)) throw new Exception("index is not in the priority queue");
            else return keys[i];
        }
    
        public void ChangeKey(int i, Key key) {
            if (i < 0 || i >= maxN) throw new IndexOutOfRangeException();
            if (!Contains(i)) throw new Exception("index is not in the priority queue");
            Key old = keys[i];
            keys[i] = key;
            if (old.CompareTo(key) > 0) Swim(qp[i]);
            else if (old.CompareTo(key) < 0) Sink(qp[i]);
        }
    
        public void Delete(int i) {
            if (i < 0 || i >= maxN) throw new IndexOutOfRangeException();
            if (!Contains(i)) throw new Exception("index is not in the priority queue");
            int index = qp[i];
            Exch(index, n--);
            Swim(index);
            Sink(index);
            qp[i] = -1;
        }
    
        private bool Less(int i, int j) {
            return keys[pq[i]].CompareTo(keys[pq[j]]) < 0;
        }
    
        private void Exch(int i, int j) {
            int swap = pq[i];
            pq[i] = pq[j];
            pq[j] = swap;
            qp[pq[i]] = i;
            qp[pq[j]] = j;
        }
    
        private void Swim(int k) {
            while (k > 1 && Less(k / 2, k)) {
                Exch(k, k / 2);
                k = k / 2;
            }
        }
    
        private void Sink(int k) {
            while (2 * k <= n) {
                int j = 2 * k;
                if (j < n && Less(j, j + 1)) j++;
                if (!Less(k, j)) break;
                Exch(k, j);
                k = j;
            }
        }
    }
}
