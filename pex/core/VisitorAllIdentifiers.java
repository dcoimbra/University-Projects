package pex.core;

import java.util.HashSet;

public class VisitorAllIdentifiers implements Visitor {
	
	private HashSet<String> _allIdentifiers;
	
	public VisitorAllIdentifiers() {
		
		_allIdentifiers =  new HashSet<String>();
	}
	
	public HashSet<String> getIdentifiers() {
		
		return _allIdentifiers;
	}
	
	public void visit(CompositeExpression expression) {
		
		for (Expression exp : expression.getArguments()) {
			
			exp.accept(this);
		}	
	}
	
	@Override
	public void visit(Set expression) {

		visit((CompositeExpression)expression);
	}

	@Override
	public void visit(Identifier exp) {
		
		_allIdentifiers.add(exp.getAsText());
	}

	@Override
	public void visit(Literal literal) {
				
	}

	@Override
	public void visit(Program program) {
		
		for (Expression expression : program.getExpressionList()) {
			
			expression.accept(this);
		}	
	}
}