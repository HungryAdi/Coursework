
public class TestAdiBT
{
	
	public static void main(String[] args) {
		
		AdiBinaryTree<Integer, Integer>tree = new AdiBinaryTree<Integer, Integer>();
		// put a bunch of integers into the hashtable
		System.out.println("putting 10K intergers into BTree");
		for (int i=0; i<10000; i++) {
			Integer int1 = new Integer(i);
			tree.put(int1, int1);	
		}
		System.out.println("BT size is "+tree.size());
		System.out.println("BT depth is "+tree.depth());

		
//		int[] stats=tree.getStats();
//		for (int i=0; i< stats.length; i++) {
//			System.out.println(i+"="+stats[i]);
//		}

		
		System.out.println("getting integer for key "+"myinteger-1234");
		Integer int2 = tree.get(1234);
		System.out.println("got integer "+int2);
		
//		System.out.println("removing integer for key "+"myinteger-500");
//		int2 = tree.remove("myinteger-500");
//		System.out.println("removed integer "+int2);
//		System.out.println("HT size is "+hashtable.size());

		
		System.out.println("check if it is still there .. ");
		int2 = tree.get(500);
		System.out.println("got integer "+int2);
		
		
	}

}

