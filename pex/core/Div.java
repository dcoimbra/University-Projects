package pex.core;

public class Div extends BinaryExpression {

	public Div(Expression arg1, Expression arg2) {
		
		super(arg1, arg2); 
	}
	
	@Override
	public String getOperator() {
		
		return "div";
	}

}
