package pex.core;

public class Call extends UnaryExpression {

	public Call(StringLiteral arg1) {
		super(arg1);  
	}

	@Override
	public String getOperator() {
		return "call";
	}

}
