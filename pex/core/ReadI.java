package pex.core;
import pex.AppIO;

public class ReadI extends CompositeExpression {
	
	AppIO _appIO;
	
	public ReadI(AppIO app) {
		_appIO = app;
	}
	
	@Override
	public String getAsText() { 
		
		return "(readi)";
	}

	@Override
	public Literal evaluate() {
		
		return new IntegerLiteral(_appIO.readInteger());
	}
}
