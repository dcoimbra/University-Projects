package pex.core;

import java.io.*;

import java.util.ArrayList;
import java.util.Collection;

public class Program implements java.io.Serializable {
	private String _name;
	private ArrayList<Expression> _expressions;
	
	
	public Program(String programName) {
		_name = programName;
		_expressions = new ArrayList<>();
	}

	public String getProgramName(){
		return _name;
	}
	
	public void add(int idx, Expression expression) {
		_expressions.add(idx, expression);
	}
	
	public void replace(int idx, Expression expression) {
		_expressions.set(idx, expression);
		
	}
	
	public void setIdentifierValue(Identifier id, Literal value) {
		// TODO
	}

	public Literal getIdentifierValue(Identifier id) {
		// TODO
		return null;
	}
	
	
	public void set(Collection<Expression> expressions) {
		_expressions = new ArrayList<>(expressions);
		
	}
	
	
	public String getAsText() {
		
		String allExpressions = "";
		
		for(Expression expression : _expressions){
			allExpressions += expression.getAsText();
		}
		
		return allExpressions;
	}
	
	
	public Literal execute() {
		// TODO
		return null;
	}
}
