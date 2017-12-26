// GuessResponse.java
//
// ICS 22 / CSE 22 Fall 2012
// In-Lab Assignment #0
//
// This enumeration contains all of the possible responses that might
// be returned when the user makes a guess in the "guess a number"
// game.  (In case we haven't talked about these in lecture yet,
// enumerations are sets of related constants.  Having declared the
// enum below, we could refer to those constants as GuessResponse.CORRECT,
// GuessResponse.TARGET_IS_SMALLER, and GuessResponse.TARGET_IS_LARGER,
// respectively.)
//
// This enumeration is, in a way, the "glue" that sticks the user
// interface and the engine together.  When the user makes a guess in
// the user interface, it gets passed to the engine; the engine, then,
// needs a way to tell the user interface whether the guess was correct,
// the target was smaller, or the target was larger.  This enumeration
// contains all three possible answers, allowing the engine to return
// one of those three answers each time.  The user interface, in turn,
// can print a different message to the user depending on which response
// it got.


public enum GuessResponse
{
	CORRECT,
	TARGET_IS_SMALLER,
	TARGET_IS_LARGER
}
