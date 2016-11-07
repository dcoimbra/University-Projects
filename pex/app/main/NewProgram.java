package pex.app.main;

//FIXME import used core classes
import pt.utl.ist.po.ui.Command;
import pt.utl.ist.po.ui.Form;
import pt.utl.ist.po.ui.InputString;

/**
 * Create new program.
 */
public class NewProgram extends Command</*FIXME Receiver class*/> {

    /**
     * @param receiver
     */
    public NewProgram(/*FIXME core class*/ receiver) {
        super(Label.NEW_PROGRAM, receiver);
    }

    /** @see pt.utl.ist.po.ui.Command#execute() */
    @Override
    public final void execute() {
        //FIXME implement
    }
}
