package pex.core;

public class Identifier extends Expression {
	
	private String _name;

	public Identifier(String idName) {
		_name = idName; 
	}

	@Override
	public String getAsText() {
		return _name;
	}
	
}
