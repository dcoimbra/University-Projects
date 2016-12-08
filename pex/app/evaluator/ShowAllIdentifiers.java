package pex.app.evaluator;

import java.util.ArrayList;

import pex.core.Program;
import pex.core.InvalidArgumentException;

import pt.utl.ist.po.ui.Display;
import pt.utl.ist.po.ui.InvalidOperation;

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
    public final void execute() throws InvalidOperation {
       
        try { 
            
            ArrayList<String> ids = entity().getAllIdentifiers();

            Display display = new Display();
            
            for (String name : ids) {
                        
                display.addNewLine(name);
            }

            display.display();
        }

        catch (InvalidArgumentException iae) {

            throw new InvalidOperation(iae.getMessage());
        }
    }
}