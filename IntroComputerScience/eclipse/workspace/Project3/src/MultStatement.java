
public class MultStatement extends Statement
{

	private char variableName;
	private int value;

	public MultStatement(char variableName, int value)
	{
		this.variableName = variableName;
		this.value = value;
	}

	
	@Override
	public void execute(ProgramState state) throws FatalException
	{
		// TODO Auto-generated method stub
		int current = state.getVariable(variableName);
		state.setVariable(variableName, current*value);
	}

}