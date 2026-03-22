<?php
/**********************************************/
/*! \file
*
* \verbatim
* $Author: bucknerk $
* $Id: information.inc,v 1.7 2008/06/30 16:58:35 bucknerk Exp $
*
* $Log: information.inc,v $
* Revision 1.7  2008/06/30 16:58:35  bucknerk
* fixed testing
*
* Revision 1.6  2008/05/28 17:47:59  bucknerk
* tweeks for testing
*
* Revision 1.5  2008/05/23 17:01:29  bucknerk
* fixed dev
*
* Revision 1.4  2008/04/30 16:25:01  bucknerk
* fixed
*
* Revision 1.3  2008/04/30 13:48:36  bucknerk
* Changed things so that this can now be used as a testing and online.  The
* database chosen is based on the directory "mass_model" or "mass_testing".
*
* Revision 1.2  2008/04/16 13:47:37  bucknerk
* added more functionality
*
* Revision 1.1  2008/03/28 17:56:32  bucknerk
* new
*
*
* Revision 1.8  2008/03/05 13:11:11  bucknerk
* fixed dbname and comments and error messages
*
* Revision 1.7  2007/11/09 16:19:50  bucknerk
* added portal and fixed typo
*
* Revision 1.6  2007/09/06 12:00:55  bucknerk
* Added bbn dbs to get_db_info
*
* Revision 1.5  2007/08/23 18:10:42  bucknerk
* Updated error handling and documentation
*
* Revision 1.4  2007/08/17 15:05:31  bucknerk
* Added a temp_path variable to the DB_info class.  This is for storage of
* temporary files.
*
* Revision 1.3  2007/08/15 19:46:14  bucknerk
* Added a new array for the possible error codes that could come with using
* http POST uploading.
*
* Revision 1.2  2007/08/13 19:35:48  bucknerk
* Updated some of the functions, modified the DB_info class to include the
* path to the evaluation folder directory, added the create_folder handler.
*
* Revision 1.1.1.1  2007/08/10 17:12:56  bucknerk
* CINA Evalution interface
*
* \endverbatim
*
* NOTE: Taken from cina_eval for mass_model.
*
* This file serves as a single point in which to store
* information on the databases and operations used
* in the rate evaluation folder management operations
* ***********************************************************/
/************************************************************/

/*!
* A simple object that can hold the basics for a database
* access during any particular client request.
* Everything in a PHP object is basically public so 
* no need for accessor functions right now.
***********************************************************/
class DB_info 
{
  var $dbname; ///< main database
  var $data_path; ///< filesystem path to reaction folders
  var $temp_path; ///< filesystem path to reaction folders
  var $password; ///< encoded password
  var $key; ///< key to decode the password
  var $username; ///< username associated with password

  //! constructor with default values so that no arguments
  //! HAVE to be given.  Not that it does much use otherwise.
  //! Pretty obvious what the parameters are for.
  function DB_info($in_name='',$in_pw='',$in_key='',$in_user='',
	$in_path='',$in_tpath='')
  {
    $this->dbname=$in_name;
    $this->password=$in_pw;
    $this->key=$in_key;
    $this->username=$in_user;
    $this->data_path=$in_path;
    $this->temp_path=$in_tpath;
  }

}

/************************************************************/
/*!
* The idea here is that I can have two copies of
* one script and it will get what it needs based on 
* the script name.  That is if it is one name it
* will be the development version otherwise it is the
* production version.  
*
* \param[in] $suite_name A string used to select the correct information
* \return The database info needed as a reference to a new DB_info 
* object or <i>false</i> if the input string does not match the current
* choices.
*
* That data is the username and password the key to demangle the pw,
* the associated suite and reaction databases, the evaluation folder path
* and the path to the temporary storage directory.
************************************************************/
function &get_db_info($suite_name )
{
  //echo "SUITE='$suite_name'<br>";
  switch($suite_name) {
    case 'mass_model':
      $tmp=new DB_info('mass_model','15/71/20/13/87/9/2/55',
	  'nuclear_masses','mass_model','/var/www/html/mass_model_data',
	  '/tmp');
      break;
    case 'mass_model_dev':
      $tmp=new DB_info('mass_model_dev','23/6/29/92/95/13/2/21',
	  'developmental','devmasses','/var/www/html/mass_model_dev_data',
	  '/tmp');
      break;
    default: $tmp=false;
  }
  return $tmp;

}

/************************************************************/
/*!
* This array is for pretty printing the output
* so I can test using a web browser.
****************************************************************/
$test_comp = array(
  'header' => "<html><head>\n<title>TEST OUTPUT</title></head>\n<body><hr><hr> %s <br><a href=\"./model_test.html\">Return</a><hr><hr>\n",
  'footer' => '</body></html>',
  'eol' => ' \N' . "<br>\n",
  'tab' => ' \T<br>');

/************************************************************/
/*!
* This array is for the output components 
* to be used during actual operation.  These
* two only obfuscate things slightly.
****************************************************************/
$run_comp = array(
'header' => '%s',
  'footer' => "\n",
  'eol' => "\n",
  'tab' => "\t");




?>
