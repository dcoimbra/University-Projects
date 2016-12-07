package pex.core;

import java.util.TreeSet;

public class VisitorInitializedIdentifiers implements Visitor {
	
	private TreeSet<String> _allInitializedIdentifiers;
	
	public VisitorInitializedIdentifiers() {
		
		_allInitializedIdentifiers =  new TreeSet<String>();
	}
	
	@Override
	public void visit(CompositeExpression expression) {
		
		for (Expression exp : expression.getArguments()) {
			
			exp.accept(this);
		}
	}
	
	@Override
	public void visit(Set expression) {
		
		Expression arg1 = expression.getFirstArgument();
		_allInitializedIdentifiers.add(arg1.getAsText());
		
		Expression arg2 = expression.getSecondArgument();
		arg2.accept(this);
	}

	@Override
	public void visit(Identifier expression) {
		
	}

	@Override
	public void visit(Literal literal) {		
	
	}
	
	public TreeSet<String> getInitializedIdentifiers() {
		
		return _allInitializedIdentifiers;
	}

	@Override
	public void visit(Program program) {
		
		for (Expression expression : program.getExpressionList()) {
			
			expression.accept(this);
		}		
	}
}