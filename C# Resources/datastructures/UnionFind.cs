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
    public class UnionFind {
        private int[] UF;
        private int count;
        
        public UnionFind(int N) {
            count = N;
            UF = new int[N];
            for (int i = 0; i < N; i++) UF[i] = -1;
        }
        
        public int find(int v) {
            return UF[v] < 0 ? v : UF[v] = find(UF[v]);
        }

        public bool connected(int v, int w) {
            return find(v) == find(w);
        }

        public int getSize(int v) {
            return -UF[find(v)];
        }
        
        public bool join(int v, int w) {
            v = find(v);
            w = find(w);
            if (v == w) return false;
            if (UF[v] > UF[w]) {
                int temp = v;
                v = w;
                w = temp;
            }
            UF[v] += UF[w];
            UF[w] = v;
            count--;
            return true;
        }

        public int getCount() {
            return count;
        }
    }
}
