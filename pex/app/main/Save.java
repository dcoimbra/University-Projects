package pex.app.main;


import java.io.FileOutputStream;
import java.io.IOException;
import java.io.ObjectOutputStream;

import pex.app.App;
import pex.core.Interpreter;

import pt.utl.ist.po.ui.Command;
import pt.utl.ist.po.ui.Form;
import pt.utl.ist.po.ui.InputString;
import pt.utl.ist.po.ui.InvalidOperation;

/**
 * Save to file under current name (if unnamed, query for name).
 */
public class Save extends Command<App> {
    /**
     * @param receiver 
     * @param receiver
     */
    public Save(App receiver) {
        super(Label.SAVE, receiver);
    }

    /** @see pt.utl.ist.po.ui.Command#execute() */
    @Override
    public final void execute() throws InvalidOperation {
        Interpreter interpreter = entity().getCurrentInterpreter();
        String fileAssociation = interpreter.getFileAssociation();
        try {
        	
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
	        
		} catch (IOException e) { //TODO criar excepcao para aqui
			throw new InvalidOperation(Message.fileNotFound()); 
		}
    }
}
