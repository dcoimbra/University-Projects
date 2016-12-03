package pex.app.evaluator;

//FIXME import used core classes

import pex.app.BadExpressionException;
import pex.app.BadPositionException;

import pex.parser.NewParser;
import pex.parser.ParserException;

import pex.core.Program;
import pex.core.Expression;

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
    
    /** 
     *  @see pt.utl.ist.po.ui.Command#execute() 
     */
    @Override
    public final void execute() throws BadExpressionException, BadPositionException {
        
        Form f1 = new Form();
        InputInteger inputPosition = new InputInteger(f1, Message.requestPosition());
        f1.parse();

        int position = inputPosition.value();

        Form f2 = new Form();
        InputString inputExpression = new InputString(f2, Message.requestExpression());
        f2.parse();

        String strExpression = inputExpression.value();

        NewParser parser = new NewParser();

        try {

            Expression expression = parser.parseString(strExpression, entity());

            entity().add(position, expression);
        }

        catch (IndexOutOfBoundsException iobe) {

            throw new BadPositionException(position);
        }

        catch (ParserException pe) {

            throw new BadExpressionException(strExpression);
        }
    }
}
