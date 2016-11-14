package pex.core;

import pt.utl.ist.po.ui.InvalidOperation;

@SuppressWarnings("serial")
public class ProgramNotFoundException extends InvalidOperation {

	public ProgramNotFoundException(String msg) {
		super(msg);
	}	
}
