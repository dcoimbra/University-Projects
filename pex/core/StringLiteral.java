package pex.core;

public class StringLiteral extends Literal {
	
	String _value;
	
	public StringLiteral(String sLiteral) {
		
		_value = sLiteral; 
	}
	
	public String stringValue() {
		
		return _value;
	}

	@Override
	public String getAsText() {
		
		return "\"" + _value + "\"";
	}

	@Override
	public String getValueAsString() {
		return _value;
	}

}
