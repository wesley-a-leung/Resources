package datastructures.trees;

import java.util.NoSuchElementException;

import datastructures.Queue;

public class AVLTreeSet<Value extends Comparable<Value>> {

    /**
     * The root node.
     */
    private Node root;

    /**
     * This class represents an inner node of the AVL tree.
     */
    private class Node {
        private Value val;       // the value
        private int height;      // height of the subtree
        private int size;        // number of nodes in subtree
        private Node left;       // left subtree
        private Node right;      // right subtree

        public Node(Value val, int height, int size) {
            this.val = val;
            this.size = size;
            this.height = height;
        }
    }

    /**
     * Initializes an empty symbol table.
     */
    public AVLTreeSet() {
    }

    /**
     * Checks if the symbol table is empty.
     * 
     * @return {@code true} if the symbol table is empty.
     */
    public boolean isEmpty() {
        return root == null;
    }

    /**
     * Returns the number values in the symbol table.
     * 
     * @return the number values pairs in the symbol table
     */
    public int size() {
        return size(root);
    }

    /**
     * Returns the number of nodes in the subtree.
     * 
     * @param x the subtree
     * 
     * @return the number of nodes in the subtree
     */
    private int size(Node x) {
        if (x == null) return 0;
        return x.size;
    }

    /**
     * Returns the height of the internal AVL tree. It is assumed that the
     * height of an empty tree is -1 and the height of a tree with just one node
     * is 0.
     * 
     * @return the height of the internal AVL tree
     */
    public int height() {
        return height(root);
    }

    /**
     * Returns the height of the subtree.
     * 
     * @param x the subtree
     * 
     * @return the height of the subtree.
     */
    private int height(Node x) {
        if (x == null) return -1;
        return x.height;
    }

    /**
     * Checks if the symbol table contains the given value.
     * 
     * @param val the value
     * @return {@code true} if the symbol table contains {@code val}
     *         and {@code false} otherwise
     * @throws IllegalArgumentException if {@code val} is {@code null}
     */
    public boolean contains(Value val) {
        if (val == null) throw new IllegalArgumentException("argument to delete() is null");
        return contains(root, val);
    }

    // auxiliary method for contains
    private boolean contains(Node x, Value val) {
        if (x == null) return false;
        else if (val.compareTo(x.val) < 0) return contains(x.left, val);
        else if (val.compareTo(x.val) > 0) return contains(x.right, val);
        return true;
    }

    /**
     * Inserts the specified value into the symbol table, allowing for duplicates.
     * 
     * @param val the value
     * @throws IllegalArgumentException if {@code val} is {@code null}
     */
    public void add(Value val) {
        if (val == null) throw new IllegalArgumentException("argument to delete() is null");
        root = add(root, val);
    }
    
    /**
     * Inserts the specified value into the symbol table, allowing for duplicates.
     * Deletes the specified value from this symbol table if the specified value is {@code null}.
     * 
     * @param x the subtree
     * @param val the value
     * @return the subtree
     */
    private Node add(Node x, Value val) {
        if (x == null) return new Node(val, 0, 1);
        int cmp = val.compareTo(x.val);
        if (cmp < 0) x.left = add(x.left, val);
        else x.right = add(x.right, val);
        return balance(x);
    }

    /**
     * Restores the AVL tree property of the subtree.
     * 
     * @param x the subtree
     * @return the subtree with restored AVL property
     */
    private Node balance(Node x) {
        if (balanceFactor(x) < -1) {
            if (balanceFactor(x.right) > 0) x.right = rotateRight(x.right);
            x = rotateLeft(x);
        } else if (balanceFactor(x) > 1) {
            if (balanceFactor(x.left) < 0) x.left = rotateLeft(x.left);
            x = rotateRight(x);
        }
        update(x);
        return x;
    }

    /**
     * Returns the balance factor of the subtree. The balance factor is defined
     * as the difference in height of the left subtree and right subtree, in
     * this order. Therefore, a subtree with a balance factor of -1, 0 or 1 has
     * the AVL property since the heights of the two child subtrees differ by at
     * most one.
     * 
     * @param x the subtree
     * @return the balance factor of the subtree
     */
    private int balanceFactor(Node x) {
        return height(x.left) - height(x.right);
    }

    /**
     * Rotates the given subtree to the right.
     * 
     * @param x the subtree
     * @return the right rotated subtree
     */
    private Node rotateRight(Node x) {
        Node y = x.left;
        x.left = y.right;
        y.right = x;
        update(x);
        update(y);
        return y;
    }

    /**
     * Rotates the given subtree to the left.
     * 
     * @param x the subtree
     * @return the left rotated subtree
     */
    private Node rotateLeft(Node x) {
        Node y = x.right;
        x.right = y.left;
        y.left = x;
        update(x);
        update(y);
        return y;
    }
    
    /**
     * Updates the size and height of the subtree.
     *
     * @param x the subtree
     */
    private void update(Node x) {
        x.size = 1 + size(x.left) + size(x.right);
        x.height = 1 + Math.max(height(x.left), height(x.right));
    }

    /**
     * Removes the specified value from the symbol table
     * 
     * @param val the value
     * @throws IllegalArgumentException if {@code val} is {@code null}
     */
    public void delete(Value val) {
        if (val == null) throw new IllegalArgumentException("argument to delete() is null");
        if (!contains(val)) return;
        root = delete(root, val);
    }

    /**
     * Removes the specified value and its associated value from the given
     * subtree.
     * 
     * @param x the subtree
     * @param val the value
     * @return the updated subtree
     */
    private Node delete(Node x, Value val) {
        int cmp = val.compareTo(x.val);
        if (cmp < 0) x.left = delete(x.left, val);
        else if (cmp > 0) x.right = delete(x.right, val);
        else {
            if (x.left == null) return x.right;
            else if (x.right == null) return x.left;
            else {
                Node y = x;
                x = min(y.right);
                x.right = deleteMin(y.right);
                x.left = y.left;
            }
        }
        return balance(x);
    }

    /**
     * Removes the smallest value from the symbol table.
     * 
     * @throws NoSuchElementException if the symbol table is empty
     */
    public void deleteMin() {
        if (isEmpty()) throw new NoSuchElementException("called deleteMin() with empty symbol table");
        root = deleteMin(root);
    }

    /**
     * Removes the smallest value from the given subtree.
     * 
     * @param x the subtree
     * @return the updated subtree
     */
    private Node deleteMin(Node x) {
        if (x.left == null) return x.right;
        x.left = deleteMin(x.left);
        return balance(x);
    }

    /**
     * Removes the largest value from the symbol table.
     * 
     * @throws NoSuchElementException if the symbol table is empty
     */
    public void deleteMax() {
        if (isEmpty()) throw new NoSuchElementException("called deleteMax() with empty symbol table");
        root = deleteMax(root);
    }

    /**
     * Removes the largest value from the given subtree.
     * 
     * @param x the subtree
     * @return the updated subtree
     */
    private Node deleteMax(Node x) {
        if (x.right == null) return x.left;
        x.right = deleteMax(x.right);
        return balance(x);
    }

    /**
     * Returns the smallest value in the symbol table.
     * 
     * @return the smallest value in the symbol table
     * @throws NoSuchElementException if the symbol table is empty
     */
    public Value min() {
        if (isEmpty()) throw new NoSuchElementException("called min() with empty symbol table");
        return min(root).val;
    }

    /**
     * Returns the node with the smallest value in the subtree.
     * 
     * @param x the subtree
     * @return the node with the smallest value in the subtree
     */
    private Node min(Node x) {
        if (x.left == null) return x;
        return min(x.left);
    }

    /**
     * Returns the largest value in the symbol table.
     * 
     * @return the largest value in the symbol table
     * @throws NoSuchElementException if the symbol table is empty
     */
    public Value max() {
        if (isEmpty()) throw new NoSuchElementException("called max() with empty symbol table");
        return max(root).val;
    }

    /**
     * Returns the node with the largest value in the subtree.
     * 
     * @param x the subtree
     * @return the node with the largest value in the subtree
     */
    private Node max(Node x) {
        if (x.right == null) return x;
        return max(x.right);
    }

    /**
     * Returns the largest value in the symbol table less than or equal to
     * {@code val}.
     * 
     * @param val the value
     * @return the largest value in the symbol table less than or equal to
     *         {@code val}
     * @throws NoSuchElementException if the symbol table is empty
     * @throws IllegalArgumentException if {@code val} is {@code null}
     */
    public Value floor(Value val) {
        if (val == null) throw new IllegalArgumentException("argument to floor() is null");
        if (isEmpty()) throw new NoSuchElementException("called floor() with empty symbol table");
        Node x = floor(root, val);
        if (x == null) return null;
        else return x.val;
    }

    /**
     * Returns the node in the subtree with the largest value less than or equal
     * to the given value.
     * 
     * @param x the subtree
     * @param val the value
     * @return the node in the subtree with the largest value less than or equal
     *         to the given value
     */
    private Node floor(Node x, Value val) {
        if (x == null) return null;
        int cmp = val.compareTo(x.val);
        if (cmp == 0) return x;
        if (cmp < 0) return floor(x.left, val);
        Node y = floor(x.right, val);
        if (y != null) return y;
        else return x;
    }

    /**
     * Returns the smallest value in the symbol table greater than or equal to
     * {@code val}.
     * 
     * @param val the value
     * @return the smallest value in the symbol table greater than or equal to
     *         {@code val}
     * @throws NoSuchElementException if the symbol table is empty
     * @throws IllegalArgumentException if {@code val} is {@code null}
     */
    public Value ceiling(Value val) {
        if (val == null) throw new IllegalArgumentException("argument to ceiling() is null");
        if (isEmpty()) throw new NoSuchElementException("called ceiling() with empty symbol table");
        Node x = ceiling(root, val);
        if (x == null) return null;
        else return x.val;
    }

    /**
     * Returns the node in the subtree with the smallest value greater than or
     * equal to the given value.
     * 
     * @param x the subtree
     * @param val the value
     * @return the node in the subtree with the smallest value greater than or
     *         equal to the given value
     */
    private Node ceiling(Node x, Value val) {
        if (x == null) return null;
        int cmp = val.compareTo(x.val);
        if (cmp == 0) return x;
        if (cmp > 0) return ceiling(x.right, val);
        Node y = ceiling(x.left, val);
        if (y != null) return y;
        else return x;
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

    /**
     * Returns all values in the symbol table following an in-order traversal.
     * 
     * @return all values in the symbol table following an in-order traversal
     */
    public Iterable<Value> values() {
        Queue<Value> queue = new Queue<Value>();
        valuesInOrder(root, queue);
        return queue;
    }

    /**
     * Adds the values in the subtree to queue following an in-order traversal.
     * 
     * @param x the subtree
     * @param queue the queue
     */
    private void valuesInOrder(Node x, Queue<Value> queue) {
        if (x == null) return;
        valuesInOrder(x.left, queue);
        queue.enqueue(x.val);
        valuesInOrder(x.right, queue);
    }

    /**
     * Returns all values in the symbol table following a level-order traversal.
     * 
     * @return all values in the symbol table following a level-order traversal.
     */
    public Iterable<Value> valuesLevelOrder() {
        Queue<Value> queue = new Queue<Value>();
        if (!isEmpty()) {
            Queue<Node> queue2 = new Queue<Node>();
            queue2.enqueue(root);
            while (!queue2.isEmpty()) {
                Node x = queue2.dequeue();
                queue.enqueue(x.val);
                if (x.left != null) queue2.enqueue(x.left);
                if (x.right != null) queue2.enqueue(x.right);
            }
        }
        return queue;
    }

    /**
     * Returns all values in the symbol table in the given range.
     * 
     * @param lo the lowest value
     * @param hi the highest value
     * @return all value in the symbol table between {@code lo} (inclusive)
     *         and {@code hi} (exclusive)
     * @throws IllegalArgumentException if either {@code lo} or {@code hi}
     *             is {@code null}
     */
    public Iterable<Value> values(Value lo, Value hi) {
        if (lo == null) throw new IllegalArgumentException("first argument to values() is null");
        if (hi == null) throw new IllegalArgumentException("second argument to values() is null");
        Queue<Value> queue = new Queue<Value>();
        values(root, queue, lo, hi);
        return queue;
    }

    /**
     * Adds the values between {@code lo} and {@code hi} in the subtree
     * to the {@code queue}.
     * 
     * @param x the subtree
     * @param queue the queue
     * @param lo the lowest value
     * @param hi the highest value
     */
    private void values(Node x, Queue<Value> queue, Value lo, Value hi) {
        if (x == null) return;
        int cmplo = lo.compareTo(x.val);
        int cmphi = hi.compareTo(x.val);
        if (cmplo < 0) values(x.left, queue, lo, hi);
        if (cmplo <= 0 && cmphi >= 0) queue.enqueue(x.val);
        if (cmphi > 0) values(x.right, queue, lo, hi);
    }

    /**
     * Returns the number of values in the symbol table in the given range.
     * 
     * @param lo minimum endpoint
     * @param hi maximum endpoint
     * @return the number of values in the symbol table between {@code lo}
     *         (inclusive) and {@code hi} (exclusive)
     * @throws IllegalArgumentException if either {@code lo} or {@code hi}
     *             is {@code null}
     */
    public int size(Value lo, Value hi) {
        if (lo == null) throw new IllegalArgumentException("first argument to size() is null");
        if (hi == null) throw new IllegalArgumentException("second argument to size() is null");
        if (lo.compareTo(hi) > 0) return 0;
        if (contains(hi)) return rank(hi) - rank(lo) + 1;
        else return rank(hi) - rank(lo);
    }
}
