package pex.app.main;

import java.io.FileInputStream;
import java.io.IOException;
import java.io.FileNotFoundException;
import java.io.ObjectInputStream;

import pex.app.App;

import pex.core.Interpreter;

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
    public Open(App receiver) {
        
        super(Label.OPEN, receiver);
    }

    /** 
     * @see pt.tecnico.po.ui.Command#execute() 
     */
    @Override
    public final void execute() throws InvalidOperation {
        
        Form f = new Form();
        InputString inputFilename = new InputString(f, Message.openFile());
        f.parse();

        String filename = inputFilename.toString();

        try {
            
            FileInputStream fpIn = new FileInputStream(filename);
            ObjectInputStream inInterp = new ObjectInputStream(fpIn);

            Interpreter loadedInterpreter = (Interpreter)inInterp.readObject();
            
            loadedInterpreter.setFileAssociation(filename);
            loadedInterpreter.setWasChangedFlag(false);
            entity().setCurrentInterpreter(loadedInterpreter);
            
	        inInterp.close();
        }
        
        catch(FileNotFoundException e) {
        	
            Display errDisplay = new Display();
            errDisplay.add(Message.fileNotFound());
            errDisplay.display();
        }
        
        catch(IOException e) {
            
           throw new InvalidOperation(e.getMessage());
        }

        catch(ClassNotFoundException e) {
        	
            throw new InvalidOperation(e.getMessage());
        }
    }
}