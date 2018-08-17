<?

global $config;
$configFile = 'config.inc';
// include_once($configFile);

include_once("../alib/alib.inc");

addIncludePath('../alib');


$db = new idb($config->mainDB);
$sql = "SELECT p.gn, p.sn, t.userID, t.module, FROM_UNIXTIME( t.time ) AS 'date' FROM `tracking` t, enrollmentdata p WHERE p.rowID = t.userID AND FROM_UNIXTIME( t.time, '%Y%m%d' ) = CURDATE( ) -1 ORDER BY t.time DESC";

$trackingRes = $db->query($sql);;
    
$fp = fopen('/Library/WebServer/Documents/bc/reports/trackingreport-'.date("mdY").'.csv', 'w');

while ($trackingArray = $trackingRes->fetch_assoc()) {
    fputcsv($fp, $trackingArray);
}

fclose($fp);

?>

