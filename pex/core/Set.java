package pex.core;

public class Set extends BinaryExpression {

	public Set(Expression arg1, Expression arg2) {
		
		super(arg1, arg2); 
	}
	
	@Override
	public String getOperator() {
		
		return "set";
	}

}
