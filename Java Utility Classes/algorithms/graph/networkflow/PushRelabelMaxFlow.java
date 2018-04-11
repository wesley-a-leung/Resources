package algorithms.graph.networkflow;

import java.util.ArrayDeque;
import java.util.Queue;

import datastructures.graph.networkflow.FlowEdge;
import datastructures.graph.networkflow.FlowNetwork;

/**
 * The {@code PushRelableMaxFlow} class represents a data type for computing a
 * <em>maximum st-flow</em> in a flow network.
 * This implementation uses the FIFO selection rule to reduce the running time
 * to <em>V</em><sup>3</sup>
 *  
 * @author Wesley Leung
 */
public class PushRelabelMaxFlow {
    private static final double FLOATING_POINT_EPSILON = 1E-10;
    
    private final int V;
    private double value;
    private int[] height;
    private double[] excess;
    private boolean[] done;
    
    /**
     * Compute a maximum flow in the network {@code G}
     * from vertex {@code s} to vertex {@code t}.
     *
     * @param  G the flow network
     * @param  s the source vertex
     * @param  t the sink vertex
     * @throws IllegalArgumentException unless {@code 0 <= s < V}
     * @throws IllegalArgumentException unless {@code 0 <= t < V}
     * @throws IllegalArgumentException if {@code s == t}
     */
    public PushRelabelMaxFlow(FlowNetwork G, int s, int t) {
        V = G.V();
        validate(s);
        validate(t);
        if (s == t) throw new IllegalArgumentException("Source equals sink");
        Queue<Integer> q = new ArrayDeque<Integer>();
        height = new int[G.V()];
        height[s] = G.V();
        excess = new double[G.V()];
        done = new boolean[G.V()];
        for (FlowEdge e : G.adj(s)) {
            int v = e.other(s);
            if (v == e.to()) {
                e.addResidualFlowTo(v, e.capacity());
                excess[s] -= e.capacity();
                excess[v] += e.capacity();
            }
            if (v != t) {
                done[v] = true;
                q.offer(v);
            }
        }
        
        while (!q.isEmpty()) {
            int v = q.peek();
            int tempHeight = 2 * G.V();
            for (FlowEdge e : G.adj(v)) {
                if (Math.abs(excess[v]) <= FLOATING_POINT_EPSILON) break;
                int w = e.other(v);
                if (e.residualCapacityTo(w) > FLOATING_POINT_EPSILON) {
                    if (height[v] > height[w]) {
                        // PUSH
                        double tempFlow = Math.min(excess[v], e.residualCapacityTo(w));
                        e.addResidualFlowTo(w, tempFlow);
                        excess[v] -= tempFlow;
                        excess[w] += tempFlow;
                        // END PUSH
                        if (!done[w] && w != s && w != t) {
                            done[w] = true;
                            q.offer(w);
                        }
                    } else {
                        tempHeight = Math.min(height[w], tempHeight);
                    }
                }
            }
            if (excess[v] > FLOATING_POINT_EPSILON) height[v] = tempHeight + 1;
            else {
                done[v] = false;
                q.poll();
            }
        }
        value = excess[t];
    }
    
    /**
     * Returns the value of the maximum flow.
     *
     * @return the value of the maximum flow
     */
    public double value()  {
        return value;
    }
    
    // throw an IllegalArgumentException if v is outside prescribed range
    private void validate(int v)  {
        if (v < 0 || v >= V)
            throw new IllegalArgumentException("vertex " + v + " is not between 0 and " + (V-1));
    }
}
