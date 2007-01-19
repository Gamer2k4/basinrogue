/*
 * Created on Jan 17, 2007 by konijn
 *
 */
package rgrd.basinrogue;

import java.io.IOException;

import org.apache.commons.logging.Log;
import org.apache.commons.logging.LogFactory;

import net.wimpi.telnetd.io.BasicTerminalIO;

//import net.wimpi.telnetd.io.TerminalIO;
//import net.wimpi.telnetd.net.Connection;

/**
 * RogueIO by
 * 
 * @author konijn
 */
public class RogueIO {

    private static Log log = LogFactory.getLog(RogueIO.class);

    //This sucks !!! But without changing the BSD I see no other way, and I dont
    // want to change the telnetd stuff
    //The problem is that with just doing a a new TerminalInfo(conenction) the
    // initialization is not completely done
    //and we still read some negotiation bytes. Who knows what else could be
    // missing ?
    public BasicTerminalIO oldPipe;

    /* See above */
    public RogueIO(BasicTerminalIO oldPipe) {
        this.oldPipe = oldPipe;
    }

    /* Convenienc, writes the output and flushes */
    public void dump(String s) throws IOException {
        oldPipe.write(s);
        oldPipe.flush();
    }

    public void dump(String s[]) throws IOException {
        for (int i = 0; i < s.length; i++)
            dump(s[i] + "\r\n");
    }

    //
    public String getString() throws IOException {
        StringBuffer sb = new StringBuffer();
        while (true) {
            int i = oldPipe.read();
            log.debug("Input(Code):" + i);
            //if -1 or -2 , something is wrong
            if (i == -1 || i == -2) {
                log.debug("Input(Code):" + i);
                return (sb.toString());
            }
            //10 is enter
            if (i == 10) {
                dump("\r\n");
                return (sb.toString());
            }
            //Sanity check, any char under 32 is a control character
            if (i > 31) {
                sb.append((char) i);
                dump(String.valueOf((char) i));
            }
        }
    }

    //Silly wrapper methods, there be Yeeks here

    public void write(byte b) throws IOException {
        oldPipe.write(b);
    }

    public void write(char ch) throws IOException {
        oldPipe.write(ch);
    }

    public void write(java.lang.String str) throws IOException {
        oldPipe.write(str);
    }

    public void eraseScreen() throws IOException {
        oldPipe.eraseScreen();
    }

    public void homeCursor() throws java.io.IOException {
        oldPipe.homeCursor();
    }

    public int read() throws IOException {
        return oldPipe.read();
    }

    public void flush() throws java.io.IOException {
        oldPipe.flush();
    }

}