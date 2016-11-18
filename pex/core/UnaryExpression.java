package pex.core;

public abstract class UnaryExpression extends CompositeExpression {
	
	private Expression _arg1;
	
	public UnaryExpression(Expression arg1) {
		
		_arg1 = arg1;
	}
	
	public Expression getFirstArgument(){
		
		return _arg1;
	}
	
	@Override
	public String getAsText() {
		
		return "(" + getOperator() + " " 
				   + _arg1.getAsText() + ")";
	}

	public abstract String getOperator();

}
