// Statement.java
//
// ICS 22 / CSE 22 Fall 2012
// Project #3: What's Simple Is True
//
// This is the abstract Statement class, from which your various statement
// classes will need to extend.  There is only one method in the Statement
// class, an execute() method that executes the statement, making any
// necessary changes to the given ProgramState.


public abstract class Statement
{
	// execute() takes a ProgramState and executes this statement, by making
	// any necessary changes to the ProgramState (e.g. a new value for the
	// program counter, changing the value of some variable, pushing or
	// popping from the line number stack).
	//
	// In the event that the execution of the statement causes a fatal error
	// that should terminate the Facile program, such as division by zero or
	// a RETURN statement without a corresponding GOSUB, a FatalException is
	// thrown.
	public abstract void execute(ProgramState state)
	throws FatalException;
}
