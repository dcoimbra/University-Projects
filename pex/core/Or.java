package pex.core;

public class Or extends BinaryExpression {

	public Or(Expression arg1, Expression arg2) {
		super(arg1, arg2); 
	}
	
	@Override
	public String getOperator() {
		return "or";
	}
}
