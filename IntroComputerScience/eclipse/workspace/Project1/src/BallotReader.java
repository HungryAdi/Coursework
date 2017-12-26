// BallotReader.java
//
// ICS 22 / CSE 22 Fall 2012
// Project #1: Perfect Candidate
//
// BallotReader consists of one static method, readBallot(), which knows
// how to read an input file containing information about a ballot,
// construct a Ballot object, put the information from the file into the
// Ballot, and return the Ballot.

import java.io.IOException;


public class BallotReader
{
	// readBallot() takes the name of an input file and reads the
	// information out of it, creating and returning a Ballot containing
	// the same information.  If reading the file fails (e.g., the
	// file does not exist), the method throws an IOException.
	public static Ballot readBallot(String filename)
	throws IOException
	{
		// ***Replace this with the correct implementation; for now, I'm
		//    always returning a hard-coded ballot with three candidates.
		Ballot ballot = new Ballot("Mayor of Simpleton");
		ballot.addCandidate(new Candidate("Joe Incumbent", "Powerful Party"));
		ballot.addCandidate(new Candidate("Mark Challenger", "Less Powerful Party"));
		ballot.addCandidate(new Candidate("Gene Unpopular", "Nobody Party"));
		return ballot;
	}
}
