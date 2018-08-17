<?
global $config;
$configFile = 'config.inc';
// include_once($configFile);

include_once("../alib/alib.inc");

addIncludePath('../alib');


$db = new idb($config->mainDB);


$results = $db->query("SELECT rowID, birthdate FROM mlaenrollment");

while($current = $results->fetch_assoc()) {
	$db->query("UPDATE mlaenrollment SET passwd = '".date("m/d/Y", $current['birthdate'])."' WHERE rowID = ".$current['rowID']);
	print $current['rowID'] ." is ".date("m/d/Y", $current['birthdate']);
}


?>