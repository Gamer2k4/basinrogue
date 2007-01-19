/*
 * Created on Jan 18, 2007 by konijn
 * Get an array of menu options, and with either alphanumeric or numeric entry
 * the user can choose an entry. Option index is returned
 * TODO: for large lists (>9), UPPERCASE would be needed, this would start with 
 * getting strings like Ranger , rObber , raNcher and get presented as
 * r) Ranger
 * o) Robber
 * n) Rancher
 */
package rgrd.basinrogue.menus;

import org.apache.commons.logging.Log;
import org.apache.commons.logging.LogFactory;

import rgrd.basinrogue.RogueIO;

/**
 * GenericMenu by
 * 
 * @author konijn
 */
public class GenericMenu {

    //Numbers each menu option, obviously this only goes up to 9
    public static final int NUMBERS = 0;

    //Obviously having entries with the same letter gives confusion
    public static final int FIRSTLETTER = 1;

    //Not yet implemented
    public static final int UPPERCASE = 2;

    //When things go wrong, return -1
    public static final int CANCEL = -1;

    private static Log log = LogFactory.getLog(GenericMenu.class);

    public static int getMenuOption(RogueIO pipe, String[] options, int method)
            throws Exception {

        //Dump the options!
        for (int i = 0; i < options.length; i++) {
            pipe.write(" ");
            switch (method) {
            case NUMBERS:
                pipe.dump(i + 1 + ") " + options[i]);
                break;
            case FIRSTLETTER:
                pipe.dump(Character.toLowerCase(options[i].charAt(0)) + ") " + options[i]);
                break;
            }
            pipe.write("\r\n");
        }
        pipe.write("\r\n => ");

        //Get an option
        while (true) {
            int i = pipe.read();
            if (i == -1 || i == -2) {
                log.debug("Input(Code):" + i);
                return CANCEL;
            }
            switch (method) {
            case NUMBERS:
                if (i > '0' && i <= '9') {
                    pipe.dump((char) i + "\r\n");
                    return i - '0';
                }
                break;
            case FIRSTLETTER:
                for (int c = 0; c < options.length; c++) {
                    if (Character.toLowerCase((char) i) == Character
                            .toLowerCase(options[c].charAt(0))) {
                        pipe.dump((char) i + "\r\n");
                        return c;
                    }
                }
                break;
            }
        }
    }
}