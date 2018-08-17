<?php
header('HTTP/1.1 200 OK');
header('Date: ' . date("D M j G:i:s T Y"));
header('Last-Modified: ' . date("D M j G:i:s T Y"));
header("Content-type: application/octet-stream");
header("Content-Disposition: attachment; filename=\"DailySoFar.csv\"");
header("Cache-Control: no-store, no-cache, no-transform, must-revalidate, private");
header("Expires: 0");


$configFile = '../config.inc';
// include_once($configFile);
include_once("../../html/alib/alib.inc");
addIncludePath('../../html/alib');
$debug->output = "firebug";


$db = new idb($config->mainDB);
$month = date('m');
$day =  date('d');
$year = date('Y');


$firstThingThisMorning = mktime(0,0,0,$month,$day,$year);


$sql = "SELECT p.gn, p.sn, t.userID, t.module, FROM_UNIXTIME( t.time ) AS 'date' FROM `tracking` t, $config->enrollmentData p WHERE p.rowID = t.userID AND t.time >= $firstThingThisMorning AND t.enrollmentID = $config->enrollmentID ORDER BY t.time DESC";

$trackingRes = $db->query($sql);

print "\"First Name\",\"Last Name\",\"ID\",\"Page\",\"Date\"\r";
while ($trackingArray = $trackingRes->fetch_assoc()) {
    print "\"".$trackingArray['gn']."\",\"".$trackingArray['sn']."\",\"".$trackingArray['userID']."\",\"".$trackingArray['module']."\",\"".$trackingArray['date']."\"\r";
}


?>
