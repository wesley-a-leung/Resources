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
    public class WeightedUF {
        private int[] par;
        private int[] size;
        private int count;
        
        public WeightedUF(int N) {
            count = N;
            par = new int[N];
            size = new int[N];
            for (int i = 0; i < N; i++) {
                par[i] = i;
                size[i] = 1;
            }
        }
        
        public int find(int p) {
            while (p != par[p]) p = par[p] = par[par[p]];
            return p;
        }

        public bool connected(int p, int q) {
            return find(p) == find(q);
        }

        public int getSize(int p) {
            return size[find(p)];
        }
        
        public bool join(int p, int q) {
            p = find(p);
            q = find(q);
            if (p == q) return false;
            if (size[p] < size[q]) {
                par[p] = q;
                size[q] += size[p];
            } else {
                par[q] = p;
                size[p] += size[q];
            }
            count--;
            return true;
        }

        public int getCount() {
            return count;
        }
    }
}
