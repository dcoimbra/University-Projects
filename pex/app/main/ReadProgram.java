package pex.app.main;

//FIXME import used core classes
import pt.utl.ist.po.ui.Command;
import pt.utl.ist.po.ui.Display;
import pt.utl.ist.po.ui.Form;
import pt.utl.ist.po.ui.InputString;

/**
 * Read existing program.
 */
public class ReadProgram extends Command</*FIXME Receiver class*/> {
    /**
     * @param receiver
     */
    public ReadProgram(/*FIXME core class*/ receiver) {
        super(Label.READ_PROGRAM, receiver);
    }

    /** @see pt.utl.ist.po.ui.Command#execute() */
    @Override
    public final void execute() {
        //FIXME implement
    }
}
