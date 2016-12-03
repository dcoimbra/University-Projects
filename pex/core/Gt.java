package pex.core;

public class Gt extends BinaryExpressionInteger {

	public Gt(Expression arg1, Expression arg2) {
		
		super(arg1, arg2); 
	}
	
	@Override
	public String getOperator() {
		
		return "gt";
	}

	@Override
	public int evaluateInteger(int int1, int int2) {

		return (int1 > int2 ? 1 : 0);
	}
}
