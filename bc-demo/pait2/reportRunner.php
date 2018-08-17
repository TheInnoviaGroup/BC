<?php /*>*/;

$include_path = "./templates;./components";
set_include_path(get_include_path() . ";" . $include_path . ";");
include_once('../alib/alib.inc');
include_once('../alib/sort.inc');
include_once('../alib/time.inc');
include_once('../alib/misc.inc');
include_once('../alib/cronutil.inc');
include_once('config.inc'); //configs.
include_once('general.inc'); // general functions and objects
include_once('offlinereports.inc');

global $debug, $AGLOBAL, $form, $db;
$debug->level = 1; // 0 to turn off, higher for more output. alib
		   // objects do a lot of chatter at level 11 and higher.
$debug->output = 'plain';
$debug->line('aLib init complete', 1);

$processName = "reportRunner";

// Must create a global db. I should make the components smart about this.
$db = new db($AGLOBAL['DEMODBURL']);


$sql = "select * from reportqueue where status = '0'";
$db->query($sql, 'ReportQueue');
while ($res = $db->nextRecord('ReportQueue')) {
  $debug->line('Running report ID: '.$res['reportID'].' type: '.$res['report'], 1);
  $string = $res['report'].'Offline('.$res['reportID'].');';
  $debug->line('Evalling: '.$string, 1);
  eval($string);
 }

exit;









?>