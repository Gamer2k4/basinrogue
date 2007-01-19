/*
 * Created on Jan 17, 2007 by konijn Rationale: this class will most likely be
 * renamed/removed/refactored We need a central place with all the player names.
 * At least for a /who command and a /say command , the player will be
 * registered with his shell session
 */
package rgrd.basinrogue.connections;

import net.wimpi.telnetd.shell.*;
import java.util.*;

import org.apache.commons.logging.Log;
import org.apache.commons.logging.LogFactory;

import rgrd.basinrogue.*;

public class Connections {

    private static Vector v = new Vector();

    private static Log log = LogFactory.getLog(Connections.class);

    public static boolean addConnection(RogueIO pipe, Shell shell)
            throws Exception {
        String user;

        while (true) {
            //clear the screen and start from zero
            pipe.eraseScreen();
            pipe.homeCursor();
            pipe.dump(BasinRogue.app + "\r\n\r\n");

            pipe.dump("Please enter your name. (blank entry aborts)\r\n\r\n    => ");
            user = pipe.getString();

            //Blank entry aborts
            if (user.trim().equals(""))
                return false;

            //getConnection checks for username, returns null if noone is active in
            // the system with that name
            if (getActiveUser(user) == null) {
                v.add(new ActiveUser(user, shell, pipe));
                pipe.dump("Welcome to RogueBasin " + user + ".");
                return true;
            }
            pipe.dump("That username is already active.");

        }
    }

    /*
     * This only removes it from our active user list, this does not disrupt the
     * connection
     */
    public static void removeConnection(String user) {
        ActiveUser au = getActiveUser(user);
        if (au == null) {
            log.debug("Could not find active user " + user);
        } else {
            v.remove(au);
        }
    }

    public static String[] getActiveUsers() {
        String activeUser[] = new String[v.size()];
        for (int i = 0; i < v.size(); i++)
            activeUser[i] = v.get(i).toString();
        return activeUser;
    }

    public static ActiveUser getActiveUser(String user) {
        String users[] = getActiveUsers();
        for (int i = 0; i < users.length; i++)
            if (users[i].equals(user))
                return (ActiveUser) v.get(i);
        return null;
    }

}