package pex.app.evaluator;

//FIXME import used core classes

import pex.app.BadExpressionException;
import pex.app.BadPositionException;

import pex.core.Program;

import pt.utl.ist.po.ui.Form;
import pt.utl.ist.po.ui.InputString;
import pt.utl.ist.po.ui.InputInteger;

/**
 * Add expression.
 */
public class AddExpression extends ProgramCommand {
    /**
     * @param receiver
     */
    public AddExpression(Program receiver) {
        super(Label.ADD_EXPRESSION, receiver);
    }
    
    /** @see pt.utl.ist.po.ui.Command#execute() */
    @Override
    public final void execute() throws BadExpressionException, BadPositionException {
        //FIXME implement
    }
}
