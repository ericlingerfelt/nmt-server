<?php
/*********************************************************/
/*! \file
 *
 *
 * \verbatim
 * $Author: elingerf $
 * $Id: mass_handlers.inc,v 1.34 2008/10/09 19:33:48 elingerf Exp $
 *
 * $Log: mass_handlers.inc,v $
 * Revision 1.34  2008/10/09 19:33:48  elingerf
 * no message
 *
 * Revision 1.32  2008/06/30 19:09:09  bucknerk
 * fixed spacing/format error in model creation.
 *
 * Revision 1.31  2008/06/30 18:56:51  bucknerk
 * testing
 *
 * Revision 1.30  2008/06/30 18:39:30  bucknerk
 * testing
 *
 * Revision 1.29  2008/06/30 17:10:33  bucknerk
 * changed newline to space in return to get_model_data
 *
 * Revision 1.28  2008/06/30 17:04:47  bucknerk
 * changed the wat uncertainties are handled (very simple) and fixed error with
 * file() constants enclosed in quotes.
 *
 * Revision 1.27  2008/06/04 17:53:48  bucknerk
 * updated
 *
 * Revision 1.26  2008/05/30 17:17:53  bucknerk
 * added lines to allow blank lines in input and uncertainty values
 *
 * Revision 1.25  2008/05/30 12:37:31  bucknerk
 * saved
 *
 * Revision 1.24  2008/05/30 11:44:35  bucknerk
 * tweek
 *
 * Revision 1.23  2008/05/29 15:25:57  bucknerk
 * Changer filename check to allow . in file name but only if followed by a character
 *
 * Revision 1.22  2008/05/28 14:30:30  bucknerk
 * Fixes, updates and repairs
 *
 * Revision 1.21  2008/05/23 17:04:41  bucknerk
 * fixed merge error
 *
 * Revision 1.20  2008/05/23 17:02:21  bucknerk
 * Added a mkdir to merge
 *
 * Revision 1.19  2008/04/29 12:26:47  bucknerk
 * bug fix
 *
 * Revision 1.18  2008/04/23 20:08:51  bucknerk
 * fixed overwrite i think
 *
 * Revision 1.17  2008/04/23 16:24:38  bucknerk
 * fixed bug
 *
 * Revision 1.16  2008/04/22 15:03:06  bucknerk
 * commented out testing output
 *
 * Revision 1.15  2008/04/22 14:53:50  bucknerk
 * fixed typo
 *
 * Revision 1.14  2008/04/22 14:22:18  bucknerk
 * fixed stuff
 *
 * Revision 1.13  2008/04/18 15:16:21  bucknerk
 * bugs
 *
 * Revision 1.12  2008/04/18 15:12:59  bucknerk
 * bugs
 *
 * Revision 1.11  2008/04/18 15:09:36  bucknerk
 * bugs
 *
 * Revision 1.10  2008/04/18 14:35:59  bucknerk
 * bugs
 *
 * Revision 1.9  2008/04/18 14:20:42  bucknerk
 * bug fix
 *
 * Revision 1.8  2008/04/18 14:10:48  bucknerk
 * bug fix
 *
 * Revision 1.7  2008/04/18 14:05:25  bucknerk
 * bug fix
 *
 * Revision 1.6  2008/04/18 13:55:56  bucknerk
 * bug fixes
 *
 * Revision 1.5  2008/04/18 13:39:10  bucknerk
 * added functionality
 *
 * Revision 1.4  2008/04/16 13:47:37  bucknerk
 * added more functionality
 *
 * Revision 1.3  2008/04/08 14:39:43  bucknerk
 * new
 *
 * Revision 1.2  2008/04/01 19:59:17  bucknerk
 * Added get_session_id (login) and logout. Updated (changed from add) create_model.
 *
 * Revision 1.1  2008/03/28 17:56:32  bucknerk
 * new
 *
 * \endverbatim
 *
 * This file contains the ACTION handling functions associated with
 * mass models.
 ************************************************************/

/************************************************************/
/*!  This function handles the operation which adds a new model.
 * We are using the DB_info "dbname" for the associated mass_model database
 * and in it there are currently on the \tbl{accounts} and \tbl{models}
 * tables.
 *
 * \param[in] $database_info  A DB_info object passed as a reference
 * \param[in] $t_array An array of text chosen at the top level for formatted the
 *
 * \input Key-value pairs of ID, NAME, REFERENCE, DESCRIPTION, DATA. AUTHOR,
 * FIELD.  These are all strings and the DATA is the content of the clients new
 * mass model input file.
 *
 * \output Key-value pair of MODEL_INDEX=n where <i>n</i> is the unique identifier
 * for the new model.
 *
 **************************************************************/

function handle_create_model(& $database_info, $t_array)
{
	if(array_key_exists('MODEL_NAME',$_POST)
	&& array_key_exists('REFERENCE',$_POST)
	&& array_key_exists('DESCRIPTION',$_POST)
	&& array_key_exists('DATA',$_POST)
	&& array_key_exists('AUTHOR',$_POST)
	&& array_key_exists('FIELD',$_POST)
	&& array_key_exists('OVERWRITE',$_POST))
	{
		decode($database_info->password, $database_info->key);
		$user=validate_session_id($database_info,$t_array);
		if(! $user) {
			$err="Could not validate session ID:{$t_array['eol']}";
			trigger_error($err,E_USER_ERROR);
			return;
		}
		$mylink=mysql_connect('localhost:3306',$database_info ->username ,
		$database_info->password);
		if(! $mylink) {
			$err= "Could not CONNECT to MySQL server as ";
			$err.= $database_info->username;
			trigger_error($err,E_USER_ERROR);
			return;
		}
		$rtn=mysql_query("USE $database_info->dbname");
		if(! $rtn) {
			$err= "USE $database_info->dbname query caused: ";
			$err.= "MYSQL ERROR: " . mysql_error() ;
			mysql_close($mylink);
			trigger_error($err,E_USER_ERROR);
			return;
		}
		update_session();

		$new_name=trim($_POST['MODEL_NAME']);
		if(strlen($new_name) == 0) {
			$err="Empty model name";
			mysql_close($mylink);
			trigger_error($err,E_USER_ERROR);
			return;
		}
		if(strlen($new_name) > 64) {
			$err="Model name to long";
			mysql_close($mylink);
			trigger_error($err,E_USER_ERROR);
			return;
		}
		if(! preg_match('/^[[:alpha:]][[:alnum:]_]*(\.[[:alnum:]]+)*$/',$new_name)) {
			$err="One or more invalid characters in model name";
			mysql_close($mylink);
			trigger_error($err,E_USER_ERROR);
			return;
		}

		if(! is_null($_POST['DESCRIPTION'])) {
			$description=preg_replace("/'/","\\'",trim($_POST['DESCRIPTION']));
		}
		else {
			$description=NULL;
		}
		if(! is_null($_POST['REFERENCE'])) {
			$reference=preg_replace("/'/","\\'",trim($_POST['REFERENCE']));
		}
		else {
			$reference=NULL;
		}
		if(! is_null($_POST['AUTHOR'])) {
			$author=preg_replace("/'/","\\'",trim($_POST['AUTHOR']));
		}
		else {
			$author=$user['FULLNAME'];
		}
		if($_POST['FIELD'] != "THEORY" && $_POST['FIELD'] != "EXPERIMENT") {
			$field="THEORY"; // no error just 'fix' it
		}
		else {
			$field=$_POST['FIELD'];
		}
		
		//First check to see if model name exists in this user's directory and if OVERWRITE is set to Y/N
		$query_str="SELECT m_index FROM models WHERE user='{$user['USERNAME']}'";
		$query_str.=" and name='$new_name'";
		$rtn=mysql_query($query_str);
		
		if(!$rtn ) {
			$err="Could not select index from database for existing file: ";
			$err.=mysql_error();
			mysql_close($mylink);
			trigger_error($err,E_USER_ERROR);
			return;
		}
		
		if( mysql_num_rows($rtn) != 0 && $_POST['OVERWRITE'] != 'Y') {
			$err="The model ".$new_name." already exists.";
			trigger_error($err,E_USER_ERROR);
			return;
		}
		
		if(mysql_num_rows($rtn) != 0){
			$row = mysql_fetch_assoc($rtn);
			$m_index = $row["m_index"];
			$update_db = true;
		}else{
			$update_db = false;
		}
		
		// note that TYPE is not being used because the only TYPE that can be
		// created is USER. They have to be MOVED to SHARED. Admin has to create
		// PUBLIC ones.
		if($update_db===false) {
			$query_str="INSERT INTO models (name,description,reference,author,field,";
			$query_str.="created,user)";
			$query_str.=" VALUES ('$new_name',";
			if(strlen($description) > 0) { // the replacement might have stripped all
				$query_str.="'$description',";
			}
			else {
				$query_str.="NULL,";
			}
			if(strlen($reference) > 0) { // the replacement might have stripped all
				$query_str.="'$reference',";
			}
			else {
				$query_str.="NULL,";
			}
			if(strlen($author) > 0) { // the replacement might have stripped all
				$query_str.="'$author',";
			}
			else {
				$query_str.="'{$user['FULLNAME']}',";
			}
			$query_str.="'$field',now(),'{$user['USERNAME']}')";
		}
		else {
			$query_str="UPDATE models SET description=";
			if(strlen($description) > 0) { // the replacement might have stripped all
				$query_str.="'$description'";
			}
			else {
				$query_str.="NULL";
			}
			$query_str.= ",reference=";
			if(strlen($reference) > 0) { // the replacement might have stripped all
				$query_str.="'$reference'";
			}
			else {
				$query_str.="NULL";
			}
			$query_str.=",modified=now()";
			$query_str.=",author=";
			if(strlen($author) > 0) { // the replacement might have stripped all
				$query_str.="'$author'";
			}
			else {
				$query_str.="'{$user['FULLNAME']}'";
			}
			$query_str .= " WHERE m_index=".$m_index;
		}
		$rtn=mysql_query($query_str);
		if(! $rtn) {
			$err="Could not insert/update new model in database: ";
			if(mysql_errno() > 0 ) {
				$err.=mysql_error();
				$err.="<br>$query_str<br>";
			}
			mysql_close($mylink);
			trigger_error($err,E_USER_ERROR);
			return;
		}
		if($update_db===false) {
			$m_index=mysql_insert_id();
		}else{
			//Delete all current rows in model_data for old model if one existed
			$query_str = "DELETE FROM model_data WHERE m_index=".$m_index;
			$rtn=mysql_query($query_str);
			if(! $rtn) {
				$err="Could not overwrite old model's data in database: ";
				if(mysql_errno() > 0 ) {
					$err.=mysql_error();
					$err.="<br>$query_str<br>";
				}
				mysql_close($mylink);
				trigger_error($err,E_USER_ERROR);
				return;
			}
		}
		
		//Now insert the new data 
		$data = $_POST["DATA"];
		$data_row_array = explode(" ", trim($data));
		foreach($data_row_array as $row){
			$data_array = explode(",", $row);
			$query_str = "INSERT INTO model_data (m_index,z,n,mass_ex,uncer) VALUES (";
			$query_str .= $m_index.",";
			$query_str .= $data_array[0].",";
			$query_str .= $data_array[1].",";
			$query_str .= $data_array[2].",";
			$query_str .= $data_array[3];
			$query_str .= ")";
			$rtn=mysql_query($query_str);
			if(! $rtn) {
				$err="Could not write model data to database: ";
				if(mysql_errno() > 0 ) {
					$err.=mysql_error();
					$err.="<br>$query_str<br>";
				}
				mysql_close($mylink);
				trigger_error($err,E_USER_ERROR);
				return;
			}
		}
		
		printf($t_array['header'],"MODEL_INDEX=$update_db\n");
		mysql_close($mylink);
		return;
	}else {
		$err= "Missing one of MODEL_NAME, DESCRIPTION, REFERENCE, DATA";
		$err.= " AUTHOR, OVERWRITE, or FIELD in input data stream";
		trigger_error($err,E_USER_ERROR);
	}
	print($t_array['footer']);
	return;
}
/************************************************************/
/*!  This function handles the operation which returns a list of all the
 * models available for a particular type, type being one of USER, PUBLIC,
 * SHARED, or ALL.
 * We are using the DB_info "dbname" for the associated mass_model database.
 *
 * \param[in] $database_info  A DB_info object passed as a reference
 * \param[in] $t_array An array of text chosen at the top level for formatted the
 *
 * \input Key-value pairs of ID, TYPE . These are all strings. If the type is ALL
 * or USER then the user associated with the session ID is selected.
 *
 * \output Key-value pair of MODEL_INDEX=n where <i>n</i> is the
 * unique identifier of the model, MODEL_NAME=string, TYPE=string and is one of
 * <i>USER</i>
 *
 **************************************************************/
function handle_get_models(& $database_info, $t_array)
{
	if(array_key_exists('TYPE',$_POST))
	{
		decode($database_info->password, $database_info->key);
		$user=validate_session_id($database_info,$t_array);
		if(! $user) {
			$err="Could not validate session ID:{$t_array['eol']}";
			trigger_error($err,E_USER_ERROR);
			return;
		}
		$mylink=mysql_connect('localhost:3306',$database_info ->username ,
		$database_info->password);
		if(! $mylink) {
			$err= "Could not CONNECT to MySQL server as ";
			$err.= $database_info->username;
			trigger_error($err,E_USER_ERROR);
			return;
		}
		$rtn=mysql_query("USE $database_info->dbname");
		if(! $rtn) {
			$err= "USE $database_info->dbname query caused: ";
			$err.= "MYSQL ERROR: " . mysql_error() ;
			mysql_close($mylink);
			trigger_error($err,E_USER_ERROR);
			return;
		}

		update_session();

		$query_str="SELECT m_index,name,type FROM models";
		switch($_POST['TYPE']) {
			case 'USER':
				$query_str.=" WHERE type='USER' AND user='{$user['USERNAME']}'";
				break;
			case 'SHARED':
				$query_str.=" WHERE type='SHARED'";
				break;
			case 'PUBLIC':
				$query_str.=" WHERE type='PUBLIC'";
				break;
			case 'ALL':
				// handle all and default the same
			default:
				$query_str.=" WHERE (type='USER' and user='{$user['USERNAME']}')";
				$query_str.=" OR type='SHARED' OR type='PUBLIC' ORDER BY name ASC";
		}
		$rtn=mysql_query($query_str);
		if(!$rtn) {
			$err= "Select query caused: ";
			$err.= "MYSQL ERROR: " . mysql_error() . "<br>";
			$err.= "$query_str<br>";
			mysql_close($mylink);
			trigger_error($err,E_USER_ERROR);
			return;
		}
		printf($t_array['header'],"");
		$row=mysql_fetch_assoc($rtn);
		while($row) {
			echo "MODEL_INDEX={$row['m_index']}{$t_array['eol']}";
			echo "MODEL_NAME={$row['name']}{$t_array['eol']}";
			echo "TYPE=" . $row['type'] . "{$t_array['eol']}";
			$row=mysql_fetch_assoc($rtn);
		}
		mysql_close($mylink);
	}
	else {
		$err= "Missing TYPE in input data stream";
		trigger_error($err,E_USER_ERROR);
	}
	print($t_array['footer']);
	return;
}
/************************************************************/
/*!  This function handles the operation which removes a user's
 * model. It can not be type PUBLIC or SHARED.
 * We are using the DB_info "dbname" for the associated mass_model database.
 *
 * \param[in] $database_info  A DB_info object passed as a reference
 * \param[in] $t_array An array of text chosen at the top level for formatted the
 *
 * \input Key-value pairs of ID, MODEL_INDEX .
 * ID is a string, MODEL_INDEX is an integer.
 *
 * \output Key-value pair of MODEL_INDEX=n where <i>n</i> is the
 * unique identifier of the model.
 *
 **************************************************************/
function handle_erase_model(& $database_info, $t_array)
{
	if(array_key_exists('MODEL_INDEX',$_POST))
	{
		decode($database_info->password, $database_info->key);
		$user=validate_session_id($database_info,$t_array);
		if(! $user) {
			$err="Could not validate session ID:{$t_array['eol']}";
			trigger_error($err,E_USER_ERROR);
			return;
		}
		$mylink=mysql_connect('localhost:3306',$database_info ->username ,
		$database_info->password);
		if(! $mylink) {
			$err= "Could not CONNECT to MySQL server as ";
			$err.= $database_info->username;
			trigger_error($err,E_USER_ERROR);
			return;
		}
		$rtn=mysql_query("USE $database_info->dbname");
		if(! $rtn) {
			$err= "USE $database_info->dbname query caused: ";
			$err.= "MYSQL ERROR: " . mysql_error() ;
			mysql_close($mylink);
			trigger_error($err,E_USER_ERROR);
			return;
		}

		update_session();
		$query_str="SELECT type,user,name FROM models WHERE ";
		$query_str.="m_index={$_POST['MODEL_INDEX']}";
		$rtn=mysql_query($query_str);
		if(!$rtn) {
			$err= "Select query caused: ";
			$err.= "MYSQL ERROR: " . mysql_error() . "<br>";
			$err.= "$query_str<br>";
			mysql_close($mylink);
			trigger_error($err,E_USER_ERROR);
			return;
		}
		if(mysql_num_rows($rtn) ==0) {
			$err= "Select query returned no rows for MODEL_INDEX ";
			$err.= "{$_POST['MODEL_INDEX']}<br>";
			mysql_close($mylink);
			trigger_error($err,E_USER_ERROR);
			return;
		}
		$row=mysql_fetch_row($rtn);
		if($row[0] != 'USER') {
			$err= "MODEL_INDEX ";
			$err.= "{$_POST['MODEL_INDEX']}: Wrong type, type = ";
			$err.=$row[0];
			mysql_close($mylink);
			trigger_error($err,E_USER_ERROR);
			return;
		}
		if($row[1] != $user['USERNAME']) {
			$err= "MODEL_INDEX ";
			$err.= "{$_POST['MODEL_INDEX']}: Not owned by ";
			$err.=$user['USERNAME'];
			mysql_close($mylink);
			trigger_error($err,E_USER_ERROR);
			return;
		}
		// now that it is validated we just delete it
		$query_str="DELETE FROM models WHERE m_index={$_POST['MODEL_INDEX']}";
		$rtn=mysql_query($query_str);
		if(!$rtn) {
			$err= "Select query caused: ";
			$err.= "MYSQL ERROR: " . mysql_error() . "<br>";
			$err.= "$query_str<br>";
			mysql_close($mylink);
			trigger_error($err,E_USER_ERROR);
			return;
		}
		$query_str="DELETE FROM model_data WHERE m_index={$_POST['MODEL_INDEX']}";
		$rtn=mysql_query($query_str);
		if(!$rtn) {
			$err= "Select query caused: ";
			$err.= "MYSQL ERROR: " . mysql_error() . "<br>";
			$err.= "$query_str<br>";
			mysql_close($mylink);
			trigger_error($err,E_USER_ERROR);
			return;
		}
		printf($t_array['header'],"MODEL_INDEX={$_POST['MODEL_INDEX']}\n");
	}
	else {
		$err= "Missing MODEL_INDEX in input data stream";
		trigger_error($err,E_USER_ERROR);
	}
	print($t_array['footer']);
	return;
}

/************************************************************/
/*!  This function handles the operation which returns all the
 * data available for a set of models.
 * We are using the DB_info "dbname" for the associated mass_model database.
 *
 * \param[in] $database_info  A DB_info object passed as a reference
 * \param[in] $t_array An array of text chosen at the top level for formatted the
 *
 * \input Key-value pairs of ID, MODEL_INDICES . MODEL_INDICES is the set of
 * model indices. If the model index is not for a PUBLIC or SHARED model it must
 * belong to the user associated with the ID
 *
 * \output Key-value pair of MODEL_INDEX=n where <i>n</i> is the
 * unique identifier of the model, DATA=Z,A,MeV Z,A,MeV... The entire set of data
 * is followed by a single newline then potentially MODEL_INDEX and DATA for the
 * rest fo the indices.
 *
 **************************************************************/
function handle_get_model_data(& $database_info, $t_array)
{
	if(array_key_exists('MODEL_INDICES',$_POST))
	{
		decode($database_info->password, $database_info->key);
		$user=validate_session_id($database_info,$t_array);
		if(! $user) {
			$err="Could not validate session ID:{$t_array['eol']}";
			trigger_error($err,E_USER_ERROR);
			return;
		}
		$mylink=mysql_connect('localhost:3306',$database_info ->username ,
		$database_info->password);
		if(! $mylink) {
			$err= "Could not CONNECT to MySQL server as ";
			$err.= $database_info->username;
			trigger_error($err,E_USER_ERROR);
			return;
		}
		$rtn=mysql_query("USE $database_info->dbname");
		if(! $rtn) {
			$err= "USE $database_info->dbname query caused: ";
			$err.= "MYSQL ERROR: " . mysql_error() ;
			mysql_close($mylink);
			trigger_error($err,E_USER_ERROR);
			return;
		}
		update_session();
		if(is_null($_POST['MODEL_INDICES'])) {
			$err= "No indices supplied";
			mysql_close($mylink);
			trigger_error($err,E_USER_ERROR);
			return;
		}
		printf($t_array['header'],"");
		$indices=explode(",",trim($_POST['MODEL_INDICES']));
		for($i=0;$i<count($indices);$i++) {
			$index=trim($indices[$i]);
			if(strlen($index) > 0) {
				$query_str="SELECT * FROM models WHERE m_index=$index";
				$rtn=mysql_query($query_str);
				if(!$rtn) {
					$err= "SELECT caused ";
					$err.= "MYSQL ERROR: " . mysql_error() . "<br>";
					$err.="$query_str<br>";
					mysql_close($mylink);
					trigger_error($err,E_USER_ERROR);
					return;
				}
				echo "MODEL_INDEX=$index\n";
				if(mysql_num_rows($rtn)==0) {
					echo "DATA=\n";
					continue;
				}
				else {
					echo "DATA=";
					$query_str = "SELECT * FROM model_data WHERE m_index=$index";
					$rtn=mysql_query($query_str);
					if(!$rtn) {
						$err= "SELECT caused ";
						$err.= "MYSQL ERROR: " . mysql_error() . "<br>";
						$err.="$query_str<br>";
						trigger_error($err,E_USER_ERROR);
						return;
					}
					while($row = mysql_fetch_assoc($rtn)){
						echo $row["z"].",";
						echo $row["n"].",";
						echo $row["mass_ex"].",";
						echo $row["uncer"]." ";
					}
					echo "\n";
				}
			}
		}
		mysql_close($mylink);
	}
	else {
		$err= "Missing MODEL_INDICES in input data stream";
		trigger_error($err,E_USER_ERROR);
	}
	print($t_array['footer']);
	return;
}
/************************************************************/
/*!  This function handles the operation which copies a user's model to
 * SHARED allowing others to access it.
 * We are using the DB_info "dbname" for the associated mass_model database
 * and in it there are currently on the \tbl{accounts} and \tbl{models}
 * tables.
 *
 * \param[in] $database_info  A DB_info object passed as a reference
 * \param[in] $t_array An array of text chosen at the top level for formatted the
 *
 * \input Key-value pairs of ID, MODEL_INDEX.
 *
 * \output Key-value pair of MODEL_INDEX=n where <i>n</i> is the unique identifier
 * for the newly copied model.
 *
 **************************************************************/

function handle_copy_model(& $database_info, $t_array)
{
	if(array_key_exists('MODEL_INDEX',$_POST))
	{
		decode($database_info->password, $database_info->key);
		$user=validate_session_id($database_info,$t_array);
		if(! $user) {
			$err="Could not validate session ID:{$t_array['eol']}";
			trigger_error($err,E_USER_ERROR);
			return;
		}
		$mylink=mysql_connect('localhost:3306',$database_info ->username ,
		$database_info->password);
		if(! $mylink) {
			$err= "Could not CONNECT to MySQL server as ";
			$err.= $database_info->username;
			trigger_error($err,E_USER_ERROR);
			return;
		}
		$rtn=mysql_query("USE $database_info->dbname");
		if(! $rtn) {
			$err= "USE $database_info->dbname query caused: ";
			$err.= "MYSQL ERROR: " . mysql_error() ;
			mysql_close($mylink);
			trigger_error($err,E_USER_ERROR);
			return;
		}
		update_session();
		$query_str="SELECT * FROM models WHERE m_index=";
		$query_str.= $_POST['MODEL_INDEX'];
		$rtn=mysql_query($query_str);
		if(!$rtn || mysql_num_rows($rtn) == 0) {
			$err= "SELECT ";
			if(mysql_errno() > 0) {
				$err.= "caused MYSQL ERROR: " . mysql_error() ;
			}
			else {
				$err.="returned no values";
			}
			mysql_close($mylink);
			trigger_error($err,E_USER_ERROR);
			return;
		}
		$row=mysql_fetch_assoc($rtn);
		if($row['user'] != $user['USERNAME']) {
			$err="{$user['USERNAME']}: Not owner of model";
			mysql_close($mylink);
			trigger_error($err,E_USER_ERROR);
			return;
		}
		if($row['type'] == "SHARED"){
			$err="Model already exists in SHARED";
			mysql_close($mylink);
			trigger_error($err,E_USER_ERROR);
			return;
		}
		
		//Create new model 
		$query_str="INSERT INTO models (name,user,description,reference,author";
		$query_str.=",type,field,created) VALUES (";
		$query_str.="'" . $row['name'] . "','" . $row['user'] . "','";
		$query_str.=$row['description'];
		$query_str.="','" . $row['reference'] . "','" . $row['author']. "',";
		$query_str.="'SHARED','" . $row['field'] . "',now())";
		$rtn=mysql_query($query_str);
		if(!$rtn) {
			$err="INSERT caused ";
			$err.="MYSQL Error: " . mysql_error();
			mysql_close($mylink);
			trigger_error($err,E_USER_ERROR);
			return;
		}
		$val=mysql_insert_id();
		
		//Create new model data
		$query_str = "SELECT * FROM model_data WHERE m_index=".$row["m_index"];
		$rtn=mysql_query($query_str);
		
		if(!$rtn) {
			$err="Could not retrieve model data";
			$err.="MYSQL Error: " . mysql_error();
			mysql_close($mylink);
			trigger_error($err,E_USER_ERROR);
			return;
		}
		
		while($row=mysql_fetch_assoc($rtn)){
			$query_str = "INSERT INTO model_data (m_index,z,n,mass_ex,uncer) VALUES (";
			$query_str .= $val.",";
			$query_str .= $row["z"].",";
			$query_str .= $row["n"].",";
			$query_str .= $row["mass_ex"].",";
			$query_str .= $row["uncer"].")";
			$rtn_inner=mysql_query($query_str);
			if(!$rtn_inner) {
				$err="Could not insert model data";
				$err.="MYSQL Error: " . mysql_error();
				mysql_close($mylink);
				trigger_error($err,E_USER_ERROR);
				return;
			}
		}
		
		printf($t_array['header'],"MODEL_INDEX=$val\n");

		mysql_close($mylink);
		return;
	}
	else {
		$err= "Missing one MODEL_INDEX";
		$err.= " from input data stream";
		trigger_error($err,E_USER_ERROR);
	}
	print($t_array['footer']);
	return;
}
/************************************************************/
/*!  This function handles the operation which returns all the
 * information NOT data available for a set of models.
 * We are using the DB_info "dbname" for the associated mass_model database.
 *
 * \param[in] $database_info  A DB_info object passed as a reference
 * \param[in] $t_array An array of text chosen at the top level for formatted the
 *
 * \input Key-value pairs of ID, MODEL_INDICES . MODEL_INDICES is the set of
 * model indices. If the model index is not for a PUBLIC or SHARED model it must
 * belong to the user associated with the ID
 *
 * \output Key-value pair of MODEL_INDEX=n where <i>n</i> is the
 * unique identifier of the model, DATA=Z,A,MeV Z,A,MeV... The entire set of data
 * is followed by a single newline then potentially MODEL_INDEX and DATA for the
 * rest fo the indices.
 *
 **************************************************************/
function handle_get_model_info(& $database_info, $t_array)
{
	if(array_key_exists('MODEL_INDICES',$_POST))
	{
		decode($database_info->password, $database_info->key);
		$user=validate_session_id($database_info,$t_array);
		if(! $user) {
			$err="Could not validate session ID:{$t_array['eol']}";
			trigger_error($err,E_USER_ERROR);
			return;
		}
		$mylink=mysql_connect('localhost:3306',$database_info ->username ,
		$database_info->password);
		if(! $mylink) {
			$err= "Could not CONNECT to MySQL server as ";
			$err.= $database_info->username;
			trigger_error($err,E_USER_ERROR);
			return;
		}
		$rtn=mysql_query("USE $database_info->dbname");
		if(! $rtn) {
			$err= "USE $database_info->dbname query caused: ";
			$err.= "MYSQL ERROR: " . mysql_error() ;
			mysql_close($mylink);
			trigger_error($err,E_USER_ERROR);
			return;
		}
		update_session();

		if(is_null($_POST['MODEL_INDICES'])) {
			$err= "No indices supplied";
			mysql_close($mylink);
			trigger_error($err,E_USER_ERROR);
			return;
		}

		printf($t_array['header'],"");
		$indices=explode(",",trim($_POST['MODEL_INDICES']));
		for($i=0;$i<count($indices);$i++) {
			$index=trim($indices[$i]);
			if(strlen($index) > 0) {
				$query_str="SELECT * FROM models  WHERE m_index=$index";
				//echo "<hr>$query_str<hr>";
				$rtn=mysql_query($query_str);
				if(!$rtn) {
					$err= "SELECT caused ";
					$err.= "MYSQL ERROR: " . mysql_error() . "<br>";
					$err.="$query_str<br>";
					mysql_close($mylink);
					trigger_error($err,E_USER_ERROR);
					return;
				}
				echo "MODEL_INDEX=$index\n";
				if(mysql_num_rows($rtn)==0) {
					echo "AUTHOR=\n";
					echo "REFERENCE=\n";
					echo "DESCRIPTION=\n";
					echo "FIELD=\n";
					echo "CREATION_DATE=\n";
					echo "MODIFICATION_DATE=\n";
					continue;
				}
				else {
					$row=mysql_fetch_assoc($rtn);
					$valid=true;
					switch($row['type']) {
						case 'USER':
							// validate that this guy is the user
							if($user['USERNAME']!=$row['user']) {
								$valid=false;
								break;
							}
						case 'SHARED':
						case 'PUBLIC':
							$valid=true;
							// catch any errors by unsetting sub_dir
					}
					if(! $valid) { // no data
						echo "AUTHOR=\n";
						echo "REFERENCE=\n";
						echo "DESCRIPTION=\n";
						echo "FIELD=\n";
						echo "CREATION_DATE=\n";
						echo "MODIFICATION_DATE=\n";
						continue;
					}
					else {
						echo "AUTHOR={$row['author']}\n";
						echo "REFERENCE=";
						echo preg_replace("/\n/","<br>",$row['reference']) . "\n";
						echo "DESCRIPTION=";
						echo preg_replace("/\n/","<br>",$row['description']) . "\n";
						echo "FIELD=" . $row['field'] . "\n";
						echo "CREATION_DATE={$row['created']}\n";
						echo "MODIFICATION_DATE={$row['modified']}\n";
					}
				}
			}
		}
		mysql_close($mylink);
	}
	else {
		$err= "Missing MODEL_INDICES in input data stream";
		trigger_error($err,E_USER_ERROR);
	}
	print($t_array['footer']);
	return;
}
/************************************************************/
/*!  This function handles the operation which merges a set of modles into
 * a single new model. The name of the new model is supplied by the user along
 * with a set (larger than size one) of indices.  The new model is created in the
 * user's space, the user becomes the author, the description is the
 * concatenation of the descriptions of the merged models, and the model itself
 * is (A u B\A) u C\(A u B\A) u D\(C\(A u B\A)) ...
 *
 * algorithimcally, given a set of models (A,B,C,D...) each containing elements
 * of the form (Z,Am,Q) where the tuple Z,Am uniquely identify an element, an
 * initially empty model MM, a temporary model T, and x an element of a model
 * then
 *
 * MM := A
 * for T in (B,C,D...)
 *   for x in T
 *     if x not in MM then
 *       add x to MM
 *     endif
 *   endfor
 * endfor
 * return MM
 *
 * We are using the DB_info "dbname" for the associated mass_model database.
 *
 * \param[in] $database_info  A DB_info object passed as a reference
 * \param[in] $t_array An array of text chosen at the top level for formatted the
 *
 * \input Key-value pairs of ID, MODEL_INDICES . MODEL_INDICES is the set of
 * model indices to be merged in priority order. If the model index is not for a
 * PUBLIC or SHARED model it must belong to the user associated with the ID
 *
 * \output Key-value pair of MODEL_INDEX=n where <i>n</i> is the
 * unique identifier of the new model
 *
 **************************************************************/
function handle_merge_models(& $database_info, $t_array)
{
	if(array_key_exists('MODEL_INDICES',$_POST)
	&& array_key_exists('MODEL_NAME',$_POST)
	&& array_key_exists('OVERWRITE',$_POST))
	{
		decode($database_info->password, $database_info->key);
		$user=validate_session_id($database_info,$t_array);
		if(! $user) {
			$err="Could not validate session ID:{$t_array['eol']}";
			trigger_error($err,E_USER_ERROR);
			return;
		}
		$mylink=mysql_connect('localhost:3306',$database_info ->username ,
		$database_info->password);
		if(! $mylink) {
			$err= "Could not CONNECT to MySQL server as ";
			$err.= $database_info->username;
			trigger_error($err,E_USER_ERROR);
			return;
		}
		$rtn=mysql_query("USE $database_info->dbname");
		if(! $rtn) {
			$err= "USE $database_info->dbname query caused: ";
			$err.= "MYSQL ERROR: " . mysql_error() ;
			mysql_close($mylink);
			trigger_error($err,E_USER_ERROR);
			return;
		}
		update_session();
		if(is_null($_POST['MODEL_INDICES'])) {
			$err= "No indices supplied";
			mysql_close($mylink);
			trigger_error($err,E_USER_ERROR);
			return;
		}
		$new_name=trim($_POST['MODEL_NAME']);
		if(strlen($new_name) == 0) {
			$err="Empty model name";
			mysql_close($mylink);
			trigger_error($err,E_USER_ERROR);
			return;
		}
		if(strlen($new_name) > 64) {
			$err="Model name to long";
			mysql_close($mylink);
			trigger_error($err,E_USER_ERROR);
			return;
		}
		if(! preg_match('/^[[:alpha:]][[:alnum:]_]*$/',$new_name)) {
			$err="One or more invalid characters in model name";
			mysql_close($mylink);
			trigger_error($err,E_USER_ERROR);
			return;
		}
		
		//First check to see if model name exists in this user's directory and if OVERWRITE is set to Y/N
		$query_str="SELECT m_index FROM models WHERE user='{$user['USERNAME']}'";
		$query_str.=" and name='$new_name'";
		$rtn=mysql_query($query_str);
		
		if(!$rtn ) {
			$err="Could not select index from database for existing file: ";
			$err.=mysql_error();
			mysql_close($mylink);
			trigger_error($err,E_USER_ERROR);
			return;
		}
		
		if( mysql_num_rows($rtn) != 0 && $_POST['OVERWRITE'] != 'Y') {
			$err="The model ".$new_name." already exists.";
			trigger_error($err,E_USER_ERROR);
			return;
		}
		
		if(mysql_num_rows($rtn) != 0){
			$row = mysql_fetch_assoc($rtn);
			$m_index = $row["m_index"];
			$update_db = true;
		}else{
			$update_db = false;
		}

		$new_model=array();
		$merged_ref="Merged Model references: ";
		$merged_desc="Merged Model";
		$indices=explode(",",trim($_POST['MODEL_INDICES']));
		for($i=0;$i<count($indices);$i++) {
			$index=trim($indices[$i]);
			if(strlen($index) > 0) {
				$query_str="SELECT * FROM models WHERE m_index=$index";
				$rtn=mysql_query($query_str);
				if(!$rtn) {
					$err= "SELECT caused ";
					$err.= "MYSQL ERROR: " . mysql_error() . "<br>";
					$err.="$query_str<br>";
					mysql_close($mylink);
					trigger_error($err,E_USER_ERROR);
					return;
				}
				if(mysql_num_rows($rtn)==0) {
					$err= "Supplied Index '$index' is not a valid model";
					mysql_close($mylink);
					trigger_error($err,E_USER_ERROR);
					return;
				}
				else {
					$row=mysql_fetch_assoc($rtn);
					$merged_desc.="; " . $row['description'];
					if($i > 0) {
						$merged_ref.="; $index: " . $row['reference'];
					}
					else {
						$merged_ref.=" $index:" . $row['reference'];
					}	
				}
			}
		}
		
		// now update the database.
		if($update_db !== false) {
			$query_str="UPDATE models SET description='$merged_desc'";
			$query_str.=",reference='$merged_ref',modified=now()";
			$query_str.=",author='{$user['USERNAME']}' WHERE m_index=$m_index";
			$rtn=mysql_query($query_str);
			if(!$rtn) {
				$err="Could not update database: ";
				$err.=mysql_error();
				mysql_close($mylink);
				trigger_error($err,E_USER_ERROR);
				return;
			}
			//Delete all current rows in model_data for old model if one existed
			$query_str = "DELETE FROM model_data WHERE m_index=".$m_index;
			$rtn=mysql_query($query_str);
			if(! $rtn) {
				$err="Could not overwrite old model's data in database: ";
				if(mysql_errno() > 0 ) {
					$err.=mysql_error();
					$err.="<br>$query_str<br>";
				}
				mysql_close($mylink);
				trigger_error($err,E_USER_ERROR);
				return;
			}
		}
		else {
			$query_str="INSERT INTO models (name,description,reference,author,field,";
			$query_str.="created,user)";
			$query_str.=" VALUES ('$new_name','$merged_desc','$merged_ref',";
			$query_str.="'{$user['USERNAME']}','theory',now(),'{$user['USERNAME']}')";
			$rtn=mysql_query($query_str);
			if(!$rtn) {
				$err="INSERT for new merged model resulted in:";
				$err.=" MYSQL Error:" . mysql_error();
				mysql_close($mylink);
				trigger_error($err,E_USER_ERROR);
				return;
			}
			$m_index=mysql_insert_id();
		}
		
		//Now we merge and insert the data
		for($i=0;$i<count($indices);$i++) {
			$index=trim($indices[$i]);
			if(strlen($index) > 0) {
				$query_str="SELECT * FROM model_data WHERE m_index=$index";
				$rtn=mysql_query($query_str);
				if(!$rtn) {
					$err= "SELECT caused ";
					$err.= "MYSQL ERROR: " . mysql_error() . "<br>";
					$err.="$query_str<br>";
					mysql_close($mylink);
					trigger_error($err,E_USER_ERROR);
					return;
				}
				if(mysql_num_rows($rtn)==0) {
					$err= "Supplied Index '$index' is not a valid model";
					mysql_close($mylink);
					trigger_error($err,E_USER_ERROR);
					return;
				}
				else {
					while($row=mysql_fetch_assoc($rtn)){
						//See if row already exists using new m_index
						$z = $row["z"];
						$n = $row["n"];
						$mass_ex = $row["mass_ex"];
						$uncer = $row["uncer"];
						$query_str_exists="SELECT * FROM model_data WHERE m_index=$m_index AND z=$z AND n=$n";
						$rtn_exists = mysql_query($query_str_exists);
						if(!$rtn_exists) {
							$err = "MYSQL ERROR: " . mysql_error() . "<br>";
							$err.="$query_str_exists<br>";
							mysql_close($mylink);
							trigger_error($err,E_USER_ERROR);
							return;
						}
						if(mysql_num_rows($rtn_exists)==0){
							$query_str_insert = "INSERT INTO model_data (m_index,z,n,mass_ex,uncer) VALUES (";
							$query_str_insert .= $m_index.",";
							$query_str_insert .= $row["z"].",";
							$query_str_insert .= $row["n"].",";
							$query_str_insert .= $row["mass_ex"].",";
							$query_str_insert .= $row["uncer"];
							$query_str_insert .= ")";
							$rtn_insert = mysql_query($query_str_insert);
							if(!$rtn_insert) {
								$err = "MYSQL ERROR: " . mysql_error() . "<br>";
								$err.="$query_str_insert<br>";
								mysql_close($mylink);
								trigger_error($err,E_USER_ERROR);
								return;
							}
						}
					}
				}
			}
		}
		
		printf($t_array['header'],"MODEL_INDEX=$m_index\n");
		mysql_close($mylink);
	}else {
		$err= "Missing MODEL_INDICES, MODEL_NAME, or OVERWRITE";
		$err.=" in input data stream";
		trigger_error($err,E_USER_ERROR);
	}
	print($t_array['footer']);
	return;
}

/************************************************************/
/*!  This function handles the operation which recieves the registration
 * information from the CGI and sends an email to the administration.
 * This is NOT automated for security reasons, don't want to automatically grant
 * full access to just anyone. So we save the information in the database in the
 * \tbl{register_requests} table and send the email with the information to
 * Michael (cc usin's).  The admin page should be able to populate a form without
 * having to retype/cut-and-paste the data.
 *
 * We are using the DB_info "dbname" for the associated mass_model database.
 *
 * \param[in] $database_info  A DB_info object passed as a reference
 * \param[in] $t_array An array of text chosen at the top level for formatted the
 *
 * \input Key-value pairs of FIRST_NAME,LAST_NAME,EMAIL,INSTITUTION,HEAR_OF_US,
 * COUNTRY, ADDRESS, INFO, and RESEARCH.
 *
 * \output Key-value pair of REGISTER_USER=1  or ERROR=string
 *
 **************************************************************/
function handle_register_user(& $database_info, $t_array)
{
	if(array_key_exists('FIRST_NAME',$_POST) &&
	array_key_exists('LAST_NAME',$_POST) &&
	array_key_exists('EMAIL',$_POST) &&
	array_key_exists('COUNTRY',$_POST) &&
	array_key_exists('INSTITUTION',$_POST) &&
	//array_key_exists('HEAR_OF_US',$_POST) &&
	array_key_exists('RESEARCH',$_POST) &&
	//array_key_exists('INFO',$_POST) &&
	array_key_exists('ADDRESS',$_POST) ) {



		decode($database_info->password, $database_info->key);
		$mylink=mysql_connect('localhost:3306',$database_info ->username ,
		$database_info->password);
		if(! $mylink) {
			$err="Could not CONNECT to MySQL server as ";
			$err.= $database_info->username . "/" . $database_info->password;
			trigger_error($err,E_USER_ERROR);
			return;
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
		 * about the only thing we need to do is 'purify' the input
		 * then store it and send an email. Force the name to only have
		 * hyphens and letters, the COUNTRY should only be 2 chars but don't know
		 * what it is going to come out as (hopefully ERIC will massage this).
		 * for the rest jsut cut out the ' that is so bad in mysql
		 */
		$clean['first_name']=preg_replace("/[^[:alpha:]-]/","",
		$_POST['FIRST_NAME']);
		$clean['last_name']=preg_replace("/[^[:alpha:]-]/","",$_POST['LAST_NAME']);
		$clean['country']=preg_replace("/[^[:alpha:]]/","",$_POST['COUNTRY']);
		$clean['institution']=preg_replace("/'/","\\'",$_POST['INSTITUTION']);
		$clean['address']=preg_replace("/'/","\\'",$_POST['ADDRESS']);
		$clean['email']=preg_replace("/'/","\\'",$_POST['EMAIL']);
		$clean['research']=preg_replace("/'/","\\'",$_POST['RESEARCH']);
		if(array_key_exists('HEAR_OF_US',$_POST))
		$clean['hear_of']=preg_replace("/'/","\\'",$_POST['HEAR_OF_US']);
		if(array_key_exists('INFO',$_POST))
		$clean['info']=preg_replace("/'/","\\'",$_POST['INFO']);
		$query_str="INSERT INTO register_requests (firstname,lastname,institution";
		$query_str.=",address,country,email,date,research";
		if(array_key_exists('HEAR_OF_US',$_POST))
		$query_str.=",hearof";
		if(array_key_exists('INFO',$_POST))
		$query_str.=",info";
		$query_str.=")";
		$query_str.=" VALUES ('{$clean['first_name']}','{$clean['last_name']}'";
		$query_str.=",'{$clean['institution']}','{$clean['address']}'";
		$query_str.=",'{$clean['country']}'";
		$query_str.=",'{$clean['email']}'";
		//$query_str.=",'{$clean['research']}','" . date('Y-m-d H:i:s') . "'";
		//This next line did not work until I moved the now() function
		//before the research.  No I don't know why I assume that there was a type
		//or something that was not a mysql error.
		$query_str.=",now(),'{$clean['research']}'";
		if(array_key_exists('HEAR_OF_US',$_POST))
		$query_str.=",'{$clean['hear_of']}'";
		if(array_key_exists('INFO',$_POST))
		$query_str.=",'{$clean['info']}'";
		$query_str.=")";
		$rtn=mysql_query($query_str);
		if(!$rtn) {
			$err="Could not log registration request: please try again later";
			$err.="<hr>$query_str<hr>" . mysql_error();
			mysql_close($mylink);
			trigger_error($err,E_USER_ERROR);
			return;
		}
		//printf($t_array['header'],"Query was-<br>$query_str<br>");
		//echo "{$t_array['footer']}";
		mysql_close($mylink);
		$body="Dear Coordinating Person or persons,\n\n";
		$body.="There has been a request for registration placed with the ";
		$body.="website.  Please respond in an expeditious fashion. ";
		$body.="The information has been logged and is repeated here.\n\n";
		foreach($clean as $key => $val) {
			$body.=strtoupper($key) . ": $val\n";
		}
		$body.="\nThanks for handling this. My arms are too short.\n\n";
		$body.="Your friendly CGI.\n";
		// prepare the message for coordinator
		$togmt=date("O");
		$now=date("Ymd") . (date("H") + ($togmt/100)) .date("is");
		$now.="." . strtoupper(substr(sha1($now),1,10));
		$now.='@nucastrodata.org';
		/*
		 * Set up the email and headers here.  Some of this stuff
		 * is intended to hide its origination
		 */
		//$to='coordinator@nuclearmasses.org';
		$to='coordinator@nuclearmasses.org';
		$hdrs= array( 'Date' => date('r'),
'From' => 'apache@ornl.gov',
'To' => 'Coordinator<coordinator@nuclearmasses.org>',
'Subject' => 'NuclearMasses Registration',
		//'Cc' =>'coordinator@nucastrodata.org',
		//Change this if you want blind copies somewhere else
		//Or comment it out if you don't want any at all.
'Bcc' => 'lingerfeltej@ornl.gov,eric.lingerfelt@gmail.com,smithms@ornl.gov,xrayburst1@yahoo.com',
'X-Mailer' => 'PHP/' . phpversion(),
'Message-id' => $now
		);
		include_once('mass_admin_registration_handlers.inc');

		$rtn=send_email($to,$hdrs,$body);
		//echo "sendmail returned '$rtn'<br>";
	}
	else {
		$err="Missing one of FIRST_NAME,LAST_NAME,EMAIL,INSTITUTION,ADDRESS,";
		$err.= "COUNTRY,HEAR_OF_US,RESEARCH,INFO";
		trigger_error($err,E_USER_ERROR);
	}
	print($t_array['footer']);
	return;
}
?>








