// FatalException.java
//
// ICS 22 / CSE 22 Fall 2012
// Project #3: What's Simple Is True
//
// FatalExceptions are thrown by the various execute() methods in the
// Statement subclasses if the execution of a statement causes the Facile
// program to have to terminate immediately due to a fatal error.  Three
// examples are fatal errors are division by zero, a RETURN statement
// being encountered before a corresponding GOSUB, and a GOTO or GOSUB
// to a non-existent line.
//
// FatalExceptions store a "reason" inside them, which explains the cause
// of the fatal error.  To get the reason from a FatalException, you can
// call the getReason() method.
//
// You should not need to modify this class at all.


public class FatalException extends Exception
{
	private String reason;


	public FatalException(String reason)
	{
		super(reason);
		this.reason = reason;
	}


	public String getReason()
	{
		return reason;
	}
}
