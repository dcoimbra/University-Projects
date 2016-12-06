package pex.core;

import java.io.Serializable;
import java.util.HashSet;

public abstract class Expression implements Serializable {
	
	public boolean isIdentifier(){
		
		return false;
	}
	
	public HashSet<String> getIdentifiers() {
		
		return new HashSet<String>();
	}
	
	public HashSet<String> getInitializedIdentifiers() throws InvalidArgumentException {
		
		return new HashSet<String>();
	}
	
	public abstract String getAsText();
	
	public abstract Literal evaluate() throws InvalidArgumentException, NoSuchProgramException;
}
