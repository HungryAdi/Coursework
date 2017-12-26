
public class AddStatement extends Statement
{
	private char variableName;
	private int value;

	public AddStatement(char variableName, int value)
	{
		this.variableName = variableName;
		this.value = value;
	}

	@Override
	public void execute(ProgramState state) throws FatalException
	{
		int current = state.getVariable(variableName);
		state.setVariable(variableName, current+value);
	}

}