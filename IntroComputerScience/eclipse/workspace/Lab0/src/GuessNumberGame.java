// GuessNumberGame.java
//
// ICS 22 / CSE 22 Fall 2012
// In-Lab Assignment #0
//
// This class implements the "engine" for the "guess a number" game.
// The engine is in charge of choosing a random number, comparing the
// user's guesses to the target number, and responding accordingly.
//
// Notice that this class does not perform input/output.  This is by
// design.  I've separated the user interface and the "engine" into
// two classes, for a very good reason: it allows the user interface
// to be changed without affecting the engine at all.  This may seem
// silly in a program as small as this one, but I'd like you to see
// the idea, as early as possible, that the name of the game is
// writing relatively simple components that do single, targeted
// jobs and know as little as possible about one another.  As it
// stands now, GuessNumberGame doesn't rely on the fact that this
// program's user interface uses the console.  That means that we
// could replace the user interface with a graphical one without
// making any modifications to GuessNumberGame.

import java.util.Random;


public class GuessNumberGame
{
	private static Random r = new Random();

	private int targetNumber;
	
	
	public GuessNumberGame(int range)
	{
		targetNumber = r.nextInt(range) + 1;
	}


	public GuessResponse checkGuess(int guess)
	{
		if (targetNumber < guess)
		{
			return GuessResponse.TARGET_IS_SMALLER;
		}
		else if (targetNumber > guess)
		{
			return GuessResponse.TARGET_IS_LARGER;
		}
		else
		{
			return GuessResponse.CORRECT;
		}
	}
}
