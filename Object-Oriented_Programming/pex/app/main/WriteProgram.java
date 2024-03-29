package pex.app.main;

import java.io.BufferedWriter;
import java.io.FileWriter;
import java.io.IOException;
import java.io.PrintWriter;

import pex.app.App;

import pex.core.Interpreter;
import pex.core.Program;

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

    /** 
     * @see pt.utl.ist.po.ui.Command#execute() 
     */
    @Override
    public final void execute() throws InvalidOperation {
    	
    	Form f1 = new Form();
        InputString inputProgramID = new InputString(f1, Message.requestProgramId());
        f1.parse();

        String programID = inputProgramID.toString();
	
	    Interpreter interpreter = entity().getCurrentInterpreter();
	    Program program = interpreter.getProgram(programID);
	        
		Form f2 = new Form();
        InputString inputFileName = new InputString(f2, Message.programFileName());
        f2.parse();

        String fileName = inputFileName.toString();

        if (program == null) { 
	    	
	    	Display errDisplay = new Display();
	    	errDisplay.add(Message.noSuchProgram(programID));
	    	errDisplay.display();

	    	return;
	    }
	
		try { 
	    
	    	program.writeToFile(fileName);
    	} 
    	
        catch (IOException e) {
			
			throw new InvalidOperation(e.getMessage());
		}
    }
}