
public class IMSorting
{
	int ARRAY_SIZE=21;
	int[] array = new int[ARRAY_SIZE];
	
	public IMSorting() {
		for (int i=0; i<ARRAY_SIZE; i++) {
			int j = (int) (Math.random()*100);
			array[i]=j;
		}
	}

	
	public void print (String header) {
		System.out.println(header);
		for (int i=0; i<ARRAY_SIZE; i++) {
			System.out.print(array[i]+" ");
		}
		System.out.println();
	}
	
	
	
	public void doInsertionSortWhile() {
		for (int i = 0; i < this.array.length; i++) {
			int current = this.array[i];
			int j = i;
			while(j > 0 && array[j-1]>current) {
				array[j] = array[j-1];
				j--;
			}
			array[j] = current;
		}
	}

	
	public void doInsertionSortFor2() {
		for (int i = 0; i < this.array.length; i++) {
			int current = this.array[i];
			int lastMovedIndex=-1;
			int j=i;
			for (j=i; j>0; j--) {
				if (array[j-1]>current) {
					array[j] = array[j-1];
					lastMovedIndex=(j-1);
				} else {
					break;
				}
			}
			if (lastMovedIndex>=0) {
				array[lastMovedIndex]=current;				
			}
			print("i/current/lastMovedIndex/j="+i+"/"+current+"/"+lastMovedIndex+"/"+j);
		}
	}
	
	
	public void doInsertionSortFor() {
		for (int i = 0; i < this.array.length; i++) {
			int current = this.array[i];
			int lastMovedIndex=-1;
			for (int j=i-1; j>=0; j--) {
				if (array[j]>current) {
					array[j+1] = array[j];
					lastMovedIndex=(j);
				} else {
					break;
				}
			}
			if (lastMovedIndex>=0) {
				array[lastMovedIndex]=current;				
			}
			print("i/current/lastMovedIndex/j="+i+"/"+current+"/"+lastMovedIndex);
		}
	}
	
	
	public void doMergeSort() {
		
	}
	
	
	
	public static void main(String[] args) {
		
		IMSorting imsorting = new IMSorting();
		imsorting.print("=== before ===");
		imsorting.doInsertionSortFor();
		imsorting.print("=== after insertion sort ===");
		
		System.out.println();
		imsorting = new IMSorting();
		imsorting.print("=== before ===");
		imsorting.doMergeSort();
		imsorting.print("=== after merge sort ===");
		
	}
}
