package pex.parser;

/**
 * Exception for representing unknown operation errors.
 */
public class UnknownOperationException extends ParserException {

    /** Serial number for serialization. */
    private static final long serialVersionUID = 201608241029L;

    /** Invalid operation. */
    String _operation;

    /**
     * @param operation invalid operation
     */
    public UnknownOperationException(String operation) {
        super("Unknown operation: " + operation);
        _operation = operation;
    }

    /**
     * @return the invalid operation
     */
    public String getInvalidOperation() {
        return _operation;
    }
}
