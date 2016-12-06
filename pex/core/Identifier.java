package pex.core;

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

	
	public void accept(Visitor v) {
		v.visit(this);
	}
}