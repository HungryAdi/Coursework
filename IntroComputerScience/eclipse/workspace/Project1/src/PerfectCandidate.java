// PerfectCandidate.java
//
// ICS 22 / CSE 22 Fall 2012
// Project #1: Perfect Candidate
//
// YOU ARE NOT REQUIRED TO READ OR UNDERSTAND THIS CODE; YOU MAY NOT
// MODIFY IT.

import java.io.IOException;
import javax.swing.JOptionPane;


public class PerfectCandidate
{
	public static void main(String[] args)
	{
		Ballot ballot = null;

		do
		{
			FileSelector inputSelector = new FileSelector(
				null, "Select Input File", "Path to input file", true);
			
			inputSelector.setVisible(true);
	
			String inputFilename = inputSelector.getSelectedFile();
	
			if (inputFilename == null)
			{
				return;
			}
	
			try
			{
				ballot = BallotReader.readBallot(inputFilename);
			}
			catch (IOException e)
			{
				JOptionPane.showMessageDialog(null,
					"Could not open and read input file",
					"Input Error",
					JOptionPane.ERROR_MESSAGE);
			}
		}
		while (ballot == null);

		BallotDialog bf = new BallotDialog(null, ballot);
		bf.setVisible(true);

		FileSelector outputSelector = new FileSelector(
			null, "Select Output File", "Path to output file", false);
			
		outputSelector.setVisible(true);
	
		String outputFilename = outputSelector.getSelectedFile();

		if (outputFilename == null)
		{
			return;
		}

		try
		{
			ResultWriter.writeResults(outputFilename, ballot);
		}
		catch (IOException e)
		{
			JOptionPane.showMessageDialog(null,
				"Could not write results to output file",
				"Output Error",
				JOptionPane.ERROR_MESSAGE);
		}
	}
}
