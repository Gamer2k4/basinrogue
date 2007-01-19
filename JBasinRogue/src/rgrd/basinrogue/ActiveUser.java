/*
 * Created on Jan 17, 2007 by konijn
 *
 */
package rgrd.basinrogue;

import net.wimpi.telnetd.shell.Shell;

/**
 * ActiveUser by @author konijn
 */
public class ActiveUser extends Object {
  
  /* Most if not all of these should be private, but for now lets be open */
  public Shell shell;
  public String user;
  public RogueIO pipe;
  
  public ActiveUser( String user , Shell shell , RogueIO pipe ){
    	this.shell = shell;
    	this.user = user;
  }
  
  public String toString(){
    return user;
  }
  
}