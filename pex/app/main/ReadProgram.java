package pex.app.main;

//FIXME import used core classes
import pt.utl.ist.po.ui.Command;
import pt.utl.ist.po.ui.Display;
import pt.utl.ist.po.ui.Form;
import pt.utl.ist.po.ui.InputString;

/**
 * Read existing program.
 */
public class ReadProgram extends Command<App> 
{
    /**
     * @param app
     */
    public ReadProgram(App app) 
    {
        super(Label.READ_PROGRAM, app);
    }

    /** @see pt.utl.ist.po.ui.Command#execute() */
    @Override
    public final void execute()
    {
        //FIXME implement

        String filename = entity().readString(Message.programFileName());

        NewParser parser = new NewParser();
                 
        Program program = parser.parseFile(filename, filename);

        entity().addProgramAux(program);
    }
}
