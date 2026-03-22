<?php
/*****************************************************************************/
/*! 
* This pages's purpose is to add an entry into the <b>accounts</b> tables 
* of the associated database.  It expects that the information is correct.
*
* \return (mixed) A boolean <i>false</i> if there is an error. An array
* contain the email address, username, plaintext password and full name if
* everything went as planned. An integer if the username is already in use
* in the accounts table of the database.
* \sideeffect Adds a new entry into the accounts table of the selected
* database.
*****************************************************************************/
  // note that admin_functions includes information.inc
  include_once("admin_functions.inc");
  //$array= explode('/',$_SERVER['PHP_SELF']); 
  //$ar=$array[count($array)-2];
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
  echo "<html>";
  echo "<head><title>MassModel Registration Email</title>";
  echo '<link rel="stylesheet" type="text/css" href="m_admin.css">';
  echo '<META http-equiv="Content-Script-Type" content="text/javascript">';
  echo '<script src="m_admin.js" type="text/javascript">';
  echo '</script>';
  echo "</head>";
  echo '<body onresize="do_img_resize()" onload="do_img_resize()">'; 
  echo '<table  id="toptable" border="1" align="center">';
  echo '<tr> ';
  echo '<td><img id="i1" src="./doc/dbinfo/nuchart.png"  height="200px"> </td>';
  echo '</tr>';
  echo '</table>';
  echo '<H2 >MassModel Site Administration: New User Registration</H2>';
  echo '<hr><hr>';


  if(array_key_exists('USERNAME',$_POST) &&
     array_key_exists('NAME',$_POST) &&
     array_key_exists('EMAIL',$_POST) &&
     array_key_exists('INSTITUTION',$_POST) &&
     array_key_exists('ENCODE',$_POST) &&
     array_key_exists('DUMMY',$_POST) &&
     array_key_exists('PW1',$_POST) &&
     array_key_exists('ADDRESS',$_POST) ) {
  
    $database_info=& get_db_info("mass_model");
    if(! $database_info) {
      $err="Could not get database information for 'mass'";
      trigger_error($err,E_USER_ERROR);
      return;
      }
    // the decode function must have already been called on the object
    decode($database_info->password,$database_info->key);
    $mylink=mysql_connect('localhost:3306',$database_info ->username ,
	  $database_info->password);
    if(! $mylink) {
      $err="Could not CONNECT to MySQL server as ";
      $err.= $database_info->username;
      trigger_error($err,E_USER_ERROR);
      return ;
      }
    $rtn=mysql_query("USE $database_info->dbname");
    if(! $rtn) {
      $err= "USE " . $database_info->dbname ;
      $err.=" query caused: MYSQL ERROR: " . mysql_error() ;
      trigger_error($err,E_USER_ERROR);
      mysql_close($mylink);
      return;
      }
    /*
    * First we check to see that the username is unique. This means make a
    * call to mysql and see if the a query returns a row. If not proceed.
    * Otherwise we need to return a nice error to the user.
    */
    $query_str="SELECT fullname FROM accounts WHERE username=";
    $query_str.="'{$_POST['USERNAME']}'";
    $rtn=mysql_query($query_str);
    if($rtn) {
      if(mysql_num_rows($rtn) > 0) {
	$row=mysql_fetch_row($rtn);
	echo '<div class="nav">';
	echo "Sorry, the username {$_POST['USERNAME']}";
	echo " is already in use by <b>" . $row[0] . "</b><br>";
	echo "Please return to the registration form and enter";
	echo " a new username. If you use the browsers back button";
	echo " you may not have to reenter anything but the ";
	echo "username and passwords.<br>Thanks<br>";
	echo '</div>';
	echo "</body></html>";
	mysql_close($mylink);
	return;
	}
      }
    else {
      $err= "Query for username failed:";
      if(mysql_errno()) {
	$err.=" MYSQL ERROR: " . mysql_error() ;
	}
      trigger_error($err,E_USER_ERROR);
      mysql_close($mylink);
      return;
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
    $query_str.="VALUES ('{$_POST['USERNAME']}',";
    $query_str.="'" . preg_replace("/'/","\\'",$_POST['NAME']) . "',";
    if(strlen(trim($_POST['NAME'])) > 64) {
      $_POST['NAME']=substr(trim($_POST['NAME']),0,64);
      }
    $query_str.="sha1('" . $plain_text . "'),";
    $query_str.="'{$_POST['EMAIL']}',";
    if(strlen(trim($_POST['COUNTRY'])) > 2) {
      $_POST['COUNTRY']=substr(trim($_POST['COUNTRY']),0,2);
      }
    $query_str.="'" . preg_replace("/'/","\\'",$_POST['COUNTRY']) . "',";
    if(strlen(trim($_POST['INSTITUTION'])) > 128) {
      $_POST['INSTITUTION']=substr(trim($_POST['INSTITUTION']),0,128);
      }
    $query_str.="'" . preg_replace("/'/","\\'",$_POST['INSTITUTION']) . "',";
    if(strlen(trim($_POST['ADDRESS'])) > 512) {
      $_POST['ADDRESS']=substr(trim($_POST['ADDRESS']),0,512);
      }
    $query_str.="'" . preg_replace("/'/","\\'",$_POST['ADDRESS']) . "',";
    if(strlen(trim($_POST['RESEARCH'])) > 256) {
      $_POST['RESEARCH']=substr(trim($_POST['RESEARCH']),0,256);
      }
    $query_str.="'" . preg_replace("/'/","\\'",$_POST['RESEARCH']) . "',";
    if(strlen(trim($_POST['INFORMATION'])) > 256) {
      $_POST['INFORMATION']=substr(trim($_POST['INFORMATION']),0,256);
      }
    $query_str.="'" . preg_replace("/'/","\\'",$_POST['INFORMATION']) . "',";
    if(strlen(trim($_POST['HEAR'])) > 256) {
      $_POST['HEAR']=substr(trim($_POST['HEAR']),0,256);
      }
    $query_str.="'" . preg_replace("/'/","\\'",$_POST['HEAR']) . "',";
    // this is a mysql function
    $query_str.="curdate(),curdate())";
    $rtn=mysql_query($query_str);	
    if(!$rtn) {
      $err="<br>Could not insert new user info into database: ";
      $err.="<br>MYSQL ERROR: " . mysql_error();
      trigger_error($err,E_USER_ERROR);
      return;
      }
    $new_index=mysql_insert_id();
    mysql_close($mylink);
    $body="Dear {$_POST['NAME']},\n\n";
    $body.=file_get_contents($database_info->data_path . "/admin/body.txt");
    $body.="\nusername: {$_POST['USERNAME']}\n";
    $body.="password: $plain_text\n\n";
    $body.=file_get_contents($database_info->data_path . "/admin/footer.txt");
    //return array($_POST['DATABASE'],$_POST['NAME'],$_POST['USERNAME'],
    //  $_POST['EMAIL'],$body);
    $body;
    echo '<div class="narrow">';
    echo '<p class="header">';
    echo "<br>The registration was successful. Use the ";
    echo "button below to send an email to the new user. The<br>";
    echo " email can be edited in-place if desired.";
    echo "</p>";
    echo '<form name="emailer" action="ready_to_send.html" ';
    echo 'method="POST" enctype="multipart/form-data>" ' . "\n";
    echo '<input type="hidden" name="ACTION" value="DO_EMAIL"';
    echo '<input type="hidden" name="DATABASE" value="';
    echo 'mass_model"' . ">\n";
    echo '<input type="hidden" name="NAME" value="' . $_POST['NAME'] . '">';
    echo '<input type="hidden" name="USER" value="' . $_POST['USERNAME'] . '">';
    echo '<input type="hidden" name="EMAIL" value="' . $_POST['EMAIL'] . '">';
    if(array_key_exists("REG_INDEX",$_POST)) {
      echo '<input type="hidden" name="REG_INDEX" value="';
      echo  $_POST['REG_INDEX'] . '">';
      }
    echo '<textarea  name="BODY" rows="20" cols="80">';
    echo $body;
    echo '</textarea>' . "<br>\n";
    echo '<input type="submit" value="SEND EMAIL">';
    echo '</form>';
    echo '<form name="cancel" method="POST" action="ready_to_send.html">';
    echo '<input type="hidden" name="NEW_INDEX" value="' . $new_index . '">';
    echo '<input type="submit" value="CANCEL EMAIL">' . "<br>";
    echo '</form>';
    echo '</div>';
    echo "</body></html>";
    }
  else {
    $err="Missing one of USERNAME,NAME,EMAIL,INSTITUTION,ADDRESS,";
    $err.= "PW1,DUMMY,ENCODE<br>\n";
    trigger_error($err,E_USER_ERROR);
    }
  return;

?>
