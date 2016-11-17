package pex;

/**
 * This interface defines the methods that should be used by the core entities to write
 * and read (strings and integers). This functionality depends on the type of user interaction
 * that the application uses.
 **/

public interface AppIO {

    /**
     * Writes a string to be presented to the user.
     *
     * @param str the string to write
     **/
    void println(String str);

    /**
     * Reads a string inputed by the user.
     *
     * @return the string written by the user.
     **/
    String readString();

    /**
     * Reads an integer inputed by the user.
     *
     * @return the number written by the user.
     **/
    int readInteger();
}
