package pex.core;

import java.io.Serializable;
import java.io.FileOutputStream;
import java.io.IOException;
import java.io.ObjectOutputStream;
import java.io.FileInputStream;
import java.io.ObjectInputStream;



import java.util.HashMap;
import java.util.Map;

import pex.AppIO;

import pex.app.main.Message;

import pex.parser.NewParser;
import pex.parser.BadSourceException;
import pex.parser.ParserException;

import pt.utl.ist.po.ui.Form;
import pt.utl.ist.po.ui.InputString;
import pt.utl.ist.po.ui.Display;
import pt.utl.ist.po.ui.InvalidOperation;

/**
 * Main core entity. Manages and executes programs.
 *
 * Supports a simple programming language.
 *
 * Grupo 4
 * @author Filipa Marques - 57842
 * @author David Coimbra  - 84708 
 */
public class Interpreter implements java.io.Serializable {
	
	/**
	 * Structure where programs are saved. Programs have their names as keys.
	 *
	 */
	private Map<String, Program> _programs = new HashMap<>();

	/**
	 * Interface for user interaction.
	 *
	 * @see pex.AppIO
     */
	private AppIO _app;

	/**
	 * Filename for saving the interpreter. Default value is null.
	 */
	private String _fileAssociation;

	/**
	 * Flag. True if the file was changed since the latest saving.
	 *
	 * Initial value is true.
	 */
	private boolean _wasChanged = true;

	/**
	 * Class constructor.
	 *
	 * @param app the current app for this session
	 */
	public Interpreter(AppIO app) {

		_app = app;
	}
	
	/**
	 * Sets the value for a given identifier.
	 *
	 * @param id the given identifier
	 * @param value the value to set
	 */
	public void setIdentifierValue(Identifier id, Literal value) {

		// TODO  
	}

	/**
	 * Gives the value associated to the given identifier.
	 *
	 * @param id the given identifier
	 * @return the associated value
	 */
	public Literal getIdentifierValue(Identifier id) {

		// TODO  
		return null;
	}
	
	/**
	 * Adds a program to the interpreter's structure.
	 * 
	 * @param program the program to be added.
	 */
	public void addProgram(Program program) {

		_programs.put(program.getProgramName(), program);
	}
	
	/**
	 * Gives the program referenced by the given name.
	 *
	 * @param name the program's name.
	 * @return the searched program if it exists, null if it doesn't
	 */
	public Program getProgram(String name) {

		return _programs.get(name);
	}

	/**
	 * Gives the associated interface for user interaction
	 */
	public AppIO getAppIO() {
		
		return _app;
	}

	/**
	 * Associates this interpreter to the given file
	 */
	public void setFileAssociation(String filename) {

		_fileAssociation = filename;
	}

	/**
	 * Gives the associated file's name
	 *
	 * @return the associated filename
	 */
	public String getFileAssociation() {
		  
		return _fileAssociation;
	}

	/**
	 * Checks if the interpreter was changed since the last saving.
	 *
	 * @return true if the interpreter was changed, false if it wasn't.
	 */
	public boolean wasChanged() {

		return _wasChanged;
	}

	/**
	 * Signals if the interpreter was changed since the last saving.
	 */
	public void setWasChangedFlag(boolean status) {

		_wasChanged = status;
	}

	public void save(String fileName) throws IOException {

        FileOutputStream fpOut = new FileOutputStream(fileName);
		ObjectOutputStream out = new ObjectOutputStream(fpOut);
        out.writeObject(this);
	            
        out.flush();
        out.close();
	}

	public Interpreter load(String fileName) throws IOException, ClassNotFoundException {

		FileInputStream fpIn = new FileInputStream(fileName);
        ObjectInputStream inInterp = new ObjectInputStream(fpIn);

        Interpreter loadedInterpreter = (Interpreter)inInterp.readObject();

        inInterp.close();
            
        loadedInterpreter.setFileAssociation(fileName);
        loadedInterpreter.setWasChangedFlag(false);
        
        return loadedInterpreter;
	}

	public void readProgramFromFile(String fileName, String programName) throws BadSourceException, ParserException {

		NewParser parser = new NewParser();        
	    Program program = parser.parseFile(fileName, programName, this);

	    this.addProgram(program);

	    this.setWasChangedFlag(true);
	}
}