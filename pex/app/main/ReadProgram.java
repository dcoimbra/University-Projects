package pex.app.main;

import pex.app.App;

import pex.core.Interpreter;
import pex.core.Program;

import pex.parser.NewParser;

import pt.utl.ist.po.ui.Command;
import pt.utl.ist.po.ui.Display;
import pt.utl.ist.po.ui.Form;
import pt.utl.ist.po.ui.InputString;
import pt.utl.ist.po.ui.InvalidOperation;

/**
 * Read existing program.
 */
public class ReadProgram extends Command<App> {
    
    /**
     * @param receiver
     */
    public ReadProgram(App receiver) {
        
        super(Label.READ_PROGRAM, receiver);
    }

    /** 
     * @see pt.utl.ist.po.ui.Command#execute() 
     */
    @Override
    public final void execute() throws InvalidOperation {
    	
        Form f = new Form();
        InputString inputFilename = new InputString(f, Message.programFileName());
        f.parse();

        String filename = inputFilename.toString();
    	
        try {        
	
	        NewParser parser = new NewParser();        
	        Program program = parser.parseFile(filename, filename);
	       
            Interpreter interpreter = entity().getCurrentInterpreter();
	        interpreter.addProgram(program);

            interpreter.setWasChangedFlag(true);
    	} 
    	
    	catch (pex.parser.BadSourceException bse) {
    		
            Display errDisplay = new Display();
            errDisplay.add(Message.fileNotFound(filename));
            errDisplay.display();
    	}
    	
    	catch (Exception e) {
    		
            throw new InvalidOperation(e.getMessage());
    	}
    }
}
