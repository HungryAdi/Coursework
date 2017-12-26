import java.io.IOException;
import java.util.ArrayList;
import java.util.Iterator;


public class Facile
{
	
	public static void main(String[] args)
	{
		if (args.length<1) {
			System.out.println("Enter Facile program file to run");
			return;
		}
		
		ArrayList<Statement> statements = new ArrayList<Statement>();
		ProgramState programState = new ProgramState();
		try
		{
			statements = Parser.parseProgram(args[0]);
		}
		catch (IOException e)
		{
			// TODO Auto-generated catch block
			System.out.println(e.getMessage());
			return;
		}
		programState.setStatements(statements);
				
		while (programState.hasStatementsToExecute()) {
			Statement statement = programState.nextStatement();
			try
			{
				statement.execute(programState);
			}
			catch (FatalException e)
			{
				// TODO Auto-generated catch block
				System.out.println(e.getMessage());
				return;
			}
		}
	
	}
}
