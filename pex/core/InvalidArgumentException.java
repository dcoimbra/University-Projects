package pex.core;

public class InvalidArgumentException extends Exception {

	public InvalidArgumentException(Expression exp) {

		super("Invalid arguments: " + exp.getAsText());
	}

}