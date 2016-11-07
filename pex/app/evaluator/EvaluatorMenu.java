package pex.app.evaluator;

//FIXME import used core classes

import pex.core.Program;

import pt.utl.ist.po.ui.Command;
import pt.utl.ist.po.ui.Menu;

/**
 * Menu for program manipulation.
 */
public class EvaluatorMenu extends Menu {

    /**
     * @param receiver
     */
    public EvaluatorMenu(Program receiver) {
        super(Label.TITLE,
              new Command<?>[] { //
                  new ShowProgram(receiver), //
                      new RunProgram(receiver), //
                      new AddExpression(receiver), //
                      new ReplaceExpression(receiver), //
                      new ShowAllIdentifiers(receiver), //
                      new ShowUninitializedIdentifiers(receiver), //
                      });
    }

}
