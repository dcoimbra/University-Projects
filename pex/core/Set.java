package pex.core;

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
	public Literal evaluate() throws InvalidArgumentException, NoSuchProgramException {
		
		Expression arg1 = getFirstArgument();
		
		if (!arg1.isIdentifier()) {
			
			throw new InvalidArgumentException(arg1);
		}
		
		Literal value = getSecondArgument().evaluate();
		
		_program.setIdentifierValue((Identifier)arg1, value);
		
		return value;
	}
	
	@Override
	public void accept(Visitor v) {
		
		v.visit(this);
	}
}