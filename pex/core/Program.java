package pex.core;

import java.io.Serializable;

import java.util.ArrayList;
import java.util.Collection;

/**
 * A program consists of a set of expressions. Can be executed by its interpreter.
 *
 * Can be stored in a file for posterior use.
 *
 * Grupo 4
 * @author Filipa Marques - 57842
 * @author David Coimbra  - 84708 
 */
public class Program implements java.io.Serializable {
	
	/**
	 * This program's name.
	 */
	private String _name;

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
	public Program(String programName) {
		
		_name = programName;
	}

	/**
	 * Gives the program's name.
	 *
	 * @return this program's name
	 */
	public String getProgramName(){
		
		return _name;
	}
	
	/**
	 * Stores the given expression in the position represented by the given index.
	 *
	 * @param idx position where the expression is stored
	 * @param expression expression to store
	 */ 
	public void add(int idx, Expression expression) {
		
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
	 * @see pex.core.Interpreter#setIdentifierValue(Identifier id, Literal value)
	 */
	public void setIdentifierValue(Identifier id, Literal value) {
		
		// TODO
	}

	/**
	 * @see pex.core.Interpreter#getIdentifierValue(Identifier id, Literal value)
	 */
	public Literal getIdentifierValue(Identifier id) {
		
		// TODO
		return null;
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
	 * Converts the entire program to a text format, in the program's language.
	 *
	 * The conversion is done expression to expression, and every expression is placed in a new line.
	 *
	 * @return the program in text format, as one String 
	 */
	public String getAsText() {
		
		String allExpressions = "";
		
		for(Expression expression : _expressions) {
		
			//call getAsText() for every expression and separate them in lines
			allExpressions = allExpressions + (expression.getAsText() + "\n");
		}
		
		return allExpressions;
	}
	
	/**
	 * Executes a program and returns its value (the same as the value of its last expression).
	 *
	 * @return the program's value.
	 */
	public Literal execute() {
		
		// TODO
		return null;
	}
}