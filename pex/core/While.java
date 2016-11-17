package pex.core;

public class While extends BinaryExpression {

	public While(Expression arg1, Expression arg2) {
		
		super(arg1, arg2); 
	}
	
	@Override
	public String getOperator() {
		
		return "while";
	}

}
