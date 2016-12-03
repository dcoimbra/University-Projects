package pex.core;

public class Add extends BinaryExpressionInteger {

	public Add(Expression arg1, Expression arg2) {
		
		super(arg1, arg2);
	}

	@Override
	public String getOperator() {
		
		return "add";
	}

	@Override
	public int evaluateInteger(int int1, int int2) {

		return int1 + int2;
	}
}
