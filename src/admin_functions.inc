<?php
/****************************************************************/
/*! \file 
*
* \verbatim
* $Author: bucknerk $
* $Date: 2008/06/04 17:49:47 $
* $Id: admin_functions.inc,v 1.10 2008/06/04 17:49:47 bucknerk Exp $
* 
* $Log: admin_functions.inc,v $
* Revision 1.10  2008/06/04 17:49:47  bucknerk
* fixed/worked at the auth_validate. the function was needlessly duplicated
* it was called with wrong argument.
*
* Revision 1.9  2008/05/28 14:30:30  bucknerk
* Fixes, updates and repairs
*
* Revision 1.8  2008/04/21 20:02:36  bucknerk
* fixed typo
*
* Revision 1.7  2008/04/21 20:01:43  bucknerk
* added stuff and tweaked get_session_id
*
* Revision 1.6  2008/04/18 13:55:55  bucknerk
* bug fixes
*
* Revision 1.5  2008/04/18 13:39:09  bucknerk
* added functionality
*
* Revision 1.4  2008/04/16 13:47:37  bucknerk
* added more functionality
*
* Revision 1.3  2008/04/08 14:39:42  bucknerk
* new
*
* Revision 1.2  2008/04/01 19:59:17  bucknerk
* Added get_session_id (login) and logout. Updated (changed from add) create_model.
*
* Revision 1.1  2008/03/28 17:56:32  bucknerk
* new
*
* Revision 1.7  2007/12/05 20:07:43  bucknerk
* fixed comments
*
* Revision 1.6  2007/09/06 12:56:50  bucknerk
* Just changed a few comments.
*
* Revision 1.5  2007/08/27 15:43:30  bucknerk
* Fixed various typos
*
* Revision 1.4  2007/08/23 18:10:42  bucknerk
* Updated error handling and documentation
*
* Revision 1.3  2007/08/17 19:54:42  bucknerk
* Changed some error reporting and changed the vaidation from two queries to
* one.
*
* Revision 1.2  2007/08/17 14:56:18  bucknerk
* Updated some of the documentation, comments and the like.
*
* Revision 1.1.1.1  2007/08/10 17:12:56  bucknerk
* CINA Evalution interface
*
* \endverbatim
*
* NOTE: Copied from cina_eval for mass_model
*
* This file conains miscellaneous helper functions that may
* be needed in several places.  These are simple administrative tasks.
*
*
*
****************************************************************/

/************************************************************/
/*!
* This function is used to check if the session ID
* is current in the appropriate database (cina or cinad).
* It makes a connection and then does a query on the 
* \tbl{sessions} table with the ID.  The ID's are supposed to be 
* unique so that if it returns any row, we should be good.
* Because part of the interface needs the user's real name
* we need retrieve that from the \tbl{accounts} table using the USERNAME.
*
* \param[in] $database_info  A DB_info object 
* \param[in] $t_array An array of textual strings for output formatting.
*
* \return An array containing USERNAME and ALIAS (the 'real' name).
*
* \input Key-value pair ID 
****************************************************************/
function validate_session_id($database_info,$t_array)
  {
    if(array_key_exists('ID',$_POST)) {
      //decode($database_info->password, $database_info->key);
      //echo $database_info->username . ": " . $database_info->password . "<br>";
      $mylink=mysql_connect('localhost:3306',$database_info ->username ,
	 $database_info->password);
      if(! $mylink) {
	$err= "Could not CONNECT to MySQL server as ";
	$err.= $database_info->username . " using " ;
	$err.= $database_info->password  ;
	trigger_error($err,E_USER_ERROR);
	return false;
	}
	$rtn=mysql_query("USE " . $database_info->dbname);
      if(! $rtn) {
	$err= "USE $database_info->dbname query caused: ";
	$err.= "MYSQL ERROR: " . mysql_error() ;
	trigger_error($err,E_USER_ERROR);
	mysql_close($mylink);
	return false;
       }
    $query_str="SELECT sessions.username,fullname FROM sessions,accounts ";
    $query_str.="WHERE id = ";
    $query_str.= "'" . trim($_POST['ID']) ."' AND " ;
    $query_str.= "accounts.username = sessions.username";
    $rtn=mysql_query($query_str,$mylink);
    if($rtn) {
      if(mysql_num_rows($rtn) > 0) {
	  $row=mysql_fetch_row($rtn);
	  return array("USERNAME" => $row[0],"FULLNAME" => $row[1]);
	  }
      else
      $err="No such session id: {$_POST['ID']}: $query_str";
      trigger_error($err,E_USER_ERROR);
      return false;
      }
    else {
      $err="Could not validate session id: {$_POST['ID']}: ";
      if(mysql_errno()) $err.=mysql_error();
      trigger_error($err,E_USER_ERROR);
      return false;
	}
    }
    else {
      $err="Missing ID from input string ";
      trigger_error($err,E_USER_ERROR);
      return false;
      }
  }


/************************************************************/
/*!
* This allows the program to recover the mangled password
* string. It is fairly simple and should work so long 
* as the storage of the mangled pw's is handled correctly.
* Takes a value passed by reference and returns the decoded
* pw in that reference.  The encoded pw is lost.  
*
* \param[in,out] $input A string of the encoded pw. It is stored as
* integer values * separated by the '/' character.  This is passed 
* by reference and will be * overwritten on exit.
* \param[in] $key The string that is the permutation key.  
*
*************************************************************/
function decode(& $input, $key)
  {
  	$input = "IhWfAbN!2";
  	
    /*$kl = strlen($key);
    $tmp=explode('/',$input);
    $sl = count($tmp);
    if($kl < $sl)
      $mykey=str_repeat($key,((int)($sl/$kl))+1);
    else $mykey=$key;
    $input='';
    for($i=0;$i< $sl;$i++)
      $input.= (chr($tmp[$i]) ^ $mykey[$i]);
    return;*/
    }

 

/************************************************************/
/*!
* This function is used when a user logs in.  It takes a USER and a PASSWORD 
* from the $_POST array and if they match in the database it creates a new 
* session ID and returns it. Session ID is 20 characters formed by creating the 
* MD5 hash (checksum) of the current date and time and using the least 
* significant 20 characters.  The MD5 values is a 32 character hex number.
*
* The username and password are 'cleaned' up by removing any non-alpha 
* characters from user name and any non-alphanumeric characters from the 
* password.  This prevents modifications to the MySQL query string.
*
* \param[in] $database_info is an object passed byu reference that
* has all the information about the databse. 
* \param[in] $t_array is an array that contains elements that are used in 
* formatting the text output.  This is changed if testing (using html output) is 
* being done as opposed to 'production'.
*
* \input Key-value pairs of USER and PASSWORD. 
*
* \output A new session ID.
*
* \sideeffects The sessions table is updated
*
*************************************************************/
function get_session_id(& $database_info,$t_array)
{
  if(array_key_exists('USER',$_POST) 
      && array_key_exists('PASSWORD',$_POST)) 
    {
    decode($database_info->password, $database_info->key);
    $mylink=mysql_connect('localhost:3306',$database_info ->username ,
       $database_info->password);
    if(! $mylink) {
      $err= "Could not CONNECT to MySQL server as ";
      $err.= $database_info->username;
      trigger_error($err,E_USER_ERROR);
      return;
      }
    $rtn=mysql_query("USE " . $database_info->dbname);
    if(! $rtn) {
      $err= "USE $database_info->dbname query caused: ";
      $err.= "MYSQL ERROR: " . mysql_error() ;
      trigger_error($err,E_USER_ERROR);
      mysql_close($mylink);
      return;
     }
    $clean_name=preg_replace("/[^[:alpha:]]/","",trim($_POST['USER']));
    $clean_pw=preg_replace("/[^[:alnum:]]/","",trim($_POST['PASSWORD']));
    if(strlen($clean_pw) != 40) {
      $err= "Invalid password or username: $clean_pw / $clean_name";
      trigger_error($err,E_USER_ERROR);
      mysql_close($mylink);
      return;
     }
    $query_str="SELECT * FROM accounts WHERE  ";
    $query_str.="username = '$clean_name' AND password='$clean_pw'";
    $rtn=mysql_query($query_str,$mylink);
    if($rtn && mysql_num_rows($rtn) == 1) {
      // this means that the username and password worked. Now generate
      // a new session ID. 
      // get the last 20 characters for the session ID
      // starting at position 12 to the end of the string.  Substr counts 
      // starting at 0 so this really does give 20 characters. As md5 returns a 
      // 32 character hex number using the least significant 20 values of it 
      // ought to help reduce the possibility of duplication if two people log 
      // in at the same time.
      $datetime=date("Y-m-d H:m:s");
      $newid=substr(md5($datetime),0,20);
      $query_str="INSERT INTO sessions (id,username,start) VALUES ";
      //$query_str.="('$newid','$clean_name','$datetime')";
      $query_str.="('$newid','$clean_name', now())";
      $rtn=mysql_query($query_str);
      if($rtn) {
	printf($t_array['header'],"ID=$newid");
	echo "{$t_array['footer']}";
	mysql_close();
	return true;
      }
      else {
	$err="Could not create new session:";
	if(mysql_errno() > 0) {
	  $err.="MYSQL: " .  mysql_error();
	}
	trigger_error($err,E_USER_ERROR);
	return false;
      }
    }
    else {
      $err="User name or Password does not validate:";
      if(mysql_errno()) $err.=mysql_error();
      trigger_error($err,E_USER_ERROR);
      return false;
    }
  }
  else {
    $err=" Missing one of USER name or PASSSWORD from input";
    trigger_error($err,E_USER_ERROR);
    return false;
  }
}

/************************************************************/
/*!
* This function is used when a user logs out.  It takes the session ID
* from the $_POST array and and 'finishes' the session.
*
* \param[in] $database_info is an object passed byu reference that
* has all the information about the databse. 
*
* \param[in] $t_array is an array that contains elements that are used in 
* formatting the text output.  This is changed if testing (using html output) is 
* being done as opposed to 'production'.
*
* \output Nothing.
*
* \sideeffects The session information is copied to a log table for statistics.
*************************************************************/
function terminate_session(& $database_info,$t_array)
{
  if(array_key_exists('ID',$_POST) )
    {
    decode($database_info->password, $database_info->key);
    $mylink=mysql_connect('localhost:3306',$database_info ->username ,
       $database_info->password);
    if(! $mylink) {
      $err= " Could not CONNECT to MySQL server as ";
      $err.= $database_info->username;
      error_log($err); 
      return;
      }
    $user=mysql_query("USE " . $database_info->dbname);
    if(! $user) {
      $err= " USE $database_info->dbname query caused: ";
      $err.= "MYSQL ERROR: " . mysql_error() ;
      error_log($err); 
      mysql_close($mylink);
      return;
     }
    update_session();
    $clean_id=preg_replace("/[^[:alnum:]]/","",trim($_POST['ID']));
    $query_str="SELECT * FROM sessions WHERE id='$clean_id'";
    $rtn=mysql_query($query_str);
    if(!$rtn || mysql_num_rows($rtn) == 0 )  {
      $err="Could not access session log on session end:";
      if(mysql_errno() != 0) {
	$err.=" " . mysql_error();
	}
      else {
	$err.=" Session ID was: $myid";
      }
      error_log($err); 
      mysql_close($mylink);
      return;
    }
    $row=mysql_fetch_assoc($rtn);
    $query_str="INSERT INTO session_log (username,id,start,lastrequest,";
    $query_str.="request,end) VALUES (";
    $query_str.= "'{$row['username']}','{$row['id']}','{$row['start']}',";
    $query_str.="'{$row['lastrequest']}','{$row['request']}',now())";
    $rtn=mysql_query($query_str);
    if(!$rtn || ! ($myid=mysql_insert_id()) )  {
      // insert id is 0 if no insert or false is no db
      $err="Could not update session log on session end:";
      if(mysql_errno() != 0) {
	$err.=" " . mysql_error();
	}
      else {
	$err.=" Insert ID was: $myid";
      }
      error_log($err); 
      mysql_close($mylink);
      return;
    }
    else {
      //echo "QUERY WAS: $query_str<br>";
      //echo "DB WAS: " . $database_info->dbname . "<br>";
      //echo "INSERT ID was:" . mysql_insert_id() . "<br>";
      $query_str="DELETE FROM sessions WHERE s_index={$row['s_index']}";
      $rtn=mysql_query($query_str);
      if(!$rtn || mysql_affected_rows() == 0) {
	$err="Could not delete session $clean_id, index {$row['s_index']}";
	error_log($err); 
	mysql_close($mylink);
	return;
      }
      printf($t_array['header'],"<br>");
    }
  mysql_close($mylink);
  }
  else {
    $err=" Missing one of USER name or PASSSWORD from input";
    error_log($err); 
  }
  return;
}

/*****************************************************************************/
/*
* This function validates the http supplied username and password against the 
* database specified by the argument. 
*
* \param[in] $dbname is the name of the database to use for account information
* to retrieve passwords and usernames.
* \return <em>true</em> is the validation succeeds and <em>false</em> otherwise.
*
*****************************************************************************/
function auth_validate($dbname)
{
  include_once('information.inc');
  if(array_key_exists('PHP_AUTH_PW',$_SERVER) &&
    array_key_exists('PHP_AUTH_USER',$_SERVER)) {
    $database_info =& get_db_info($dbname);
    if(! $database_info) {
      $err="ERROR: Could not get database information for ";
      $err.= $dbname;
      $err.="\n";
      error_log($err);
      return $err;
      }
    decode($database_info->password,$database_info->key); 

    $mylink=mysql_connect('localhost:3306',$database_info ->username ,
	  $database_info->password);
    if(! $mylink) {
      $err="ERROR: COULD NOT CONNECT to MySQL server as ";
      $err.= $database_info->username . "\n";
      error_log($err);
      return $aerr;
      }
    $rtn=mysql_query("USE $database_info->dbname");
    if(! $rtn) {
      $err= "ERROR: USE " . $database_info->dbname ;
      $err.=" query caused: MYSQL ERROR: " . mysql_error() ;
      $err.= "\n";
      error_log($err);
      mysql_close($mylink);
      return $err;
      }
    //  BUT the names are not the things. MySQL is a little strange in that you 
    //  can use the numeric value OR the string name but you must use them in 
    //  the correct context. Using the &amp; operator is a numeric 
    //  operation and so "permissions &amp; 'admin'" results in a warning 
    //  because the string is converted to a numeric value, which is out of 
    //  range of the set, and a warning is issued.  However "permissions &amp;
    //  16 works just fine.  Note that binary "10000" is 2<sup>4</sup> which is 
    //  16.
    $query_str="SELECT username FROM accounts WHERE ";
    $query_str.="password=sha1('" . $_SERVER['PHP_AUTH_PW'] . "')";
    $query_str.=" AND permission & 16";
    $rtn=mysql_query($query_str);
    if(!$rtn) {
      $err= "ERROR: $query_str\n" ;
      $err.="caused: MYSQL ERROR: " . mysql_error() ;
      $err.= "\n";
      error_log($err);
      mysql_close($mylink);
      return $err;
      }
    if(mysql_num_rows($rtn) == 0) {
      mysql_close($mylink);
      error_log("NO MATCH: $query_str\n");
      return "NO MATCH: $query_str";
      }
    $row=mysql_fetch_row($rtn);
    if($row[0] == $_SERVER['PHP_AUTH_USER'] ) {
      mysql_close($mylink);
      return true;
    }
  }
  else {
    $buf="";
    foreach($_SERVER as $key => $value) {
      $buf.="$key => $value<br>";
      }
    error_log("INVALID REQUEST: \n$buf\n");
    return $buf;
  }
}

function update_session()
{
    $query_str="UPDATE sessions set lastrequest=now(),request='";
    $query_str.=$_POST['ACTION'] . "' WHERE id=";
    $query_str.="'{$_POST['ID']}'";
    $rtn=mysql_query($query_str);
    if(!$rtn) {
      $err= "UPDATE session error: ";
      $err.= " MYSQL ERROR: " . mysql_error() ;
      error_log($err);
      return;
      }
}
  ?>
