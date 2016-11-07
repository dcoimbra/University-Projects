package pex.app;

import pt.utl.ist.po.ui.InvalidOperation;

/**
 * Exception for representing format errors when parsing a single expression.
 */
@SuppressWarnings("nls")
public class BadExpressionException extends InvalidOperation {
  
  /** Serial number for serialization. */
  private static final long serialVersionUID = 201608241029L;
  
  /** Original leaf expression. */
  String _description;

  /**
   * @param description
   */
  public BadExpressionException(String description) {
    _description = description;
  }
  
  /**
   * @return the description
   */
  public String getDescription() {
    return _description;
  }

  /** @see pt.tecnico.po.ui.DialogException#getMessage() */
  @Override
  public String getMessage() {
    return "Problemas na expressão: " + _description;
  }
}
