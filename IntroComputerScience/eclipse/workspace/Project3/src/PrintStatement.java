
public class PrintStatement extends Statement
{
	private char variableName;

	public PrintStatement(char variableName) {
		this.variableName=variableName;
	}

	
	@Override
	public void execute(ProgramState state) throws FatalException
	{
		System.out.println(this.variableName+"="+state.getVariable(this.variableName));

	}

}
