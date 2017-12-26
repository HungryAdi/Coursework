// LetStatement.java
//
// ICS 22 / CSE 22 Fall 2012
// Project #3: What's Simple Is True
//
// The LetStatement class encapsulates a LET statement in a Facile program.
// A LET statement looks like this:
//
//     LET A 3
//
// Primarily, it consists of two things: a variable name (denoted by a single
// character) and an integer value.  So, a LetStatement object contains a
// character and an integer, which is the important information contained in
// a LET statement.
//
// You'll need to similarly define subclasses for each of the other kinds of
// statements in a Facile program.


public class LetStatement extends Statement
{
	private char variableName;
	private int value;

	public LetStatement(char variableName, int value)
	{
		this.variableName = variableName;
		this.value = value;
	}
	

	// The LetStatement version of execute() should make two changes to the
	// state of the program:
	//
	//    * set the value of the appropriate variable
	//    * increment the program counter
	//
	// There is no way for a LET statement to fail, so there is no reason for
	// it to ever throw a FatalException.  Nevertheless, the signature of the
	// method must match the signature of the execute() method in Statement,
	// so we'll leave the "throws" clause alone.
	
	public void execute(ProgramState state)
	throws FatalException
	{
		state.setVariable(this.variableName, this.value);
	}
}
