package pex.core;


import java.util.HashMap;
import java.util.Map;

import pex.AppIO;

public class Interpreter {
	
	private Map<String, Program> _programs;
	private AppIO _app;

	public Interpreter(AppIO app) {
		_programs = new HashMap<>();
		_app = app;
	}
	
	public void setIdentifierValue(Identifier id, Literal value) {
		// TODO  
	}

	public Literal getIdentifierValue(Identifier id) {
		// TODO  
		return null;
	}
	
	public void addProgram(Program program) {
		_programs.put(program.getProgramName(), program);
	}
	
	public Program getProgram(String name) {
		if (_programs.containsKey(name)) {
			return _programs.get(name);
		}
		return null;
	}

	public AppIO getAppIO() {
		
		return _app;
	}

	public void setFileAssociation(String filename) {
		// TODO  
		
	}

	public String getFileAssociation() {
		// TODO  
		return null;
	}
}
