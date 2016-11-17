package pex.core;

public class Gt extends BinaryExpression {

	public Gt(Expression arg1, Expression arg2) {
		
		super(arg1, arg2); 
	}
	
	@Override
	public String getOperator() {
		
		return "gt";
	}
}
