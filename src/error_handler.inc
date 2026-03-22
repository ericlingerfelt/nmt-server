<?php
/*****************************************************************************/
/*! \file
*
* \verbatim
* $Author: bucknerk $
* $Id: error_handler.inc,v 1.2 2008/05/28 14:30:30 bucknerk Exp $
*
* $Log: error_handler.inc,v $
* Revision 1.2  2008/05/28 14:30:30  bucknerk
* Fixes, updates and repairs
*
* Revision 1.1  2008/03/28 17:56:32  bucknerk
* new
*
*
* Revision 1.8  2008/03/11 14:24:41  bucknerk
*
* Updated some things so error messages are a little better. Still needs work. 
* Also changed the admin-login process because the data base field (permission) 
* needs to be an int for now, else CINA crashes.
*
* Revision 1.7  2008/02/25 20:47:39  bucknerk
* minor changes
*
* Revision 1.6  2007/12/05 20:07:43  bucknerk
* fixed comments
*
* Revision 1.5  2007/12/05 19:24:58  bucknerk
* fixed error reporting
*
* Revision 1.4  2007/08/27 15:43:30  bucknerk
* Fixed various typos
*
* Revision 1.3  2007/08/23 18:10:42  bucknerk
* Updated error handling and documentation
*
* Revision 1.2  2007/08/17 19:57:58  bucknerk
* Turnd of the echo of the error string, too noisy with display errors on.
*
* Revision 1.1  2007/08/17 18:16:10  bucknerk
* New file that contains a simple error handling function. Uses XML to format
* the errors.  Got basic function from PHP manual and made a few small
* changes.  BTW the current logfile is /var/log/php_error.log.
*
*
*
*
* \endverbatim
* 
* NOTE: Taken from cina_eval for the mass_model.
*
* This file contains the error_handling routine. All it really does is
* format the errors more nicely.  Not like php does not already have a
* method to report the errors.  So from this we will log the errors and
* diferentiate between user errors (incoming script) and code/operational
* errors.  I will turn ALL errors on in the <tt>/etc/php.ini</tt> file. 
* Currently the log file is <tt>/var/log/php_error.log</tt> and it needs to be
* world <b>writable (chmod 0666) </b> because PHP scripts may run as 
* 'nobody' or the file owner during development.
*
*
*****************************************************************************/

/*****************************************************************************/
/*!
* 
* This function is taken in part from the php manual. The parameters are as
* defined for the user-defined error handling function. Note that not all of them
* need be there.
*
* \param[in] $errno  The PHP error number
* \param[in] $errmsg The string representation of the error
* \param[in] $filename The full name of the script generating the error (optional)
* \param[in] $linenum The linenumber where the error was triggered (optional)
* \param[in] $vars This is actually an array that is the variable context at the
* moment the error occurred, which means that it contains ALL the variables and
* values in the scope of the line/file of the error.. (optional)
* \return Returns nothing but does have side-effects.
* \sideeffect This prints the error message to standard output and may cause
* it to be logged to the error log specified in <tt size="+2">/etc/php.ini</tt> file.
* What occurs depends on the value of the <i>$error_logging</i> global variable.
*
* One of the issues with error logs is that they need to be "rotated" when
* they become too large. Hopefully logrotate will handle that. When doing
* production DO NOT log errors except to test.  Because that means that warnings
* and notices (non-fatal) PHP syntax type errors are sent to the log and NOT to the
* screen.  This is not a good thing.  If you want to use the handler, then make it
* echo the error instead of logging it while you are in development.  
*****************************************************************************/
function error_handler($errno, $errmsg, $filename, $linenum, $vars)
{


  // define an assoc array of error string
  // E_ERROR, E_PARSE and the COMPILE and CORE errors CANNOT
  // be handled.
  $errortype = array (
    E_WARNING => 'ERROR=PHP Warning',
    E_NOTICE => 'ERROR=PHP Notice',
    E_USER_ERROR => 'ERROR=User Error',
    E_USER_WARNING => 'ERROR=User Warning',
    E_USER_NOTICE => 'ERROR=User Notice',
    );
  // put together the error string
  $err=$errortype[$errno];
  $err .= ": " .  $errmsg ;
  $err .= " in " .  $filename ;
  $err .= " at " .  $linenum .  "\n"; 

  if(array_key_exists('log',$GLOBALS) && $GLOBALS['log']) {
    // save to the error log for production only
    error_log($err);
    //echo "<hr>Error logged</hr>";
  }
  echo "$err";
}



