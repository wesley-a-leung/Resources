package algorithms.graph.networkflow;

import java.util.ArrayDeque;
import java.util.Queue;

import datastructures.graph.networkflow.FlowEdge;
import datastructures.graph.networkflow.FlowNetwork;

/**
 * The {@code DinicMaxFlow} class represents a data type for computing a
 * <em>maximum st-flow</em> in a flow network.
 * This implementation uses the concepts of level graph and flow blocking.
 * The constructor takes time proportional to <em>V</em><sup>2</sup><em>E</em>
 * in the worst case and extra space (not including the network)
 * proportional to <em>V</em><sup>2</sup><em>E</em>, where <em>V</em> is the number of vertices
 * and <em>E</em> is the number of edges.
 * Afterwards, the {@code inCut()} and {@code value()} methods take
 * constant time.
 * <p>
 * If the capacities and initial flow values are all integers, then this
 * implementation guarantees to compute an integer-valued maximum flow.
 * If the capacities and floating-point numbers, then floating-point
 * roundoff error can accumulate.
 *  
 * @author Wesley Leung
 */
public class DinicMaxFlow {
    private final int V;
    private double value;
    private int[] level;
    private int[] start;
    
    /**
     * Compute a maximum flow and minimum cut in the network {@code G}
     * from vertex {@code s} to vertex {@code t}.
     *
     * @param  G the flow network
     * @param  s the source vertex
     * @param  t the sink vertex
     * @throws IllegalArgumentException unless {@code 0 <= s < V}
     * @throws IllegalArgumentException unless {@code 0 <= t < V}
     * @throws IllegalArgumentException if {@code s == t}
     */
    public DinicMaxFlow(FlowNetwork G, int s, int t) {
        V = G.V();
        validate(s);
        validate(t);
        if (s == t) throw new IllegalArgumentException("Source equals sink");
        level = new int[V];
        start = new int[V];
        while (hasAugmentingPath(G, s, t)) {
            for (int v = 0; v < V; v++) {
                start[v] = 0;
            }
            double flow;
            while ((flow = sendFlow(G, s, t, start, Double.POSITIVE_INFINITY)) > 0) {
                value += flow;
            }
        }
    }
    
    /**
     * Returns the value of the maximum flow.
     *
     * @return the value of the maximum flow
     */
    public double value()  {
        return value;
    }

    /**
     * Returns true if the specified vertex is on the {@code s} side of the mincut.
     *
     * @param  v vertex
     * @return {@code true} if vertex {@code v} is on the {@code s} side of the micut;
     *         {@code false} otherwise
     * @throws IllegalArgumentException unless {@code 0 <= v < V}
     */
    public boolean inCut(int v)  {
        validate(v);
        return level[v] != -1;
    }
    
    // throw an IllegalArgumentException if v is outside prescribed range
    private void validate(int v)  {
        if (v < 0 || v >= V)
            throw new IllegalArgumentException("vertex " + v + " is not between 0 and " + (V-1));
    }
    
    // is there an augmenting path? 
    // this implementation finds a shortest augmenting path (fewest number of edges)
    private boolean hasAugmentingPath(FlowNetwork G, int s, int t) {
        for (int v = 0; v < V; v++) {
            level[v] = -1;
        }
        level[s] = 0;
        Queue<Integer> queue = new ArrayDeque<Integer>();
        queue.offer(s);
        while (!queue.isEmpty()) {
            int v = queue.poll();
            for (FlowEdge e : G.adj(v)) {
                int w = e.other(v);
                if (level[w] < 0 && e.residualCapacityTo(w) > 0) {
                    level[w] = level[v] + 1;
                    queue.offer(w);
                }
            }
        }
        return level[t] != -1;
    }
    
    // v : current vertex
    // t : sink
    // start : start[v] stores the number of edges that have been explored from v
    // flow : current flow sent from parent
    private double sendFlow(FlowNetwork G, int v, int t, int[] start, double flow) {
        if (v == t) return flow;
        for ( ; start[v] < G.adj(v).size(); start[v]++) {
            FlowEdge e = G.adj(v).get(start[v]);
            int w = e.other(v);
            if (level[w] == level[v] + 1 && e.residualCapacityTo(w) > 0) {
                double curFlow = Math.min(flow, e.residualCapacityTo(w));
                double tempFlow = sendFlow(G, w, t, start, curFlow);
                if (tempFlow > 0) {
                    e.addResidualFlowTo(w, tempFlow);
                    return tempFlow;
                }
            }
        }
        return 0.0;
    }
}
