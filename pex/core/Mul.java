package pex.core;

public class Mul extends BinaryExpression {

	public Mul(Expression arg1, Expression arg2) {
		super(arg1, arg2); 
	}
	
	@Override
	public String getOperator() {
		return "mul";
	}

}
