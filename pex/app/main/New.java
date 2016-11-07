package pex.app.main;

//FIXME import used core classes
import pt.utl.ist.po.ui.Command;
import pt.utl.ist.po.ui.Display;
import pt.utl.ist.po.ui.Form;
import pt.utl.ist.po.ui.InputBoolean;

/**
 * Command for creating a new interpreter.
 */
public class New extends Command</*FIXME Receiver class*/> {
    /**
     * @param receiver
     */
    public New(/*FIXME core class*/ receiver) {
        super(Label.NEW, receiver);
    }

    /** @see pt.utl.ist.po.ui.Command#execute() */
    @Override
    public final void execute() {
        //FIXME implement
    }
}
