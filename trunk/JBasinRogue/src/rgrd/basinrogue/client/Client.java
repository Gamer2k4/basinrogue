/*
 * Created on Jan 19, 2007 by tomdemuyt
 *
 */
package rgrd.basinrogue.client;

import de.mud.telnet.TelnetWrapper;
import java.io.IOException;

/**
 * Client by @author tomdemuyt
 */
public class Client extends TelnetWrapper {

  public Client() {
    super();
  }
  
  private void writeChar( char c ) throws IOException{
    byte singleByte[] = { (byte)c };
    write( singleByte );
  }

  public static void main(String[] args) {
    Client telnet = new Client();
    //int amount;
    //byte bytes[] = new byte[255];

    String server = ( args.length==0?"localhost":args[0] );
           
    try {
      telnet.connect(server, 23);
      
      //amount = telnet.read( bytes );
      //System.out.println( new String( bytes, 0 , amount ) );
      
      System.out.println( telnet.waitfor( "=> " ) );
      
      telnet.writeChar( 'l' );
      
      System.out.println( telnet.waitfor( "=> " ) );
      
    } catch(java.io.IOException e) {
      e.printStackTrace();
    }
  }
}
