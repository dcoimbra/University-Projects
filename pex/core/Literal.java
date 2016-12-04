package pex.core;

public abstract class Literal extends Expression {

	public Literal evaluate() {
		return this;
	}
	
	public boolean isIntegerLiteral(){
		 return false;
	}
	
	public abstract String getValueAsString();
}
