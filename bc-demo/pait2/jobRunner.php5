#!/usr/bin/php5/php
<?PHP
$include_path = dirname(__FILE__) ."/alib/:". dirname(__FILE__) ."/components/:". dirname(__FILE__) ."/templates/:";
set_include_path(get_include_path() . ":" . $include_path . ":");
include_once('alib/alib.inc');
include_once('alib/sort.inc');
include_once('alib/time.inc');
include_once('alib/misc.inc');
include_once('config5.inc'); //configs.
include_once('general.inc'); // general functions
global $debug, $AGLOBAL, $form, $db;
$debug->level = 3; // 0 to turn off, higher for more output. alib
		   // objects do a lot of chatter at level 11 and higher.
$debug->output = 'plain';
$debug->line('aLib init complete', 1);

$processName = "jobRunner";
// Must create a global db. I should make the components smart about this.
$db = new db($AGLOBAL['DEMODBURL']);
$sheetDB = createSheetDB();
$next = TRUE;




## Here's our while loop. Enjoy.
while ($next) {
//  $next = FALSE;
  $sql = "select * from jobqueue where status=0 limit 0,1";
  $debug->line("1: $sql\n",1);
  $sheetDB->query($sql, 'queue');
  if ($sheetDB->Size('queue') > 0) {
    $row = $sheetDB->nextRecord('queue', MYSQL_ASSOC);
    $cleanRow = $row;
    $deleteme = "update jobqueue set status = 1 where created = '".$row['created']."' AND jobFunction = '".$row['jobFunction']."' AND sheet1 = '".$row['sheet1']."' AND sheet2 = '".$row['sheet2']."'";
    $debug->line("2: $deleteme\n",1);
    $sheetDB->query($deleteme);
    switch($row['jobFunction']) {
   ## Here is were you add your routines for new functions
      case "import":
      doImport($row, FALSE);
      break;
    case "importTest":
      doImport($row, TRUE);
      break;
      case "unmatchedPeople":
      unmatchedPeople($row);
      break;
 /*     case "unmatchedSpouses":
      unmatchedSpouses($row);
      break;
      case "comparePAIT":
      comparePAIT($row);
      break; */
      case "compareTwo":
	compareTwo($row);
      break;
    case "mergeTwo":
      mergeTwo($row);
      break;
    }
  } else {
    $next = FALSE;
  }
  
  
 }
 
function doImport($reportConfig, $test=TRUE) {
  include_once('importMaps.inc');
  global $db, $sheetDB, $debug, $columnMap;
  if ($test) {
    //And now our missing data report:
      $sheetDB->query("insert into reports set sheetID = '".$reportConfig['sheet1']."', status=0, type='importTest', name='Import Test Report (".date(PAITDATE, time()).")', owner='".$reportConfig['owner']."', time='".time()."';", "report");
      $reportConfig['reportID'] = $sheetDB->insertID();
  } else {
     $sheetDB->query("insert into reports set sheetID = '".$reportConfig['sheet1']."', status=0, type='import', name='Import Report (".date(PAITDATE, time()).")', owner='".$reportConfig['owner']."', time='".time()."';", "report");
      $reportConfig['reportID'] = $sheetDB->insertID();
  }
  
  
  $sheet = 'sheet-'.$reportConfig['sheet1'];
  findPeople($sheet);
  $descSQL = 'describe `'.$sheet.'`';
  $sheetDB->query($descSQL);
  $sheetDesc = $sheetDB->allRecords();
  $sheetCols = array();
  $created = array();
  foreach ($sheetDesc as $row) {
    if ($row['Field'] != 'rowID' && $row['Field'] != 'duplicates' && !stristr('ignore', $row['Field'])) {
    $sheetCols[] = $row['Field'];
    }
  }
  $configSQL = "select * from sheets where sheetID = '".$reportConfig['sheet1']."'";
  $sheetDB->query($configSQL);
  $debug->line($configSQL, 1);
  $sheetInfo = $sheetDB->nextRecord();
  $importConfig = unserialize($sheetInfo['importConfig']);
  $headerRow = $importConfig['headerStuff']->headerrow;
  $importType = $importConfig['headerStuff']->importType; // Census, Ratefile, Policy Info
  $policyType = $importConfig['headerStuff']->policyType;
    $policyStatus = $importConfig['headerStuff']->policyStatus;
      $defaultStatus = $importConfig['headerStuff']->defaultStatus;
      $dateType = $importConfig['headerStuff']->dateType;
  $companyID = $importConfig['headerStuff']->company;
  $carrierID = $importConfig['headerStuff']->carrier;
    $headerAction = $importConfig['headerStuff']->headerAction;
    $headerSub1Action = $importConfig['headerStuff']->headerSub1Action;
    $headerSub2Action = $importConfig['headerStuff']->headerSub2Action;
    $getDataSQL = "select * from `$sheet` where rowID != '".$headerRow."'";
    $sheetDB->query($getDataSQL, 'sheetData');
$debug->line($getDataSQL, 1);
    while ($row = $sheetDB->nextRecord('sheetData', MYSQL_ASSOC)) {
      $personID = $row['personID'];
      if ($personID == -1) {
        if ($created[$row['ssn']] > 0) {
          $personID = $created[$row['ssn']];
        } elseif ($created[$row['employeeID']] > 0) {
          $personID = $created[$row['employeeID']];
        } 
      }
      $people = array();
      $companypeople = array();
      $policyinfo = array();
      $comphistory = array();
      $leaveasis = array();
      if ($personID > 0) {
        //Update.
        $type = 'Update';
        $itype = 'update';
        $oldSQL1 = "select * from people where personID = '".$personID."'";
        $oldSQL2 = "select * from companypeople where personID = '".$personID."'";
        $db->query($oldSQL1);
        $oldPeople = $db->nextRecord('Default', MYSQL_ASSOC);
        $debug->line('oldPeople: '.$oldSQL1, 2);
        $db->query($oldSQL2);
        $oldCompanyPeople = $db->nextRecord('Default', MYSQL_ASSOC);
      } else {
        $type = 'Add';
        $itype = 'insert';
      }
      foreach ($row as $col => $val) {
        if (stristr($col, 'date')) {
          $val = getXLSDate($val, $dateType);
        }
        $debug->line('building: '.$col.' : '.$val, 2);
        #assign row data to appropriate tables.
        if (in_array($col, $columnMap['people'])) {
          if (stristr($col, 'gender')) {
            if (stristr($col, 'f')) {
              $people[$col] = 1;
            } elseif (stristr($col, 'm')) {
              $people[$col] = 2;
            } else {
              $people[$col] = 0;
            }
          } else {
          $people[$col] = $val;
          }
        } elseif (in_array($col, $columnMap['companyPeople'])) {
          $companypeople[$col] = $val;
        } elseif (in_array($col, $columnMap['policyInfo'])) {
          $policyinfo[$col] = $val;
        } elseif (in_array($col, $columnMap['comphistory'])) {
          $array = array();
          $array['type'] = $col;
          $array['amount'] = $val;
          $array['year'] = date('Y', time());
          $array['updated'] = time();
          $array['personID'] = $personID;
          $array['companyID'] = $companyID;
          $comphistory[] = $array;
        } else {
          $leaveasis[$col] = $val;
        }
      } #End table assignments
      
      if (count($people) > 0) {
        $debug->line('updating person '.$personID,2);
        if ($type != 'Update') {
          unset($people['personID']);
        } else {
        $people['personID'] = $personID;
        }
        $people['companyID'] = $companyID;
        
        if($type == 'Update') {
          $sql = $db->prepare($itype, 'people', $people, " where personID = '".$personID."'");
        $debug->line('peopleSQL: '.$sql,2);
        if (!$test) {
          $db->query($sql, 'updatePeople');
        }
            $sn = ($people['sn'])?$people['sn']:$oldPeople['sn'];
            $gn = ($people['gn'])?$people['gn']:$oldPeople['gn'];
            $ssn = ($people['ssn'])?$people['ssn']:$oldPeople['ssn'];
          logDiffs('people', $people, $oldPeople, $reportConfig, $sn, $gn, $ssn, $test);
        } else {
          $sql = $db->prepare($itype, 'people', $people);
        $debug->line('peopleSQL: '.$sql,2);
        if (!$test) {
          $db->query($sql, 'updatePeople');
        }
          $personID = $db->insertID();
          
          $sn = $people['sn'];
          $gn = $people['gn'];
          $ssn = ($people['ssn'])?$people['ssn']:$people['employeeID'];
          $created[$ssn]=$personID;
          $sqlT = "insert into `%s` set reportID = '%s', sheetID = '%s', uniqueID = '%s', gn = '%s', sn='%s', `type`='%s', `field`='%s', `from`='%s', `to`='%s'";
          $sheetDB->query(sprintf($sqlT, ($test)?'importtestreport':'importreport', $reportConfig['reportID'], $reportConfig['sheet1'], $ssn, $gn, $sn, 'Created', 'people', '0', $personID));
        }
      }
      if (count($companypeople) > 0) {
        $companypeople['personID'] = $personID;
        $companypeople['companyID'] = $companyID;
        if($type == 'Update') {
        $sql = $db->prepare($itype, 'companypeople', $companypeople," where personID = '".$personID."'");
        $debug->line('companypeopleSQL: '.$sql,2);
        if (!$test) {
          $db->query($sql, 'updateCompanyPeople');
        }
          logDiffs('companypeople', $companypeople, $oldCompanyPeople, $reportConfig, $sn, $gn, $ssn, $test);
        } else {
          $sql = $db->prepare($itype, 'companypeople', $companypeople);
        $debug->line('companypeopleSQL: '.$sql,2);
        if (!$test) {
          $db->query($sql, 'updateCompanyPeople');
        }
        
        }
      }
      if (count($policyinfo) > 0) {
        $policyinfo['personID'] = $personID;
        $policyinfo['companyID'] = $companyID;
        $sql = $db->prepare('INSERT', 'policyinfo', $policyinfo);
        $debug->line('policyinfoSQL: '.$sql,2);
        if (!$test) {
          $db->query($sql, 'updatePolicyInfo');
        }
         $sqlT = "insert into `%s` set reportID = '%s', sheetID = '%s', uniqueID = '%s', gn = '%s', sn='%s', `type`='%s', `field`='%s', `from`='%s', `to`='%s'";
         $sheetDB->query(sprintf($sqlT, ($test)?'importtestreport':'importreport', $reportConfig['reportID'], $reportConfig['sheet1'], $ssn, $gn, $sn, 'Added policy', 'policyinfo', '', ''));
      }
      if (count($comphistory) > 0) {
        foreach ($comphistory as $compArray) {
        $compArray['personID'] = $personID;
        $compArray['companyID'] = $companyID;
        $sql = $db->prepare('INSERT', 'comphistory', $compArray);
        $debug->line('comphistorySQL: '.$sql,2);
        if (!$test) {
          $db->query($sql, 'comphistoryInfo');
        }
         $sqlT = "insert into `%s` set reportID = '%s', sheetID = '%s', uniqueID = '%s', gn = '%s', sn='%s', `type`='%s', `field`='%s', `from`='%s', `to`='%s'";
         $sheetDB->query(sprintf($sqlT, ($test)?'importtestreport':'importreport', $reportConfig['reportID'], $reportConfig['sheet1'], $ssn, $gn, $sn, 'Added Compensation', 'comphistory', '', ''));
      }
      }
    }
    
    $unlockSQL = "update jobqueue set status = 2 where created = '".$reportConfig['created']."' AND jobFunction = '".$reportConfig['jobFunction']."' AND sheet1 = '".$reportConfig['sheet1']."' AND sheet2 = '".$reportConfig['sheet2']."'";
    $sheetDB->query($unlockSQL);
    $debug->line($unlockSQL,1);
    $reportSQL = "update reports set status=2 where reportID = '".$reportConfig['reportID']."'";
    $sheetDB->query($reportSQL);
    
}


function logDiffs($table, $new, $old, $config, $sn, $gn, $ssn, $test) {
  global $sheetDB, $debug;
  $diff1 = array_keys(array_diff_assoc($new, $old));
    $diff2 = array_keys(array_diff_key($new, $old));
    $diff = array_unique(array_merge($diff1, $diff2));
    $sqlT = "insert into `%s` set reportID = '%s', sheetID = '%s', uniqueID = '%s', gn = '%s', sn='%s', `type`='%s', `field`='%s', `from`='%s', `to`='%s'";
  foreach ($diff as $col) {
    $debug->line('diff: '.sprintf($sqlT, ($test)?'importtestreport':'importreport', $config['reportID'], $config['sheet1'], $ssn, $gn, $sn, 'Update', $table.'.'.$col, $old[$col], $new[$col]),3);
    $sheetDB->query(sprintf($sqlT, ($test)?'importtestreport':'importreport', $config['reportID'], $config['sheet1'], $ssn, $gn, $sn, 'Update', $table.'.'.$col, $old[$col], $new[$col]));
  }
}

function findPeople($sheet) {
  global $db, $sheetDB, $debug;
  //Check to see if we have already done this:
  $checkSQL = 'describe `'.$sheet.'` `personID`';
  $sheetDB->query($checkSQL, 'sheetDescription');
  if ($sheetDB->Size('sheetDescription') == 0) {
    $alterSQL = 'ALTER TABLE `'.$sheet.'` ADD `personID` INT( 64 ) NOT NULL AFTER `rowID` ;';
    $debug->line($alterSQL, 3);
    $sheetDB->query($alterSQL, 'addPersonIDCol');
  }
  $myCompCol = FALSE;
  $checkSQL = 'describe `'.$sheet.'` `ssn`';
  $sheetDB->query($checkSQL, 'sheetDescription');
  if ($sheetDB->Size('sheetDescription') == 1) {
   //We have ssn!
   $cleanupSQL = 'UPDATE `'.$sheet.'`'." set ssn = REPLACE(ssn, '-', '') WHERE ssn LIKE '%-%-%';";
   $sheetDB->query($cleanupSQL);
   $myCompCol = 'ssn';
  } else {
    $checkSQL = 'describe `'.$sheet.'` `employeeID`';
    $sheetDB->query($checkSQL, 'sheetDescription');
    if ($sheetDB->Size('sheetDescription') == 1) {
      $myCompCol = 'employeeID';
    }
  }
  $debug->line('Comp: '.$myCompCol, 3);
  if ($myCompCol) {
    $iterateSQL = 'SELECT `rowID`, `'.$myCompCol.'` from `'.$sheet.'`';
    $matchSQL = 'SELECT personID from people where `'.$myCompCol.'` LIKE'." '%%%s%%'";
    $updateSQL = 'UPDATE `'.$sheet.'` set personID ='." '%s' where rowID = '%s'";
    $sheetDB->query($iterateSQL, 'iterateRows');
    $debug->line($iterateSQL, 3);
      while($row = $sheetDB->nextRecord('iterateRows')) {
        $personID = -1;
        $debug->line(sprintf($matchSQL, $row[$myCompCol]),2);
        $db->query(sprintf($matchSQL, $row[$myCompCol]), 'checkMe');
        if ($db->Size('checkMe') > 0) {
          $tempRow = $db->nextRecord('checkMe');
          $personID = $tempRow['personID'];
        } else {
          $debug->line('size: '.$db->Size('checkMe'),1);
          $debug->variable($db, 1);
        }
        $debug->line(sprintf($updateSQL, $personID, $row['rowID']),2);
        $sheetDB->query(sprintf($updateSQL, $personID, $row['rowID']), 'updatePersonID');
      }
      return TRUE;
  } else {
    return FALSE;
  }
}

function unmatchedPeople($row) {
  global $debug, $db, $sheetDB;
  $debug->line('unmatchedPeople',1);
  $debug->variable($row,1);
  $sheet = 'sheet-'.$row['sheet1'];
  if(findPeople($sheet)) {
    $reportSQL = "insert into reports set sheetID = '".$row['sheet1']."', type = 'unmatchedPeople', time='".time()."', status='2', owner='".$row['owner']."', name='Unmatched People Report'";
    $sheetDB->query($reportSQL);
    $unlockSQL = "update jobqueue set status = 2 where created = '".$row['created']."' AND jobFunction = '".$row['jobFunction']."' AND sheet1 = '".$row['sheet1']."' AND sheet2 = '".$row['sheet2']."'";
    $sheetDB->query($unlockSQL);
  } else {
  
  }
}


function findSpouses($sheet) {
  global $db, $sheetDB, $debug;
  //Check to see if we have already done this:
  $checkSQL = 'describe `'.$sheet.'` `spouseID`';
  $sheetDB->query($checkSQL, 'sheetDescription');
  if ($sheetDB->Size('sheetDescription') == 0) {
    $alterSQL = 'ALTER TABLE `'.$sheet.'` ADD `spouseID` INT( 64 ) NOT NULL AFTER `rowID` ;';
    $debug->line($alterSQL, 3);
    $sheetDB->query($alterSQL, 'addPersonIDCol');
  }
  $myCompCol = FALSE;
  $checkSQL = 'describe `'.$sheet.'` `ssn`';
  $sheetDB->query($checkSQL, 'sheetDescription');
  if ($sheetDB->Size('sheetDescription') == 1) {
   //We have ssn!
   $cleanupSQL = 'UPDATE `'.$sheet.'`'." set ssn = REPLACE(ssn, '-', '') WHERE ssn LIKE '%-%-%';";
   $sheetDB->query($cleanupSQL);
   $myCompCol = 'ssn';
  } else {
    $checkSQL = 'describe `'.$sheet.'` `employeeID`';
    $sheetDB->query($checkSQL, 'sheetDescription');
    if ($sheetDB->Size('sheetDescription') == 1) {
      $myCompCol = 'employeeID';
    }
  }
  $debug->line('Comp: '.$myCompCol, 3);
  if ($myCompCol) {
    $iterateSQL = 'SELECT `rowID`, `'.$myCompCol.'` from `'.$sheet.'`';
    $matchSQL = 'SELECT spouseID from spouse where `'.$myCompCol.'` LIKE'." '%%%s%%'";
    $updateSQL = 'UPDATE `'.$sheet.'` set spouseID ='." '%s' where rowID = '%s'";
    $sheetDB->query($iterateSQL, 'iterateRows');
    $debug->line($iterateSQL, 3);
      while($row = $sheetDB->nextRecord('iterateRows')) {
        $personID = -1;
        $debug->line(sprintf($matchSQL, $row[$myCompCol]));
        $db->query(sprintf($matchSQL, $row[$myCompCol]), 'checkMe');
        if ($db->Size('checkMe') > 0) {
          $tempRow = $db->nextRecord('checkMe');
          $personID = $tempRow['spouseID'];
        } else {
          $debug->line('size: '.$db->Size('checkMe'),1);
          $debug->variable($db, 1);
        }
        $debug->line(sprintf($updateSQL, $personID, $row['rowID']),2);
        $sheetDB->query(sprintf($updateSQL, $personID, $row['rowID']), 'updatePersonID');
      }
      return TRUE;
  } else {
    return FALSE;
  }
}

function unmatchedSpouses($row) {
  global $debug, $db, $sheetDB;
  $debug->line('unmatchedSpouses',1);
  $debug->variable($row,1);
  $sheet = 'sheet-'.$row['sheet1'];
  if(findSpouses($sheet)) {
    $reportSQL = "insert into reports set sheetID = '".$row['sheet1']."', type = 'unmatchedSpouses', time='".time()."', status='2', owner='".$row['owner']."', name='Unmatched Spouse Report'";
    $sheetDB->query($reportSQL);
    $unlockSQL = "update jobqueue set status = 2 where created = '".$row['created']."' AND jobFunction = '".$row['jobFunction']."' AND sheet1 = '".$row['sheet1']."' AND sheet2 = '".$row['sheet2']."'";
    $sheetDB->query($unlockSQL);
  } else {
  
  }
}

function mergeTwo($reportConfig) {
  global $db, $sheetDB, $debug;
  $debug->line('Compare Two',1);
  $debug->variable($row,1);
  $errors = FALSE;
  $sheet1 = 'sheet-'.$reportConfig['sheet1'];
  $sheet2 = 'sheet-'.$reportConfig['sheet2'];
  $jobConfig = unserialize($reportConfig['jobConfig']);
  if ($jobConfig['primary'] == 'sheet1') {
    $primary = $sheet1;
    $secondary = $sheet2;
    $primaryID = $reportConfig['sheet1'];
    $secondaryID = $reportConfig['sheet2'];
  } else {
    $primary = $sheet2;
    $primaryID = $reportConfig['sheet2'];
    $secondaryID = $reportConfig['sheet1'];
    $secondary = $sheet1;
  }
  $sql1 = 'describe `'.$primary.'`';
    $sql2 = 'describe `'.$secondary.'`';
    $sheetDB = createSheetDB();
    $sheetDB->query($sql1);
    $table1Desc = $sheetDB->allRecords();
    $sheetDB->query($sql2);
    $table2Desc = $sheetDB->allRecords();
    $table1Fields = array();
    $table2Fields = array();
    foreach ($table1Desc as $row) {
        $table1Fields[] = $row['Field'];
    }
    foreach ($table2Desc as $row) {
        $table2Fields[] = $row['Field'];
    }
      $checkFields = array_intersect($table1Fields, $table2Fields);
    $debug->line('Comparing columns:',1);
    $debug->variable($checkFields);
    
    $uniqueCol = FALSE;
    if (in_array('ssn', $checkFields)) {
      $uniqueCol = 'ssn';
    } else if (in_array('employeeID', $checkFields)) {
      $uniqueCol = 'employeeID';
    } else {
      $errors[] = "No unique identifier found.";
    }
    if ($uniqueCol) {
      #Zeroth we find out our sheet names.
      $sheet1SQL = "select * from wstoc where sheetID = '".$primaryID."'";
      $sheet2SQL = "select * from wstoc where sheetID = '".$secondaryID."'";
      $sheetDB->query($sheet1SQL);
      $sheet1Info = $sheetDB->nextRecord();
      $sheetDB->query($sheet2SQL);
      $sheet2Info = $sheetDB->nextRecord();
      $sheet1Name = $sheet1Info['sheetName'];
      $sheet2Name = $sheet2Info['sheetName'];
      #First, we find out our new sheet ID by creating a sheet record:
      $sheetSQL = "insert into sheets set type = 'merged', time='".time()."', status = 0, owner = '".$reportConfig['owner']."', filename='".$sheet1Name." and ".$sheet2Name."'";
      $sheetDB->query($sheetSQL, 'createSheetMerge');
      $mergeID = $sheetDB->insertID();
      $createTable = 'CREATE TABLE `sheet-'.$mergeID.'` SELECT * from `'.$primary.'`';
      $sheetDB->query($createTable, 'CreateMergeTable');
      
      //Great. We've created the table and populated it with folks from primary. Now we need to add the extra columns from the secondary table.
      $extraColumns = array_diff($table2Fields, $table1Fields);
      if (count($extraColumns) > 0) {
      $alterSQL = 'ALTER TABLE `sheet-'.$mergeID.'`';
      $updateSQL1 = 'UPDATE `sheet-'.$mergeID.'` as t1, `'.$secondary.'` as t2 SET t1.`'.$uniqueCol.'` = t2.`'.$uniqueCol.'`, ';
      $updateSQL2 = 'INSERT INTO `sheet-'.$mergeID.'` (';
      $updateSQL3 = '';
      foreach ($table2Desc as $row) {
        if (in_array($row['Field'], $extraColumns)) {
          $alterSQL .= ' ADD `'.$row['Field'].'` '.$row['Type'].' NULL, ';
          $updateSQL1 .= 't1.`'.$row['Field'].'` = t2.`'.$row['Field'].'`, ';
          }
          $updateSQL2 .= '`'.$row['Field'].'`, ';
          $updateSQL3 .= 't1.`'.$row['Field'].'`, ';
        }
        $alterSQL = substr($alterSQL, 0, -2);
        $sheetDB->query($alterSQL, 'AlterMergeTable');
        $debug->line('AlterSQL: '.$alterSQL, 2);
        
        //Now we update everyone that we already inserted.
        $updateSQL = substr($updateSQL1, 0, -2);
        $updateSQL .= 'where t1.`'.$uniqueCol.'` = t2.`'.$uniqueCol.'`';
        $debug->line('UpdateSQL: '.$updateSQL,2);
        $sheetDB->query($updateSQL);
      }
      
      // Now we add everyone from the secondary table who wasn't in the primary table. This is another really fugly sql.
      $updateSQL = substr($updateSQL2, 0, -2);
      $updateSQL .= ') SELECT ';
      $updateSQL .= substr($updateSQL3, 0, -2);
      $updateSQL .= ' FROM `'.$secondary.'` as t1 LEFT JOIN `'.$primary.'` as t2 USING (`'.$uniqueCol.'`) where t2.`'.$uniqueCol.'` IS NULL';
      $debug->line('Hairy UpdateSQL: '.$updateSQL, 2);
      $sheetDB->query($updateSQL);
      
      //Drop the duplicates column...
      if (in_array('duplicates', $table1Fields) || in_array('duplicates', $table2Fields)) {
        $dropSQL = ' ALTER TABLE `sheet-'.$mergeID.'` DROP `duplicates`  ';
        $sheetDB->query($dropSQL);
      }
      
      //Now we update the sheet table, the jobqueue table and the wstoc table. And the we'll be done!
      $unlockSQL = "update jobqueue set status = 2 where created = '".$reportConfig['created']."' AND jobFunction = '".$reportConfig['jobFunction']."' AND sheet1 = '".$reportConfig['sheet1']."' AND sheet2 = '".$reportConfig['sheet2']."'";
    $sheetDB->query($unlockSQL);
    $debug->line($unlockSQL,1);
    $updateSheet = "update sheets set status = 2 where sheetID = '$mergeID'";
    $sheetDB->query($updateSheet);
    $updateWsToc1 = "insert into wstoc set wsID = '".$jobConfig['workspace1']."', sheetID = '$mergeID', sheetName = 'merged:".$sheet1Name." and ".$sheet2Name."', state='2'";
    $sheetDB->query($updateWsToc1);
    if ($jobConfig['workspace1'] != $jobConfig['workspace2']) {
    $updateWsToc2 = "insert into wstoc set wsID = '".$jobConfig['workspace2']."', sheetID = '$mergeID', sheetName = 'merged:".$sheet1Name." and ".$sheet2Name."', state='2'";
    $sheetDB->query($updateWsToc2);
    }  
      
      //And now our missing data report:
      $sheetDB->query("insert into reports set sheetID = '".$mergeID."', status=2, type='missingData', name='Missing Data', owner='".$reportConfig['owner']."', time='".time()."';", "report");
      
      //And our duplicates report:
      #Duplicate checking... If we have no unique column, we can't check for duplicates
        $alterSQLD = "ALTER TABLE `sheet-".$mergeID."` ADD `duplicates` INT( 64 ) NOT NULL DEFAULT '0' AFTER `rowID` ;";
        
        $sheetDB->query($alterSQLD);
        $debug->line($alterSQLD, 2);
        $duplicateSQL = "UPDATE `sheet-".$mergeID."` LEFT JOIN (
                            SELECT `".$uniqueCol."`, count( `".$uniqueCol."` ) AS total
                            FROM `sheet-".$mergeID."`
                            GROUP BY `".$uniqueCol."`
                            HAVING total >1
                            ) AS dupTot
                        USING ( `".$uniqueCol."` )
                        SET duplicates = total;";
                        #Wow, that's gnarly.
        $sheetDB->query($duplicateSQL);
        $debug->line($duplicateSQL, 2);
        //$returnValue['sql'][] = $duplicateSQL;
        $addDupeReport = "insert into reports set sheetID = '".$mergeID."', type = 'duplicateRows', time = '".time()."', status=2, owner='".$reportConfig['owner']."', name='Duplicate Entries'";
        $sheetDB->query($addDupeReport);
        $debug->line($addDupeReport, 2);
       // $returnValue['sql'][] = $addDupeReport;
    
      
    }
 }

function compareTwo($reportConfig) {
  global $debug, $db, $sheetDB;
  
  $debug->line('Compare Two',1);
  $debug->variable($row,1);
  $errors = FALSE;
  $sheet1 = 'sheet-'.$reportConfig['sheet1'];
  $sheet2 = 'sheet-'.$reportConfig['sheet2'];
  $sql1 = 'describe `'.$sheet1.'`';
    $sql2 = 'describe `'.$sheet2.'`';
    $sheetDB = createSheetDB();
    $sheetDB->query($sql1);
    $table1Desc = $sheetDB->allRecords();
    $sheetDB->query($sql2);
    $table2Desc = $sheetDB->allRecords();
    $table1Fields = array();
    $table2Fields = array();
    foreach ($table1Desc as $row) {
        $table1Fields[] = $row['Field'];
    }
    foreach ($table2Desc as $row) {
        $table2Fields[] = $row['Field'];
    }
    $checkFields = array_intersect($table1Fields, $table2Fields);
    $debug->line('Comparing columns:',1);
    $debug->variable($checkFields);
    
    $uniqueCol = FALSE;
    if (in_array('ssn', $checkFields)) {
      $uniqueCol = 'ssn';
    } else if (in_array('employeeID', $checkFields)) {
      $uniqueCol = 'employeeID';
    } else {
      $errors[] = "No unique identifier found.";
    }
    if ($uniqueCol) {
      $sqlT = 'select s1.`%s` as value1, s2.`%s` as value2, s1.`%s` as uniqueID, s1.rowID as rowID1, s2.rowID as rowID2 from `%s` as s1, `%s` as s2 where s1.%s = s2.%s AND s1.%s != s2.%s group by rowID1, rowID2';
      $sqlTi = "insert into sheetdiffs set `sheet1` = '%s', `sheet2` = '%s', `column` = '%s', `uniqueID` = '%s', `rowID1` = '%s', `value1` = '%s', `rowID2` = '%s', `value2` = '%s';";
      //Fugly.
      foreach ($checkFields as $col) {
	if ($col != 'rowID') {
	$fuglySQL = sprintf($sqlT, $col, $col, $uniqueCol, $sheet1, $sheet2, $uniqueCol, $uniqueCol, $col, $col);
	$debug->line('Fugly SQL: '.$fuglySQL, 1);
	$sheetDB->query($fuglySQL, 'checkFields');
	if ($sheetDB->Size('checkFields') > 0) {
	  while ($row1 = $sheetDB->nextRecord('checkFields')) {
	    $insertSQL = sprintf($sqlTi, $reportConfig['sheet1'], $reportConfig['sheet2'], $col, $row1['uniqueID'], $row1['rowID1'], $row1['value1'], $row1['rowID2'], $row1['value2']);
	    $debug->line($insertSQL,1);
	    $sheetDB->query($insertSQL, 'insertReportRow');
	  }
	}
      }
      }
    $reportSQL1 = "insert into reports set sheetID = '".$reportConfig['sheet1']."', type = 'compareTwo', time='".time()."', status='2', owner='".$reportConfig['owner']."', name='Sheet Comparison', config='".$reportConfig['sheet1']."-".$reportConfig['sheet2']."'";
    $sheetDB->query($reportSQL1);
    $debug->line($reportSQL1,1);
    $reportSQL2 = "insert into reports set sheetID = '".$reportConfig['sheet2']."', type = 'compareTwo', time='".time()."', status='2', owner='".$reportConfig['owner']."', name='Sheet Comparison', config='".$reportConfig['sheet1']."-".$reportConfig['sheet2']."'";
    $sheetDB->query($reportSQL2);
    $debug->line($reportSQL2,1);
    $unlockSQL = "update jobqueue set status = 2 where created = '".$reportConfig['created']."' AND jobFunction = '".$reportConfig['jobFunction']."' AND sheet1 = '".$reportConfig['sheet1']."' AND sheet2 = '".$reportConfig['sheet2']."'";
    $sheetDB->query($unlockSQL);
    $debug->line($unlockSQL,1);
    } else {
    $unlockSQL = "update jobqueue set status = -1 where created = '".$reportConfig['created']."' AND jobFunction = '".$reportConfig['jobFunction']."' AND sheet1 = '".$reportConfig['sheet1']."' AND sheet2 = '".$reportConfig['sheet2']."'";
    $sheetDB->query($unlockSQL);
    
    }
  
}

function getXLSDate($date, $dateType) {
  global $debug;
#Dates are special. Note also, gotta have PHP 5.1.0 to have dates before 1970.
      if (preg_match("/\//",$date)) {
	$date = explode("/",$date);
	if ($date[2] < 1900) { $date[2] += 1900; };
      $utime = mktime(1,1,1,$date[0],$date[1],$date[2]);
      $debug->line('slashdate', 1);
      } elseif (preg_match("/\-/",$date)) {
	$date = explode("-",$date);
	if ($date[2] < 1900) { $date[2] += 1900; };
      $utime = mktime(1,1,1,$date[0],$date[1],$date[2]);
      $debug->line('dashdate', 1);
      } elseif (preg_match("/\./",$date)) {

	$date = explode('.',$date);
	if ($date[0] < 13) {
	  if ($date[2] < 1900) { $date[2] += 1900; };
	  $utime = mktime(1,1,1,$date[0],$date[1],$date[2]);
	  $debug->line('dotdate', 1);
	} else {
	  if ($dateType == "zakkis2") {
	  // Stupid zakkis.ca uses days since 1/1/1900. Okay. We can do that. Convert days to seconds:
	  $seconds = $date * 86400;
	  // Utimes are measured since 1/1/1970, so we have to adjust by -220896000, which is the utime of 1/1/1900
	  $utime = $seconds - 2082988800;
	  $debug->line('stupid zakkis2 date:'.$date." ".$seconds." ".$utime, 1);
	  } elseif ($dateType == "rawdate") {

	    $utime = $date;

	  } elseif ($dateType == "rawText") {
	    $utime = $date;
	  } else {
	  // Stupid zakkis.ca uses days since 1/1/1900. Okay. We can do that. Convert days to seconds:
	  $seconds = $date * 86400;
	  // Utimes are measured since 1/1/1970, so we have to adjust by -220896000, which is the utime of 1/1/1900
	  $utime = $seconds - 2208960000;
	  $debug->line('stupid zakkis1 date:'.$date." ".$seconds." ".$utime, 1);

	  }
	}
	
      
      } elseif ($date == 0) {
	$utime = NULL;
      } else {
	//      $date = $xl->getDateArray($date);
	//	if ($date['year'] < 1900) { $date['year'] += 1900; };
	//$utime = mktime(1,1,1,$date['month'],$date['day'],$date['year']);
	// Stupid zakkis.ca uses days since 1/1/1900. Okay. We can do that. Convert days to seconds:
	$seconds = $date * 86400;
	// Utimes are measured since 1/1/1970, so we have to adjust by -2208985200, which is the utime of 1/1/1900.
	$utime = $seconds - 2209107600;
      $debug->line('stupid zakkis date:'.$date." ".$seconds." ".$utime, 1);
      }
      return $utime;
  }

exit;

##Individual sheets functions go here.
## Each is expected to take an argument that is the jobqueue data array


?>