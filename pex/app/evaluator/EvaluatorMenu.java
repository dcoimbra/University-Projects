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
     * @param program
     */
    public EvaluatorMenu(Program program) {
        super(Label.TITLE,
              new Command<?>[] { 
                      new ShowProgram(program), 
                      new RunProgram(program), 
                      new AddExpression(program),
                      new ReplaceExpression(program),
                      new ShowAllIdentifiers(program),
                      new ShowUninitializedIdentifiers(program),
                      });
    }

}
