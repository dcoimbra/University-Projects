package pex.core;

public class Mul extends BinaryExpressionInteger {

	public Mul(Expression arg1, Expression arg2) {
		
		super(arg1, arg2); 
	}
	
	@Override
	public String getOperator() {
		
		return "mul";
	}

	@Override
	public int evaluateInteger(int int1, int int2) {

		return int1 * int2;
	}
}