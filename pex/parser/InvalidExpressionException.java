package pex.parser;

/**
 * Class for representing a syntax error problem: expression does not follow syntax.
 */
public class InvalidExpressionException extends ParserException {
    /** Serial number for serialization. */
    private static final long serialVersionUID = 201608241029L;

    /** The number of the line containing the invalid expression. */
    private int _lineNumber;

    /**
     * @param line number of the line containing the invalid expression
     */
    public InvalidExpressionException(int line) {
        super("Syntax error in line " + line);
        _lineNumber = line;
    }

    /**
     * @return line number of the line containing the invalid expression
     */
    public int getLineNumber() {
        return _lineNumber;
    }

}
