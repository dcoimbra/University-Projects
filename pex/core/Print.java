package pex.core;

import java.util.ArrayList;

import pex.AppIO;

public class Print extends VariadicExpression {
	AppIO _appIO;

	public Print(ArrayList<Expression> args, AppIO app) {
		
		super(args);
		_appIO = app;
	}

	@Override
	public String getOperator() {
		
		return "print";
	}

	@Override
	public Literal evaluate() {
		
		Literal result = new IntegerLiteral(0);
		String resultString = "";
        		
		for(Expression arg: getArguments()) {
			
			result = arg.evaluate();
			
			resultString += result.getValueAsString();   
		}
		
		_appIO.println(resultString);
		return result;
	}

}
