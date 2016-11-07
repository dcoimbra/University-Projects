/* $Id: $ */
package pex.parser;

/**
 * Class for representing a syntax error problem: missing closing parenthesis in expression.
 */
public class MissingClosingParenthesisException extends ParserException {

    /** Serial number for serialization. */
    private static final long serialVersionUID = 201608241029L;

    /** Invalid expression. */
    private String _expression;

    /**
     * @param expression invalid expression
     */
    public MissingClosingParenthesisException(String expression) {
        super("Missing closing parenthesis in expression: " + expression);
        _expression = expression;
    }

    /**
     * @return the invalid expression
     */
    public String getInvalidOperation() {
        return _expression;
    }

}
