package pex.app.main;

import java.io.FileInputStream;
import java.io.FileNotFoundException;
import java.io.IOException;
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
     * @param receiver
     */
    public Open(App receiver) {
        super(Label.OPEN, receiver);
    }

    /** @see pt.tecnico.po.ui.Command#execute() */
    @Override
    public final void execute() throws InvalidOperation {
        
        Display prompt = new Display();
        prompt.add(Message.openFile());
        prompt.display();

        String filename = entity().readString();

        try
        {
            FileInputStream fpIn = new FileInputStream(filename);
            ObjectInputStream inInterp = new ObjectInputStream(fpIn);

            Interpreter loadedInterpreter = (Interpreter)inInterp.readObject();

            loadedInterpreter.setFileAssociation(filename);
            entity().setCurrentInterpreter(loadedInterpreter);
            
	        inInterp.close();
        }
//TODO tirar os println's e implementer as respectivas excepcoes (no pex.core )
        catch(FileNotFoundException e)
        {
            throw new InvalidOperation(Message.fileNotFound());
        }
        
        catch(IOException e)
        {
        	throw new InvalidOperation(Message.fileNotFound());
        }
        
        catch(ClassNotFoundException e)
        {
        	throw new InvalidOperation(Message.fileNotFound());
        }
    }

}
