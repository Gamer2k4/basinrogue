/*
 * Created on Jan 16, 2007 by tomdemuyt
 *
 */
package rgrd.basinrogue;

import net.wimpi.telnetd.io.BasicTerminalIO;
import net.wimpi.telnetd.net.Connection;
import net.wimpi.telnetd.net.ConnectionData;
import net.wimpi.telnetd.io.TerminalIO;
import java.io.IOException;

/**
 * ConnectionInfo by @author tomdemuyt
 */
public class ConnectionInfo {

  static void dumpConnectionInfo( Connection con , BasicTerminalIO pipe) throws IOException {
    
    ConnectionData cd = con.getConnectionData();
    //output header
    pipe.write(BasicTerminalIO.CRLF + "DEBUG: Active Connection" + BasicTerminalIO.CRLF);
    pipe.write("------------------------" + BasicTerminalIO.CRLF);

    //output connection data
    pipe.write("Connected from: " + cd.getHostName() +
        "[" + cd.getHostAddress() + ":" + cd.getPort() + "]"
        + BasicTerminalIO.CRLF);
    pipe.write("Guessed Locale: " + cd.getLocale() +
        BasicTerminalIO.CRLF);
    pipe.write(BasicTerminalIO.CRLF);
    //output negotiated terminal properties
    pipe.write("Negotiated Terminal Type: " +
        cd.getNegotiatedTerminalType() + BasicTerminalIO.CRLF);
    pipe.write("Negotiated Columns: " + cd.getTerminalColumns() +
        BasicTerminalIO.CRLF);
    pipe.write("Negotiated Rows: " + cd.getTerminalRows() +
        BasicTerminalIO.CRLF);

    //output of assigned terminal instance (the cast is a hack, please
    //do not copy for other TCommands, because it would break the
    //decoupling of interface and implementation!
    pipe.write(BasicTerminalIO.CRLF);
    pipe.write("Assigned Terminal instance: " +   ((TerminalIO) pipe).getTerminal());
    pipe.write(BasicTerminalIO.CRLF);
    pipe.write("Environment: " + cd.getEnvironment().toString());
    pipe.write(BasicTerminalIO.CRLF);
    //output footer
    pipe.write("-----------------------------------------------" +
        BasicTerminalIO.CRLF + BasicTerminalIO.CRLF);

    pipe.flush();
  
  }
}
