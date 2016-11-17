package pex.core;

public class Le extends BinaryExpression {

	public Le(Expression arg1, Expression arg2) {
		
		super(arg1, arg2); 
	}
	
	@Override
	public String getOperator() {
		
		return "le";
	}

}
