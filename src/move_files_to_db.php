<?php

include 'information.inc';
include 'admin_functions.inc';

$database_info =& get_db_info("mass_model");
decode($database_info->password, $database_info->key);
$link=mysql_connect('localhost:3306',$database_info ->username ,$database_info->password);
$rtn=mysql_query("USE $database_info->dbname");

$query_str="DELETE FROM model_data;";
$rtn=mysql_query($query_str);

//Get PUBLIC models first
echo("PUBLIC MODELS<br>");
$query_str="SELECT m_index,name FROM models WHERE type='PUBLIC';";
$rtn=mysql_query($query_str);
$row=mysql_fetch_assoc($rtn);
while($row)
{
	echo("MODEL NAME = ".$row['name']."<br>");
	$m_index = $row['m_index'];
	$name = $row['name'];
	writeFileToDB("/var/www/html/mass_model_data/PUBLIC/".$name, $m_index);
	$row=mysql_fetch_assoc($rtn);
}

//Get SHARED models next
echo("SHARED MODELS<br>");
$query_str="SELECT m_index,name FROM models WHERE type='SHARED';";
$rtn=mysql_query($query_str);
$row=mysql_fetch_assoc($rtn);
while($row)
{
	echo("MODEL NAME = ".$row['name']."<br>");
	$m_index = $row['m_index'];
	$name = $row['name'];
	writeFileToDB("/var/www/html/mass_model_data/SHARED/".$name, $m_index);
	$row=mysql_fetch_assoc($rtn);
}

//Get USER models last
echo("USER MODELS<br>");
$query_str="SELECT m_index,name,user FROM models WHERE type='USER';";
$rtn=mysql_query($query_str);
$row=mysql_fetch_assoc($rtn);
while($row)
{
	echo("MODEL NAME = ".$row['name']."<br>");
	$m_index = $row['m_index'];
	$name = $row['name'];
	$user = $row['user'];
	writeFileToDB("/var/www/html/mass_model_data/USER/".$user."/".$name, $m_index);
	$row=mysql_fetch_assoc($rtn);
}

function writeFileToDB($filename, $m_index)
{
	$listing=file($filename, FILE_IGNORE_NEW_LINES|FILE_SKIP_EMPTY_LINES);
	foreach($listing as $val) {
		$tmp_line=preg_split("/ +/",trim($val));
		$z = $tmp_line[0];
		$n = $tmp_line[1];
		$mass_ex = $tmp_line[2];
		$uncer = 0.0;
		if(count($tmp_line)==4) {
			$uncer = $tmp_line[3];
		}
		$query_str="INSERT INTO model_data (m_index,z,n,mass_ex,uncer) VALUES ("
						.$m_index.","
						.$z.","
						.$n.","
						.$mass_ex.","
						.$uncer.");";
		mysql_query($query_str);
	}
}
?>
