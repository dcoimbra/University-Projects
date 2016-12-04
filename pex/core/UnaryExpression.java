package pex.core;

import java.util.ArrayList;
import java.util.HashSet;
import java.util.List;

public abstract class UnaryExpression extends CompositeExpression {
	
	private Expression _arg1;
	
	public UnaryExpression(Expression arg1) {
		
		_arg1 = arg1;
	}
	
	public Expression getFirstArgument(){
		
		return _arg1;
	}
	
	@Override
	public List<Expression> getArguments() {
		List<Expression> args = new ArrayList<Expression>();
		args.add(getFirstArgument());

		return args;
	}
	
	@Override
	public String getAsText() {
		
		return "(" + getOperator() + " " 
				   + _arg1.getAsText() + ")";
	}

	public abstract String getOperator();

}
