package pex.core;

public class Mod extends BinaryExpressionInteger {

	public Mod(Expression arg1, Expression arg2) {
		
		super(arg1, arg2); 
	}
	
	@Override
	public String getOperator() {
		
		return "mod";
	}

	@Override
	public int evaluateInteger(int int1, int int2) {

		return int1 % int2;
	}
}
