package pex.core;

public class IntegerLiteral extends Literal {
	
	int _value;
	
	public IntegerLiteral(int iLiteral) {
		
		_value = iLiteral; 
	}

	@Override
	public String getAsText() {
		
		return Integer.toString(_value);
	}

}
