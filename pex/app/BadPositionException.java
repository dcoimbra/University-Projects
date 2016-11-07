package pex.app;

import pt.utl.ist.po.ui.InvalidOperation;

/**
 * Exception for representing an invalid index for placing an expression.
 */
@SuppressWarnings("nls")
public class BadPositionException extends InvalidOperation {
  
  /** Serial number for serialization. */
  private static final long serialVersionUID = 201608241029L;
  
  /** Invalid index */
  int _position;

  /**
   * @param position
   */
  public BadPositionException(int position) {
    _position = position;
  }
  
  /**
   * @return the position
   */
  public int getPosition() {
    return _position;
  }

  /** @see pt.tecnico.po.ui.DialogException#getMessage() */
  @Override
  public String getMessage() {
    return "Posição inválida: " + _position;
  }
  
}
