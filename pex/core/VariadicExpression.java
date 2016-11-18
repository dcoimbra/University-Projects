package pex.core;

import java.util.ArrayList;
import java.util.List;

public abstract class VariadicExpression extends CompositeExpression {
	
	private ArrayList<Expression>_args;
	
	public VariadicExpression(ArrayList<Expression> args) {
	
		_args = args;
	}
	
	public List<Expression> getArguments(){
	
		return _args;
	}
	
	@Override
	public String getAsText() {
		
		String asText = "(" + getOperator();
		
		for(Expression argument : _args) {
	
			asText += " " + argument.getAsText();
		}
		
		return asText + ")\n";
	}

	public abstract String getOperator();
}
