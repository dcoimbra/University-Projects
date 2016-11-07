package pex.app.evaluator;

//FIXME import core classes
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

    /** @see pt.utl.ist.po.ui.Command#execute() */
    @Override
    public final void execute() {
        //FIXME implement
    }
}
