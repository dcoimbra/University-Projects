package pex.core;

import java.util.Collection;

public class Program {
	private String _name;
	
	
	public Program(String programName) {
		_name = programName;
	}

	public String getProgramName(){
		return _name;
	}
	
	public void add(int idx, Expression expression) {
		// TODO 
		
	}
	
	public void replace(int idx, Expression expression) {
		// TODO 
		
	}
	
	public void setIdentifierValue(Identifier id, Literal value) {
		// TODO
	}

	public Literal getIdentifierValue(Identifier id) {
		// TODO
		return null;
	}
	
	
	public void set(Collection<Expression> expressions) { //está no newParser...
		// TODO
		
	}
	
	
	public String getAsText() {
		// TODO
		return "";
	}
	
	
	public Literal execute() {
		// TODO
		return null;
	}
}
