package pex.core;

public abstract class BinaryExpressionInteger extends BinaryExpression {

	public BinaryExpressionInteger(Expression arg1, Expression arg2) {
		super(arg1, arg2);
	}

	@Override
	public Literal evaluate() {
		Literal arg1 = getFirstArgument().evaluate();
		Literal arg2 = getSecondArgument().evaluate();
		
		if(arg1.isIntegerLiteral() && arg2.isIntegerLiteral()){
			int int1 = ((IntegerLiteral)arg1).intValue();
			int int2 = ((IntegerLiteral)arg2).intValue();
			
			return new IntegerLiteral(evaluateInteger(int1, int2));
		}
		
		System.out.println("argumentos inválidos\n");
		return null;
	}
	
	public abstract int evaluateInteger(int int1, int int2);

}
