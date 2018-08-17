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

$processName = "dataReconciler";

// Must create a global db. I should make the components smart about this.
$db = new db($AGLOBAL['DEMODBURL']);


include_once('importMaps.inc');

if( !function_exists('memory_get_usage') )
{
  //print "redefining memory_get_usage\n";
   function memory_get_usage()
   {
       //If its Windows
       //Tested on Win XP Pro SP2. Should work on Win 2003 Server too
       //Doesn't work for 2000
       //If you need it to work for 2000 look at http://us2.php.net/manual/en/function.memory-get-usage.php#54642
       if ( substr(PHP_OS,0,3) == 'WIN')
       {
               if ( substr( PHP_OS, 0, 3 ) == 'WIN' )
               {
                   $output = array();
                   exec( 'tasklist /FI "PID eq ' . getmypid() . '" /FO LIST', $output );
      
                   return preg_replace( '/[\D]/', '', $output[5] ) * 1024;
               }
       }else
       {
           //We now assume the OS is UNIX
           //Tested on Mac OS X 10.4.6 and Linux Red Hat Enterprise 4
           //This should work on most UNIX systems
           $pid = getmypid();
           exec("ps -eo%mem,rss,pid | grep $pid", $output);
           $output = explode("  ", $output[0]);
           //rss is given in 1024 byte units
           return $output[1] * 1024;
       }
   }
} 



//if (cronLock($processName, $db)) {
  $sql = "select * from rawdatatoc where status = '2'";
  $db->query($sql, 'dataTOC');
  if ($db->Size('dataTOC') > 0) {
    $sql2 = "update rawdatatoc set status = '20', time = '".time()."' where status = '2'";
    $db->query($sql2, 'dataTOClock');
    while ($res = $db->nextRecord('dataTOC')) {
      $config = unserialize($res['config']);
      $config['companyID'] = $res['companyID'];
      $config['carrierID'] = $res['carrierID'];
      $config['type'] = $res['type'];
      $config['owner'] = $res['owner'];
      $config['importID'] = $res['importID'];
      reconcileData($config);
	$sql2 = "update rawdatatoc set status = '3', time = '".time()."' where importID = '".$res['importID']."'";
    $db->query($sql2, 'dataTOClock');
    ;
    }
  }
// cronUnLock($processName, $db);
//} else {
//  exit;
//}



function reconcileData($config) {
  global $db, $debug, $uploadPath;
global $headerMap, $compensationMap, $compTypes, $policyMap, $personStatus;
$debug->line('reconcilingData',1);
  //var_dump($config);
$worksheet = ($config['worksheet'])?$config['worksheet']:0;
  $tablename = $config['type'].'-'.$config['importID'].'-'.$config['companyID'].'-'.$config['carrierID'].'-'.$worksheet;
  $form = $config['headerStuff'];
  $dateType = $config['dateType'];
  $debug->variable($config, 5);
  $headerRowSQL = "select * from `$tablename` where rowID = '".$form->headerrow."'";
  $debug->line($headerRowSQL, 1);
  $db->query($headerRowSQL);
  $headerRowTemp = $db->nextRecord();
  $headerRow = array();
  foreach($headerRowTemp as $key => $val) {
    if (! is_numeric($key) && $key != 'rowID') {
      $headerRow[] = $val;
    }
  } // Get rid of cruft.

  $defaultStatus = $form->defaultStatus;
  $overrideStatus = $form->overrideStatus;
  $headerAction = $form->headerAction;
  $headerSub1Action = $form->headerSub1Action;
  $headerSub2Action = $form->headerSub2Action;
  $now = time();
  $year = date("Y", $now);
  $previousYear = $year - 1;
#Okay. Here's the loop.
$emptyRows = 0;
$tablecreated = 0;
$dudes = 0;
  $extrafields = array();

  $countSQL = "select count(*) as numrows from `$tablename`";
  $db->query($countSQL);
  $res = $db->nextRecord();
  $numrows = $res['numrows'];
  $myRowcount = 0;
  $interval = 250;
  //print "$numrows\n";
  while ($myRowcount <= $numrows) {
if ($myRowcount > 0) {
    $db->free('getTableData');
   // print "[".date("H:i:s")."] Mem: ".memory_get_usage()."\n";
  }
 if ($myRowcount + $interval > $numrows) {
    $interval = $numrows - $myRowcount + 1;
  }
  $dataSQL = "select * from `$tablename` where rowID != '".$form->headerrow."' limit $myRowcount, $interval";
  //print "$myRowcount: $dataSQL\n";
  $myRowcount += $interval;
  $db->query($dataSQL, 'getTableData');
$debug->line($dataSQL, 1);
  while ($dataTemp = $db->nextRecord('getTableData')) {
    // $db->free('getTableData');
$actionTaken = array('importID' => $config['importID'],
'rowID' => $dataTemp['rowID']);
$actionTaken['action'] = 'None';
//$debug->variable($data, 5);
  $data = array();
  foreach($dataTemp as $key => $val) {
    if (! is_numeric($key) && $key != 'rowID') {
      $data[] = $val;
    }
  } // Get rid of cruft.

$debug->line('row gotten', 1);
  $datacount = count($data);
  $headercount = count($headerRow);
  $countdiff = $headercount - $datacount;
  if ($countdiff >= 0 && $countdiff <= 2) { #A match! Or close enough.
//$debug->line('a diff!',1);
$carrierID = $config['carrierID'];

if (is_array($config['xlCarriers'])) {
	$mykey = array_search('carrier', $headerAction);
	$mycol = "field".array_search($mykey, array_keys($headerAction));
$carrierID = $config['xlCarriers'][$dataTemp[$mycol]];
$debug->line('Setting carrier to: '.$carrierID." ".$mycol." ".$dataTemp[$mycol], 1);

}

# My arrays are named for tables: people, companypeople, comphistory, policyinfo, rawdatatoc and rawdata.
					     $people = array('companyID' => $config['companyID'],
							     );
  $companypeople = array('companyID' => $config['companyID']);
  $comphistory = array();
  $rawdatatoc = array('companyID' => $config['companyID'],
		      'carrierID' => $carrierID,
		      'type' => ($carrierID != 9 && $carrierID != 0)?"bid":"census",
		      'time' => $now);  
  $dothis = array();
  global $policyTypes;
  $policyinfo = array('policyStatus' => $config['policyStatus'],
		      'paidBy' => $config['paidBy'],
		      'policyType' => $policyTypes[$config['policyType']]);
  $count = 0;

  foreach ($headerRow as $header) {

    $temph = preg_replace('/[\s]+/', '', $header);
    $dest = $headerAction[$temph];
    if ($dest == 6) {
      $dest = $headerSub2Action[$temph];
      $headerAction[$temph] = $dest;
    }
    if ($dest == 1) {
      $dest = $headerSub1Action[$temph];
      $debug->line('dest: '.$dest,2);
      $debug->variable($headerSub1Action,2);
      //    $headerAction[$temph] = $dest;
      $debug->line("CompH: header: $header", 1);
      if (!preg_match("/(\d\d\d\d)/", $header)) {
	$header = preg_replace('/annual/i', $year, $header);
	$header = preg_replace('/current year/i', $year, $header);
	$header = preg_replace('/previous year/i', $previousYear, $header);
	$header = preg_replace('/current/i', $year, $header);
	$header = preg_replace('/previous/i', $previousYear, $header);
      }
      if (preg_match("/(\d\d\d\d)/", $header, $matches)) {
	$headeryear = $matches[1];
      }
      // Try to catch "current and previous" text things.
      $debug->line("CompH: new header: $header $dest $headeryear", 1);
      $debug->line("CompH: data: ".$data[$count], 1);
      $comphistory[$header]['data'] = $data[$count];
      $comphistory[$header]['type'] = $dest;
      $comphistory[$header]['year'] = $headeryear;
      $comphistory[$header]['updated'] = time();
      $dothis['comphistory'] = 1;
      $actionTaken['compensationAdded'][$type] = 1;
    }
    if (preg_match('/(hiredate|birthdate|effectivedate|reconsiderationdate)/i', $dest)) {
#Dates are special. Note also, gotta have PHP 5.1.0 to have dates before 1970.
      if (preg_match("/\//",$data[$count])) {
	$date = explode("/",$data[$count]);
	if ($date[2] < 1900) { $date[2] += 1900; };
      $utime = mktime(1,1,1,$date[0],$date[1],$date[2]);
      $debug->line('slashdate', 1);
      } elseif (preg_match("/\-/",$data[$count])) {
	$date = explode("-",$data[$count]);
	if ($date[2] < 1900) { $date[2] += 1900; };
      $utime = mktime(1,1,1,$date[0],$date[1],$date[2]);
      $debug->line('dashdate', 1);
      } elseif (preg_match("/\./",$data[$count])) {

	$date = explode('.',$data[$count]);
	if ($date[0] < 13) {
	  if ($date[2] < 1900) { $date[2] += 1900; };
	  $utime = mktime(1,1,1,$date[0],$date[1],$date[2]);
	  $debug->line('dotdate', 1);
	} else {
	  if ($dateType == "zakkis2") {
	  // Stupid zakkis.ca uses days since 1/1/1900. Okay. We can do that. Convert days to seconds:
	  $seconds = $data[$count] * 86400;
	  // Utimes are measured since 1/1/1970, so we have to adjust by -220896000, which is the utime of 1/1/1900
	  $utime = $seconds - 2082988800;
	  $debug->line('stupid zakkis2 date:'.$data[$count]." ".$seconds." ".$utime, 1);
	  } elseif ($dateType == "rawdate") {

	    $utime = $data[$count];

	  } elseif ($dateType == "rawText") {
	    $utime = $data[$count];
	  } else {
	  // Stupid zakkis.ca uses days since 1/1/1900. Okay. We can do that. Convert days to seconds:
	  $seconds = $data[$count] * 86400;
	  // Utimes are measured since 1/1/1970, so we have to adjust by -220896000, which is the utime of 1/1/1900
	  $utime = $seconds - 2208960000;
	  $debug->line('stupid zakkis1 date:'.$data[$count]." ".$seconds." ".$utime, 1);

	  }
	}
	
      
      } elseif ($data[$count] == 0) {
	$utime = NULL;
      } else {
	//      $date = $xl->getDateArray($data[$count]);
	//	if ($date['year'] < 1900) { $date['year'] += 1900; };
	//$utime = mktime(1,1,1,$date['month'],$date['day'],$date['year']);
	// Stupid zakkis.ca uses days since 1/1/1900. Okay. We can do that. Convert days to seconds:
	$seconds = $data[$count] * 86400;
	// Utimes are measured since 1/1/1970, so we have to adjust by -2208985200, which is the utime of 1/1/1900.
	$utime = $seconds - 2208985200;
      $debug->line('stupid zakkis date:'.$data[$count]." ".$seconds." ".$utime, 1);
      }
      $debug->line('utime data: '.$utime,1);
      $debug->variable($date,1);
       switch ($dest) {
        case "hiredate":
	  $companypeople[$dest] = $utime;
	  $dothis['companypeople'] = 1;
	  break;
       case "birthdate":
	 $people[$dest] = $utime;
	 break;
       default:
	 $policyinfo[$dest] = $utime;
	 break;
       }
       
    } elseif (preg_match('/(title|employeeID|department)/', $dest)) {
      $companypeople[$dest] = $data[$count];
      $dothis['companypeople'] = 1;
    } elseif (preg_match('/(policyNumber|policyForm|benefitAmount|premium|premiumTerm|eliminationPeriod|benefitPeriod|riskNumber|class|stateAppSigned|catastrophic|discount|mnda|cola|gpi|ndi|ltc|residual|recovery|wib|groupLTD|extendedPartial|lc|fio)/', $dest)) {
      $policyinfo[$dest] = $data[$count];
      $dothis['policyinfo'] = 1;
      $actionTaken['policyAdded'] = 1;
    } elseif ($dest == "ssn") {
      $people[$dest] = intval(preg_replace('/\-/', '', $data[$count]));
    } elseif ($dest == "gender") {
      $people[$dest] = (preg_match('/f/i', $data[$count]))?1:2;
    } elseif (preg_match('/(gn|sn|mn|cn|city|state|postal|address|address2|email|workphone|homephone|cellphone|status|facility)/', $dest)) {
      $people[$dest] = preg_replace('/\'/', '&#039;', $data[$count]);
    } elseif (preg_match('/amendment/', $dest)) {
      $policyinfo[$dest] .= "\n".$data[$count];
    } elseif ($dest == 3) {
      
#they gave us first/middle names.
      $crap = explode(" ", $data[$count]);
      $people['gn'] = $crap[0];
      $people['mn'] = $crap[1];
    } elseif ($dest == 4) {
#last and suffix
      $crap = explode(", ", $data[$count]);
      if (count($crap) > 1) {
	$people['sn'] = $crap[0];
	$people['suffix'] = $crap[1];
	
      } else {
	$crap = explode(" ", $data[$count]);
	$people['sn'] = $crap[0];
	$people['suffix'] = $crap[1];
      }
    } elseif ($dest == 5) {
      $thingy = 0;
 
      if (strtolower($data[$count]) == "yes" || strtolower($data[$count]) == "y") {
	$thingy = 1;
     }
      $policyinfo['tobaccoUser'] = $thingy;
    } elseif ($dest != 2) {
      $rawdata[$header] = $data[$count];
      $dothis['rawdata'] = 1;
    }
    $count++;
  }
  
#Now that we've got that sorted, let's find out who this is.
  $personID = findPerson($data, $headerRow, $headerAction, $config['companyID']);
  if ($personID > 0) {
#Repeat:
$actionTaken['action'] = "Updated";
    $itype = "replace";
    $people['personID'] = $personID;
    $companypeople['personID'] = $personID;
    $rawdata['personID'] = $personID;
    $policyinfo['personID'] = $personID;
    $sql = "select * from people where personID = '$personID'";
    $db->query($sql);
    global $updateOnly;
$updateOnly = $config['updateOnly'];
    $row = $db->nextRecord();
    if ($updateOnly) {
      $mystatus = $row['status'];
    }
    foreach ($people as $key => $val) {
      if (!$updateOnly || $key != "status"){
        if ($val != $row[$key]) {
	$actionTaken['statusChange'] = $row[$key]." => ".$val;
	$log .= "$key was ".$row[$key]." and was changed to ".$val."<br />\n";
	$row[$key] = $val;
	}
      }
    }
    

    if ($updateOnly) {
      $row['status']=$mystatus;
    }

	if ($overrideStatus == "on") {
		$row['status'] = $defaultStatus;
		$actionTaken['statusChange'] = "Status set to ".$row['status'];

	}


    $sql1 = $db->prepare($itype, 'people', $row);
    
    $db->query($sql1);
    $debug->line("1: ".$sql1, 1);
  } else {
    global $updateOnly;
    if ($updateOnly) {
      continue;
    }
    $itype = "insert";
    $actionTaken['action'] = "Added";
    if ((! $people['status']) || $overrideStatus == "on") { 
      $people['status'] = $defaultStatus; 
    } else {
      $debug->line('Crap: '.$people['status'].' '.$overrideStatus);
    }
$actionTaken['statusChange'] = "Status set to ".$people['status'];
    $sql1 = $db->prepare($itype, 'people', $people);
    $db->query($sql1);
    $debug->line("2: ".$sql1, 1);
    $personID = $db->insertID();
    $companypeople['personID'] = $personID;
    $rawdata['personID'] = $personID;
    $policyinfo['personID'] = $personID;
  }


  $dudes++;
  #Do it!
  if ($dothis['companypeople']) {
  $sql2 = $db->prepare($itype, 'companypeople', $companypeople);
  $db->query($sql2);
    $debug->line("3: ".$sql2, 1);
  }

if ($dothis['policyinfo']) {
  // We always insert policy info. Not update.
  $rawdatatoc['type'] = "ratefile";
  $policyinfo['importID'] = $config['importID'];
}    
if (count($comphistory) > 0) {
    foreach($comphistory as $title => $array) {
      $array = array('personID' => $personID,
		     'companyID' => $config['companyID'],
		     'title' => $type,
		     'amount' => $array['data'],
		     'year' => $array['year'],
		     'type' => $array['type']);
    $sql3 = $db->prepare($itype, 'comphistory', $array);

    $db->query($sql3);
    $debug->line("4: ".$sql3, 1);
    }
  } else {
    $debug->line("Comphistory", 1);
    $debug->variable($comphistory,1);
  }

  if (count(array_keys($rawdata))>1) {
    if ($tablecreated == 0) {
    //$sql4 = $db->prepare($itype, 'rawdatatoc', $rawdatatoc);
    //$db->query($sql4);
    // $debug->line("5: ".$sql4, 1);
   $tableID = $config['importID'];
   $importID = $tableID;
    $table = $rawdatatoc['type']."-".$rawdatatoc['carrierID']."-".$rawdatatoc['companyID']."-".$tableID;
    }
    #Now. Yes. We create our own table. Ugh.
	$tbl_SQL .= "CREATE TABLE IF NOT EXISTS `$table` ( `iuid` int(32) NOT NULL auto_increment,";
	$fieldname = array();
	foreach ($rawdata as $fc => $val) {
			// Prepare table structure
	  if (preg_match('/[A-Za-z0-9]/', $fc)) {
	    $fieldname[$fc] = preg_replace ("/\%/", "Percent", $fc);
			$fieldname[$fc] = preg_replace ( "/[^a-zA-Z0-9]/", "", $fieldname[$fc] );
			$fieldname[$fc] = preg_replace ("/(\s)+/", "", $fieldname[$fc]);
			//			$fieldname[$fc] = ereg_replace ( "^[0-9]+", "", $fc );
			
			$tbl_SQL .= $fieldname[$fc] . " text NOT NULL,";
		}
	}
	//$tbl_SQL = rtrim($tbl_SQL, ',');
	
	$tbl_SQL .= "PRIMARY KEY  (`iuid`)) TYPE=MyISAM DEFAULT CHARSET=utf8";
	if ($tablecreated == 0) {$db->query($tbl_SQL);     $debug->line("6: ".$tbl_SQL, 1);$tablecreated = 1; $extrafields = $fieldname;}

	$insertme = array();
	foreach ($extrafields as $fc => $col) {
	  $insertme[$col] = $rawdata[$fc];
	}
	if ($dothis['policyinfo']) {
	$sql5 = $db->prepare('INSERT', "`".$table."`", $insertme);
	} else {
	$sql5 = $db->prepare($itype, "`".$table."`", $insertme);
	}
	$db->query($sql5);
    $debug->line("7: ".$sql5, 1);
    //print "$sql5<br />\n";
    $rawdata = array();
    //exit;
  }

  if ($dothis['policyinfo']) {
  $policyinfo['carrierID'] = ($carrierID)?$carrierID:9;
  $policyinfo['importID'] = $importID;
  $rawdata['policyNumber'] = $policyinfo['policyNumber'];
  $sql5 = $db->prepare('INSERT', 'policyinfo', $policyinfo);
  $db->query($sql5);
  $uid = $db->insertID();
    $debug->line("6: ".$sql5, 1);
  }
    $array = array('username' => "System",
		 'type' => 'person',
		 'id' => $personID,
		 'notes' => $log,
		 'category' => "System Import",
		 'time' => time());
  $sql = $db->prepare('insert', 'log', $array);
  $db->query($sql);
$debug->line($sql,1);
$actionTaken['logID'] = $db->insertID();
$actionTaken['personID'] = $personID;
 $compTypesAdded = $actionTaken['compensationAdded'];
 unset($actionTaken['compensationAdded']);
 $actionTaken['compensationAdded'] = natsort(array_keys($compTypesAdded));
 $sql = $db->prepare('insert', 'changereport', $actionTaken);
 $db->query($sql);
 $debug->line($sql,1);
  }

  }
  }

}


function findPerson($data, $headerRow, $headerAction, $companyID) {
global $config, $form;
// This seriously needs to be rewritten.
$headerTemp = $headerRow;
$headerRow = array();
foreach ($headerTemp as $header ) {
$headerRow[] = preg_replace("/\s+/", "", $header);
}

global $db, $debug;
$sql = NULL;
if (in_array('ssn', $headerAction)) {

#Woot, SSN. That's my boy.
#First, which header is the ssn:
$header = array_search('ssn', $headerAction);
#Now, which element in the row is that?
$cell = array_search($header, $headerRow);
#Now, what's that value?
$ssn = $data[$cell];
 $ssnnodash = intval(preg_replace('/-/', '', $ssn));
$ssnlpad = sprintf("%09d", $ssnnodash);
 $sql = "select personID from people where ssn = '$ssn' OR ssn = '$ssnnodash' OR ssn = '$ssnlpad'";
 } elseif (in_array('employeeID', $headerAction)) {
$header = array_search('employeeID', $headerAction);
$cell = array_search($header, $headerRow);
$id = $data[$cell];
$sql = "select personID from companypeople where companyID = '".$config['companyID']." and employeeID = '$id'";
} elseif (in_array('sn', $headerAction) && (in_array('gn', $headerAction) || in_array('3', $headerAction))) {
#lucky us, we have a first and last name!
$headerSN = array_search('sn', $headerAction);
if (in_array('gn', $headerAction)) {
$headerGN = array_search('gn', $headerAction);
} else {
$headerGN = array_search('3', $headerAction);
}
$cellSN = array_search($headerSN, $headerRow);
$cellGN = array_search($headerGN, $headerRow);
$debug->line("Header SN/GN: $headerSN $headerGN", 1);
$debug->line("cellSN/GN: $cellSN $cellGN");
$sn = $data[$cellSN];
$gn = $data[$cellGN];
if (preg_match("/ /", $gn)) {
$crap = explode(" ", $gn);
$gn = $crap[0];
}
$sql = "select personID from people where sn = '$sn' AND gn = '$gn' AND companyID = '$companyID'";
} elseif (in_array('cn', $headerAction)) {
$header = array_search('cn', $headerAction);
$cell = array_search($header, $headerRow);
$cn = $data[$cell];
$sql = "select personID from people where cn = '$cn' AND companyID = '$companyID'";
}
if ($sql) {
$debug->line("Couldn't find person for:", 1);
$debug->variable($data, 1);
$debug->variable($headerAction, 1);
$debug->variable($headerRow, 1);
$debug->line("Findperson SQL: $sql", 1);
$db->query($sql);
$res = $db->nextRecord();
$retval = $res['personID'];
} else {
$retval = 0;
}
return $retval;
}



function mapThis($headerRow) {
  $retval = array();
  $map = array('#.*#i' => "Leave As Is",  #This is always first. Leave it.
	       '#id$#i' => "Employee ID",
	       '#name#i' => "Full Name",
    '#fname#i' => "First Name",
    '#lname#i' => "Last Name",
	       '#givenname#i' => "First Name",
	       '#firstname#i' => "First Name",
	       '#surname#i' => "Last Name",
	       '#lastname#i' => "Last Name",
    '#middlename#i' => "Middle Name",
    '#middleinitial#i' => "Middle Name",
    '#middle#i' => "Middle Name",
	       '#social#i' => "Social Security Number",
	       '#soc#i' => "Social Security Number",
	       '#ssn#i' => "Social Security Number",
	       '#gender#i' => "Gender",
	       '#sex#i' => "Gender",
	       '#birth#i' => "Birth Date",
	       '#^dob$#i' => "Birth Date",
	       '#city#i' => "City",
	       '#postal#i' => "Postal Code",
	       '#zip#i' => "Postal Code",
    '#addr.*2#i' => "Address 2",
    '#addr.*two#i' => "Address 2",
	       '#addr#i' => "Address",
	       '#workphone#i' => "Work Phone",
	       '#homephone#i' => "Home Phone",
	       '#cellphone#i' => "Cell Phone",
    '#email#i' => "E-mail",
    '#e-mail#i' => "E-mail",
	       '#employee#i' => "Employee ID",
	       '#title#i' => "Job Title",
    '#department#i' => "Department/Division",
'#dept#i' => "Department/Division",
'#division#i' => "Department/Division",
'#div#i' => "Department/Division",
	       '#hiredate#i' => "Hire Date",
    '#^ep$#i' => 'map2',
    '#^bp$#i' => 'map2',
    '#premium#i' => 'map2',
    '#premiumterm#i' => 'map2',
    '#form#i' => 'map2',
    '#catastrophic#i' => 'map2',
    '#occ#i' => 'map2',
    '#policy#i' => 'map2',
'#signed#i' => 'map2',
'#class#i' => 'map2',
'#catas#i' => 'map2',
'#tob#i' => 'map2',
'#prem.*term#i' => 'map2',
'#occ#i' => 'map2', 
'#res#i' => 'map2',
'#risk#i' => 'map2',
    '#effectivedate#i' => 'map2',
	       '#state#i' => "State",
	       '#^st$#i' => "State",
    '#benefit#i' => 'map2',
    '#smoker#i' => 'map2',
    '#discount#i' => 'map2',
    '#mnda#i' => 'map2',
    '#cola#i' => 'map2',
	       '#salary#i' => "map3",
	       '#basecomp#i' => "map3",
	       '#bonus#i' => "map3",
	       '#commission#i' => "map3",
	       '#incentivecomp#i' => "map3",
	       ); #This has to be the last one. Add new matches above.
	       $map2 = array('#form#' => "Policy Form",
    '#^ep$#i' => "Elimination Period",
    '#^bp$#i' => "Benefit Period",
		 '#benefit period#i' => "Benefit Period",
    '#premium.*term#i' => "Premium Term",
    '#premium.*form#i' => "Premium Form",
    '#premium#i' => "Premium",
    '#policy#i' => "Policy Number",
    '#effectivedate#i' => "Effective Date",
    '#benefit#i' => "Benefit Amount",
    '#tob#i' => "Tobacco Use",
    '#smoker#i' => "Tobacco Use",
    '#discount#i' => "Discount",
    '#mnda#i' => "MNDA",
		 '#cola#i' => "COLA",
'#signed#i' => 'State App Signed',
'#class#i' => 'Class',
'#catas#i' => 'Catastrophic',
'#res#i' => 'Residual',
'#risk#i' => 'Risk Number',
		 '#catastrophic#i' => "Catastrophic",
);
global $compTypes;
$map3 = array();
foreach ($compTypes as $name => $val) {
$map3["#".strtolower(preg_replace('/[\s]+/', '', $name))."#i"] = $name;
}
#1 = comp, 2 = policy
		    $count = 0;
  foreach ($headerRow as $header) {
    $checkme = preg_replace('/[\s]+/', '', $header); #Check against header with whitespace stripped. 'cuz I said so.
    foreach ($map as $regexp => $name) {
if (preg_match($regexp, $checkme)) {
if ($name == "map2") {
foreach($map2 as $regexp2=>$newname) {
if (preg_match($regexp2, $checkme)) {
$retval[0][$header] = "Policy Information";
$retval[2][$header] = $newname;
$retval[1][$header] = "Leave As Is";
}
}
} elseif ($name == "map3") {
foreach($map3 as $regexp3=>$newname) {
if (preg_match($regexp3, $checkme)) {
$retval[0][$header] = "Include in Compensation History";
$retval[1][$header] = $name;
$retval[2][$header] = "Leave As Is";
}}
} else {
$retval[0][$header] = $name;
$retval[1][$header] = "Leave As Is";
$retval[2][$header] = "Leave As Is";
}
}
}
 }   

  
return($retval);
}



?>
