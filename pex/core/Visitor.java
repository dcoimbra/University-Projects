package pex.core;

public interface Visitor {
	
	public abstract void visit(CompositeExpression expression);
	
	public abstract void visit(Set expression);

	public abstract void visit(Identifier expression);
	
	public abstract void visit(Literal literal);
	
	public abstract void visit(Program program);
}
