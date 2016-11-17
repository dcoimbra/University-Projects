package pex.core;

public class Ge extends BinaryExpression {

	public Ge(Expression arg1, Expression arg2) {
		
		super(arg1, arg2); 
	}
	
	@Override
	public String getOperator() {
		
		return "ge";
	}

}
