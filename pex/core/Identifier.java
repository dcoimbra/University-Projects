package pex.core;

import java.util.HashSet;

public class Identifier extends Expression {
	
	private String _name;
	private Program _program;

	public Identifier(String idName, Program program) {
		
		_name = idName; 
		_program = program;		
	}
	
	
	@Override
	public boolean isIdentifier() {
		return true;
	}

	@Override
	public String getAsText() {
		
		return _name;
	}

	@Override
	public Literal evaluate() {
		 
		return _program.getIdentifierValue(this);
	}


	@Override
	public HashSet<String> getIdentifiers() {
		
		HashSet<String> result =  new HashSet<String>();
		result.add(_name);
		
		return result;
	}
	
}
