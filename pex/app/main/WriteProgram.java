package pex.app.main;

import java.io.IOException;

//FIXME import used core classes
import pt.utl.ist.po.ui.Command;
import pt.utl.ist.po.ui.Display;
import pt.utl.ist.po.ui.Form;
import pt.utl.ist.po.ui.InputString;
import pt.utl.ist.po.ui.InvalidOperation;

/**
 * Write (save) program to file.
 */
public class WriteProgram extends Command</*FIXME Receiver class*/> {
    /**
     * @param receiver
     */
    public WriteProgram(/*FIXME core class*/ receiver) {
        super(Label.WRITE_PROGRAM, receiver);
    }

   /** 
    * @see pt.utl.ist.po.ui.Command#execute() 
    */
    @Override
    public final void execute() throws InvalidOperation {
        //FIXME implement
    }
}
