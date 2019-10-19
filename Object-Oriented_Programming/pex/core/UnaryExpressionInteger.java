package pex.core;

public abstract class UnaryExpressionInteger extends UnaryExpression {

	public UnaryExpressionInteger(Expression arg1) {
		
		super(arg1);
	}

	@Override
	public final Literal evaluate() throws InvalidArgumentException, NoSuchProgramException {
		
		Literal arg1 = getFirstArgument().evaluate();
		
		if (arg1.isIntegerLiteral()){
			
			int int1 = ((IntegerLiteral)arg1).intValue();
			
			return new IntegerLiteral(evaluateInteger(int1));
		}
		
		throw new InvalidArgumentException(this);
	}
	
	public abstract int evaluateInteger(int int1);

}
