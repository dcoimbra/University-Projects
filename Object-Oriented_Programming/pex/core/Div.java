package pex.core;

public class Div extends BinaryExpressionInteger {

	public Div(Expression arg1, Expression arg2) {
		
		super(arg1, arg2); 
	}
	
	@Override
	public String getOperator() {
		
		return "div";
	}

	@Override
	public int evaluateInteger(int int1, int int2) {

		return int1 / int2;
	}
}