package pex.core;

import java.util.ArrayList;
import java.util.HashSet;
import java.util.List;

public abstract class TernaryExpression extends BinaryExpression {
	
	private Expression _arg3;
	
	public TernaryExpression(Expression arg1, Expression arg2, Expression arg3) {
		
		super(arg1, arg2);
		_arg3 = arg3;
	}
	
	public Expression getThirdArgument(){
		
		return _arg3;
	}
	
	@Override
	public List<Expression> getArguments() {
		List<Expression> args = new ArrayList<Expression>();
		args.add(getFirstArgument());
		args.add(getSecondArgument());
		args.add(_arg3);
		return args;
	}
	
	@Override
	public String getAsText() {
		
		return "(" + getOperator() + " " 
				   + getFirstArgument().getAsText() + " " 
				   + getSecondArgument().getAsText()+ " " 
				   + _arg3.getAsText() + ")";
	}

	public abstract String getOperator();

}
