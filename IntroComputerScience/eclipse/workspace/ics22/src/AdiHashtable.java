import java.util.LinkedList; 
import java.util.Iterator;

public class AdiHashtable<K, V>
{

	private int size;
	private LinkedList[] arr;
	private int ARRAY_SIZE = 100;

	public AdiHashtable() {
		this.size = 0;
		this.arr = new LinkedList[ARRAY_SIZE];
	}


	public int size () {
		return this.size;
	}

	public boolean isEmpty() {
		return (this.size==0);
	}

	//TODO: implement this
	public V get (String key) {
		int index = this.getIndex(key);
		LinkedList<HashtableNode<String,V>> list;
		list = arr[index];
		if (list==null) {
			return null;
		}
		Iterator<HashtableNode<String,V>> i = list.iterator();
		while(i.hasNext()) {
			HashtableNode<String, V> node=i.next();
			if (node.keyMatches(key)) {
				return node.value;
			}
		}
		return null;
	}

	//TODO: implement this
	public void put(String key, V value) {
		int index = this.getIndex(key);
		LinkedList<HashtableNode<String,V>> list;
		list=arr[index];
		if (list==null) {
			list = new LinkedList<HashtableNode<String,V>>();
			arr[index]=list;
		}
		Iterator<HashtableNode<String,V>> i = list.iterator();
		HashtableNode<String, V> matched=null;
		while (i.hasNext()) {
			HashtableNode<String, V> node=i.next();
			if (node.keyMatches(key)) {
				matched=node;
				matched.value=value;
				break;
			}
		}
		if(matched==null) {
			matched= new HashtableNode<String, V>(key, value);
			list.add(matched);
			this.size++;
		}
	}

	//TODO: implement this
	public V remove (String key) {
		int index = this.getIndex(key);
		LinkedList<HashtableNode<String,V>> list = arr[index];
		if (list==null) {
			return null;
		}
		Iterator<HashtableNode<String,V>> i = list.iterator();
		HashtableNode<String, V> node=null;
		while(i.hasNext()) {
			node=i.next();
			if (node.keyMatches(key)) {
				break;
			}
		}
		if (node !=null) {
			list.remove(node);
			size--;
			return node.value;
		}
		return null;
	}

	public int[] getStats () {
		int[] stats = new int[ARRAY_SIZE];
		for (int i=0; i< this.arr.length; i++) {
			stats[i]=arr[i].size();
		}
		return stats;
	}
	
	private int getIndex(String key) {
		int store = 0;
		store = (key.hashCode())%(this.ARRAY_SIZE);
		store=Math.abs(store);
		return store;
	}
	
	
	
	private class HashtableNode<String, V> {

		String key;
		V value;

		HashtableNode(String key, V value) {
			this.key=key;
			this.value=value;
		}

		public V getValue() {
			return this.value;
		}

		//TODO: Implement this
		boolean keyMatches(String key) {
			if (this.key.equals(key)) {
				return true;
			}
			return false;
		}
	}
}