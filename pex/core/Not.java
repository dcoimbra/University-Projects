package pex.core;

public class Not extends UnaryExpressionInteger {

	public Not(Expression arg1) {
		
		super(arg1);  
	}

	@Override
	public String getOperator() {
		
		return "not";
	}

	@Override
	public int evaluateInteger(int int1) {
		
		return (int1 == 0 ? 1 : 0);
	}

}
