package pex.core;

import pt.utl.ist.po.ui.InputInteger;

public class IntegerLiteral extends Literal {
	
	int _value;
	
	public IntegerLiteral(int iLiteral) {
		
		_value = iLiteral; 
	}
	
	public int intValue() {
		
		return _value;
	}
	
	@Override
	public boolean isIntegerLiteral(){
		 return true;
	}
	
	@Override
	public String getAsText() {
		
		return (Integer.toString(_value));
	}

}
