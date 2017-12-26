
public class AdiLinkedList <E>
{

	private Node<E> first = null;
	private int size = 0;
	
	public AdiLinkedList ()
	{
	}
	
	public void addFirst (E e) {
		Node<E> node = new Node<E>(e);
		if (this.first==null) {
			first=node;
		} else {
			node.next=this.first;
			this.first=node;
		}
		size++;
	}
	
	
	public E removeFirst ( ) {
		if (this.size==0) {
			return null;
		} else if (this.size==1) {
			E e = first.element;
			first = null;
			size--;
			return e;
		} else {
			E e = first.element;
			first = first.next;
			size--;
			return e;
		}
	}
	
	
	
	public int size() {
		return this.size;
	}
	
	public boolean isEmpty() {
		return (this.size==0);
	}
	
	
	class Node <E> {
		E element = null;
		Node<E> next = null;
		
		Node(E e) {
			this.element=e;
		}
	}
	
}
