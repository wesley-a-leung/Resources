package datastructures;

public class SplayTreeSet<Value extends Comparable<Value>>  {

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
        Queue<Value> order = new Queue<Value>();
        traverse(root, order);
        return order;
    }
    
    private void traverse(Node x, Queue<Value> order) {
        if (x == null) return;
        traverse(x.left, order);
        order.enqueue(x.val);
        traverse(x.right, order);
    }
    
    
   /***************************************************************************
    * Splay tree function.
    * **********************************************************************/
    // splay value in the tree rooted at Node h. If a node with that value exists,
    //   it is splayed to the root of the tree. If it does not, the last node
    //   along the search path for the value is splayed to the root.
    private Node splay(Node h, Value val) {
        if (h == null) return null;
        int cmp1 = val.compareTo(h.val);
        if (cmp1 < 0) {
            // key not in tree, so we're done
            if (h.left == null)  return h;
            int cmp2 = val.compareTo(h.left.val);
            if (cmp2 < 0) {
                h.left.left = splay(h.left.left, val);
                h = rotateRight(h);
            } else if (cmp2 > 0) {
                h.left.right = splay(h.left.right, val);
                if (h.left.right != null) h.left = rotateLeft(h.left);
            }
            if (h.left == null) return h;
            else return rotateRight(h);
        } else if (cmp1 > 0) { 
            // key not in tree, so we're done
            if (h.right == null) return h;
            int cmp2 = val.compareTo(h.right.val);
            if (cmp2 < 0) {
                h.right.left = splay(h.right.left, val);
                if (h.right.left != null) h.right = rotateRight(h.right);
            } else if (cmp2 > 0) {
                h.right.right = splay(h.right.right, val);
                h = rotateLeft(h);
            }
            if (h.right == null) return h;
            else return rotateLeft(h);
        } else {
            return h;
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
    private Node rotateRight(Node h) {
        Node x = h.left;
        h.left = x.right;
        x.right = h;
        update(h);
        update(x);
        return x;
    }

    // left rotate
    private Node rotateLeft(Node h) {
        Node x = h.right;
        h.right = x.left;
        x.left = h;
        update(h);
        update(x);
        return x;
    }
}
