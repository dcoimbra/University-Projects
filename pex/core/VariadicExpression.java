package pex.core;

import java.util.ArrayList;
import java.util.List;

public abstract class VariadicExpression extends CompositeExpression {
	
	private ArrayList<Expression>_args;
	
	public VariadicExpression(ArrayList<Expression> args) {
	
		_args = args;
	}
	
	@Override
	public List<Expression> getArguments(){
	
		return _args;
	}
	
	@Override
	public String getAsText() {
		
		StringBuffer asTextBuf = new StringBuffer("(" + getOperator());
		
		for (Expression argument : _args) {
	
			asTextBuf.append(" " + argument.getAsText());
		}
		
		String asText = asTextBuf.toString();

		return asText + ")";
	}

	public abstract String getOperator();
}