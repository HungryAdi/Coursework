
public class TestAdiHT
{
	
	public static void main(String[] args) {
		
		AdiHashtable<String, Integer>hashtable = new AdiHashtable<String, Integer>();
		// put a bunch of integers into the hashtable
		System.out.println("putting 10K intergers into HT");
		for (int i=0; i<10000; i++) {
			Integer int1 = new Integer(i);
			hashtable.put("myinteger-"+i, int1);	
		}
		System.out.println("HT size is "+hashtable.size());
		int[] stats=hashtable.getStats();
		for (int i=0; i< stats.length; i++) {
			System.out.println(i+"="+stats[i]);
		}

		
		System.out.println("getting integer for key "+"myinteger-1234");
		Integer int2 = hashtable.get("myinteger-1234");
		System.out.println("got integer "+int2);
		
		System.out.println("removing integer for key "+"myinteger-500");
		int2 = hashtable.remove("myinteger-500");
		System.out.println("removed integer "+int2);
		System.out.println("HT size is "+hashtable.size());

		
		System.out.println("check if it is still there .. ");
		int2 = hashtable.get("myinteger-500");
		System.out.println("got integer "+int2);
		
		
	}

}
