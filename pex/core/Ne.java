package pex.core;

public class Ne extends BinaryExpression {

	public Ne(Expression arg1, Expression arg2) {
		
		super(arg1, arg2); 
	}
	
	@Override
	public String getOperator() {
		
		return "ne";
	}
}
