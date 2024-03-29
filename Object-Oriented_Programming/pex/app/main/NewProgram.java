package pex.app.main;

import pex.app.App;

import pex.core.Interpreter;
import pex.core.Program;

import pt.utl.ist.po.ui.Command;
import pt.utl.ist.po.ui.Form;
import pt.utl.ist.po.ui.InputString;
import pt.utl.ist.po.ui.Display;

/**
 * Create new program.
 */
public class NewProgram extends Command<App> {

    /**
     * @param receiver 
     */
    public NewProgram(App receiver) {
        
        super(Label.NEW_PROGRAM, receiver);
    }

    /** 
     * @see pt.utl.ist.po.ui.Command#execute() 
     */
    @Override
    public final void execute() {

        Form f = new Form();
        InputString inputProgramID = new InputString(f, Message.requestProgramId());
        f.parse();
    	
        String programID = inputProgramID.toString();

        Interpreter interpreter = entity().getCurrentInterpreter();

        Program program = new Program(programID, interpreter);

        interpreter.addProgram(program);
    }
}