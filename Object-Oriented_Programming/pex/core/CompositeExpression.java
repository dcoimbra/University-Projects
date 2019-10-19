package pex.core;

import java.util.ArrayList;
import java.util.List;

public abstract class CompositeExpression extends Expression {

	public List<Expression> getArguments() {
		
		return new ArrayList<Expression>();
	}
	
	public void accept(Visitor v) {
		
		v.visit(this);
	}	
}