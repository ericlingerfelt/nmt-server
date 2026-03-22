<?php

/*****************************************************************************/
/*! \file
*
* \verbatim
*  $Author: elingerf $
*  $Date: 2008/09/16 19:58:37 $
*  $Id: mass_admin_handlers.inc,v 1.6 2008/09/16 19:58:37 elingerf Exp $
*
*  $Log: mass_admin_handlers.inc,v $
*  Revision 1.6  2008/09/16 19:58:37  elingerf
*  no message
*
*  Revision 1.5  2008/05/30 12:33:36  bucknerk
*  delete listing error
*
*  Revision 1.4  2008/05/28 14:30:30  bucknerk
*  Fixes, updates and repairs
*
*  Revision 1.3  2008/04/22 14:22:18  bucknerk
*  fixed stuff
*
*  Revision 1.2  2008/04/16 13:47:37  bucknerk
*  added more functionality
*
*  Revision 1.1  2008/04/08 14:39:43  bucknerk
*  new
*
*  Revision 1.11  2008/03/06 18:39:26  bucknerk
*  changed how the php_errorlog is handled/parsed and the output formatted.
*
*  Revision 1.10  2008/03/05 13:34:30  bucknerk
*  minor changes
*
*  Revision 1.9  2008/02/25 20:45:06  bucknerk
*  minor changes
*
*  Revision 1.8  2007/12/05 19:37:01  bucknerk
*  fixed doc comment
*
*  Revision 1.7  2007/11/09 16:22:17  bucknerk
*  changed a displayed title
*
*  Revision 1.6  2007/10/15 18:05:31  bucknerk
*  Removed the handle_registration function. It is now in
*  site_admin_registration_handlers.inc.
*
*  Revision 1.5  2007/09/19 15:52:15  bucknerk
*  Moved functions around, they were not actually the proper files. Also
*  changed some of the display and added a check of username for uniqueness
*  before registering a new user.
*
*  Revision 1.4  2007/09/19 12:45:48  bucknerk
*  reaction_handlers.inc
*
*  Revision 1.3  2007/09/07 13:12:18  bucknerk
*  site_admin.css
*
*  Revision 1.2  2007/09/06 12:03:20  bucknerk
*  Changed at about line 347 to italicize the output line.  Added
*  "handle_registration" function.
*
*  Revision 1.1  2007/08/23 18:08:54  bucknerk
*  New files. Idea is to do straight web-based stuff with these, no Java.
*
*  
* \endverbatim
*
*  This file contains those fuunctions that are used bye the site_admin.php
*  file.  It is much easier to read these here than one huge file of code.
*  These only access the database(s) as necessisary and return HTML formatted
*  data.  Not really a big deal. Please note that this information is not
*  currently being returned across any connection, is it supposed to be viewed
*  directly as a web page in a browser. Any newlines that you see are for the
*  pretty-printing of the HTML if is necessary to figure why it does not
*  display what is wanted.
*****************************************************************************/

/*****************************************************************************/
/*!  
* This function's purpose is to display information on the current
* accounts.  All validation was done before this and we only have to return
* the data that we have recovered.
*
* \param[in] $database_info  A DB_info object passed as a reference
* \param[in] $t_array An array of text chosen at the top level for formatting 
* the form
* \output An html document that has the account information in tablular form
* \return None
* \sideeffect None
*****************************************************************************/
function handle_get_account_data(& $database_info, $t_array) {
    
    $mylink=mysql_connect('localhost:3306',$database_info ->username ,
	  $database_info->password);
    if(! $mylink) {
      $err="Could not connect to MySQL server as ";
      $err.= $database_info->username . "<br>\n";
      trigger_error($err,E_USER_ERROR);
      return;
      }
    $rtn=mysql_query("USE $database_info->dbname");
    if(! $rtn) {
      $err= "USE " . $database_info->dbname ;
      $err.=" query caused: MYSQL ERROR: " . mysql_error() ;
      $err.= "<br>\n";
      trigger_error($err,E_USER_ERROR);
      mysql_close($mylink);
      return;
      }
    $query_str="SELECT username,fullname,email,institution,country ";
    $query_str.="FROM accounts ORDER BY country,institution";
    $rtn=mysql_query($query_str,$mylink);	
    if($rtn) {
      if(mysql_num_rows($rtn) > 0 ) {
	echo "<h2>Current Accounts for ";
	echo strtoupper($database_info->dbname) . "</h2>\n"; 
	echo '<table width="99%" border="1">' ."\n";
	echo "<tr>\n";
	echo "<th>Username</th><th>Full Name</th>\n";
	echo "<th>Email</th><th>Institution</th>\n";
	echo "<th>Country</th></tr>\n";
	$row=mysql_fetch_row($rtn);
	while($row) {
	  echo "<tr><td>{$row[0]}</td><td>{$row[1]}</td>\n";
	  echo "<td>{$row[2]}</td><td>{$row[3]}</td>\n";
	  echo "<td>{$row[4]}</td></tr>\n";
	  $row=mysql_fetch_row($rtn);
	  }
	echo "</table>\n";
	}
      else {
	// assume that there is really no error
	echo "<h3><i>There are no accounts for</i> - ";
	echo $database_info->dbname . "</h3>\n";
	  }
      }
    else {
      $err="Could not retrieve account info from database: ";
      $err.="MYSQL ERROR: " . mysql_error();
      trigger_error($err,E_USER_ERROR);
      }
    mysql_close($mylink);
    return;
  }

/*****************************************************************************/
/*!  This function prints out the PHP error log.  This log is the file
* <tt>/var/log/php_error.log</tt>.  All validation was done before this and
* we only have to return the data that we have recovered.  \deprecated In
* this case, part of the information in the php error logs is XML encoded.
* Just because it is.  If you do not understand XML just go to www.w3c.org
* and use one of the very basic XML tutorials. Nothing earthshaking here
* except that our parser requires a VALID document. This will be removed
* soon because I don't believe that we will continue with XML in the error
* logs, takes up too much space and does not really add anything. 
* 
* There is one major difference between this and the rest of the functions
* we have been looking at, php errors are in a file so we won't use mysql
* functions.
*
* The format of the error_log entries is always the date/time of the entry
* enclosed in square brackets followed by ANY other text. 
*
* \param[in] $my_dbinfo  A DB_info object passed as a reference
* \param[in] $text_array An array of text chosen at the top level for
* formatting the form
* \output An html document that has the PHP error log data in tablular form
* \return None
* \sideeffect Outputs a table containing the contents of the PHP error log
*****************************************************************************/

function handle_get_php_errorlog($my_dbinfo,$text_array)
{

  // Again we use the @ to supress an error message
  // opening the command output as a readonly file 
  $fp=popen('tail -50 /var/log/php_error.log','r');
  if(!$fp) {
    $err= "ERROR: Could not open tail -50 /var/log/php_error.log<br>\n";
    trigger_error($err,E_USER_ERROR);
    return;
    }
  echo '<h2>USER errors</h2>'. "\n";
  echo '<table border="1" width="99%">'. "\n";
  echo '<tr><th >Date</th><th>Type</th>'. "\n";
  echo '<th width="40%">Msg</th><th width="20%">Filename</th>';
  echo '<th width="5%">Lineno</th></tr>'."\n";

  while($input=fgets($fp)) {
    if(preg_match('/\[.*\]/',$input)) {
      $fields=preg_split("/:/",$input,4);
      //$mydate=preg_replace("/\].*/","",$fields[0]);
      $mydate=preg_replace("/\[/","",$fields[0]);
      $mydate.= ":" . $fields[1] ;
      $mydate.= ":" . preg_replace("/\].*/","",$fields[2]);
      // the date
      echo "<tr><td>$mydate</td>\n";
      //echo "1. " . $fields[1] . "<br>";
      //echo "2. " . $fields[2] . "<hr>";
      $buf=preg_replace("/.*\]/","",$fields[2]);
      // the error type
      echo "<td>$buf </td>\n";
      if(array_key_exists(3,$fields)) {
	$fields=preg_split("/ in /",$fields[3],2);
	}
      // the message
      echo "<td>" . $fields[0] . "</td>\n";
      if(array_key_exists(1,$fields)) {
	$fields=preg_split("/ on line /",$fields[1],2);
	// the filename
	echo "<td>" . preg_replace("/^.*www/","",$fields[0]) . "</td>\n";
	// the line number
	if(array_key_exists(1,$fields)) {
	  echo "<td>" . trim($fields[1]) . "</td>\n</tr>\n";
	  }
	else {
	  echo "<td>&nbsp;</td></tr>\n";
	  }
	}
      else {
	echo "<td>&nbsp;</td><td>&nbsp;</td></tr>\n";
	}
    }
  }
  fclose($fp);
  // this is the end of the valid document body and we 
  // call the parser with <b><i>true</i></b> to tell it to 
  // clean up and finish.
  echo "</table></div></body></html>";
  return;
  }


/*****************************************************************************/
/*!  This functions purpose is to display information on the current
* sessions.  All validation was done before this and we only have to return
* the data that we have recovered.
*
* \param[in] $database_info  A DB_info object passed as a reference
* \param[in] $t_array An array of text chosen at the top level for
* formatting the form
* \output An html document that has the session information in tablular
* \sideeffect None
*****************************************************************************/
function handle_get_session_data(& $database_info, $t_array) {
    
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
    $query_str="SELECT sessions.username,fullname,lastrequest,request ";
    $query_str.="FROM sessions,accounts WHERE ";
    $query_str.="accounts.username=sessions.username ";
    $rtn=mysql_query($query_str,$mylink);	
    if($rtn) {
      if(mysql_num_rows($rtn) > 0 ) {
	echo "<h2>Current Sessions for";
	echo strtoupper($database_info->dbname) . "</h2>\n"; 
	echo '<div style="text-align:center;">' . "\n";
	echo '<form action="mass_admin.html" method="POST">' . "\n";
	echo '<input type="hidden" name="ADMIN_ACTION"';
        echo ' value="PURGE_SESSIONS">' . "\n";
	echo '<input type="hidden" value="mass_model" name="DATABASE">' . "\n";
	echo '<input type="submit" value="Purge Sessions to Log">' . "\n";
	echo "</form></div>\n";

	echo '<table width="99%" border="1">' ."\n";
	echo "<tr>\n";
	echo "<th>User</th><th>Full Name</th>\n";
	echo "<th>Last Action</th><th>Last Action Date</th></tr>\n";
	$row=mysql_fetch_row($rtn);
	while($row) {
	  echo "<tr><td>{$row[0]}</td><td>{$row[1]}</td>\n";
	  echo "<td>{$row[3]}</td><td>{$row[2]}</td></tr>\n";
	  $row=mysql_fetch_row($rtn);
	  }
	echo "</table>\n";
	}
      else {
	// assume that there is really no error
	echo "<h3><i>There are no current sessions for</i> - ";
	echo $database_info->dbname . "</h3>\n";
	  }
      }
    else {
      $err="Could not retrieve session info from database: ";
      $err.="MYSQL ERROR: " . mysql_error();
      trigger_error($err,E_USER_ERROR);
      }
    mysql_close($mylink);
    return;
  }


/***********************************************************/
/*! 
*
* This is simple function used by site_admin.php to do one thing,
* read, parse and display the contents of the registration
* table/registration file for the associated database.  The reason 
* for this to allow the administrators to back-check the registration
* process. 
* 
* \param[in] $my_dbinfo  A DB_info object passed as a reference
* \param[in] $t_array An array of text chosen at the top level for
* formatting the form
* \output An html table that contains the registration information.
* \sideeffect None
*
***********************************************************/

function handle_get_registrations($my_dbinfo,$t_array)
{
  $mylink=mysql_connect('localhost:3306',$my_dbinfo ->username ,
	$my_dbinfo->password);
  if(! $mylink) {
    $err="ERROR: COULD NOT CONNECT to MySQL server as ";
    $err.= $my_dbinfo->username . "<br>\n";
    trigger_error($err,E_USER_ERROR);
    return;
    }
  $rtn=mysql_query("USE $my_dbinfo->dbname");
  if(! $rtn) {
    $err= "USE " . $my_dbinfo->dbname ;
    $err.=" query caused: MYSQL ERROR: " . mysql_error() ;
    $err.= "<br>\n";
    trigger_error($err,E_USER_ERROR);
    mysql_close($mylink);
    return;
    }
  $query_str="SELECT lastname,firstname,date,complete,email,";
  $query_str.="institution,";
  $query_str.="address,research,hearof,info ";
  $query_str.="FROM register_requests";
  $regarray=mysql_query($query_str);
  if(! $regarray) {
    $err="Could not get Registration information<br>";
    if(mysql_errno() > 0) {
      $err.= mysql_error() . "<br>";
      }
    trigger_error($err,E_USER_ERROR);
    mysql_close($mylink);
    return;
    }
  echo '<table width="99%" border="2"><caption>Registration Data</caption>';
  echo "<tr>\n";
  echo "<th>Last Name</th>\n";
  echo "<th>First Name</th>\n";
  echo "<th>Date</th>\n";
  echo "<th>Time</th>\n";
  echo "<th>Completed</th>\n";
  echo "<th>Email</th>\n";
  echo "</tr>\n";
  echo "<tr>\n";
  echo '<th colspan="3">Institution</th>' . "\n";
  echo '<th colspan="3">Mailing Address</th>' . "\n";
  echo "</tr>\n";
  echo "<tr>\n";
  echo '<th colspan="2">Research Type</th>' . "\n";
  echo '<th colspan="2">Hear About Us</th>' . "\n";
  echo '<th colspan="2">Additional Info</th>' . "\n";
  echo "</tr>\n";
  $row=mysql_fetch_row($regarray);
  while($row) {
    echo '<tr >' . "\n";
    echo '<td class="regtl">' . "{$row[0]}</td>\n"; // last
    echo '<td class="regtm">' . "{$row[1]}</td>\n"; // first
    $dte=explode(' ',$row[2]);
    echo '<td class="regtm">' . "{$dte[0]}</td>\n"; //date
    echo '<td class="regtm">' . "{$dte[1]}</td>\n"; // time
    echo '<td class="regtm">' . "{$row[3]}</td>\n"; //IP (not saved)
    echo '<td class="regtr">' . "{$row[4]}</td>\n"; //email
    echo "</tr>\n";
    echo '<tr >' . "\n";
    //institution
    echo '<td colspan="3" class="regml">' . $row[5] . '</td>' . "\n";
    //address
    echo '<td colspan="3" class="regmr">' . $row[6] . '</td>' . "\n";
    echo "</tr>\n";
    echo '<tr class="reg">' . "\n";
    //research
    echo '<td colspan="2" class="regbl">' . $row[7] . '</td>' . "\n";
    //heardof
    echo '<td colspan="2" class="regbm">' . $row[8] . ' </td>' . "\n";
    //info
    echo '<td colspan="2" class="regbr">' . $row[9] . '</td>' . "\n";
    echo "</tr>\n";
    $row=mysql_fetch_row($regarray);
    }
  echo "</table>\n";
  return;
}

/**************************************************************************/
/*!
* This function actually sends the email. It is called from handle_email() as 
* well as from handle_create_file(). 
*
* I had originally planned to use the system utilities but that is a
* somewhat painful process. This has been updated to create a two-part mime 
* message, plain-text and html. 
*
* <h3>Depends On:</h3> the Pear Mail and Mail_mime packages.  This easily 
* retrieved using the <b>pear</b> command line tool, works similar to 
* <i>rpm</i> or <i>up2date</i>. 
*
* \sideeffect Sends a preformatted email to a newly registered user. You
* can change any of the Cc, Bcc etc by simply modifying the $hdrs array
* contents.
*
*****************************************************************************/

function send_email($sendto,$headers,$body,$hbody=null)
{
    include('Mail.php');
    if(! is_null($hbody)) {
      include('Mail/mime.php');
      // pass this whatever you use on your system as a line ending
      // on Windows this would be <cr><lf> which is \r\n
      $mime = new Mail_mime("\n");

      // have to call these in this order!
      $mime->setTXTBody($body);
      $mime->setHTMLBody($hbody);
      $body=$mime->get();
      $headers=$mime->headers($headers);
    }
    /*
    * The -f is passed to sendmail and becomes the "return address"
    */
    $mail=& Mail::factory('mail','-f coordinator@nuclearmasses.org');

    $rtn=$mail->send($sendto,$headers,$body);
    return $rtn;
}

/*****************************************************************************/
/*!  This functions purpose is purge the non-current
* sessions.  All validation was done before this and we only have to return
* the data that we have recovered.
*
* \param[in] $database_info  A DB_info object passed as a reference
* \param[in] $t_array An array of text chosen at the top level for
* formatting the form
* \output An html document that has the session information in tablular of the
* remaining sessions after the purge.
* \sideeffect None
*****************************************************************************/
function handle_purge_sessions(& $database_info, $t_array) 
{
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
  $end_date=date('Y-m-d H:i:s',(time() - 86400));
  $query_str="SELECT * FROM sessions ";
  $query_str.="WHERE (lastrequest IS NOT NULL AND lastrequest <";
  $query_str.=" '$end_date') OR (lastrequest IS NULL AND start < '$end_date')";
  $rtn=mysql_query($query_str,$mylink);	
  if($rtn) {
    if(($rc=mysql_num_rows($rtn)) > 0 ) {
      while($row=mysql_fetch_assoc($rtn)) {
	$query_str="INSERT INTO session_log (username,id,start,lastrequest,";
	$query_str.="request,end) VALUES (";
	$query_str.= "'{$row['username']}','{$row['id']}','{$row['start']}',";
	$query_str.="'{$row['lastrequest']}','{$row['request']}',now())";
	$lrtn=mysql_query($query_str);
	if(!$lrtn || ! ($myid=mysql_insert_id()) )  {
	  // insert id is 0 if no insert or false is no db
	  $err="Could not update session log on session end:";
	  if(mysql_errno() != 0) {
	    $err.=" " . mysql_error();
	    }
	  else {
	    $err.=" Insert ID was: $myid";
	  }
	  trigger_error($err,E_USER_NOTICE); 
	}
	else {
	  $query_str="DELETE FROM sessions WHERE s_index={$row['s_index']}";
	  $lrtn=mysql_query($query_str);
	  if(!$lrtn || mysql_affected_rows() == 0) {
	    $err="Could not delete session $clean_id, index {$row['s_index']}";
	    trigger_error($err,E_USER_NOTICE); 
	  }
	}
      }
      echo "<h3>Purged $rc sessions from - ";
      echo $database_info->dbname . "</h3>\n";
    }
    else {
      // assume that there is really no error
      echo "<h3><i>There are no sessions to purge</i> - ";
      echo $database_info->dbname . "</h3>\n";
    }
  }
  else {
    $err="Could not retrieve session info from database: ";
    $err.="MYSQL ERROR: " . mysql_error();
    trigger_error($err,E_USER_ERROR);
    echo "</body></html>";
    return;
    }
  $query_str="SELECT sessions.username,fullname,lastrequest,request ";
  $query_str.="FROM sessions,accounts WHERE ";
  $query_str.="accounts.username=sessions.username ";
  $rtn=mysql_query($query_str,$mylink);	
  if($rtn) {
    echo "<h2>Current Sessions Remaining for";
    echo strtoupper($database_info->dbname) . ":$end_date</h2>\n"; 
    echo '<div style="text-align:center;">' . "\n";
    echo '<form action="mass_admin.html" method="POST">' . "\n";
    echo '<input type="hidden" name="ADMIN_ACTION"';
    echo ' value="PURGE_SESSIONS">' . "\n";
    echo '<input type="hidden" value="mass_model" name="DATABASE">' . "\n";
    echo '<input type="submit" value="Purge Sessions to Log">' . "\n";
    echo "</form></div>\n";

    echo '<table width="99%" border="1">' ."\n";
    echo "<tr>\n";
    echo "<th>User</th><th>Full Name</th>\n";
    echo "<th>Last Action</th><th>Last Action Date</th></tr>\n";
    $row=mysql_fetch_row($rtn);
    while($row) {
      echo "<tr><td>{$row[0]}</td><td>{$row[1]}</td>\n";
      echo "<td>{$row[3]}</td><td>{$row[2]}</td></tr>\n";
      $row=mysql_fetch_row($rtn);
      }
    echo "</table>\n";
    }
  else {
    $err="Could not retrieve session info from database: ";
    $err.="MYSQL ERROR: " . mysql_error();
    trigger_error($err,E_USER_ERROR);
    }
  mysql_close($mylink);
  return;
}
/*****************************************************************************/
/*!  This functions purpose is list the models currently available in a form
* so they can be selected for deletion.
*
* \param[in] $database_info  A DB_info object passed as a reference
* \param[in] $t_array An array of text chosen at the top level for
* formatting the form
* \output An html document that has the models and can be used to select one for 
* deletion
* \sideeffect None
*****************************************************************************/
function handle_delete_models(& $database_info, $t_array) 
{
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
  $query_str="SELECT m_index,user,name,type,added,updated FROM models ";
  $query_str.="ORDER BY type,user,name";
  $rtn=mysql_query($query_str);	
  if($rtn && mysql_num_rows($rtn) > 0) {
    echo "<h2>Current Models for";
    echo strtoupper($database_info->dbname) . "</h2>\n"; 
    echo '<table width="99%" border="1">' ."\n";
    echo "<tr>\n";
    echo "<th>&nbsp;</th><th>User</th><th>Model Name</th><th>Type</th>\n";
    echo "<th>Date Added</th><th>Last Updated</th></tr>\n";
    while($row=mysql_fetch_row($rtn)) {
      echo "<tr>\n";
      echo '<td align="center">';
      echo '<form action="mass_admin.html" method="POST">' . "\n";
      echo '<input type="hidden" name="ADMIN_ACTION" value="VERIFY_DELETE">';
      echo '<input type="hidden" name="M_INDEX" value="' . $row[0];
      echo '">' . "\n";
      echo '<input type="hidden" name="DATABASE" value="';
      echo $_POST['DATABASE'] . '">';
      echo '<input type="submit" value="DELETE">' . "\n";
      echo '</form>';
      echo "<td>{$row[1]}</td>\n";
      echo "<td>{$row[2]}</td>\n";
      echo "<td>{$row[3]}</td>\n";
      echo "<td>{$row[4]}</td>\n";
      echo "<td>{$row[5]}</td></tr>\n";
      }
    echo "</table>\n";
    echo "</body></html>";
  }
  else {
    $err="Could not retrieve model info from database: ";
    $err.="MYSQL ERROR: " . mysql_error();
    trigger_error($err,E_USER_ERROR);
    echo "</body></html>";
    return;
    }
  return;
}


/*****************************************************************************/
/*!  This functions purpose is to remove a model completely, from both the 
* database and the filesystem.
*
* \param[in] $database_info  A DB_info object passed as a reference
* \param[in] $t_array An array of text chosen at the top level for
* formatting the form
* \output An html document that has the session information in tablular
* \sideeffect Delete the model file from the filesystem and remove the entry 
* from the database.
*****************************************************************************/
function handle_verify_delete_model(& $database_info, $t_array) 
{
  if(array_key_exists('M_INDEX',$_POST)) {

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
    $query_str="SELECT * FROM models ";
    $query_str.="WHERE m_index='{$_POST['M_INDEX']}'";
    $rtn=mysql_query($query_str);	
    if($rtn && mysql_num_rows($rtn) == 1) {
      $row=mysql_fetch_assoc($rtn);
      echo '<div class="narrow">' . "\n";
      echo '<table border="1" width="100%">';
      echo '<caption>Model to Delete</caption>' . "\n";
      echo "<tr><th>Name</th><th>Type</th><th>Creator</th></tr>\n";
      echo "<tr><td>{$row['name']}</td>\n";
      echo "<td>" . strtoupper($row['type']) . "</td>\n";
      echo "<td>{$row['user']}</td></tr>\n";
      echo '<tr><td align="center">' . "\n";
      echo '<form script="float: left;" action="mass_admin.html" method="POST">' . "\n";
      echo '<input type="hidden" name="ADMIN_ACTION" value="REALLY_DELETE">';
      echo "\n";
      echo '<input type="hidden" value="' . $_POST['DATABASE'];
      echo '" name="DATABASE">';
      echo "\n";
      echo '<input type="hidden" name="M_INDEX" value="';
      echo $row['m_index'] . '">' . "\n";
      echo '<input type="submit" value="DELETE MODEL"></form></td>' . "\n";
      echo "<td>&nbsp;</td>\n";
      echo '<td align="center">' . "\n";
      echo '<form action="mass_admin.html" method="POST">' . "\n";
      echo '<input type="hidden" value="DELETE_MODELS" name="ADMIN_ACTION">';
      echo "\n";
      echo '<input type="hidden" value="' . $_POST['DATABASE'];
      echo '" name="DATABASE">';
      echo "\n";
      echo '<input type="submit" value="CANCEL">' . "\n";
      echo "</td></tr></table></div>\n";
      }
      else {
	// assume that there is really no error
	echo "<h3>Nothing found to delete</h3>\n";
	if(mysql_errno() > 0) {
	  echo mysql_error();
	}
      }
    mysql_close($mylink);

  }
  else {
    $err="ERROR = Missing TYPE or NAME";
  }
  echo "</body></html>";
  return;
}


/*****************************************************************************/
/*!  This functions purpose is to remove a model completely, from both the 
* database and the filesystem.
*
* \param[in] $database_info  A DB_info object passed as a reference
* \param[in] $t_array An array of text chosen at the top level for
* formatting the form
* \output An html document that has the session information in tablular
* \sideeffect Delete the model file from the filesystem and remove the entry 
* from the database.
*****************************************************************************/
function handle_really_delete_model(& $database_info, $t_array) 
{
  if(array_key_exists('M_INDEX',$_POST)) {

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
    $query_str="DELETE FROM models ";
    $query_str.="WHERE m_index='{$_POST['M_INDEX']}'";
    $rtn=mysql_query($query_str);	
    if($rtn && mysql_affected_rows() == 1) {
      echo "The requested model has been deleted.<br>\n";
      echo '<table><tr><td>';
      echo '<form action="mass_admin.html" method="POST">' . "\n" ;
      echo '<input type="hidden" value="DELETE_MODELS" name="ADMIN_ACTION">';
      echo "\n";
      echo '<input type="hidden" value="' . $_POST['DATABASE'];
      echo '" name="DATABASE">';
      echo "\n";
      echo '<input type="submit" value="Delete More Models">' . "\n";
      echo "</form></td><td>";
      echo '<input type="button" onclick="adminHome();"';
      echo ' value="Return to Admin Page">';
      echo "</td></tr></table>\n";
      
      }
    else {
      echo "<h3>ERROR deleting selected model</h3>\n";
	if(mysql_errno() > 0) {
	  echo mysql_error();
	}
      }
    mysql_close($mylink);
  }
  else {
    $err="ERROR = Missing M_INDEX";
  }
  echo "</body></html>";
  return;
}
?>
