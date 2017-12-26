public class GoToStatement extends Statement
{
	private int lineNumber;

	public GoToStatement(int value)
	{
		this.lineNumber = value;
	}

	
	@Override
	public void execute(ProgramState state) throws FatalException
	{
		// TODO Auto-generated method stub
		state.setGoToLineNumber (this.lineNumber);
	}

}