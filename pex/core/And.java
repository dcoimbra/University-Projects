package pex.core;

public class And extends BinaryExpression {

	public And(Expression arg1, Expression arg2) {
		
		super(arg1, arg2); 
	}
	
	@Override
	public String getOperator() {
		
		return "and";
	}

}
