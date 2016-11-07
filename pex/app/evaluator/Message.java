package pex.app.evaluator;

/** Messages for menu interactions. */
@SuppressWarnings("nls")
public interface Message {

    /**
     * @return prompt for identifier
     */
    static String requestId() {
        return "Identificador: ";
    }

    /**
     * @return prompt for new identifier
     */
    static String requestNewId() {
        return "Novo identificador: ";
    }
    
    /**
     * @param name
     * @return return error message for unknown section
     */
    static String noSuchIdentifier(String name) {
        return "O identificador '" + name + "' não existe.";
    }

    /**
     * @param name
     * @param dependent
     * @return error message for identifier removal
     */
    static String identifierInUse(String name, String dependent) {
        return "O identificador '" + name + "' está em uso na definição de '" + dependent + "'.";
    }

    /**
     * @return prompt for position
     */
    static String requestPosition() {
        return "Posição do programa: ";
    }

    /**
     * @return prompt for expression
     */
    static String requestExpression() {
        return "Expressão: ";
    }
}
