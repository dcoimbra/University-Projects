package pex.app.main;

//FIXME import used core classes
import pt.utl.ist.po.ui.Command;
import pt.utl.ist.po.ui.Form;
import pt.utl.ist.po.ui.InputString;

/**
 * Create new program.
 */
public class NewProgram extends Command<App> 
{

    /**
     * @param app
     */
    public NewProgram(App app) 
    {
        super(Label.NEW_PROGRAM, app);
    }

    /** @see pt.utl.ist.po.ui.Command#execute() */
    @Override
    public final void execute() 
    {
        //FIXME implement

        String programID = entity().readString(Message.requestProgramId());
        
        Program program = new Program(programID);

        entity().addProgramAux(program);
    }
}
