package pex.core;

import java.util.ArrayList;

public class Seq extends VariadicExpression {

	public Seq(ArrayList<Expression> args) {
		
		super(args);  
	}


	@Override
	public String getOperator() {
		
		return "seq";
	}


	@Override
	public Literal evaluate() throws InvalidArgumentException, NoSuchProgramException {
		
		Literal result = new IntegerLiteral(0);
		
		for(Expression arg: getArguments()) {
			
			result = arg.evaluate();
		}
		
		return result;
	}

}
