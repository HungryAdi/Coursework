
public class EndStatement extends Statement
{

	@Override
	public void execute(ProgramState state) throws FatalException
	{
		// TODO Auto-generated method stub
		state.endReached();
	}

}
