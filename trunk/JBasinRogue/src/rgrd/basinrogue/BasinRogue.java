/*
 * Created on Jan 16, 2007 by tomdemuyt
 *
 */
package rgrd.basinrogue;

/* So you are thinking of working on BasinRogue ?
 * StyleGuide : 
 *        no tabs, use spaces, 4 of them
 *        functions and instances are camel case starting with a lowercase
 *        Objects are camelcase starting with an uppercase
 *        Do not comment the obvious
 *
 *    How to run it ?
 * 		Use the ant file, default task is runit, so you can just call ant in the root dir or use Eclipse
 * 
 *    Which IDE ?
 * 		Use Eclipse. If you find xxx better, please do not ask any questions. The answer will be : use Eclipse
 * 
 * You want access to SVN
 *    	Myself, Antoine, Gamer2k4 and Cavalaria Christophe cant grant you access
 *        I would prefer new people to give a patch so we can see what kind of coder we are giving access
 * 
 *    Would feature x be great to code ?
 *         Please propose it on the wiki first : 
 */

import java.io.IOException;

import org.apache.commons.logging.Log;
import org.apache.commons.logging.LogFactory;
import rgrd.basinrogue.connections.ConnectionInfo;

//import net.wimpi.telnetd.io.*;
/*

 import net.wimpi.telnetd.io.terminal.TerminalManager;
 import net.wimpi.telnetd.io.toolkit.BufferOverflowException;
 import net.wimpi.telnetd.io.toolkit.Point;
 import net.wimpi.telnetd.io.toolkit.Selection;
 import net.wimpi.telnetd.io.toolkit.Statusbar;
 import net.wimpi.telnetd.io.toolkit.Titlebar;
 */
import net.wimpi.telnetd.net.Connection;
import net.wimpi.telnetd.net.ConnectionEvent;
import net.wimpi.telnetd.shell.*;
import rgrd.basinrogue.connections.*;
import rgrd.basinrogue.menus.*;

/**
 * BasinRogue by
 * 
 * @author tomdemuyt
 */
public class BasinRogue implements Shell {

    public static final String app = "RogueBasin 0.0.1";

    private static Log log = LogFactory.getLog(BasinRogue.class);

    private Connection con;

    public RogueIO pipe;

    private static boolean initialized = false;

    private static boolean initializing = false;

    public static void main(String[] args) {
        //DoME ;)
    }

    public void run(Connection con) {
        try {
            //Store connection, create and store pipe
            this.con = con;
            pipe = new RogueIO(con.getTerminalIO());
            con.addConnectionListener(this);

            boolean done;

            //clear the screen and start from zero
            pipe.eraseScreen();
            pipe.homeCursor();

            pipe.dump(app + "\r\n");
            //Dump all known info about the connection for later reporting use
            ConnectionInfo.dumpConnectionInfo(con, pipe.oldPipe);

            //Initialize if not yet initialized
            if (!initialized && !initializing) {
                //We set it to true first, so that if we initialize for a long time, it
                // wont happen twice
                initializing = true;
                initialize();
                initialized = true;
                initializing = false;
            }
            //If the user logs in while someone else is still initializing the game,
            // let him wait
            while (initializing) {
                pipe.dump(app + " is still initializing");
                pipe.read();
            }

            done = false;

            if (initialMenu() != 2) {

                Connections.addConnection(pipe, this);

                while (!done) {
                    int i = pipe.read();
                    if (i == -1 || i == -2) {
                        log.debug("Input(Code):" + i);
                        done = true;
                    }

                    if (i == 10) {
                        done = true;
                    }

                    // The / is for console mode, only console command so far is who
                    if (i == '/') {
                        pipe.dump("/");
                        String command = pipe.getString();

                        if (command.equals("who")) {
                            pipe.dump(Connections.getActiveUsers());
                        }

                        //Once we are more advanced, this has to appear on a certain area
                        // of the screen.
                        //Maybe go in a buffer or something
                        if (command.startsWith("say")) {
                            String user = command.split(" ")[1];
                            RogueIO pipe = Connections.getActiveUser(user).pipe;
                            pipe.dump(command.substring(user.length() + 7));
                        }

                    }

                }
            }
            pipe.homeCursor();
            pipe.eraseScreen();
            pipe.write("@ will miss you. Bye!.\r\n\r\n");
            pipe.flush();

        } catch (Exception ex) {
            log.error("run()", ex);
        }
    }//run

    private void initialize() {
        //Load Town to run around in
    }

    private int initialMenu() throws Exception {
        String options[] = { "Login", "Register", "Quit", };

        return GenericMenu.getMenuOption(pipe, options, GenericMenu.FIRSTLETTER);
    }

    /* ALL THAT IS BELOW IS BOILER PLATE CODE TO KEEP telnetd2 HAPPY */
    /* LOOKING AT IT IS A WASTE OF TIME */
    /* HOWEVER, IF YOU ARE READING THIS ... */

    public static Shell createShell() {
        return new BasinRogue();
    }

    public void connectionLogoutRequest(ConnectionEvent ce) {
        try {
            pipe.write("@ will miss you. Bye!");
            pipe.flush();
        } catch (IOException e) {
            log.error("Failed log out message", e);
        }
    }

    public void connectionIdle(ConnectionEvent ce) {
        try {
            pipe.write("@ explodes in boredom. Bye!");
            pipe.flush();
        } catch (IOException e) {
            log.error("Failed idle message.", e);
        }
    }

    public void connectionTimedOut(ConnectionEvent ce) {
        try {
            pipe.write("Connection timed out!");
            pipe.flush();
            con.close();
        } catch (IOException e) {
            log.error("Failed time out or disconnection", e);
        }
    }

    public void connectionSentBreak(ConnectionEvent ce) {
        try {
            pipe.write("@ doesnt want a break!");
            pipe.flush();
        } catch (IOException e) {
            log.error("Failed giving a break", e);
        }
    }

}