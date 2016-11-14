package pex.core;

public class Mod extends BinaryExpression {

	public Mod(Expression arg1, Expression arg2) {
		super(arg1, arg2); 
	}
	
	@Override
	public String getOperator() {
		return "mod";
	}
}
