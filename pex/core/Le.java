package pex.core;

public class Le extends BinaryExpressionInteger {

	public Le(Expression arg1, Expression arg2) {
		
		super(arg1, arg2); 
	}
	
	@Override
	public String getOperator() {
		
		return "le";
	}

	@Override
	public int evaluateInteger(int int1, int int2) {

		return (int1 <= int2 ? 1 : 0);
	}
}