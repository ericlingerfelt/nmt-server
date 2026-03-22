<?php

/*****************************************************************************/
/*! 
* This function's purpose is to add an entry into the <b>accounts</b> tables 
* of the associated database.  It expects that the information is correct
* and it uses a java function run a shell to compute the password to store
* in the database. This java function and the one that the Java interface
* is using produce the same results. The only difference in the two is the
* method in which the encrypted data is converted into a string.  The
* encryption is SHA1 and is a one-way hash.
*
* \param[in] $database_info  A DB_info object passed as a reference
* \param[in] $t_array An array of text chosen at the top level for formatted the
* \return (mixed) A boolean <i>false</i> if there is an error. An array
* contain the email address, username, plaintext password and full name if
* everything went as planned. An integer if the username is already in use
* in the accounts table of the database.
* \sideeffect Adds a new entry into the accounts table of the selected
* database.
*****************************************************************************/
function handle_registration(& $database_info, $t_array)
  {
    if(array_key_exists('USERNAME',$_POST) &&
       array_key_exists('NAME',$_POST) &&
       array_key_exists('EMAIL',$_POST) &&
       array_key_exists('INSTITUTION',$_POST) &&
       array_key_exists('ENCODE',$_POST) &&
       array_key_exists('DUMMY',$_POST) &&
       array_key_exists('PW1',$_POST) &&
       array_key_exists('ADDRESS',$_POST) ) {
    
      // the decode function must have already been called on the object
      $mylink=mysql_connect('localhost:3306',$database_info ->username ,
	    $database_info->password);
      if(! $mylink) {
	$err="Could not CONNECT to MySQL server as ";
	$err.= $database_info->username;
	trigger_error($err,E_USER_ERROR);
	return false;
	}
      $rtn=mysql_query("USE $database_info->dbname");
      if(! $rtn) {
	$err= "USE " . $database_info->dbname ;
	$err.=" query caused: MYSQL ERROR: " . mysql_error() ;
	trigger_error($err,E_USER_ERROR);
	mysql_close($mylink);
	return false;
	}
      /*
      * First we check to see that the username is unique. This means make a
      * call to mysql and see if the a query returns a row. If not proceed.
      * Otherwise we need to return a nice error to the user.
      */
      $query_str="SELECT fullname FROM accounts WHERE username=";
      $query_str.="'{$_POST['USER']}'";
      $rtn=mysql_query($query_str);
      if($rtn) {
	if(mysql_num_rows($rtn) > 0) {
	  $row=mysql_fetch_row($rtn);
	  echo "Sorry, the username {$_POST['USER']}";
	  echo " is already in use by <b>" . $row[0] . "</b><br>";
	  echo "Please return to the registration form and enter";
	  echo " a new username. If you use the browsers back button";
	  echo " you may not have to reenter anything but the ";
	  echo "username and passwords.<br>Thanks<br>";
	  mysql_close($mylink);
	  return 2;
	  }
	}
      else {
	$err= "Query for username failed:";
	if(mysql_errno()) {
	  $err.=" MYSQL ERROR: " . mysql_error() ;
	  }
	trigger_error($err,E_USER_ERROR);
	mysql_close($mylink);
	return false;
	}
      /* 
      * here we decode the password using a simple
      * XOR operation.  Not very secure for everyday operations but for 
      * a one-time deal it should be safe enough. Especially as it would
      * take a serious hacker to figure out what I am using to encode
      * the pw string with.
      */
      $val=rand();
      $arr=explode(',',$_POST['ENCODE']);
      for($i=0;$i<count($arr);$i++) {
	$arr[$i] = chr(ord($_POST[$_POST['PW1']][$i]) ^ $arr[$i]);
	}
      $plain_text=implode($arr);
      //$pwd=@system('java mysha1 ' . $plain_text . "> /tmp/$val.safile" );
      //$string=file_get_contents("/tmp/$val.safile");
      //unlink("/tmp/$val.safile");
      $re=0;
      $inf=0;
      $query_str="INSERT into accounts ";
      $query_str.="(username,fullname,password,email,country,institution";
      $query_str.= ",address,research,info,hearabout,createdate,pwchangedate) ";
      $query_str.="VALUES ('{$_POST['USER']}',";
      $query_str.="'" . preg_replace("/'/","\\'",$_POST['NAME']) . "',";
      if(strlen(trim($_POST['NAME'])) > 64) {
	$_POST['NAME']=substr(trim($_POST['NAME']),0,64);
	}
      $query_str.="sha1('" . $plain_text . "'),";
      $query_str.="'{$_POST['EMAIL']}',";
      $query_str.="'" . preg_replace("/'/","\\'",$_POST['COUNTRY']) . "',";
      if(strlen(trim($_POST['COUNTRY'])) > 128) {
	$_POST['COUNTRY']=substr(trim($_POST['COUNTRY']),0,128);
	}
      $query_str.="'" . preg_replace("/'/","\\'",$_POST['INSTITUTION']) . "',";
      if(strlen(trim($_POST['INSTITUTION'])) > 128) {
	$_POST['INSTITUTION']=substr(trim($_POST['INSTITUTION']),0,128);
	}
      $query_str.="'" . preg_replace("/'/","\\'",$_POST['ADDRESS']) . "',";
      if(strlen(trim($_POST['ADDRESS'])) > 512) {
	$_POST['ADDRESS']=substr(trim($_POST['ADDRESS']),0,512);
	}
      $query_str.="'" . preg_replace("/'/","\\'",$_POST['RESEARCH']) . "',";
      if(strlen(trim($_POST['RESEARCH'])) > 256) {
	$_POST['RESEARCH']=substr(trim($_POST['RESEARCH']),0,256);
	}
      $query_str.="'" . preg_replace("/'/","\\'",$_POST['INFORMATION']) . "',";
      if(strlen(trim($_POST['INFORMATION'])) > 256) {
	$_POST['INFORMATION']=substr(trim($_POST['INFORMATION']),0,256);
	}
      $query_str.="'" . preg_replace("/'/","\\'",$_POST['HEAR']) . "',";
      if(strlen(trim($_POST['HEAR'])) > 256) {
	$_POST['HEAR']=substr(trim($_POST['HEAR']),0,256);
	}
      // this is a mysql function
      $query_str.="curdate(),curdate())";
      $rtn=mysql_query($query_str,$mylink);	
      if(!$rtn) {
	$err="<br>Could not insert new user info into database: ";
	$err.="<br>MYSQL ERROR: " . mysql_error();
	trigger_error($err,E_USER_ERROR);
	return;
	}
      mysql_close($mylink);
      switch($database_info->dbname){
	case 'cina':
	  $command='/var/www/pilot_files/bin/create_user';
	  $rtn=true;
	  break;
	case 'cinad':
	  $command='/var/www/pilot_files_dev/bin/create_user';
	  $rtn=true;
	  break;
	default:
	  $rtn=false;
	  }
      $command.=" {$_POST['USERNAME']}";
      $rtn=@system($command);
      if($rtn) {
	echo "Error occurred making directories for {$_POST['USERNAME']}";
	echo "<br>Command was: $command<br>";
	}
      switch($_POST['DATABASE']){
        case "cina":
	  $base_path='/var/www/pilot_files/adm/';
	  break;
        case "cinad":
	  $base_path='/var/www/pilot_files_dev/adm/';
	  break;
        case "bbn":
        case "bbndev":
	}
      $body="Dear {$_POST['NAME']},\n\n";
      $body.=file_get_contents($base_path . "body.txt");
      $body.="\nusername: {$_POST['USERNAME']}\n";
      $body.="password: $plain_text\n\n";
      $body.=file_get_contents($base_path . "footer.txt");
      return array($_POST['DATABASE'],$_POST['NAME'],$_POST['USERNAME'],
      	$_POST['EMAIL'],$body);
      }
    else {
      $err="Missing one of USERNAME,NAME,EMAIL,INSTITUTION,ADDRESS,";
      $err.= "PW1,DUMMY,ENCODE<br>\n";
      trigger_error($err,E_USER_ERROR);
      }
    return false;
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

/**************************************************************************/
/*!
* This function only prepares an email. It is driven primarily by the output
* from handle_registration().  That is passed back to this function via
* site_admin_reg.php.
*
* Changed this so that the body is sent in from the registration handler
* function. This is so that the user can preview it before it is sent. It
* should even be editable. But I also had to remove the \tbl{\\r} (carriage
* returns) that the web browser put in. How nasty is that?
*
* \input The data it needs is all from the $_POST array.
*
*****************************************************************************/

function handle_email()
{
  if(array_key_exists('FULLNAME',$_POST) &&
     array_key_exists('UNAME',$_POST) &&
     array_key_exists('BODY',$_POST) &&
     array_key_exists('EMAILTO',$_POST)) {
     //$fp=fopen("/tmp/email_check.txt","w+");
    /*
    * hack up a message ID to help disguise where  the email comes
    * from.  Not perfect but makes it less obvious that it comes from ORNL
    */
    $togmt=date("O");
    $now=date("Ymd") . (date("H") + ($togmt/100)) .date("is");
    $now.="." . strtoupper(substr(sha1($now),1,10));
    $now.='@nucastrodata.org';
    /*
    * Set up the email and headers here.  Some of this stuff
    * is intended to hide its origination
    */
    $to=$_POST['EMAILTO'];
    $hdrs= array( 'Date' => date('r'),
      'From' => 'Coordinator<coordinator@nucastrodata.org> ',
      'To' => $_POST['FULLNAME'] . "<{$_POST['EMAILTO']}>",
      'Subject' => 'NUCASTRODATA Registration',
      'Cc' =>'coordinator@nucastrodata.org',
      //Change this if you want blind copies somewhere else
      //Or comment it out if you don't want any at all.
      'Bcc' => 'lingerfeltej@ornl.gov,eric.lingerfelt@gmail.com,smithms@ornl.gov,xrayburst1@yahoo.com',
      'X-Mailer' => 'PHP/' . phpversion(),
      'Message-id' => $now
    );
    $patt=array("/\r/");
    $repl=array('');
    //fwrite($fp,$hdrs . preg_replace($patt,$repl,$_POST['BODY']));

    $rtn=send_email($to,$hdrs,preg_replace($patt,$repl,$_POST['BODY']));
    //fwrite($fp,"\n\nSent the email\nReturn value was $rtn\n");
    }
  else {
    $err="Missing one of FULLNAME,UNAME,EMAILTO<br>\n";
    trigger_error($err,E_USER_ERROR);
    }
  return;
}


/*****************************************************************************/
/*! 
* This function's purpose is to validate a password that you have already
* entered. It really should not be here but the users will never see this
* and it can help make sure that any access problems are not on the server
* end.
*
* \param[in] $database_info  A DB_info object passed as a reference
* \param[in] $t_array An array of text chosen at the top level for formatted the
* \output A table containing the entries in the database account table whose
* passwords match the submitted one.
*****************************************************************************/
function validate_password(& $database_info, $t_array)
  {
    if(array_key_exists('PW1',$_POST) &&
    array_key_exists('ENCODE',$_POST) &&
    array_key_exists('DUMMY',$_POST)) {

    
      // the decode function must have already been called on the object
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
      /* 
      * here is decode the password using a simple
      * XOR operation.  Not very secure for everyday operations but for 
      * a one-time deal it should be safe enough. Especially as it would
      * take a very serious hacker to figure out what I am using to encode
      * the pw string with.
      */
      $val=rand();
      $arr=explode(',',$_POST['ENCODE']);
      /*
      echo "$arr<br>";
      print_r($arr);
      echo "<br>";
      echo $_POST['PW1'] . "<br>";
      */
      for($i=0;$i<count($arr);$i++) {
	$arr[$i] = chr(ord($_POST[$_POST['PW1']][$i]) ^ $arr[$i]);
	}
      /*
      echo implode($arr);
      echo "<br>";
      */
      //$pwd=@system('java mysha1 ' . implode($arr) . "> /tmp/$val.safile" );
      //$string=file_get_contents("/tmp/$val.safile");
      //unlink("/tmp/$val.safile");
      $query_str="SELECT username,fullname,email,institution,country";
      $query_str.=" FROM accounts WHERE password=";
      $query_str.= "sha1('" . implode($arr) . "')";

      $rtn=mysql_query($query_str);
      if($rtn) {
	if(mysql_num_rows($rtn) > 0) {
	  echo '<table width="100%">';
	  echo '<tr><th>USERNAME</th><th>FULL NAME</th>';
	  echo '<th>EMAIL</th><th>INSTITUTION</th><th>COUNTRY</th></tr>';
	  $row=mysql_fetch_row($rtn);
	  while($row) {
	    echo "<tr><td>{$row[0]}</td>\n";
	    echo "<td>{$row[1]}</td>\n";
	    echo "<td>{$row[2]}</td>\n";
	    echo "<td>{$row[3]}</td>\n";
	    echo "<td>{$row[4]}</td></tr>\n";
	    $row=mysql_fetch_row($rtn);
	    }
	  echo "</table>";
	  }
	else {
	  echo "Password did not match<br>";
	  }
	}
      else {
	$err="Could not access database: ";
	$err.="MYSQL ERROR: " . mysql_error();
	trigger_error($err,E_USER_ERROR);
	}
      mysql_close($mylink);
      }
    else {
      $err="Missing one of ENCODE,PW1, {$_POST['PW1']}<br>";
      trigger_error($err,E_USER_ERROR);
      }
    return;
  }
  
/*****************************************************************************/
/*! 
* This function's purpose is to reset a password for a user that is already 
* registered. It is one of those things that has to be done periodically 
* for any system with passwords.
*
* \param[in] $database_info  A DB_info object passed as a reference
* \param[in] $t_array An array of text chosen at the top level for formatted the
* \output Notice of whether or not the reset was successful.
*****************************************************************************/
function handle_pw_reset(& $database_info, $t_array)
  {
    if(array_key_exists('PW1',$_POST) &&
    array_key_exists('USER',$_POST) &&
    array_key_exists('ENCODE',$_POST) &&
    array_key_exists('DUMMY',$_POST)) {
      // the decode function must have already been called on the object
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
      /* 
      * here is decode the password using a simple
      * XOR operation.  Not very secure for everyday operations but for 
      * a one-time deal it should be safe enough. Especially as it would
      * take a very serious hacker to figure out what I am using to encode
      * the pw string with and when it is coming.
      */
      $val=rand();
      $arr=explode(',',$_POST['ENCODE']);
      /*
      echo "$arr<br>";
      print_r($arr);
      echo "<br>";
      echo $_POST['PW1'] . "<br>";
      */
      for($i=0;$i<count($arr);$i++) {
	$arr[$i] = chr(ord($_POST[$_POST['PW1']][$i]) ^ $arr[$i]);
	}
      /*
      echo implode($arr);
      echo "<br>";
      return;
      */
      //$pwd=@system('java mysha1 ' . implode($arr) . "> /tmp/$val.safile" );
      //$string=file_get_contents("/tmp/$val.safile");
      //unlink("/tmp/$val.safile");
      $query_str="UPDATE accounts SET password=sha1('$string') WHERE";
      $query_str.=" username=";
      $query_str.= "'" . $_POST['USER'] . "'";

      $rtn=mysql_query($query_str);
      echo '<table width="100%" border="1">';
      echo '<tr><th align="center">RESULTS</th></tr>' . "\n";
      if($rtn) {
	if(mysql_affected_rows() > 0) {
	  echo '<tr><td align="center">' . "\n";
	  echo "Password reset for user <b>";
	  echo $_POST['USER'] . "</b><br>\n";
	  echo "Please use the validation form to verify this.</td></tr>\n";
	  }
	else {
	  echo "<tr><td align=\"center\">\n";
	  echo "Password <b>NOT</b> reset!!<br>\n";
	  echo "No errors, either the passwords were the same OR <br>\n";
	  echo "you should suspect an incorrect username.<br>\n";
	  echo "</td></tr>\n";
	  }
	}
      else {
	$err="Could not update database: <br>\n";
	$err.="MYSQL ERROR: " . mysql_error() ."<br>\n";
	trigger_error($err,E_USER_ERROR);
	}
      echo "</table>\n";
      mysql_close($mylink);
      }
    else {
      $err="Missing one of USER,ENCODE,PW<br>\n";
      trigger_error($err,E_USER_ERROR);
      }
    return;
  }
?>
