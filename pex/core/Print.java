package pex.core;

import java.util.ArrayList;

import pex.AppIO;

public class Print extends VariadicExpression {
	
	AppIO _appIO;

	public Print(ArrayList<Expression> args, AppIO app) {
		
		super (args);
		_appIO = app;
	}

	@Override
	public String getOperator() {
		
		return "print";
	}

	@Override
	public Literal evaluate() throws InvalidArgumentException, NoSuchProgramException {
		
		Literal result = new IntegerLiteral(0);
		StringBuffer resultStringBuf = new StringBuffer();
        		
		for (Expression arg : getArguments()) {
			
			result = arg.evaluate();
			
			resultStringBuf.append(result.getValueAsString());   
		}
		
		String resultString = resultStringBuf.toString();
		
		_appIO.println(resultString);		
		return result;
	}
}