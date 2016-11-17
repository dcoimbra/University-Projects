package pex.app.main;

import pex.app.App;
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
    	
        entity().println(Message.requestProgramId());
        String filename = entity().readString();
    	
        try {        
	
	        NewParser parser = new NewParser();        
	        Program program = parser.parseFile(filename, filename);
	
	        entity().getCurrentInterpreter().addProgram(program);
    	} 
    	
    	catch (pex.parser.BadSourceException bse){
    		throw new InvalidOperation(Message.fileNotFound(filename));
    	}
    	
    	catch (Exception e) {
    		throw new InvalidOperation(e.getMessage());
    	}
    }
}
