package pex.core;

public class Or extends BinaryExpression {

	public Or(Expression arg1, Expression arg2) {
		
		super(arg1, arg2); 
	}
	
	@Override
	public String getOperator() {
		
		return "or";
	}

	@Override
	public Literal evaluate() throws InvalidArgumentException, NoSuchProgramException {

		Literal arg1 = getFirstArgument().evaluate();
		
		if (!arg1.isIntegerLiteral()) {
			
			throw new InvalidArgumentException(this);
		}
		
		int int1 = ((IntegerLiteral)arg1).intValue();
		
		if (int1 != 0) {
			
			return new IntegerLiteral(1);
		}
		
		Literal arg2 = getSecondArgument().evaluate();
		
		if (!arg2.isIntegerLiteral()) {
			
			throw new InvalidArgumentException(this);
		}
		
		int int2 = ((IntegerLiteral)arg2).intValue();
		
		if (int2 != 0) {
			
			return new IntegerLiteral(1);
		}
		
		return new IntegerLiteral(0);
	}
}