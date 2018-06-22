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
    public class UF {
        private int[] par;
        private byte[] rank;
        private int count;
        
        public UF(int N) {
            count = N;
            par = new int[N];
            rank = new byte[N];
            for (int i = 0; i < N; i++) {
                par[i] = i;
                rank[i] = 0;
            }
        }
        
        public int find(int p) {
            while (p != par[p]) p = par[p] = par[par[p]];
            return p;
        }

        public bool connected(int p, int q) {
            return find(p) == find(q);
        }

        public byte getRank(int p) {
            return rank[find(p)];
        }
        
        public bool join(int p, int q) {
            p = find(p);
            q = find(q);
            if (p == q) return false;
            if (rank[p] < rank[q]) par[p] = q;
            else if (rank[p] > rank[q]) par[q] = p;
            else {
                par[q] = p;
                rank[p]++;
            }
            count--;
            return true;
        }

        public int getCount() {
            return count;
        }
    }
}
