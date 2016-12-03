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
	public Literal evaluate() {
		
		StringLiteral literalName = (StringLiteral) getFirstArgument();
		
		Program program = _program.getProgram(literalName.stringValue());
		
		if (program == null) {
			System.out.println("o programa não existe... :( ");
			return new IntegerLiteral(0);
		}
		
		return program.execute();
	}

}
