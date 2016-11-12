package pex.app.main;

import java.io.IOException;

//FIXME import used core classes
import pt.utl.ist.po.ui.Command;
import pt.utl.ist.po.ui.Form;
import pt.utl.ist.po.ui.InputString;
import pt.utl.ist.po.ui.InvalidOperation;

/**
 * Save to file under current name (if unnamed, query for name).
 */
public class Save extends Command<App> {
    /**
     * @param app
     */
    public Save(App app) {
        super(Label.SAVE, app);
    }

    /** @see pt.utl.ist.po.ui.Command#execute() */
    @Override
    public final void execute() throws InvalidOperation {
        
        Interpreter interpreter = entity().getInterpreter();
        String fileAssociation = interpreter.getFileAssociation();

        if (fileAssociation == null)
        {
            fileAssociation = entity().readString(Message.newSaveAs());
            interpreter.setFileAssociation(fileAssociation);
        }

        FileOutputStream fpOut = new FileOutputStream(fileAssociation);
        ObjectOutputStream out = new ObjectOutputStream(fpOut);
            
        out.writeObject(interpreter);
            
        out.flush();
        out.close();
    }
}
