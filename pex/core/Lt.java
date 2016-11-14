package pex.core;

public class Lt extends BinaryExpression {

	public Lt(Expression arg1, Expression arg2) {
		super(arg1, arg2); 
	}
	
	@Override
	public String getOperator() {
		return "lt";
	}
}
