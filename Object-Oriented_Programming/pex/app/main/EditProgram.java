package pex.app.main;

import pex.app.App;
import pex.app.evaluator.EvaluatorMenu;
import pex.app.evaluator.ShowProgram;

import pex.core.Interpreter;
import pex.core.Program;

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

    /** 
     * @see pt.utl.ist.po.ui.Command#execute()
     */
    @Override
    public final void execute() {
       
        Interpreter interpreter = entity().getCurrentInterpreter();

        Form f = new Form();
        InputString inputProgramID = new InputString(f, Message.requestProgramId());
        f.parse();
        
        String programID = inputProgramID.toString();
        
        Program program = interpreter.getProgram(programID);

        if (program == null) {

            Display errDisplay = new Display();
            errDisplay.add(Message.noSuchProgram(programID));
            errDisplay.display();

            return;
        }
        
        EvaluatorMenu menu = new EvaluatorMenu(program);
        menu.open();
    }
}