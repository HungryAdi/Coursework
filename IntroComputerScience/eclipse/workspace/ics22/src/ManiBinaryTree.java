public class ManiBinaryTree<K extends Comparable<K>,V>
{
	
	private Node root;
	private int size=0;

	public int size() {
		return this.size;
	}
	
	public V get(K k) {
		if (root==null) {
			return null;
		}
		return getRecursive(root, k);
	}
	
	private V getRecursive(Node node, K k) {
		if (node==null) {
			return null;
		}
		if (node.key.equals(k)) {
			return node.value;
		} else if (node.key.compareTo(k)<1) {
			return getRecursive(node.right, k);
		} else {
			return getRecursive(node.left, k);
		}
	}
	
	public void put(K k, V v) {
		Node node = new Node(k,v);
		if (root==null) {
			root = node;
			return;
		}
		putRecursive(root, node);
		size++;
	}
	
	
	private void putRecursive(Node root, Node node) {
		if (node.key.compareTo(root.key)<1) {
			if (root.left==null) {
				root.left=node;
			} else {
				putRecursive(root.left, node);				
			}
		} else {
			if (root.right==null) {
				root.right=node;
			} else {				
				putRecursive(root.right, node);
			}
		}
	}
	
	
	public int depth () {
		if (root==null) {
			return 0;
		}
		return depthRecursive(root, 1);
	}
	
	
	private int depthRecursive(Node node, int depth) {
		if (node==null) {
			return depth;
		}
		return depthRecursive(node.right, depth+1);
	}
	
	
	
	private class Node {	
		K key;
		V value;
		
		Node left;
		Node right;
		
		public Node (K k, V v) {
			this.key=k;
			this.value=v;
		}
	}
}
