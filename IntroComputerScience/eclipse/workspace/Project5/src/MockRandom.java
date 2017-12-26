// MockRandom.java
//
// ICS 22 / CSE 22 Fall 2012
// Project #5: Signal to Noise
//
// This is a "mock object" that behaves sort of like a Random object from the
// Java library, except that it returns a predetermined sequence of numbers,
// rather than choosing the numbers randomly.  While this wouldn't be a good
// thing to use in our random sentence generation program, it's a great help
// in testing, because it allows us to pass a "mock" random number generator
// into the sentence generation algorithm which will ultimately give us output
// that we can predict.
//
// An example of using MockRandom follows:
//
//     ArrayList<Integer> a = new ArrayList<Integer>();
//     a.add(5);
//     a.add(7);
//     a.add(9);
//
//     // This MockRandom will generate the numbers 5, 7, 9 in sequence,
//     // repeating, forever.
//     MockRandom mr = new MockRandom(a);
//
//     // The output here will be 5, 7, 9, 5, 7, 9, 5.
//     for (int i = 0; i < 7; i++)
//         System.out.println(mr.nextInt());

import java.util.ArrayList;
import java.util.Random;


public class MockRandom extends Random
{
	private ArrayList<Integer> results;
	private int nextNum;


	public MockRandom(ArrayList<Integer> results)
	{
		this.results = results;
		nextNum = 0;
	}
	

	public int nextInt()
	{
		int nextResult = Math.abs(results.get(nextNum));
		nextNum = (nextNum + 1) % results.size();
		return nextResult;
	}
	
	
	public int nextInt(int n)
	{
		return Math.abs(nextInt()) % n;
	}
}
