package pex.core;

import java.util.ArrayList;
import java.util.HashSet;
import java.util.List;

public abstract class CompositeExpression extends Expression {

	public List<Expression> getArguments() {
		
		return new ArrayList<Expression>();
	}
	
	public HashSet<String> getIdentifiers() {
		
		HashSet<String> result =  new HashSet<String>();
		
		for (Expression expression : getArguments()) {
			
			result.addAll(expression.getIdentifiers());
		}
		
		return result;
	}
	
	public HashSet<String> getInitializedIdentifiers() throws InvalidArgumentException {
		
		HashSet<String> result =  new HashSet<String>();
		
		for (Expression expression : getArguments()) {
			
			result.addAll(expression.getInitializedIdentifiers());
		}
		
		return result;
	}
}
