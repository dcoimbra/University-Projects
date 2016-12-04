package pex.core;

import java.util.HashSet;

public class Set extends BinaryExpression {
	private Program _program;

	public Set(Expression arg1, Expression arg2, Program program) {
		
		super(arg1, arg2);
		_program = program;
	}
	
	@Override
	public String getOperator() {
		
		return "set";
	}

	@Override
	public Literal evaluate() {
		
		Expression arg1 = getFirstArgument();
		
		if(!arg1.isIdentifier()) {
			System.out.println("argumentos inválidos.");
			return null;
		}
		
		Literal value = getSecondArgument().evaluate();
		_program.setIdentifierValue((Identifier)arg1, value);
		
		return value;
	}
	
	@Override
	public HashSet<String> getInitializedIdentifiers() {
		
		Expression arg1 = getFirstArgument();
		
		if(!arg1.isIdentifier()) {
			System.out.println("argumentos inválidos.");
			return null;
		}
		
		HashSet<String> result =  new HashSet<String>();
		result.add(arg1.getAsText());
		
		return result;
	}
	

}
