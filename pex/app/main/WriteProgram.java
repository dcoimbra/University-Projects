package pex.app.main;

import java.io.BufferedWriter;
import java.io.FileWriter;
import java.io.IOException;
import java.io.PrintWriter;

import pex.app.App;
import pex.core.Interpreter;
import pex.core.Program;
import pex.core.ProgramNotFoundException;

import pt.utl.ist.po.ui.Command;
import pt.utl.ist.po.ui.Display;
import pt.utl.ist.po.ui.Form;
import pt.utl.ist.po.ui.InputString;
import pt.utl.ist.po.ui.InvalidOperation;

/**
 * Write (save) program to file.
 */
public class WriteProgram extends Command<App> {
    /**
     * @param receiver
     */
    public WriteProgram(App receiver) {
        super(Label.WRITE_PROGRAM, receiver);
    }

    /** @see pt.utl.ist.po.ui.Command#execute() */
    @Override
    public final void execute() throws InvalidOperation
    {
    	try {

    		Display promptPID = new Display();
        	promptPID.add(Message.requestProgramId());
        	promptPID.display();

	        String programID = entity().readString();
	
	        Interpreter interpreter = entity().getCurrentInterpreter();
	        Program program = interpreter.getProgram(programID);
	        
	        if (program == null){
	        	throw new ProgramNotFoundException(Message.noSuchProgram(programID));
	        }
	        
	        String programCode = program.getAsText();
	
	        Display promptFile = new Display();
        	promptFile.add(Message.programFileName());
        	promptFile.display();

	        String filename = entity().readString();
	
	        FileWriter out = new FileWriter(filename);
			
	        BufferedWriter buffer = new BufferedWriter(out);
	        PrintWriter printer = new PrintWriter(buffer);
	
	        printer.write(programCode);
	        
	        printer.close();
    	} 
    	
        catch (IOException e) { //TODO como lidar com estas excepcoes?
			throw new InvalidOperation(e.getMessage());
		}
    }
}
