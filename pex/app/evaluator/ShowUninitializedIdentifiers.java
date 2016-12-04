package pex.app.evaluator;

import java.util.ArrayList;

//FIXME import core classes
import pex.core.Program;

import pt.utl.ist.po.ui.Display;

/**
 * Show uninitialized identifiers.
 */
public class ShowUninitializedIdentifiers extends ProgramCommand {

    /**
     * @param receiver
     */
    public ShowUninitializedIdentifiers(Program receiver) {
        
        super(Label.SHOW_UNINITIALIZED_IDENTIFIERS, receiver);
    }

    /** 
     * @see pt.utl.ist.po.ui.Command#execute() 
     */
    @Override
    public final void execute() {
        
    	ArrayList<String> ids = entity().getSortedUninitializedIdentifiers();
    	
    	for(String name : ids) {
    		Display display = new Display();
    		display.add(name);
            display.display();
    	}
    }
}
