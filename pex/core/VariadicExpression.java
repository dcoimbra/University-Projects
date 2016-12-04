package pex.core;

import java.util.ArrayList;
import java.util.HashSet;
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
		
		String asText = "(" + getOperator();
		
		for(Expression argument : _args) {
	
			asText += " " + argument.getAsText();
		}
		
		return asText + ")";
	}

	public abstract String getOperator();
}
