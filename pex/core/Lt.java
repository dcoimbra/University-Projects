package pex.core;

public class Lt extends BinaryExpressionInteger {

	public Lt(Expression arg1, Expression arg2) {
	
		super(arg1, arg2); 
	}
	
	@Override
	public String getOperator() {
	
		return "lt";
	}

	@Override
	public int evaluateInteger(int int1, int int2) {

		return (int1 < int2 ? 1 : 0);
	}
}
