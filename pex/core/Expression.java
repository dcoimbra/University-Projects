package pex.core;

import java.io.Serializable;

public abstract class Expression implements java.io.Serializable{
	
	public boolean isIdentifier(){
		return false;
	}
	
	public abstract String getAsText();
	
	public abstract Literal evaluate();

}
