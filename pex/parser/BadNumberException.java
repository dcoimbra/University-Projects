/* $Id: $ */
package pex.parser;

/**
 * Exception for representing errors concerning invalid integer literals.
 */
public class BadNumberException extends ParserException {

  /** Serial number for serialization. */
  private static final long serialVersionUID = 201608241029L;

  /** Original number expression. */
  String _number;

  /**
   * @param description
   */
  public BadNumberException(String number) {
    super("Invalid integer literal: " + number);
    _number = number;
  }

  /**
   * @return the number
   */
  public String getNumber() {
    return _number;
  }

}
