package pex.core;
import pex.AppIO;
import pex.app.App;

public class ReadS extends CompositeExpression {
	AppIO _appIO;

	public ReadS(AppIO app) {
		_appIO = app;
	}
	
	@Override
	public String getAsText() { 
		
		return "(reads)";
	}

	@Override
	public Literal evaluate() {
		
		return new StringLiteral(_appIO.readString());
	}

}
