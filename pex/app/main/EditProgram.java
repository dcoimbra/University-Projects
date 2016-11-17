package pex.app.main;

import pex.app.App;
import pex.app.evaluator.EvaluatorMenu;
import pex.app.evaluator.ShowProgram;

import pex.core.Interpreter;
import pex.core.Program;
import pex.core.ProgramNotFoundException;

import pt.utl.ist.po.ui.Command;
import pt.utl.ist.po.ui.InputString;
import pt.utl.ist.po.ui.Menu;
import pt.utl.ist.po.ui.Display;
import pt.utl.ist.po.ui.Form;


/**
 * Open menu for managing programs.
 */
public class EditProgram extends Command<App> {

    /**
     * @param receiver
     */
    public EditProgram(App receiver) {
        super(Label.MANAGE_PROGRAM, receiver);
    }

    /** @throws ProgramNotFoundException 
     * @see pt.utl.ist.po.ui.Command#execute() */
    @Override
    public final void execute() throws ProgramNotFoundException {
       
        Interpreter interpreter = entity().getCurrentInterpreter();

        Display prompt = new Display();
        prompt.add(Message.requestProgramId());
        prompt.display();

        String programID = entity().readString();
        Program program = interpreter.getProgram(programID);

        if (program == null){
        	throw new ProgramNotFoundException(Message.noSuchProgram(programID));
        }
        
        EvaluatorMenu menu = new EvaluatorMenu(program);
        menu.open();
    }

}