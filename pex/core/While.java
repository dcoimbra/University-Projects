package pex.core;

public class While extends BinaryExpression {

	public While(Expression arg1, Expression arg2) {
		
		super(arg1, arg2); 
	}
	
	@Override
	public String getOperator() {
		
		return "while";
	}

	@Override
	public Literal evaluate() {
		
		while(true) {
			
			Literal arg1 = getFirstArgument().evaluate();
			
			if(!arg1.isIntegerLiteral()) {
				System.out.println("argumentos inválidos\n");
				return null;
			}
			
			if(((IntegerLiteral)arg1).intValue() == 0) {
				return new IntegerLiteral(0);
			}
			
			Literal arg2 = getSecondArgument().evaluate();			
		}

	}

}
