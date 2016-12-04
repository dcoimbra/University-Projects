package pex.app.evaluator;

import java.util.ArrayList;
import pex.app.main.Message;
import pex.core.Program;
import pt.utl.ist.po.ui.Display;

/**
 * Show all program identifiers.
 */
public class ShowAllIdentifiers extends ProgramCommand {

    /**
     * @param receiver
     */
    public ShowAllIdentifiers(Program receiver) {
        
        super(Label.SHOW_ALL_IDENTIFIERS, receiver);
    }

    /** 
     * @see pt.utl.ist.po.ui.Command#execute() 
     */ 
    @Override
    public final void execute() {
        
    	ArrayList<String> ids = entity().getSortedIdentifiers();
    	
    	for(String name : ids) {
    		Display display = new Display();
    		display.add(name);
            display.display();
    	}
        
    }
}
