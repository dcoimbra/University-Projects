package pex.core;

public class Neg extends UnaryExpression {

	public Neg(Expression arg1) {
		
		super(arg1); 
	}


	@Override
	public String getOperator() {
		
		return "neg";
	}

}
