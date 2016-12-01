package pex.app.main;

import pex.app.App;

import pex.core.Interpreter;
import pex.core.Program;

import pex.parser.NewParser;
import pex.parser.BadSourceException;
import pex.parser.ParserException;

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
	
	        Interpreter interpreter = entity().getCurrentInterpreter();
            interpreter.readProgramFromFile(filename, filename);
    	}
    	
    	catch (BadSourceException bse) {
    		
            Display errDisplay = new Display();
            errDisplay.add(Message.fileNotFound(filename));
            errDisplay.display();
    	}

        catch (ParserException e) {
                
                // no behavior described: just present the problem
                e.printStackTrace();
            }
    }
}
