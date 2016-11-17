package pex.core;

public class If extends TernaryExpression {

	public If(Expression arg1, Expression arg2, Expression arg3) {
		
		super(arg1, arg2, arg3); 
	}
	
	@Override
	public String getOperator() {
		
		return "if";
	}

}
