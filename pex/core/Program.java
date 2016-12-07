package pex.core;

import java.io.Serializable;
import java.io.BufferedWriter;
import java.io.FileWriter;
import java.io.IOException;
import java.io.PrintWriter;

import java.util.ArrayList;
import java.util.Collection;
import java.util.Collections;
import java.util.TreeSet;

/**
 * A program consists of a set of expressions. Can be executed by its interpreter.
 *
 * Can be stored in a file for posterior use.
 *
 * @author Grupo 4
 * @author Filipa Marques - 57842
 * @author David Coimbra  - 84708 
 */
public class Program implements Serializable, Element {
	
	/**
	 * This program's name.
	 */
	private String _name;
	private Interpreter _currentInterpreter;

	/**
	 * Structure for storing the program's expressions.
	 *
	 * The expressions are stored and retrieved by their position index.
	 */
	private ArrayList<Expression> _expressions = new ArrayList<>();
	
	/**
	 * Class constructor.
	 *
	 * @param programName this program's name.
	 */
	public Program(String programName, Interpreter currentInterpreter) {
		
		_name = programName;
		_currentInterpreter = currentInterpreter;
	}

	/**
	 * @return the current program's name
	 */
	public String getName() {
		
		return _name;
	}
	
	/**
	 * @return the program with the given name
	 */
 	public Program getProgram(String name) {
		
		return _currentInterpreter.getProgram(name);
	}

	/**
	 * @return the current interpreter
	 */
	public Interpreter getCurrentInterpreter() {

		return _currentInterpreter;
	}
	
	/**
	 * @return this program's expressions
	 */
	public ArrayList<Expression> getExpressionList() {
	
		return _expressions;
	}
	
	/**
	 * Sets the program's expressions to the given collection of expressions.
	 *
	 * The collection is "converted" to an ArrayList.
	 * 
	 * @param expressions collection of expressions to set 
	 */
	public void set(Collection<Expression> expressions) {
		
		_expressions = new ArrayList<>(expressions);	
	}
	
	/**
	 * Stores the given expression in the position represented by the given index.
	 *
	 * If sucessful, shifts the expression currently at that position (if any) and any subsequent elements to the right.
	 *
	 * @param idx position where the expression is stored
	 * @param expression expression to store
	 * 
	 * @throws IndexOutOfBoundsException if idx is out of range
	 */ 
	public void add(int idx, Expression expression) throws IndexOutOfBoundsException {
		
		_expressions.add(idx, expression);
	}
	
	/**
	 * Replaces the expression in the given position by the given expression.
	 *
	 * @param idx position to be replaced
	 * @param expression expression to replace
	 */
	public void replace(int idx, Expression expression) {
		
		_expressions.set(idx, expression);
	}
	
	/**
	 * Sets the value for a given identifier.
	 *
	 * @param id the given identifier
	 * @param value the value to set
	 */
	public void setIdentifierValue(Identifier id, Literal value) {
		
		_currentInterpreter.setIdentifierValue(id, value);
	}

	/**
	 * Returns the value associated to the given identifier.
	 *
	 * @param id the given identifier
	 * @return the associated value
	 */
	public Literal getIdentifierValue(Identifier id) {
		
		return _currentInterpreter.getIdentifierValue(id);
	}
	
	/**
	 * Accepts a visit by a given visitor
	 *
	 * @param the given visitor
	 */
	@Override
	public void accept(Visitor v) {
		
		v.visit(this);	
	}
	
	/**
	 * Returns the identifiers shared by the programs.
	 *
	 * @return set that contains all identifiers
	 */
	public ArrayList<String> getAllIdentifiers() throws InvalidArgumentException {
		
		VisitorAllIdentifiers allIdentifiers = new VisitorAllIdentifiers();
		TreeSet<String> ids;
		ArrayList<String> result;
		
		accept(allIdentifiers);
		
		ids = allIdentifiers.getIdentifiers();
		result = new ArrayList<String>(ids);

		return result;
	}
	
	/**
	 * Returns the initialized identifiers shared by the programs.
	 *
	 * @return set that contains all initialized identifiers
	 */
	private ArrayList<String> getInitializedIdentifiers() throws InvalidArgumentException {
		
		VisitorInitializedIdentifiers allInitializedIdentifiers = new VisitorInitializedIdentifiers();
		TreeSet<String> ids;
		ArrayList<String> result;
		
		accept(allInitializedIdentifiers);
		
		ids = allInitializedIdentifiers.getInitializedIdentifiers();
		result = new ArrayList<String>(ids);

		return result;	
	}
	
	/**
	 * Returns the unitialized identifiers shared by the programs.
	 * 
	 * @return set that contains all unitialized identifiers
	 */
	public ArrayList<String> getUnitilializedIdentifiers() throws InvalidArgumentException {
		
		ArrayList<String> result = getAllIdentifiers();
		
		result.removeAll(getInitializedIdentifiers());
		
		return result;
	}
	
	/**
	 * Converts the entire program to a text format, in the program's language.
	 *
	 * The conversion is done expression by expression, and every expression is placed in a new line.
	 *
	 * @return the program in text format, as one String 
	 */
	public String getAsText() {
		
		StringBuilder allExpressionsBuf = new StringBuilder();
		
		for(Expression expression : _expressions) {
		
			//call getAsText() for every expression and separate them in lines
			allExpressionsBuf.append(expression.getAsText() + "\n");
		}
		
		String allExpressions = allExpressionsBuf.toString();
		return allExpressions;
	}

	/**
	 * Writes the program's textual form to a file.
	 *
	 * @param file to write in
	 */
	public void writeToFile(String fileName) throws IOException {

		String programCode = this.getAsText();

		FileWriter out = new FileWriter(fileName);
	    BufferedWriter buffer = new BufferedWriter(out);
	    PrintWriter printer = new PrintWriter(buffer);
	
   		printer.write(programCode);
	   	printer.close();
	}
	
	/**
	 * Executes a program and returns its value (the same as the value of its last expression).
	 *
	 * @return the program's value.
	 */
	public Literal execute() throws InvalidArgumentException, NoSuchProgramException {
		
		Literal result = new IntegerLiteral(0);
		
		for (Expression expression : _expressions) {
			
			result = expression.evaluate();
		}
		
		return result;
	}
}