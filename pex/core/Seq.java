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

}
