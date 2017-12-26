
public class ReturnStatement extends Statement
{

	@Override
	public void execute(ProgramState state) throws FatalException
	{
		// TODO Auto-generated method stub

		state.returnFromSub();
	}

}
