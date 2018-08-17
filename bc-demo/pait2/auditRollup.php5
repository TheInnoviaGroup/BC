#!/usr/bin/php5/php -q
<?PHP
error_reporting(E_ERROR|E_WARNING|E_PARSE);
$include_path = dirname(__FILE__) ."/alib/:". dirname(__FILE__) ."/components/:". dirname(__FILE__) ."/templates/:";
set_include_path(get_include_path() . ":" . $include_path . ":");
include_once('alib/alib.inc');
include_once('alib/sort.inc');
include_once('alib/time.inc');
include_once('alib/misc.inc');
include_once('config5.inc'); //configs.
include_once('general.inc'); // general functions and objects

global $debug, $AGLOBAL, $form, $db;
$debug->level = 0; // 0 to turn off, higher for more output. alib
		   // objects do a lot of chatter at level 11 and higher.
$debug->output = 'plain';
$debug->line('aLib init complete', 1);

$processName = "auditRollup";
$rolledUp = FALSE;
// Must create a global db. I should make the components smart about this.
$db = new db($AGLOBAL['AUDITDBURL']);
$next = TRUE;
$lostRecords = 0;



## Here's our while loop. Enjoy.
while ($next) {
//  $next = FALSE;
  $sql = "select * from rawtransactions limit 0,1";
  $debug->line("1: $sql\n",1);
  $db->query($sql, 'queue');
  if ($db->Size('queue') > 0) {
    $row = $db->nextRecord('queue', MYSQL_ASSOC);
    $cleanRow = $row;
    $deleteme = "delete from rawtransactions where time = '".$row['time']."' AND action = '".$row['action']."' AND source = '".$row['source']."'";
    $debug->line("2: $deleteme\n",1);
    $db->query($deleteme);
    $rolledUp = FALSE;
    $row['sourceDetails'] = unserialize(stripslashes($row['sourceDetails']));
    $row['from'] = unserialize(stripslashes($row['from']));
    $row['to'] = unserialize(stripslashes($row['to']));

    /* Now we make our diff. The diff here is a list of keys that either:
     a. have different values in to and from
     b. exist in from and not in to or vice versa.
    */
    $diff1 = array_keys(array_diff_assoc($row['to'], $row['from']));
    $diff2 = array_keys(array_diff_assoc($row['from'], $row['to'])); #doubly paranoid.
    $diff3 = array_keys(array_diff_key($row['to'], $row['from']));
    $diff4 = array_keys(array_diff_key($row['from'], $row['to']));

    $row['diff'] = array_unique(array_merge($diff1, $diff2, $diff3, $diff4));


   ## Here is were you add your routines for rolling up individual entries.
    personAccountHistory($row);
    manualLogEntry($row);
    
    
   if (!$rolledUp) {
       lostRecord($cleanRow);
       $lostRecords++;
    }
  } else {
    $next = FALSE;
  }
  
  
 }

if ($lostRecords > 0) {
  print "Alert: $lostRecords records were uncatagorizable.\n";
}

exit;

##Individual rollup functions go here.
## Each is expected to take an argument that is the change data array
## and each is expected to set the global $rolledUp variable to true if they rolled up the data.

##Nothing should ever hit this. But just in case it does, this is where uncategorized records go.
function lostRecord($record) {
    global $db, $debug;
    unset($record['diff']);

    $record['sourceDetails'] = addslashes($record['sourceDetails']);
    $record['from'] = addslashes($record['from']);
    $record['to'] = addslashes($record['to']);
  
  $sql = $db->prepare('insert', 'losttransactions', $record);
  $debug->line("LR1: $sql\n",1);
  $db->query($sql);
}

function manualLogEntry($record) {
  if ($record['table'] == 'log') {
    extract($record['to']);
    global $db, $rolledUp;
    $rolledUp = TRUE;
    if ($type == 'person') { //Add more here for other types later.
      $text = "Type: ".$category."<br />".$notes;
      $insert['personID'] = $id;
      $insert['username'] = $record['sourceDetails']['username'];
      $insert['action'] = 'Log';
      $insert['from'] = '';
      $insert['to'] = $text;
      $insert['time'] = $time;
    }
    $sql = $db->prepare('insert', 'personaccounthistory', $insert);
    $db->query($sql, 'insertPersonAH');
  }
}

function personAccountHistory($record) {
  global $debug;
  $tablesIcareAbout = array('people', 'companypeople', 'policyinfo', 'comphistory', 'spouse');
  $debug->line("PAH1: Here.\n",1);
  if (in_array(strtolower($record['table']), $tablesIcareAbout)) {
    $debug->line("PAH2: I care.\n",1);
    global $db, $rolledUp;
    $rolledUp = TRUE;
    extract($record);
    $insert = array();
    $insert['personID'] = $from['personID'];
    $insert['source'] = $source;
    if ($source == 'user') {
      $debug->line('User source', 1);
      if (is_array($sourceDetails)) {
	$insert['username'] = $sourceDetails['username'];
      } else {
      $insert['username'] = $sourceDetails;
      }
    } else if ($source == 'import') {
      $insert['importID'] = $sourceDetails['importID'];
      $insert['sourceDetails'] = addslashes(serialize($sourceDetails));
    }
    $insert['action'] = $action;
    $insert['time'] = $time;
    $debug->line('From:',1);
    $debug->variable($from,2);
    $debug->line('To:',1);
    $debug->variable($to,2);
    foreach ($diff as $key) {
      $debug->line('Checking '.$key,1);
      if (preg_match('/[A-Za-z]/',$key) && (array_key_exists($key, $to) || $action == 'delete' ) && $from[$key] != $to[$key]) {
        #We have a real change.
        $insert['field'] = $table.'.'.$key;
        $insert['from'] = $from[$key];
        $insert['to'] = $to[$key];
        $sql = $db->prepare('insert', 'personaccounthistory', $insert);
        $debug->line("PAH3: $sql\n",1);
        $db->query($sql, 'insertPersonAH');
      }
    }
    
  }
}



?>