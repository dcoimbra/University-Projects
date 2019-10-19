package pex.core;

public class Eq extends BinaryExpressionInteger {

	public Eq(Expression arg1, Expression arg2) {
		
		super(arg1, arg2); 
	}
	
	@Override
	public String getOperator() {
		
		return "eq";
	}

	@Override
	public int evaluateInteger(int int1, int int2) {

		return (int1 == int2 ? 1 : 0);
	}
}