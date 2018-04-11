package datastructures.trees;

import java.util.ArrayDeque;
import java.util.Queue;

public class SplayTreeSet<Value extends Comparable<Value>> {

    private Node root;   // root of the BST

    // BST helper node data type
    private class Node {
        private Value val;
        private Node left, right;   // left and right subtrees
        private int size;

        public Node(Value val, int size) {
            this.val = val;
            this.size = size;
        }
    }
    
    public SplayTreeSet() {}

    public boolean contains(Value val) {
        if (root == null) return false;
        root = splay(root, val);
        return val.compareTo(root.val) == 0;
    }
    
    /**
     * Returns the kth smallest value in the symbol table.
     * 
     * @param k the order statistic
     * @return the kth smallest value in the symbol table
     * @throws IllegalArgumentException unless {@code k} is between 0 and
     *             {@code size() -1 }
     */
    public Value select(int k) {
        if (k < 0 || k >= size()) throw new IllegalArgumentException("k is not in range 0-" + (size() - 1));
        return select(root, k).val;
    }
    
    // auxiliary method for select
    private Node select(Node x, int k) {
        if (x == null) return null;
        int t = size(x.left);
        if (t > k) return select(x.left, k);
        else if (t < k) return select(x.right, k - t - 1);
        return x;
    }
    
    /**
     * Returns the number of values in the symbol table strictly less than
     * {@code val}.
     * 
     * @param val the value
     * @return the number of values in the symbol table strictly less than
     *         {@code val}
     * @throws IllegalArgumentException if {@code val} is {@code null}
     */
    public int rank(Value val) {
        if (val == null) throw new IllegalArgumentException("argument to rank() is null");
        root = splay(root, val);
        return rank(root, val);
    }

    /**
     * Returns the number of values in the subtree less than val.
     * 
     * @param val the value
     * @param x the subtree
     * @return the number of values in the subtree less than val
     */
    private int rank(Node x, Value val) {
        if (x == null) return 0;
        if (val.compareTo(x.val) <= 0) return rank(x.left, val);
        else return 1 + size(x.left) + rank(x.right, val);
    }  

   /***************************************************************************
    *  Splay tree insertion.
    ***************************************************************************/
    public void add(Value val) {
        // splay value to root
        if (root == null) {
            root = new Node(val, 1);
            return;
        }
        root = splay(root, val);
        int cmp = val.compareTo(root.val);
        // Insert new node at root
        if (cmp < 0) {
            Node n = new Node(val, 1);
            n.left = root.left;
            n.right = root;
            root.left = null;
            update(root);
            root = n;
        }
        // Insert new node at root
        else {
            Node n = new Node(val, 1);
            n.right = root.right;
            n.left = root;
            root.right = null;
            update(root);
            root = n;
        }
        update(root);
    }
    
   /***************************************************************************
    *  Splay tree deletion.
    ***************************************************************************/
    /* This splays the value, then does a slightly modified Hibbard deletion on
     * the root (if it is the node to be deleted; if it is not, the value was 
     * not in the tree). The modification is that rather than swapping the
     * root (call it node A) with its successor, it's successor (call it Node B)
     * is moved to the root position by splaying for the deletion value in A's 
     * right subtree. Finally, A's right child is made the new root's right 
     * child.
     */
    public void remove(Value val) {
        if (root == null) return; // empty tree
        root = splay(root, val);
        int cmp = val.compareTo(root.val);
        if (cmp == 0) {
            if (root.left == null) {
                root = root.right;
            } else {
                Node x = root.right;
                root = splay(root.left, val);
                root.right = x;
            }
            update(root);
        }
        // else: it wasn't in the tree to remove
    }
    
    /**
     * Returns an iterator that iterates over the values in order
     * 
     * @return an iterator that iterates over the values in order
     */
    public Iterable<Value> values() {
        Queue<Value> order = new ArrayDeque<Value>();
        traverse(root, order);
        return order;
    }
    
    private void traverse(Node x, Queue<Value> order) {
        if (x == null) return;
        traverse(x.left, order);
        order.offer(x.val);
        traverse(x.right, order);
    }
    
    
   /***************************************************************************
    * Splay tree function.
    * **********************************************************************/
    // splay value in the tree rooted at Node x. If a node with that value exists,
    //   it is splayed to the root of the tree. If it does not, the last node
    //   along the search path for the value is splayed to the root.
    private Node splay(Node x, Value val) {
        if (x == null) return null;
        int cmp1 = val.compareTo(x.val);
        if (cmp1 < 0) {
            // key not in tree, so we're done
            if (x.left == null) return x;
            int cmp2 = val.compareTo(x.left.val);
            if (cmp2 < 0) {
                x.left.left = splay(x.left.left, val);
                x = rotateRight(x);
            } else if (cmp2 > 0) {
                x.left.right = splay(x.left.right, val);
                if (x.left.right != null) x.left = rotateLeft(x.left);
            }
            if (x.left == null) return x;
            else return rotateRight(x);
        } else if (cmp1 > 0) { 
            // key not in tree, so we're done
            if (x.right == null) return x;
            int cmp2 = val.compareTo(x.right.val);
            if (cmp2 < 0) {
                x.right.left = splay(x.right.left, val);
                if (x.right.left != null) x.right = rotateRight(x.right);
            } else if (cmp2 > 0) {
                x.right.right = splay(x.right.right, val);
                x = rotateLeft(x);
            }
            if (x.right == null) return x;
            else return rotateLeft(x);
        } else {
            return x;
        }
    }


   /***************************************************************************
    *  Helper functions.
    ***************************************************************************/
    
    public int size() {
        return size(root);
    }
    
    private int size(Node x) {
        if (x == null) return 0;
        return x.size;
    }
    
    /**
     * Updates the size  of the subtree.
     *
     * @param x the subtree
     */
    private void update(Node x) {
        if (x == null) return;
        x.size = 1 + size(x.left) + size(x.right);
    }
    
    // right rotate
    private Node rotateRight(Node x) {
        Node y = x.left;
        x.left = y.right;
        y.right = x;
        update(x);
        update(y);
        return y;
    }

    // left rotate
    private Node rotateLeft(Node x) {
        Node y = x.right;
        x.right = y.left;
        y.left = x;
        update(x);
        update(y);
        return y;
    }
}
