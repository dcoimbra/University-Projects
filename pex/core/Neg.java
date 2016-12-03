package pex.core;

public class Neg extends UnaryExpressionInteger {

	public Neg(Expression arg1) {
		
		super(arg1); 
	}


	@Override
	public String getOperator() {
		
		return "neg";
	}


	@Override
	public int evaluateInteger(int int1) {
		return (-int1);
	}

}
