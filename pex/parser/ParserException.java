package pex.parser;

/**
 * Class for representing a syntax error problem.
 */
public class ParserException extends Exception {

  /** Serial number for serialization. */
  private static final long serialVersionUID = 201608241029L;

  /**
   * Default constructor
   */
  public ParserException() {
    // do nothing
  }

  /**
   * @param description
   * @param cause
   */
    public ParserException(String description, Exception cause) {
        super(description, cause);
  }

  /**
   * @param description
   */
  public ParserException(String description) {
    super(description);
  }

  /**
   * @param cause
   */
  public ParserException(Exception cause) {
    super(cause);
  }

}
