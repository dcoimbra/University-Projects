package pex.core;

public class Sub extends BinaryExpression {

	public Sub(Expression arg1, Expression arg2) {
		
		super(arg1, arg2); 
	}
	
	@Override
	public String getOperator() {
		return "sub";
	}

}
