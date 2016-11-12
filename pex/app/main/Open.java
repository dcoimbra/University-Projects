package pex.app.main;

import java.io.FileNotFoundException;
import java.io.IOException;

import pex.app.main.Message;

//FIXME import used core classes
import pt.utl.ist.po.ui.Command;
import pt.utl.ist.po.ui.Display;
import pt.utl.ist.po.ui.Form;
import pt.utl.ist.po.ui.InputString;
import pt.utl.ist.po.ui.InvalidOperation;

/**
 * Open existing interpreter.
 */
public class Open extends Command<App> {
    /**
     * @param receiver
     */
    public Open(App app) {
        super(Label.OPEN, app);
    }

    /** @see pt.tecnico.po.ui.Command#execute() */
    @Override
    public final void execute() throws InvalidOperation, FileNotFoundException {
        //FIXME implement

        String filename = entity().readString(Message.openFile());

        try
        {
            FileInputStream fpIn = new FileInputStream(filename);
            ObjectInputStream inInterp = new ObjectInputStream(fpIn);

            Interpreter interpreter = (Interpreter)inInterp.readObject();

            interpreter.setFileAssociation(filename);
            entity().setInterpreter(interpreter);
        }

        catch(FileNotFoundException e)
        {
            entity().println(Message.fileNotFound());
        }
    }
}
