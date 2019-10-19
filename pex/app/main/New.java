package pex.app.main;

import pex.app.App;
import pex.core.Interpreter;

import pt.utl.ist.po.ui.Command;
import pt.utl.ist.po.ui.Display;
import pt.utl.ist.po.ui.Form;
import pt.utl.ist.po.ui.InputBoolean;

/**
 * Command for creating a new interpreter.
 */
public class New extends Command<App> {
    
    /**
     * @param receiver
     */
    public New(App receiver) {
        
        super(Label.NEW, receiver);
    }

    /** 
     * @see pt.utl.ist.po.ui.Command#execute() 
     */
    @Override
    public final void execute() {
        
        App app = entity();
    	Interpreter newInterpreter = new Interpreter(app);
    	app.setCurrentInterpreter(newInterpreter);        
    }
}