package pex.app.main;

import java.io.IOException;

//FIXME import used core classes
import pt.utl.ist.po.ui.Command;
import pt.utl.ist.po.ui.Display;
import pt.utl.ist.po.ui.Form;
import pt.utl.ist.po.ui.InputString;
import pt.utl.ist.po.ui.InvalidOperation;

/**
 * Write (save) program to file.
 */
public class WriteProgram extends Command<App> 
{
    /**
     * @param app
     */
    public WriteProgram(App app) 
    {
        super(Label.WRITE_PROGRAM, app);
    }

   /** 
    * @see pt.utl.ist.po.ui.Command#execute() 
    */
    @Override
    public final void execute() throws InvalidOperation 
    {
        //FIXME implement

        String programID = entity().readString(Message.requestProgramId());
        
        Interpreter interpreter = entity().getInterpreter();
        Program program = interpreter.getProgram(programID);
        String programCode = program.getAsText();

        String filename = entity().readString(Message.programFileName());

        FileWriter out = new FileWriter(filename);
        BufferedWriter buffer = new BufferedWriter(out);
        PrintWriter printer = new PrintWriter(buffer);

        printer.write(programCode);
    }
}
