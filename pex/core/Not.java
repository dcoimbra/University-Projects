package pex.core;

public class Not extends UnaryExpression {

	public Not(Expression arg1) {
		super(arg1);  
	}

	@Override
	public String getOperator() {
		return "not";
	}

}
