package pex.app;

//import pex.core.Program;

import pex.parser.NewParser;

import pex.AppIO;
import pex.parser.ParserException;
import pex.app.main.MainMenu;

import pt.utl.ist.po.ui.Form;
import pt.utl.ist.po.ui.InputString;
import pt.utl.ist.po.ui.InputInteger;
import pt.utl.ist.po.ui.Display;

import static pt.utl.ist.po.ui.UserInteraction.IO;

/**
 * This is a sample client for the expression evaluator.
 * It uses a text-based user interface.
 */
public class App implements AppIO 
{
    private Interpreter _interpreter = null;

    public App() 
    {
    
    }

    /**
     * Writes a string to be presented to the user.
     *
     * @param str the string to write
     **/
    public void println(String str) 
    {   
        Display display = new Display();

        display.add(str);

        display.display();
    }

    /**
     * Reads a string inputed by the user.
     *
     * @return the string written by the user.
     **/
    public String readString(String message) 
    {   
        Form f = new Form();
        InputString inS = new InputString(f, message);
        f.parse();

        return inS.toString();
    }

    /**
     * Reads an integer inputed by the user.
     *
     * @return the number written by the user.
     **/
    public int readInteger(String message) 
    {
        Form f = new Form();
        InputInteger inI = new InputInteger(f, message);
        f.parse();

        return inI.value();
    }

    public void setInterpreter(Interpreter interpreter)
    {
        _interpreter = interpreter;
    }

    public Interpreter getIntepreter()
    {
        return _interpreter;
    }

    public void addProgramAux(Program program)
    {
        _interpreter.addProgram(program);
    }

    /**
     * @param args
     */
    public static void main(String[] args) throws ParserException
    {
        //FIXME create main core object
        App app = new App();

        _interpreter = new Interpreter(app);

        String datafile = System.getProperty("import"); //$NON-NLS-1$
        if (datafile != null) {
            
            try {
                //FIXME read import file into core object (use Parser instance)
                NewParser parser = new NewParser();
                 
                Program program = parser.parseFile(datafile, "import");

                app.addProgramAux(program);
            } 

            catch (ParserException e) {
                // no behavior described: just present the problem
                e.printStackTrace();
            }
        }
        
        MainMenu menu = new MainMenu(app);
        menu.open();

        IO.close();
    }
}
