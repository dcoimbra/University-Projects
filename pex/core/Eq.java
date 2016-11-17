package pex.core;

public class Eq extends BinaryExpression {

	public Eq(Expression arg1, Expression arg2) {
		
		super(arg1, arg2); 
	}
	
	@Override
	public String getOperator() {
		
		return "eq";
	}

}
