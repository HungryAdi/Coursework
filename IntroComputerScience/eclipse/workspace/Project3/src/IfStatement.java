
public class IfStatement extends Statement
{

	private char variableName;
	private String op;
	private int value;
	private int lineNumber;
	
	public IfStatement( char variableName, String op, int value, int lineNumber) {
		this.variableName=variableName;
		this.op=op;
		this.value=value;
		this.lineNumber=lineNumber;
	}
	
	
	@Override
	public void execute(ProgramState state) throws FatalException
	{
		int variable=state.getVariable(this.variableName);
		boolean evaluatesTrue = false;
		if (op.equals("<")) {
			evaluatesTrue = (variable < this.value);
		} else if (op.equals("<=")) {
			evaluatesTrue = (variable <= this.value);			
		} else if (op.equals(">")) {
			evaluatesTrue = (variable > this.value);			
		} else if (op.equals(">=")) {
			evaluatesTrue = (variable >= this.value);			
		} else if (op.equals("<>")) {
			evaluatesTrue = ( (variable < this.value) || (variable > this.value) );			
		} else if (op.equals("=")) {
			evaluatesTrue = (variable == this.value);			
		}
		if (evaluatesTrue) {
			state.setGoToLineNumber (this.lineNumber);
		}
	}

}
