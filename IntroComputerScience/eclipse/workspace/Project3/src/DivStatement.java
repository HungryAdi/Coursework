public class DivStatement extends Statement
{

	private char variableName;
	private int value;

	public DivStatement(char variableName, int value)
	{
		this.variableName = variableName;
		this.value = value;
	}

	@Override
	public void execute(ProgramState state) throws FatalException
	{
		if (this.value==0) {
			throw new FatalException("Cannot divide by zero");
		}
		int current = state.getVariable(this.variableName);
		state.setVariable(variableName, current/this.value);
	}

}
