package pex.core;

import java.util.ArrayList;
import java.util.List;

public abstract class BinaryExpression extends UnaryExpression {

	private Expression _arg2;
	
	public BinaryExpression(Expression arg1, Expression arg2) {
		
		super(arg1);
		_arg2 = arg2;
	}
	
	public Expression getSecondArgument(){
		
		return _arg2;
	}
	
	@Override
	public List<Expression> getArguments() {
		
		List<Expression> args = new ArrayList<Expression>();
		
		args.add(getFirstArgument());
		args.add(_arg2);
		
		return args;
	}
	
	@Override
	public String getAsText() {
		
		return ("(" + getOperator() + " " 
					+ getFirstArgument().getAsText() + " " 
					+ _arg2.getAsText() + ")");
	}

	public abstract String getOperator();
}