/*
 * Created on Jan 16, 2007 by tomdemuyt
 *
 */
package rgrd.basinrogue;

//import net.wimpi.telnetd.io.BasicTerminalIO;
//import net.wimpi.telnetd.io.TerminalIO;
//import net.wimpi.telnetd.io.terminal.ColorHelper;
//import net.wimpi.telnetd.io.terminal.TerminalManager;
//import net.wimpi.telnetd.io.toolkit.*;
//import net.wimpi.telnetd.net.Connection;
//import net.wimpi.telnetd.net.ConnectionData;
//import net.wimpi.telnetd.net.ConnectionEvent;
//import org.apache.commons.logging.Log;
//import org.apache.commons.logging.LogFactory;

import java.io.IOException;

import org.apache.commons.logging.Log;
import org.apache.commons.logging.LogFactory;

import net.wimpi.telnetd.io.BasicTerminalIO;
/*
import net.wimpi.telnetd.io.TerminalIO;
import net.wimpi.telnetd.io.terminal.ColorHelper;
import net.wimpi.telnetd.io.terminal.TerminalManager;
import net.wimpi.telnetd.io.toolkit.BufferOverflowException;
import net.wimpi.telnetd.io.toolkit.Checkbox;
import net.wimpi.telnetd.io.toolkit.Editarea;
import net.wimpi.telnetd.io.toolkit.Editfield;
import net.wimpi.telnetd.io.toolkit.InputFilter;
import net.wimpi.telnetd.io.toolkit.Label;
import net.wimpi.telnetd.io.toolkit.Pager;
import net.wimpi.telnetd.io.toolkit.Point;
import net.wimpi.telnetd.io.toolkit.Selection;
import net.wimpi.telnetd.io.toolkit.Statusbar;
import net.wimpi.telnetd.io.toolkit.Titlebar;
*/
//import net.wimpi.telnetd.io.toolkit.Editfield;
import net.wimpi.telnetd.net.Connection;

import net.wimpi.telnetd.net.ConnectionEvent;
import net.wimpi.telnetd.shell.*;

/**
 * BasinRogue by @author tomdemuyt
 */
public class BasinRogue implements Shell { 
  
  private static Log log = LogFactory.getLog(BasinRogue.class);
  private Connection con;
  private BasicTerminalIO pipe;
  
  private static boolean initialized = false;
  private static boolean initializing = false;;


  public static void main(String[] args) {
    //DoME ;)
  }
  
  private void dump( String s ) throws IOException  {
    pipe.write( s );
    pipe.flush();
  }
  
  public void run(Connection con) {
    try {
      this.con = con;
      pipe = con.getTerminalIO();
      con.addConnectionListener(this);

      //clear the screen and start from zero
      pipe.eraseScreen();
      pipe.homeCursor();

      //We just read any key
      dump("Welcome to BasinRogue 0.0.1\r\n");
      ConnectionInfo.dumpConnectionInfo(con , pipe );
      
      if(!initialized && !initializing){
        //We set it to true first, so that if we initialize for a long time, it wont happen twice
        initializing = true;
        initialize();
        initialized = true;
        initializing = false;
      }
      //If the user logs in while someone else is still initializing the game, let him wait
      while( initializing ){
        dump("BasinRogue is still initializing");
        pipe.read();
      }
      
      boolean done = false;
      while (!done) {
        int i = pipe.read();
        if (i == -1 || i == -2) {
          log.debug("Input(Code):" + i);
          done = true;
        }
        if (i == 10) {
          done = true;
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
  
  private void initialize(){
    //Load Town to run around in
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
