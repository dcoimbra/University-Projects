package pex.core;

public class Call extends UnaryExpression {
	
	private Program _program;

	public Call(StringLiteral arg1, Program program) {
		
		super(arg1);
		_program = program;
	}

	@Override
	public String getOperator() {
		
		return "call";
	}

	@Override
	public Literal evaluate() throws InvalidArgumentException, NoSuchProgramException {
		
		StringLiteral literalName = (StringLiteral) getFirstArgument();
		
		Program program = _program.getProgram(literalName.stringValue());
		
		if (program == null) {
			
			throw new NoSuchProgramException(literalName.stringValue());
		}
		
		return program.execute();
	}
}