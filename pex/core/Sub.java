package pex.core;

public class Sub extends BinaryExpressionInteger {

	public Sub(Expression arg1, Expression arg2) {
		
		super(arg1, arg2); 
	}
	
	@Override
	public String getOperator() {
		
		return "sub";
	}

	@Override
	public int evaluateInteger(int int1, int int2) {

		return int1 - int2;
	}

}
