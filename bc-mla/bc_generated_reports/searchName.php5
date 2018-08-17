<?
global $debug, $config;
$debugLevel = 0;
$configFile = '../config.inc';
// include_once($configFile);
include_once("../../html/alib/alib.inc");
addIncludePath('../../html/alib');
$debug->output = "firebug";

$searchFor = $_POST['searchFor'];

$db = new idb($config->mainDB);

$sql = "SELECT p.gn, p.sn, t.userID, t.module, FROM_UNIXTIME( t.time ) AS 'date' FROM `tracking` t, $config->enrollmentData p WHERE p.rowID = t.userID AND (p.sn LIKE '$searchFor' OR p.gn LIKE '$searchFor') AND t.enrollmentID = $config->enrollmentID ORDER BY t.time DESC";

$searchRes = $db->query($sql);
?>

<table width="100%" cellspacing="5" cellpadding="0" align="center" bgcolor="#FFFFFF">
	<tr>
		<td><b>Name</b></td>
		<td><b>Page</b></td>
		<td><b>Date / Time</b></td>
	</tr>
<?
$total = 0;
while ($reportRow = $searchRes->fetch_assoc()): ?>
<tr>
		<td><?=$reportRow['gn'];?> <?=$reportRow['sn'];?></td>
		<td><?=$reportRow['module'];?></td>
		<td><?=$reportRow['date'];?></td>
	</tr>
<? endwhile ?>
</table>

