package pex.core;

import java.io.*;

import java.util.HashMap;
import java.util.Map;

import pex.AppIO;

public class Interpreter implements java.io.Serializable {
	
	private Map<String, Program> _programs;
	private AppIO _app;
	private String _fileAssociation = null;

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

		_fileAssociation = filename;
	}

	public String getFileAssociation() {
		  
		return _fileAssociation;
	}
}
