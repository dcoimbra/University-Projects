package pex.app.evaluator;

//FIXME import core classes
import pex.core.Program;

import pt.utl.ist.po.ui.Display;


/**
 * Show program (present code).
 */
public class ShowProgram extends ProgramCommand {

    /**
     * @param receiver
     */
    public ShowProgram(Program program) {
        super(Label.SHOW_PROGRAM, program);
    }

    /** @see pt.utl.ist.po.ui.Command#execute() */
    @Override
    public final void execute() {
        String programCode = entity().getAsText();

        Display display = new Display();

        display.add(programCode);

        display.display();
    }
}
