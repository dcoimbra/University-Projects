package pex.app.evaluator;

//FIXME import used core classes
import pex.core.Program;
import pt.utl.ist.po.ui.Command;

/**
 * Commands for programs.
 */
public abstract class ProgramCommand extends Command<Program> {
    /**
     * @param label 
     * @param receiver
     */
    public ProgramCommand(String label, Program receiver) {
        super(label, receiver);
    }
}
