package pex.app.evaluator;

//FIXME import used core classes

import pex.app.BadExpressionException;
import pex.app.BadPositionException;

import pex.core.Program;

import pt.utl.ist.po.ui.Form;
import pt.utl.ist.po.ui.InputString;
import pt.utl.ist.po.ui.InputInteger;

/**
 * Replace expression in program.
 */
public class ReplaceExpression extends ProgramCommand {
    /**
     * @param receiver
     */
    public ReplaceExpression(Program receiver) {
        super(Label.REPLACE_EXPRESSION, receiver);
    }

    /** @see pt.utl.ist.po.ui.Command#execute() */
    @Override
    public final void execute() throws BadExpressionException, BadPositionException {
        //FIXME implement
    }
}
