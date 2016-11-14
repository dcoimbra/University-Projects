package pex.core;

import java.util.ArrayList;

public class Print extends VariadicExpression {

	public Print(ArrayList<Expression> args) {
		super(args);  
	}

	@Override
	public String getOperator() {
		return "print";
	}

}
