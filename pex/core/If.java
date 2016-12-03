package pex.core;

public class If extends TernaryExpression {

	public If(Expression arg1, Expression arg2, Expression arg3) {
		
		super(arg1, arg2, arg3); 
	}
	
	@Override
	public String getOperator() {
		
		return "if";
	}

	@Override
	public Literal evaluate() {
		
		Literal arg1 = getFirstArgument().evaluate();
		
		if(!arg1.isIntegerLiteral()) {
			System.out.println("argumentos inválidos\n");
			return null;
		}
		
		if(((IntegerLiteral)arg1).intValue() != 0) {
			Literal arg2 = getSecondArgument().evaluate();
			return arg2;
		}
		
		Literal arg3 = getThirdArgument().evaluate();
		return arg3;
	}

}
