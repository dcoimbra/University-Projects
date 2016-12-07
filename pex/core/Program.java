package pex.core;

import java.io.Serializable;
import java.io.BufferedWriter;
import java.io.FileWriter;
import java.io.IOException;
import java.io.PrintWriter;

import java.util.ArrayList;
import java.util.Collection;
import java.util.Collections;
import java.util.HashSet;

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
	 * Returns the program's name.
	 *
	 * @return the current program's name
	 */
	public String getName() {
		
		return _name;
	}
	
	/**
	 * Returns the program with the given name.
	 *
	 * @return the program with the given name
	 */
 	public Program getProgram(String name) {
		
		return _currentInterpreter.getProgram(name);
	}

	public Interpreter getCurrentInterpreter() {

		return _currentInterpreter;
	}
	
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
	private HashSet<String> getAllIdentifiers() {
		
		VisitorAllIdentifiers allIdentifiers = new VisitorAllIdentifiers();
		
		accept(allIdentifiers);
		
		return allIdentifiers.getIdentifiers();	
	}
	
	/**
	 * Returns the initialized identifiers shared by the programs.
	 *
	 * @return set that contains all initialized identifiers
	 */
	private HashSet<String> getInitializedIdentifiers() throws InvalidArgumentException {
		
		VisitorInitializedIdentifiers allInitializedIdentifiers = new VisitorInitializedIdentifiers();
		
		accept(allInitializedIdentifiers);
		
		return allInitializedIdentifiers.getInitializedIdentifiers();	
	}
	
	/**
	 * Returns the unitialized identifiers shared by the programs.
	 * 
	 * @return set that contains all unitialized identifiers
	 */
	private HashSet<String> getUnitilializedIdentifiers() throws InvalidArgumentException {
		
		HashSet<String> result = getAllIdentifiers();
		
		result.removeAll(getInitializedIdentifiers());
		
		return result;
	}
	
	/**
	 * Sorts a set of given identifiers
	 *
	 * @return sorted identifier list
	 */
	private ArrayList<String> sortIdentifiers(HashSet<String> ids) throws InvalidArgumentException {
		
		ArrayList<String> result = new ArrayList<String>(ids); 
		
		Collections.sort(result);
		
		return result;
	}
	
	/**
	 * @return sorted identifier list 
	 */
	public ArrayList<String> getSortedIdentifiers() throws InvalidArgumentException {
		
		return sortIdentifiers(getAllIdentifiers());
	}

	/**
	 * @return sorted list of uninitialized identifiers
	 */
	public ArrayList<String> getSortedUninitializedIdentifiers() throws InvalidArgumentException {
		
		return sortIdentifiers(getUnitilializedIdentifiers());
	}
	
	/**
	 * Converts the entire program to a text format, in the program's language.
	 *
	 * The conversion is done expression by expression, and every expression is placed in a new line.
	 *
	 * @return the program in text format, as one String 
	 */
	public String getAsText() {
		
		StringBuffer allExpressionsBuf = new StringBuffer();
		
		for(Expression expression : _expressions) {
		
			//call getAsText() for every expression and separate them in lines
			allExpressionsBuf.append(expression.getAsText() + "\n");
		}
		
		String allExpressions = allExpressionsBuf.toString();
		return allExpressions;
	}

	/**
	 * Writes the program's textual form to a file.
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