<?php
/*! \file 
*
* \verbatim
* $Author: bucknerk $
* $Date: 2008/06/04 18:06:42 $
*
* $Log: mass_admin_reg.php,v $
* Revision 1.5  2008/06/04 18:06:42  bucknerk
* deleted the duplicate auth_validate function
*
* Revision 1.4  2008/06/04 17:49:47  bucknerk
* fixed/worked at the auth_validate. the function was needlessly duplicated
* it was called with wrong argument.
*
* Revision 1.3  2008/04/30 13:48:36  bucknerk
* Changed things so that this can now be used as a testing and online.  The
* database chosen is based on the directory "mass_model" or "mass_testing".
*
* Revision 1.2  2008/04/18 13:39:10  bucknerk
* added functionality
*
* Revision 1.1  2008/04/08 14:39:43  bucknerk
* new
*
*
***************************************************************************
* This was taken from the cina_eval/site_admin_reg.php in its entirety then 
* modified for the mass_model
*
* Revision 1.9  2008/03/26 20:07:53  bucknerk
* I think I have the waiting point finder giving good responses now and
* that it runs correctly with zones like 01, 05 and so on.  Really just took data from the old cinad_main and put it in the cinad_eval then renamed the cinad_eval and moved old cinad_main to miscellaneous.
*
* Revision 1.8  2008/03/11 14:24:41  bucknerk
*
* Updated some things so error messages are a little better. Still needs work. Also changed the admin-logni process because the data base field (permission) needs to be an int for now, else CINA crashes.
*
* Revision 1.7  2008/03/06 18:40:03  bucknerk
* changed email handling slightly
*
* Revision 1.6  2008/03/05 13:34:30  bucknerk
* minor changes
*
* Revision 1.5  2008/02/25 20:45:06  bucknerk
* minor changes
*
* Revision 1.4  2007/11/09 16:23:05  bucknerk
* nothing
*
* Revision 1.3  2007/10/15 19:09:04  bucknerk
* Added a browser redirection and fleshed out/fixed the emailing operation
* including a method for editing the content before sending.
*
* Revision 1.2  2007/09/19 15:52:15  bucknerk
* Moved functions around, they were not actually the proper files. Also
* changed some of the display and added a check of username for uniqueness
* before registering a new user.
*
* Revision 1.1  2007/09/19 12:33:56  bucknerk
* New Files
*
* \endverbatim
*
* This handles the registration functions.  It includes entering a new user
* account, sending a registration email, and setting a basic user directory
* structure so that we won't get those darn Error 29 messages.
* It is meant to be purely web-based with no Java interface.
*
* \section sreg Included Files
*
* The following files are always included:\n
* error_handler.inc \n
* information.inc \n
* admin_functions.inc \n
* site_admin_registration_handlers.inc \n
*
* \section weird Closing Window
*
* This generates an odd little piece of html depending on whether the page
* was accessed using <b>GET</b> or <b>POST</b>. If get was used, the 
* &lt;body&gt; tag is generated with a call to a javascript function,
* closeme(), which should close the window.  Otherwise, this tag has calls
* to do_img_resize() to try to get the 3 images to remain completely
* viewable no matter the window size.  
*
* The first bit of code is designed to redirect the browser to the main 
* administration page when the DO_EMAIL action comes in.
* The literature indicates that the script should exit at this time.
* It also says that the script must not output any html/data before the
* header() call. 
*
* Next is a piece that tells the browser to request a password and username 
* before allowing the page the proceed. Using the 'realm' means that the browser 
* saves the password and username so that the next request for the same realm is 
* answered with the saved pw/username.  FYI these are sent as a single header 
* object which is base64encode of "username:password" and the colon is required.
*/

/*
* This function validates the http supplied username and password against the 
* database specified by the argument. 
*
* \param[in] $dbname is the name of the database to use for account information
* to retrieve passwords and usernames.
* \return <em>true</em> is the validation succeeds and <em>false</em> otherwise.
*
*****************************************************************************/

/****************************************************************************/

  if(array_key_exists('ADMIN_ACTION',$_POST) &&  
    $_POST['ADMIN_ACTION']=='DO_EMAIL') 
    {
      header('location: mass_admin.html');
      //DON'T output anything yet, if you do then when the
      //new form is loaded is an error because of the header.
      //doing that forces an immediate return.
      // what the location does is a redirect
    }
  include_once("admin_functions.inc");
  if(($rtn=auth_validate("mass_model")) !== true ) 
  {
    // not going to print the errors unless testing
    // as long as the php is correct all the other errors will
    // be hidden
    header('WWW-Authenticate: Basic realm="CINA_ADMIN"');
    echo "<html><head><title>Not Authorized</title></head><body>";
    echo "ERROR: 401 Authorization required<br><hr>";
    echo "</body></html>";
    exit(1);
  }



?>

<!-- DOCTYPE HTML PUBLIC "-//W3C//DTD HTML 4.01 Transitional//EN"
     "http://www.w3.org/TR/html4/loose.dtd" -->

<html>
  <head>
    <title>MassModel Registration</title>
    <link rel="stylesheet" type="text/css" href="m_model.css">
    <META http-equiv="Content-Script-Type" content="text/javascript">
    <script src="m_model.js" type="text/javascript">
    </script>
  </head>

<?php
  if( $_SERVER['REQUEST_METHOD'] != "POST") 
    echo '<body onload="closeme()">';
  else echo '<body onresize="do_img_resize()" onload="do_img_resize()">'; 
?>
  <table id="toptable" border="1" align="center">
  <tr> 
    <td><img id="i1" src="left.png" > </td>
    <td><img id="i2" src="stars.png" > </td>
    <td><img id="i3" src="right.png" > </td>
  </tr>
  </table>
  <H2 >MassModel Site Administration: User Registration</H2>
  <hr><hr>
    <?php
      if(array_key_exists('ACTION',$_POST)) {
	switch($_POST['ACTION']) {
	  case 'DO_EMAIL':
	    break;
	  case 'REGISTER':
	    echo 'You have completed the registration, if there were';
	    echo ' no errors you should now have a chance to send ';
	    echo ' the new user email.';
	    echo '<hr><hr>';
	    echo '<H2>Results</H2>';
	    echo '<div class="nav">';
	    echo '<form action="site_admin.php" method="POST">';
	    echo '<input type="submit" value="RETURN">';
	    echo '</form>';
	    break;
	  case 'VALIDATE':
	    echo 'The results of the password validation are below.';
	    echo '<hr><hr>';
	    echo '<H2>Results</H2>';
	    echo '<div class="nav">';
	    echo '<form action="site_admin.php" method="POST">';
	    echo '<input type="submit" value="RETURN">';
	    echo '</form>';
	    break;
	  case 'RESET':
	    echo 'The results of the password reset are below.';
	    echo '<hr><hr>';
	    echo '<H2>Results</H2>';
	    echo '<div class="nav">';
	    echo '<form action="site_admin.php" method="POST">';
	    echo '<input type="submit" value="RETURN">';
	    echo '</form>';
	    break;
	  default:
	  }
      }
      else {

	echo 'This is the page used primarily to register a new user. ';
	echo 'Please fill in';
	echo ' the form below and submit.  If registration is successful,';
	echo ' you can continue and send the new user an email.';
        echo '<hr><hr>';
	echo '<H2>Options</H2>';
	/*
	echo '<div class="nav">';
	echo '<form action="site_admin_reg_help.php" method="POST">';
	echo '<input type="submit" value="HELP">';
	echo '</form>';
	*/
      }
     ?>
    </div>

<?php
  /// This array stores all the formatting characters for the output
  $text_array =& $test_comp;
  $my_dbinfo =& get_db_info('mass_model');
  if(! $my_dbinfo) {
    $adminerr="Could not get database information for 'mass_model'".
    trigger_error($adminerr,E_USER_ERROR);
    return;
    }
  decode($my_dbinfo->password,$my_dbinfo->key); 
  $mylink=mysql_connect('localhost:3306',$database_info ->username ,
	$database_info->password);
  if(! $mylink) {
    $err="ERROR: COULD NOT CONNECT to MySQL server as ";
    $err.= $database_info->username . "<br>\n";
    trigger_error($err,E_USER_ERROR);
    return;
    }
  $rtn=mysql_query("USE $database_info->dbname");
  if(! $rtn) {
    $err= "ERROR: USE " . $database_info->dbname ;
    $err.=" query caused: MYSQL ERROR: " . mysql_error() ;
    $err.= "<br>\n";
    trigger_error($err,E_USER_ERROR);
    mysql_close($mylink);
    return;
    }
  $query_str="SELECT * FROM register_requests WHERE complete IS NOT NULL";
  $rtn=mysql_query($query_str);
  if($rtn) {
    $adminerr="Sorry, could not access database: ";
    $adminerr.="MYSQL Error: " . mysql_error();
    trigger_error($adminerr,E_USER_ERROR);
    return;
    }
  

  if( array_key_exists('ADMIN_ACTION',$_POST)) {
    include_once('admin_functions.inc');
    include_once('information.inc');
    include_once('error_handler.inc');
    include_once('mass_model_registration_handlers.inc');
    /// The information I need to manipulate the databases

    switch($_POST['ADMIN_ACTION']) {
      case 'REGISTER':
	echo '<div class="display">';
	$rtn=handle_registration($my_dbinfo,$text_array);
	if(is_array($rtn)) {
	  echo '<p class="header">';
	  echo "<br>The registration was successful. Use the ";
	  echo "button below to send an email to the new user. The<br>";
	  echo " email can be edited in-place if desired.";
	  echo "</p>";
	  echo '<form name="emailer" action="site_admin_reg.php" ';
	  echo 'method="POST" enctype="multipart/form-data>" ' . "\n";
	  echo '<table border="0" width="80%" align="center">';
	  echo '<tr><td>';
	  echo '<input type="submit" value="SEND EMAIL">';
	  echo '<input type="hidden" name="ACTION" value="DO_EMAIL"';
	  echo '<input type="hidden" name="DATABASE" value="';
	  echo "{$rtn[0]}\">\n";
	  echo '<input type="hidden" name="FULLNAME" value="';
	  echo "{$rtn[1]}\">\n";
	  echo '<input type="hidden" name="UNAME" value="';
	  echo "{$rtn[2]}\">\n";
	  echo '<input type="hidden" name="EMAILTO" value="';
	  echo "{$rtn[3]}\">\n";
	  echo '</td><td>';
	  echo '<textarea  name="BODY" rows="20" cols="80">';
	  echo $rtn[4];
	  echo '</textarea>';
	  echo '</td></tr>';
	  echo '</form>';
	  }
	elseif(is_bool($rtn)){
	  echo "Sorry, an error occurred.  Please correct and try again.\n";
	  }
	elseif(is_int($rtn)) {
	  echo "Don't know what happened, Please try again.\n";
	}
	echo '</div>';
	break;
      case 'VALIDATE':
	//$my_dbinfo =& get_db_info('mass_model');
	//if(! $my_dbinfo) {
	//  $adminerr="Could not get database information for 'mass_model'";
	//  trigger_error($adminerr,E_USER_ERROR);
	//  return;
	//  }
	//decode($my_dbinfo->password,$my_dbinfo->key); 
	echo '<div class="display">';
	validate_password($my_dbinfo,$text_array);
	echo '</div>';
	break;
      case 'DO_EMAIL':
	handle_email();
	// write a 'whole' document because its the correct thing to do
        echo "<html><body></body></html>";
	break;
      case 'RESET':
	//$my_dbinfo =& get_db_info('mass_model');
	//if(! $my_dbinfo) {
	//  $adminerr="Could not get database information for 'mass_model'";
	//  trigger_error($adminerr,E_USER_ERROR);
	//  return;
	//  }
	//decode($my_dbinfo->password,$my_dbinfo->key); 
	echo '<div class="display">';
	handle_pw_reset($my_dbinfo,$text_array);
	echo '</div>';
	break;
      default:
	$adminerr= "Unknown function: {$_POST['ADMIN_ACTION']}";
	trigger_error($adminerr,E_USER_ERROR);
      }
    return;
    }
  else {
    // output a set of forms for the user to select options from
    echo '<div class="norm">';
    echo '<ul>'. "\n";
    echo '<li><h3>Register New User</h3>'. "\n";
    /*
    * the site registration form.  This allows a choice of database
    * the the user enters all the required info, on submission a javascript
    * will do a quick and dirty validation, bounce the passwords and then 
    * mangle them if they are the same.
    */
    echo '<form name="register" action="registration.php" method="POST"';
    echo ' enctype="multipart/form-data" ';
    echo 'onsubmit="return reg_checkscript();">'. "\n";
    echo '<input type="hidden" value="REGISTER" 
    		name="ADMIN_ACTION">'. "\n";
    echo '<input type="hidden" value="" 
    		name="ENCODE">'. "\n";
    echo '<input type="hidden" value="some value" 
    		name="DUMMY">'. "\n";
    echo '<table width="900" border="10">';
    echo '<caption>New User Registration</caption>';
    /*
    echo '<tr align="center" valign="top"><td colspan="4">';
    echo '<br>DATABASE to register: ';
    echo '<select  size="1" name="DATABASE">' . "\n";
    echo '<option>cina</option>' . "\n";
    echo '<option>cinad</option>' . "\n";
    echo '<option>bbn</option>' . "\n";
    echo '<option>bbndev</option>' . "\n";
    echo '</select>' . "\n";
    echo '</td>';
    echo "\n";
    echo '</tr>';
    */

    echo '<tr>';
    echo '<td class="blank">FULL NAME: </td>';
    echo '<td class="blank">';
    echo '<input tabindex="1" type="text" name="NAME" size="30"';
    echo ' class="req">' . "\n";
    echo '</td>';
    echo '<td rowspan="3" class="blank">ADDRESS:</td>';
    echo '<td rowspan="3" class="blank">';
    echo '<textarea tabindex="8" rows="4" cols="40" name="ADDRESS"';
    echo ' class="req">';
    echo '</textarea></td>'. "\n";
    echo '</tr>';

    echo '<tr>';
    echo '<td class="blank">USERNAME: </td>';
    echo '<td class="blank">';
    echo '<input tabindex="2" type="text" name="USERNAME" class="req">'. "\n";
    echo '</td>';
    echo '</tr>';

    echo '<tr>';
    echo '<td class="blank">EMAIL: </td>';
    echo '<td class="blank">';
    echo '<input tabindex="3" type="text" size="40" name="EMAIL"';
    echo ' class="req">'. "\n";
    echo '</td>';

    echo '<tr>';
    echo '<td class="blank">INSTITUTION: </td>';
    echo '<td class="blank">';
    echo '<input tabindex="4" type="text" name="INSTITUTION" size="40"';
    echo ' class="req">'. "\n";
    echo '</td>';
    echo '<td rowspan="3" class="blank">RESEARCH: </td>';
    echo '<td rowspan="3" class="blank">';
    echo '<textarea tabindex="9" rows="4" cols="40" name="RESEARCH"';
    echo ' class="noreq">';
    echo '</textarea></td>'. "\n";
    echo '</tr>';

    echo '<tr>';
    echo '<td class="blank">COUNTRY: </td>';
    echo '<td class="blank">';
    echo '<input tabindex="5" type="text" name="COUNTRY" class="noreq">'. "\n";
    echo '</td>';

    echo '<tr>';
    echo '<td class="blank">PASSWORD: </td>';
    echo '<td class="blank">';
    echo '<input tabindex="6" type="password" name="PW1" class="req">'. "\n";
    echo '</td>';
    echo '</tr>';

    echo '<tr>';
    echo '<td class="blank">PASSWORD (again): </td>';
    echo '<td class="blank">';
    echo '<input tabindex="7" type="password" name="PW2"  class="req">'. "\n";
    echo '</td>';
    echo '<td rowspan="2" class="blank">INFORMATION: </td>';
    echo '<td rowspan="2" class="blank">';
    echo '<textarea tabindex="10" rows="2" cols="40" name="INFORMATION"';
    echo ' class="noreq">';
    echo '</textarea></td>'. "\n";
    echo '</tr>';

    echo '<tr>';
    echo '<td class="blank" colspan="2">';
    echo 'Use this link to choose a lower-case UCAMS password:';
    echo '<a href="https://ucams.ornl.gov/cgi-bin/cgiwrap/ucams';
    echo '/genpw/ucamgen.cgi?page=INTRO" target="_blank">UCAMS</a>';
    echo '</td>';
    echo '</tr>';
    echo '<tr>';
    echo '<td class="blank" colspan="2" >';
    echo " Passwords must be at least 8 characters long.";
    echo '</td>';
    echo '<td rowspan="2" class="blank">HEAR of SUITE: </td>';
    echo '<td rowspan="2" class="blank">';
    echo '<textarea tabindex="11" rows="2" cols="40" name="HEAR"';
    echo ' class="noreq">';
    echo '</textarea></td>'. "\n";
    echo '</tr>';


    echo '<tr>';
    echo '<td class="blank" align="center" >';
    echo '<input tabindex="12" type="submit" value="Submit Registration">';
    echo '</td><td align="center" class="blank">';
    echo '<input  type="reset" >'. "\n";
    echo '</td>';
    echo '</tr>';


    echo '</table>';
    echo '</form>'. "\n";
    echo '<li><h3>Validate Password</h3>'. "\n";
    /*
    * on submission a javascript does 
    * a quick and dirty validation and mangles the password for sending.
    */
    echo '<form name="validate" action="site_admin_reg.php" method="POST"';
    echo ' enctype="multipart/form-data" ';
    echo 'onsubmit="return val_checkscript();">'. "\n";
    echo '<input type="hidden" value="VALIDATE" 
    		name="ADMIN_ACTION">'. "\n";
    echo '<input type="hidden" value="" 
    		name="ENCODE">'. "\n";
    echo '<input type="hidden" value="some value" 
    		name="DUMMY">'. "\n";
    echo '<table width="700" border="10">';
    echo '<caption>Password Validation </caption>';
    /*
    echo '<tr align="center" valign="top"><td colspan="2">';
    echo '<br>DATABASE to check: ';
    echo '<select  size="1" name="DATABASE">' . "\n";
    echo '<option>cina</option>' . "\n";
    echo '<option>cinad</option>' . "\n";
    echo '<option>bbn</option>' . "\n";
    echo '<option>bbndev</option>' . "\n";
    echo '</select>' . "\n";
    echo '</td></tr>';
    */

    echo '<tr><td>';
    echo 'PASSWORD:';
    echo '</td><td>';
    echo '<input  type="password" name="PW1">'. "\n";
    echo '</td></tr>';

    echo '<tr><td align="right">';
    echo '<input  type="submit" value="Check">'. "\n";
    echo '</td><td align="left">';
    echo '<input  type="reset" >'. "\n";
    echo '</td></tr>';


    echo '</table>';
    echo '</form>'. "\n";
    echo '<li><h3>Reset A Password</h3>'. "\n";
    /*
    * on submission a javascript does 
    * a quick and dirty validation and mangles the password for sending.
    */
    echo '<form name="reset" action="site_admin_reg.php" method="POST"';
    echo ' enctype="multipart/form-data" ';
    echo 'onsubmit="return val_resetscript();">'. "\n";
    echo '<input type="hidden" value="RESET" 
    		name="ADMIN_ACTION">'. "\n";
    echo '<input type="hidden" value="" 
    		name="ENCODE">'. "\n";
    echo '<input type="hidden" value="some value" 
    		name="DUMMY">'. "\n";
    echo '<table width="700" border="10">';
    echo '<caption>Password Reset </caption>';
    /*
    echo '<tr align="center" valign="top"><td colspan="2">';
    echo '<br>DATABASE to use: ';
    echo '<select  size="1" name="DATABASE">' . "\n";
    echo '<option>cina</option>' . "\n";
    echo '<option>cinad</option>' . "\n";
    echo '<option>bbn</option>' . "\n";
    echo '<option>bbndev</option>' . "\n";
    echo '</select>' . "\n";
    echo '</td></tr>';
    */

    echo '<tr><td>';
    echo 'USERNAME: ';
    echo '<input  type="text" name="USER">'. "\n";
    echo '</td><td>';
    echo 'NEW PASSWORD: ';
    echo '<input  type="password" name="PW1">'. "\n";
    echo '</td></tr>';

    echo '<tr><td align="right">';
    echo '<input  type="submit" value="Reset Password">'. "\n";
    echo '</td><td align="left">';
    echo '<input  type="reset" value="Clear Form" >'. "\n";
    echo '</td></tr>';


    echo '</table>';
    echo '</form>'. "\n";
    echo '</ul>'. "\n";
    echo '</div>';
    echo "<hr><hr><hr>\n";
    }


?>
  </body>
</html>

