public class GoSubStatement extends Statement
{
	private int lineNumber;

	public GoSubStatement(int line) {
		this.lineNumber = line;
	}

	@Override
	public void execute(ProgramState state) throws FatalException
	{
		// TODO Auto-generated method stub
		state.goSub(this.lineNumber);
	}

}