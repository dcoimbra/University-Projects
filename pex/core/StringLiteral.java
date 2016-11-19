package pex.core;

public class StringLiteral extends Literal {
	
	String _value;
	
	public StringLiteral(String sLiteral) {
		
		_value = sLiteral; 
	}

	@Override
	public String getAsText() {
		
		return "\"" + _value + "\"";
	}

}
