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
    public class MinPQ<Key> {
        private int n;
        private Key[] pq;
        private IComparer<Key> cmp;
    
        public MinPQ(int initCapacity, IComparer<Key> cmp = null) {
            this.cmp = cmp;
            n = 0;
            pq = new Key[initCapacity + 1];
        }
        
        public MinPQ(IComparer<Key> cmp = null) : this(1, cmp) {}
    
        public bool IsEmpty() {
            return n == 0;
        }
    
        public int Count() {
            return n;
        }
        
        private void Resize(int capacity) {
            Key[] temp = new Key[capacity];
            for (int i = 1; i <= n; i++) temp[i] = pq[i];
            pq = temp;
        }
        
        public void Insert(Key x) {
            if (n == pq.Length - 1) Resize(2 * pq.Length);
            pq[++n] = x;
            Swim(n);
        }
    
        public Key Min() {
            if (n == 0) throw new Exception("Priority queue underflow");
            return pq[1];
        }
    
        public Key DeleteMin() {
            if (n == 0) throw new Exception("Priority queue underflow");
            Key min = pq[1];
            Exch(1, n--);
            Sink(1);
            if (n > 0 && n == (pq.Length - 1) / 4) Resize(pq.Length / 2);
            return min;
        }
        
        private bool Greater(Key a, Key b) {
            if (cmp == null) return ((IComparable<Key>) a).CompareTo(b) > 0;
            return cmp.Compare(a, b) > 0;
        }
    
        private void Exch(int i, int j) {
            Key swap = pq[i];
            pq[i] = pq[j];
            pq[j] = swap;
        }
    
        private void Swim(int k) {
            while (k > 1 && Greater(pq[k / 2], pq[k])) {
                Exch(k, k / 2);
                k = k / 2;
            }
        }
    
        private void Sink(int k) {
            while (2 * k <= n) {
                int j = 2 * k;
                if (j < n && Greater(pq[j], pq[j + 1])) j++;
                if (!Greater(pq[k], pq[j])) break;
                Exch(k, j);
                k = j;
            }
        }
    }
}
