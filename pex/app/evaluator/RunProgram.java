package pex.app.evaluator;

import pex.core.Program;
import pex.core.InvalidArgumentException;
import pex.core.NoSuchProgramException;

import pt.utl.ist.po.ui.InvalidOperation;

/**
 * Run program.
 */
public class RunProgram extends ProgramCommand {
  
    /**
     * @param receiver
     */
    public RunProgram(Program receiver) {
        
        super(Label.RUN_PROGRAM, receiver);
    }

    /** 
     * @see pt.utl.ist.po.ui.Command#execute() 
     */
    @Override
    public final void execute() throws InvalidOperation {
        
        try {
            
            entity().execute();
        }

        catch (InvalidArgumentException iae) {

            throw new InvalidOperation(iae.getMessage());
        }

        catch (NoSuchProgramException nspe) {

            throw new InvalidOperation(nspe.getMessage());
        }
    }
}