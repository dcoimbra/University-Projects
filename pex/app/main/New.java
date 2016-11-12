package pex.app.main;

//FIXME import used core classes
import pt.utl.ist.po.ui.Command;
import pt.utl.ist.po.ui.Display;
import pt.utl.ist.po.ui.Form;
import pt.utl.ist.po.ui.InputBoolean;

/**
 * Command for creating a new interpreter.
 */
public class New extends Command<App> 
{
    /**
     * @param app
     */
    public New(App app) 
    {
        super(Label.NEW, app);
    }

    /** @see pt.utl.ist.po.ui.Command#execute() */
    @Override
    public final void execute() 
    {
        Interpreter interpreter = new Interpreter(entity());
        
        interpreter.setFileAssociation(null);

        entity().setInterpreter(interpreter);
    }
}
