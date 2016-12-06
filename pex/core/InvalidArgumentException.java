package pex.core;

public class InvalidArgumentException extends Exception {

	public InvalidArgumentException(Expression arg1) {

		super("Invalid argument: " + arg1.getAsText());
	}

	public InvalidArgumentException(Expression arg1, Expression arg2) {

		super("Invalid arguments: " + arg1.getAsText() + ", " + arg2.getAsText());
	}
}