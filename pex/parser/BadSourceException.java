package pex.parser;

/**
 * Exception for representing errors on the source of the expressions to parse.
 */
public class BadSourceException extends ParserException {
  
    /** Serial number for serialization. */
    private static final long serialVersionUID = 201608241029L;
  
    /** Source to parse. */
    String _source;

    /**
     * @param description 
     * @param cause 
     */
    public BadSourceException(String source, Exception cause) {
        super("Error while processing " + source, cause);
        _source = source;
    }
  
    /**
     * @return the source
     */
    public String getSource() {
        return _source;
    }
}
