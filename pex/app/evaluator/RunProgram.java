package pex.app.evaluator;

//FIXME import used core classes
import pex.core.Program;

/**
 * Run program.
 */
public class RunProgram extends ProgramCommand {
  
    /**
     * @param receiver
     */
    public RunProgram(Program program) {
        super(Label.RUN_PROGRAM, program);
    }

    /** @see pt.utl.ist.po.ui.Command#execute() */
    @Override
    public final void execute() {
        //FIXME implement
    }
}
