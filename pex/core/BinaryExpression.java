package pex.core;

public abstract class BinaryExpression extends UnaryExpression {

	private Expression _arg2;
	
	public BinaryExpression(Expression arg1, Expression arg2) {
		super(arg1);
		_arg2 = arg2;
	}
	
	public Expression getSecondArgument(){
		return _arg2;
	}
	
	@Override
	public String getAsText() {
		return "(" + getOperator() + " " + getFirstArgument().getAsText() + " " + _arg2.getAsText() + ")";
	}

	public abstract String getOperator();
}
