package pex.app;

import pex.AppIO;

import pex.parser.ParserException;

import pex.app.main.MainMenu;

import pex.core.Interpreter;

import java.io.Serializable;

import pt.utl.ist.po.ui.Form;
import pt.utl.ist.po.ui.InputString;
import pt.utl.ist.po.ui.InputInteger;
import pt.utl.ist.po.ui.Display;

import pt.utl.ist.po.ui.UserInteraction;

/**
 * This is a sample client for the expression evaluator.
 * It uses a text-based user interface.
 */
public class App implements AppIO, Serializable {
    
    /**
     * Current interpreter, associated to this app.
     */
    private Interpreter _currentInterpreter = null;

    /**
     * Class constructor.
     */
    public App() {

    }

    /**
     * Writes a string to be presented to the user.
     *
     * @param str the string to write
     */
    public void println(String str) {
    	
        Display display = new Display();
        display.add(str);
        display.display();
    }

    /**
     * Reads a string inputed by the user.
     *
     * @return the string written by the user.
     */
    public String readString() {
    	
        Form f = new Form();
        InputString inS = new InputString(f, "");
        f.parse();

        return inS.toString();
    }

    /**
     * Reads an integer inputed by the user.
     *
     * @return the number written by the user.
     */
    public int readInteger() {
    	
        Form f = new Form();
        InputInteger inI = new InputInteger(f, "");
        f.parse();

        return inI.value();
    }
    
    /**
     * Sets current interpreter to the given interpreter.
     *
     * @param the interpreter to set
     */
    public void setCurrentInterpreter(Interpreter interpreter) {
    	
        _currentInterpreter = interpreter;
    }

    /**
     * Gives the current interpreter.
     * 
     * @return the current interpreter.
     */
    public Interpreter getCurrentInterpreter() {
        
        return _currentInterpreter;
    }    
    
    /**
     * Entry point for the application.
     *  
     * Sets a new interpreter and opens the main menu. 
     * 
     * @param args - none apply
     */
    public static void main(String[] args) {

        App app = new App();
        
        Interpreter interpreter = new Interpreter(app);
        app.setCurrentInterpreter(interpreter);

        //check if the user wants to start the application with a program
        String datafile = System.getProperty("import"); //$NON-NLS-1$
        
        if (datafile != null) {
            
            try {
            	
                app.getCurrentInterpreter().readProgramFromFile(datafile, "import");
            }

            catch (ParserException e) {
                
                // no behavior described: just present the problem
                e.printStackTrace();
            }
        }
        
        MainMenu menu = new MainMenu(app);
        menu.open();

        UserInteraction.IO.close();
    }
}