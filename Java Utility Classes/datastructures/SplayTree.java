package datastructures;

public class SplayTree<Key extends Comparable<Key>, Value>  {

    private Node root;   // root of the BST

    // BST helper node data type
    private class Node {
        private Key key;            // key
        private Value value;        // associated data
        private Node left, right;   // left and right subtrees
        private int size;

        public Node(Key key, Value value, int size) {
            this.key   = key;
            this.value = value;
            this.size = size;
        }
    }

    public boolean contains(Key key) {
        return get(key) != null;
    }

    // return value associated with the given key
    // if no such value, return null
    public Value get(Key key) {
        root = splay(root, key);
        int cmp = key.compareTo(root.key);
        if (cmp == 0) return root.value;
        else          return null;
    }
    
    /**
     * Returns the kth smallest key in the symbol table.
     * 
     * @param k the order statistic
     * @return the kth smallest key in the symbol table
     * @throws IllegalArgumentException unless {@code k} is between 0 and
     *             {@code size() -1 }
     */
    public Key select(int k) {
        if (k < 0 || k >= size()) throw new IllegalArgumentException("k is not in range 0-" + (size() - 1));
        Node x = select(root, k);
        return x.key;
    }
    
    /**
     * Returns the node with key the kth smallest key in the subtree.
     * 
     * @param x the subtree
     * @param k the kth smallest key in the subtree
     * @return the node with key the kth smallest key in the subtree
     */
    private Node select(Node x, int k) {
        if (x == null) return null;
        int t = size(x.left);
        if (t > k) return select(x.left, k);
        else if (t < k) return select(x.right, k - t - 1);
        else return x;
    }

    /**
     * Returns the number of keys in the symbol table strictly less than
     * {@code key}.
     * 
     * @param key the key
     * @return the number of keys in the symbol table strictly less than
     *         {@code key}
     * @throws IllegalArgumentException if {@code key} is {@code null}
     */
    public int rank(Key key) {
        if (key == null) throw new IllegalArgumentException("argument to rank() is null");
        root = splay(root, key);
        return rank(root, key);
    }

    /**
     * Returns the number of keys in the subtree less than key.
     * 
     * @param key the key
     * @param x the subtree
     * @return the number of keys in the subtree less than key
     */
    private int rank(Node x, Key key) {
        if (x == null) return 0;
        if (key.compareTo(x.key) <= 0) return rank(x.left, key);
        else return 1 + size(x.left) + rank(x.right, key);
    }

   /***************************************************************************
    *  Splay tree insertion.
    ***************************************************************************/
    public void put(Key key, Value value) {
        // splay key to root
        if (root == null) {
            root = new Node(key, value, 1);
            return;
        }
        root = splay(root, key);
        int cmp = key.compareTo(root.key);
        // Insert new node at root
        if (cmp < 0) {
            Node n = new Node(key, value, 1);
            n.left = root.left;
            n.right = root;
            root.left = null;
            update(root);
            root = n;
        }
        // Insert new node at root
        else if (cmp > 0) {
            Node n = new Node(key, value, 1);
            n.right = root.right;
            n.left = root;
            root.right = null;
            update(root);
            root = n;
        }
        // It was a duplicate key. Simply replace the value
        else {
            root.value = value;
        }
        update(root);
    }
    
   /***************************************************************************
    *  Splay tree deletion.
    ***************************************************************************/
    /* This splays the key, then does a slightly modified Hibbard deletion on
     * the root (if it is the node to be deleted; if it is not, the key was 
     * not in the tree). The modification is that rather than swapping the
     * root (call it node A) with its successor, it's successor (call it Node B)
     * is moved to the root position by splaying for the deletion key in A's 
     * right subtree. Finally, A's right child is made the new root's right 
     * child.
     */
    public void remove(Key key) {
        if (root == null) return; // empty tree
        root = splay(root, key);
        int cmp = key.compareTo(root.key);
        if (cmp == 0) {
            if (root.left == null) {
                root = root.right;
            } else {
                Node x = root.right;
                root = root.left;
                splay(root, key);
                root.right = x;
            }
            update(root);
        }

        // else: it wasn't in the tree to remove
    }
    
    /**
     * Returns an iterator that iterates over the keys in order
     * 
     * @return an iterator that iterates over the keys in order
     */
    public Iterable<Key> keys() {
        Queue<Key> order = new Queue<Key>();
        traverse(root, order);
        return order;
    }
    
    private void traverse(Node x, Queue<Key> order) {
        if (x == null) return;
        traverse(x.left, order);
        order.enqueue(x.key);
        traverse(x.right, order);
    }
    
    
   /***************************************************************************
    * Splay tree function.
    * **********************************************************************/
    // splay key in the tree rooted at Node h. If a node with that key exists,
    //   it is splayed to the root of the tree. If it does not, the last node
    //   along the search path for the key is splayed to the root.
    private Node splay(Node h, Key key) {
        if (h == null) return null;
        int cmp1 = key.compareTo(h.key);
        if (cmp1 < 0) {
            // key not in tree, so we're done
            if (h.left == null)  return h;
            int cmp2 = key.compareTo(h.left.key);
            if (cmp2 < 0) {
                h.left.left = splay(h.left.left, key);
                h = rotateRight(h);
            } else if (cmp2 > 0) {
                h.left.right = splay(h.left.right, key);
                if (h.left.right != null) h.left = rotateLeft(h.left);
            }
            if (h.left == null) return h;
            else return rotateRight(h);
        } else if (cmp1 > 0) { 
            // key not in tree, so we're done
            if (h.right == null) return h;
            int cmp2 = key.compareTo(h.right.key);
            if (cmp2 < 0) {
                h.right.left  = splay(h.right.left, key);
                if (h.right.left != null) h.right = rotateRight(h.right);
            } else if (cmp2 > 0) {
                h.right.right = splay(h.right.right, key);
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
