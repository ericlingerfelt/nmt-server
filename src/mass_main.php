<?php
/*****************************************************************************/
/*! \mainpage notitle
 *
 * <br> <h1 align="center">Mass Model Code Documentation</h1>
 *
 * \section intro Introduction
 *
 * This is the automatically (using doxygen) generated documentation of the
 * PHP code used to run the 'backend' of the Mass Model Suite
 * component.  There is a single
 * <b>.php</b> file, <tt>mass_main.php</tt> which is the focus of the
 * interface actions and it then <i>includes</i> other <b>.inc</b> files as
 * necessary.
 *
 *
 * \section probs Potential Problems
 *
 * Currently we do not see any issues. However there are always potential
 * problems.
 * <ul>
 * <li>The amount of data that can be sent (received) via the
 * input (output) stream.  The input is primarily an Apache server limitation
 * (LimitRequestBody) and the value should currently be 10 megabytes (10485760).
 * This is set in <tt>/etc/httpd/conf/httpd.conf</tt> and requires that the
 * web server be
 * restarted to take effect. How or if this affects the client we do not yet
 * know.
 * <li>Currently all passwords are stored using SHA1 key.  We do not
 * expect this to change at any time in the foreseeable future but if it does
 * we will need to have all the users change their passwords.
 * <li>File compression. PHP can support zip but the version we are using does not
 * have that support compiled in and we are limited to what is compiled.  It
 * can do gzip but this is not necessarily compatible will all of our users
 * (especially Windows clients). A work around will be to use the (Linux)
 * system zip and unzip utilities which should  not be a large issue.
 * </ul>
 *
 * \section php PHP
 *
 * We are using PHP for the 'backend' language because it is no easy to use
 * and well integrated with the webserver and MySQL. It is Open Source and
 * robust.  It is difficult for the bad guys to break into because unless the
 * programmer does some truly bad things, the client cannot view any script.
 *
 * Please note that we are currently (08/22/2007) using PHP v. 4.3.9 and
 * Apache v. 2.0.52 (with mod_ssl) and MySQL v. 5.0.27.  This means that
 * there things documented in the PHP manual that ARE NOT avaiable in
 * practice simply because the world has moved on this server is locked into
 * an configuration.
 *
 * \section cgi CGI Information
 *
 * This code receives input from the clients via the HTTP POST method.  We
 * are not currently using the <b>multipart/form-data</b> option for file
 * uploads.  In PHP this data is <i>global</i>. It is available in the array
 * $_POST which like all PHP arrays is associative. A simple function,
 * array_key_exists() can be used to see what data components have been
 * passed and their values extracted using standard array notation.
 *
 * All output is via the HTTP connection by simply writing to standard
 * output.  No special streams have to be opened.  Theoretically we should
 * send back a Content-Type descriptor but at the moment do not.
 *
 * \section msq MySQL
 *
 * The MySQL interface is through PHP compiled-in functions.  There are a
 * number of these but primarily we use the <b>mysql_connect()</b>,
 * <b>mysql_query()</b>, <b>mysql_close()</b> functions to directly access
 * the selected database/tables and then use several functions to check the
 * number of rows returned in the query and extract those values.  Error
 * handling is done by always checking the function return values and using
 * <b>mysql_errno()</b> and <b>mysql_error()</b> to test/recover the errors.
 * It is possible to have a 'non-fatal' mysql error if no data is returned
 * but the return value is no false. In which case mysql_errno() will return
 * a non-zero value and mysql_error() will get the string representation.
 *
 * \section ge General Error Handling
 *
 * Error handling in PHP is similar to error handling in high-level languages
 * such as C.  Return values and error codes are check.  Error reporting is
 * somewhat different.  PHP uses <tt>/etc/php.ini</tt> to determine how to
 * report errors, which ones to report and whether or not to log those
 * errors. During the development phase I highly recommend that error logging
 * in NOT used because a non-fatal parse error (like a mistake in a regular
 * expression) can result in literally thousands of lines being written to
 * the log file. But once the program reaches the production point, error
 * logging should be turned on and error reporting turned off (to keep the
 * clients from detecting/exploiting errors in your script).
 *
 * The PHP manual discusses all the options in the <tt>/etc/php.ini</tt>
 * file.  In addition, you can change some of these on a per-script basis
 * using the ini_set() function.  I have added an error_handler() function
 * that is substituted for the PHP default error handler. This function is
 * NOT called for all errors. Parse errors and Core errors occur BEFORE THE
 * SCRIPT RUNS. These cannot be 'handled'.  Only the Warning, Notice and User
 * errors can be handled.  This function can also either log the error OR
 * echo it out. The reason for even using this functions is to have a
 * consistent way to display user errors and make error messages easier to
 * generate.
 *
 *
 ******************************************************************************/

/*! \file
 *
 * \verbatim
 * $Author: elingerf $
 * $Id: mass_main.php,v 1.11 2008/10/09 19:33:48 elingerf Exp $
 *
 * $Log: mass_main.php,v $
 * Revision 1.11  2008/10/09 19:33:48  elingerf
 * no message
 *
 * Revision 1.10  2008/05/28 17:47:59  bucknerk
 * tweeks for testing
 *
 * Revision 1.9  2008/05/28 14:51:13  bucknerk
 * fixed conflicts
 *
 * Revision 1.8  2008/05/28 14:30:30  bucknerk
 * Fixes, updates and repairs
 *
 * Revision 1.7  2008/04/30 13:48:36  bucknerk
 * Changed things so that this can now be used as a testing and online.  The
 * database chosen is based on the directory "mass_model" or "mass_testing".
 *
 * Revision 1.6  2008/04/18 13:39:10  bucknerk
 * added functionality
 *
 * Revision 1.5  2008/04/16 13:48:59  bucknerk
 * added more stuff
 *
 * Revision 1.4  2008/04/08 14:39:43  bucknerk
 * new
 *
 * Revision 1.3  2008/04/01 19:59:17  bucknerk
 * Added get_session_id (login) and logout. Updated (changed from add) create_model.
 *
 * Revision 1.2  2008/03/28 17:57:16  bucknerk
 * minor stuff
 *
 * Revision 1.1  2008/03/27 17:46:30  bucknerk
 * in the beginning
 *
 *
 * \endverbatim
 *
 * This is the <b>main</b> point of interface with the php modules that
 * will be handling the interface with the mass model suite.
 *
 * \section reffunc Referenced Functions:
 *
 * \section incf Included Files:
 * The following files are always included:<br>
 *
 ********************************************************************/
header('Cache-Control: no-cache, no-store, must-revalidate'); //HTTP/1.1
header('Expires: Thur, 01 Jan 1970 00:00:01 GMT');
header('Pragma: no-cache'); //HTTP/1.0

include('error_handler.inc');
$old_handler=set_error_handler('error_handler');

// file that contains the object definition and the actual info I want
// and the function that contains the correct info. Could hard-code into
// the object as well.
include 'information.inc';
include 'admin_functions.inc';

/*if(array_key_exists("TESTING",$_POST) && $_POST['TESTING']==1) {
 $suffix="dev";
 /// This array stores all the formatting characters for the output
 $text_array =& $test_comp;
 }
 else {*/
$text_array =& $run_comp;
//}
// This is all the information I need to manipulate the correct database
$my_dbinfo =& get_db_info("mass_model");
if(! $my_dbinfo) {
	$merr="Could not get database information for mass_model\n";
	trigger_error($merr,E_USER_ERROR);
	return;
}
//
// Now figure out what the client wants from me.
//
if(array_key_exists('ACTION',$_POST)) {
	switch($_POST['ACTION']) {
		case 'GET_ID':
			get_session_id($my_dbinfo,$text_array);
			break;
		case 'REGISTER_USER':
			include('mass_handlers.inc');
			handle_register_user($my_dbinfo,$text_array);
			break;
		case 'LOGOUT':
			terminate_session($my_dbinfo,$text_array);
			break;
		case 'GET_MODELS':
			include('mass_handlers.inc');
			handle_get_models($my_dbinfo,$text_array);
			break;
		case 'GET_MODEL_DATA':
			include('mass_handlers.inc');
			handle_get_model_data($my_dbinfo,$text_array);
			break;
		case 'GET_MODEL_INFO':
			include('mass_handlers.inc');
			handle_get_model_info($my_dbinfo,$text_array);
			break;
		case 'ERASE_MODEL':
			include('mass_handlers.inc');
			handle_erase_model($my_dbinfo,$text_array);
			break;
		case 'CREATE_MODEL':
			include('mass_handlers.inc');
			handle_create_model($my_dbinfo,$text_array);
			break;
		case 'MERGE_MODELS':
			include('mass_handlers.inc');
			handle_merge_models($my_dbinfo,$text_array);
			break;
		case 'COPY_MODEL_TO_SHARED':
			include('mass_handlers.inc');
			handle_copy_model($my_dbinfo,$text_array);
			break;
		default:
			trigger_error("ILLEGAL ACTION: {$_POST['ACTION']}\n",E_USER_ERROR);
			return;
	}
}
else {
	trigger_error("NO ACTION SPECIFIED:\n",E_USER_ERROR);
	return;
}

?>
