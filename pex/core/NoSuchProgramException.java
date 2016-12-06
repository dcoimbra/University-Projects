package pex.core;

public class NoSuchProgramException extends Exception {

	public NoSuchProgramException(String programName) {

		super("No such program: " + programName);
	}
}