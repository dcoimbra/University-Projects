package pex.core;

public abstract class TernaryExpression extends BinaryExpression {
	
	private Expression _arg3;
	
	public TernaryExpression(Expression arg1, Expression arg2, Expression arg3) {
		super(arg1, arg2);
		_arg3 = arg3;
	}
	
	public Expression getThirdArgument(){
		return _arg3;
	}
	
	@Override
	public String getAsText() {
		return "(" + getOperator() + " " + getFirstArgument().getAsText() + " " + getSecondArgument().getAsText()+ " " + _arg3.getAsText() + ")";
	}

	public abstract String getOperator();

}
