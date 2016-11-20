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
import pt.utl.ist.po.ui.Display;

/**
 * Save to file under current name (if unnamed, query for name).
 */
public class Save extends Command<App> {
    
    /** 
     * @param receiver
     */
    public Save(App receiver) {
        
        super(Label.SAVE, receiver);
    }

    /** 
     * @see pt.utl.ist.po.ui.Command#execute() 
     */
    @Override
    public final void execute() throws InvalidOperation {

        Interpreter interpreter = entity().getCurrentInterpreter();

        if (!(interpreter.wasChanged())) {

        	return;
        }

        String fileAssociation = interpreter.getFileAssociation();
        	
	    if (fileAssociation == null) {

	        Form f = new Form();
	        InputString inputFileAssociation = new InputString(f, Message.newSaveAs());
	        f.parse();

	        fileAssociation = inputFileAssociation.toString();
	        interpreter.setFileAssociation(fileAssociation);

	    }

	    try {
	
	        FileOutputStream fpOut = new FileOutputStream(fileAssociation);
			ObjectOutputStream out = new ObjectOutputStream(fpOut);
	        out.writeObject(interpreter);
	            
	        out.flush();
	        out.close();
		   
	        interpreter.setWasChangedFlag(false);
		} 
	    

		catch (IOException e) {
			
			throw new InvalidOperation(e.getMessage()); 
		}
    }
}
