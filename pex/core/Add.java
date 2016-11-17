package pex.core;

public class Add extends BinaryExpression {

	public Add(Expression arg1, Expression arg2) {
		
		super(arg1, arg2);
	}

	@Override
	public String getOperator() {
		
		return "add";
	}

}
