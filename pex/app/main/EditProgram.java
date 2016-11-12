package pex.app.main;

//FIXME import used core classes
import pex.app.evaluator.EvaluatorMenu;
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
    public EditProgram(App app) {
        super(Label.MANAGE_PROGRAM, app);
    }

    /** @see pt.utl.ist.po.ui.Command#execute() */
    @Override
    public final void execute() {
        //FIXME implement

        Interpreter interpreter = entity().getInterpreter();
        String programID = entity().readString(Message.requestProgramId());
        Program program = interpreter.getProgram(programID);

        EvaluatorMenu menu = new EvaluatorMenu(program);
        menu.open();
    }

}
