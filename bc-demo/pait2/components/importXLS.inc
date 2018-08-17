<?
if (include_once('excelparser.php')) {
global $debug;
$debug->line('included excelparser.php', 1);
}
include_once('exceldate.php');
//global $headerMap;

global $updateOnly;
$updateOnly = FALSE;

#If you add something here, make sure it's below in step 5.
$headerMap = array("Leave As Is" => 0,
                   "Ignore Column" => 2,
		   "Include in Compensation History" => 1,
		   "Full Name" => "cn",
		   "First Name" => "gn",
                   "Middle Name" => "mn",
                   "First and Middle Names" => 3,
                   "Last Name" => "sn",
		   "Last Name, Suffix" => 4,
		   "Gender" => "gender",
		   "Birth Date" => "birthdate",
		   "Social Security Number" => "ssn",
		   "City" => "city",
		   "State" => "state",
		   "Postal Code" => "postal",
		   "Address" => "address",
                   "Address 2" => "address2",
		   "Work Phone" => "workphone",
		   "Cell Phone" => "cellphone",
		   "Home Phone" => "homephone",
		   "Employee ID" => "employeeID",
		   "Job Title" => "title",
		   "Hire Date" => "hiredate",
                   "Department/Division" => 'department',
                   "Status" => "status",
                   "E-mail" => "email",
                   "Policy Information" => 6
		   );
$starter = array("Leave As Is" => 0,
                   "Ignore Column" => 2);
$compTypes = array_merge($starter, getCompensationTypes());

$compensationMap = $compTypes;

$policyMap = array("Leave As Is" => 0,
                   "Ignore Column" => 2,
"Effective Date" => "effectiveDate",
"Policy Form" => "policyForm",
"Policy Number" => "policyNumber",
"Benefit Amount" => "benefitAmount",
"Premium" => "premium",
"Premium Term" => "premiumTerm",
"Elimination Period" => "eliminationPeriod",
"Benefit Period" => "benefitPeriod",
"Risk Number" => "riskNumber",
"Class" => "class",
"State App Signed" => "stateAppSigned",
"Amendment Info" => "amendment",
"Reconsideration Date" => "reconsiderationDate",
"Tobacco Use" => 5,
"Catastrophic" => "catastrophic",
"Discount" => "discount",
"MNDA" => "mnda",
"COLA" => "cola");


switch ($form->action) {
case "dataWiz":
  dataWiz();
  break;
case "pending":
  pending();
  break;
case "deletepending":
  deletepending();
  break;
}

function pending() {
  global $db, $form, $body, $debug;
$id = $form->uploadID;
$sql = "select filename from pendinguploads where uploadID = '$id'";
$db->query($sql);
$res = $db->nextRecord();
dataWiz($res['filename']);

}

function deletepending() {
  global $db, $form, $body, $debug;
$id = $form->uploadID;
$sql = "delete from pendinguploads where uploadID = '$id'";
$db->query($sql);
include_once('welcomeScreen.inc');
}


function dataWiz($file = NULL) {
  global $db, $form, $body, $debug;

  $wizTemp = new template('importXLS.good.ihtml');
//  $wizTemp = new template('importXLS.ihtml');
  $wizTemp->set('company', $form->company);
  $wizTemp->set('carrier', $form->carrier);
$wizTemp->set('policyStatus', $form->policyStatus);
$wizTemp->set('paidBy', $form->paidBy);
$wizTemp->set('policyType', $form->policyType);
if ($form->updateOnly) {
   $updateOnly = $form->updateOnly;
}
$wizTemp->set('updateOnly', ($updateOnly)?1:0);
if ($file) {
$filename = $file;
} else {
  $filename = $form->file;
}
  $step = ($form->step)?$form->step+1:0;
$debug->line('Inside dataWiz()', 1);
  // Step 0, which company and or carrier.
  if ($step == 0) {
    $debug->line('Step is 0, setting template step to 1', 1);
    $wizTemp->set('step', 1);
       $companies = array();
    $carriers = array('None, Company Census Only' => 0);
    $sql = "select companyID, name from companies";
$debug->line($sql, 1);
    $db->query($sql, 'companyList');
    while ($res = $db->nextRecord('companyList')) {
      $companies[$res['name']] = $res['companyID'];
    }
 
    $sql = "select carrierID, name from carriers where carrierID > 0";
    $db->query($sql, 'carrierList');
$debug->line($sql, 1);
    while ($res = $db->nextRecord('carrierList')) {
      $carriers[$res['name']] = $res['carrierID'];
    }
    global $policyStatuses;
$wizTemp->set('policyStats', $policyStatuses);
global $policyTypes;
$wizTemp->set('policyTypes', $policyTypes);

       $wizTemp->set('companies', $companies);
       $wizTemp->set('carriers', $carriers);
  $debug->variable($wizTemp, 1);
 
  }

$debug->line('Checking to see if step '.$step.' is > 0', 1);
  if ($step > 0) { 
$debug->line('yes', 1);
    list($xl, $error) = getExcel($filename);
    if ($error) {
$debug->line('error!: '.$error,1);
      $wizTemp->set('error', $error);
      $body .= $wizTemp->parse();
      return;
    }
    // No errors. Right. What step are we on?
  }

$debug->line('Checking to see if step '.$step.' is == 2', 1);

  // Step 2 is to identify which sheet to use, if there are more than one sheet.
  if ($step == 2) {
	$ws_number = count($xl->worksheet['name']);
	if ($ws_number < 1) {
	  $wizTemp->set('error', "No worksheets in the spreadsheet. Please check the original and try again.");
	  $body .= $wizTemp->parse();
	  return;
	} elseif ($ws_number > 1) {
	  $wizTemp->set('step', 2);
	  $wizTemp->set('xl', $xl);
	  
	} else {
	  # Exactly one, skip to next step.
	  $step = 3;
	}
  }
  // There is no step 1. Step 2 lets us identify which row is the row where data starts.
$debug->line('Checking to see if step '.$step.' is == 3', 1);

  if ($step == 3) {
    $sheetnum = ($form->worksheet)?$form->worksheet:0;
    $sheet = $xl->worksheet['data'][$sheetnum];
    $wizTemp->set('step', $step);
    $wizTemp->set('xl', $xl);
    $wizTemp->set('ws', $sheet);
    $wizTemp->set('sheetnum', $sheetnum);
  }

  #finally we actually do a little work here. Now what we do is take the header row, and try to suss out what it actually means.
$debug->line('Checking to see if step '.$step.' is == 4', 1);

  if ($step == 4) {
global $personStatus, $policyMap, $compensationMap;
    $sheetnum = $form->sheetnum;
    $sheet = $xl->worksheet['data'][$sheetnum];
    $headerRow = getData($xl, $sheetnum, $form->headerrow);
    $exampleRow = getData($xl, $sheetnum, $form->headerrow + 1);
    $threefer = mapThis($headerRow);
$mapped = $threefer[0];
$sub1mapped = $threefer[1];
$sub2mapped = $threefer[2];
    global $headerMap;
$wizTemp->set('personStatus', $personStatus);
    $wizTemp->set('exampleRow', $exampleRow);
    $wizTemp->set('headerMap', $headerMap);
$wizTemp->set('sub2HeaderMap', $policyMap);
$wizTemp->set('sub1HeaderMap', $compensationMap);
 //   $debug->variable($policyMap, 1);

    $wizTemp->set('headerRow', $headerRow);
    $wizTemp->set('headerRowNum', $form->headerrow);
    $wizTemp->set('mapped', $mapped);
    $wizTemp->set('sub1mapped', $sub1mapped);
    $wizTemp->set('sub2mapped', $sub2mapped);
    $wizTemp->set('step', $step);
    $wizTemp->set('xl', $xl);
    $wizTemp->set('ws', $sheet);
    $wizTemp->set('sheetnum', $sheetnum);

  }
  $debug->line('Checking to see if step '.$step.' is == 5', 1);

  if ($step == 5) {
$log = "";
#Woo hoo! Now we get to put this stuff in the db.
# There are five, yes five, places we store this info. Ugh. Here we go!
    $sheetnum = $form->sheetnum;
    $sheet = $xl->worksheet['data'][$sheetnum];
    $headerRow = getData($xl, $sheetnum, $form->headerrow);
    $dataRow = $form->headerrow + 1;
$defaultStatus = $form->defaultStatus;
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
for ($z = $dataRow; $z <= $sheet['max_row']; $z++) {
  $data = getData($xl, $sheetnum, $z); 
  $datacount = count($data);
  $headercount = count($headerRow);
  $countdiff = $headercount - $datacount;
  if ($countdiff >= 0 && $countdiff <= 2) { #A match! Or close enough.
# My arrays are named for tables: people, companypeople, comphistory, policyinfo, rawdatatoc and rawdata.
					     $people = array('companyID' => $form->company,
							     );
  $companypeople = array('companyID' => $form->company);
  $comphistory = array();
  $rawdatatoc = array('companyID' => $form->company,
		      'carrierID' => $form->carrier,
		      'type' => ($form->carrier)?"bid":"census",
		      'time' => $now);  
  $dothis = array();
  global $policyTypes;
  $policyinfo = array('policyStatus' => $form->policyStatus,
		      'paidBy' => $form->paidBy,
		      'policyType' => $policyTypes[$form->policyType]);
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
      $debug->line('dest: '.$dest);
          $debug->variable($headerSub1Action);
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
	if ($date[2] < 1900) { $date[2] += 1900; };
      $utime = mktime(1,1,1,$date[0],$date[1],$date[2]);
      $debug->line('dotdate', 1);
      } else {
      $date = $xl->getDateArray($data[$count]);
	if ($date['year'] < 1900) { $date['year'] += 1900; };
      $utime = mktime(1,1,1,$date['month'],$date['day'],$date['year']);
      $debug->line('arraydate', 1);
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
    } elseif (preg_match('/(policyNumber|policyForm|benefitAmount|premium|premiumTerm|eliminationPeriod|benefitPeriod|riskNumber|class|stateAppSigned|catastrophic|discount|mnda|cola)/', $dest)) {
      $policyinfo[$dest] = $data[$count];
      $dothis['policyinfo'] = 1;
    } elseif ($dest == "ssn") {
      $people[$dest] = preg_replace('/\-/', '', $data[$count]);
    } elseif ($dest == "gender") {
      $people[$dest] = (preg_match('/f/i', $data[$count]))?1:2;
    } elseif (preg_match('/(gn|sn|mn|cn|city|state|postal|address|workphone|homephone|cellphone|status)/', $dest)) {
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
  $personID = findPerson($data, $headerRow, $headerAction, $form->company);
  if ($personID > 0) {
#Repeat:
    $itype = "replace";
    $people['personID'] = $personID;
    $companypeople['personID'] = $personID;
    $rawdata['personID'] = $personID;
    $policyinfo['personID'] = $personID;
    $sql = "select * from people where personID = '$personID'";
    $db->query($sql);
    global $updateOnly;
    $row = $db->nextRecord();
    if ($updateOnly) {
      $mystatus = $row['status'];
    }
    foreach ($people as $key => $val) {
      if (!$updateOnly || $key != "status"){
	if ($val != $row[$key]) {
	$log .= "$key was ".$row[$key]." and was changed to ".$val."<br />\n";
	$row[$key] = $val;
	}
      }
    }
    

    if ($updateOnly) {
      $row['status']=$mystatus;
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
    if (! $people['status']) { $people['status'] = $defaultStatus; }
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
}    
if (count($comphistory) > 0) {
    foreach($comphistory as $title => $array) {
      $array = array('personID' => $personID,
		     'companyID' => $form->company,
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
    $sql4 = $db->prepare($itype, 'rawdatatoc', $rawdatatoc);
    $db->query($sql4);
     $debug->line("5: ".$sql4, 1);
   $tableID = $db->insertID();
   $importID = $tableID;
    $table = $rawdatatoc['type']."-".$rawdatatoc['carrierID']."-".$rawdatatoc['companyID']."-".$tableID;
    }
    #Now. Yes. We create our own table. Ugh.
	$tbl_SQL .= "CREATE TABLE IF NOT EXISTS `$table` ( `iuid` int(32) NOT NULL auto_increment,";
	$fieldname = array();
	foreach ($rawdata as $fc => $val) {
			// Prepare table structure
			$fieldname[$fc] = preg_replace ( "/[^a-zA-Z0-9]/", "", $fc );
			$fieldname[$fc] = preg_replace ("/(\s)+/", "", $fieldname[$fc]);
			//			$fieldname[$fc] = ereg_replace ( "^[0-9]+", "", $fc );
			
			$tbl_SQL .= $fieldname[$fc] . " text NOT NULL,";
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
  $policyinfo['carrierID'] = ($form->carrier)?$form->carrier:9;
  $policyinfo['importID'] = $importID;
  $rawdata['policyNumber'] = $policyinfo['policyNumber'];
  $sql5 = $db->prepare('INSERT', 'policyinfo', $policyinfo);
  $db->query($sql5);
  $uid = $db->insertID();
    $debug->line("6: ".$sql5, 1);
  }
  global $login;
    $array = array('username' => $login->user->username,
		 'type' => 'person',
		 'id' => $personID,
		 'notes' => $log,
		 'category' => "System Import",
		 'time' => time());
  $sql = $db->prepare('insert', 'log', $array);
  $db->query($sql);

  
  }
    $wizTemp->set('exampleRow', $exampleRow);
    $wizTemp->set('headerMap', $headerMap);
    $wizTemp->set('sub2HeaderMap', $policyMap);
    $wizTemp->set('sub1HeaderMap', $compensationMap);
    $wizTemp->set('headerRow', $headerRow);
    $wizTemp->set('headerRowNum', $form->headerrow);
    $wizTemp->set('mapped', $mapped);
    $wizTemp->set('step', $step);
    $wizTemp->set('xl', $xl);
    $wizTemp->set('ws', $sheet);
    $wizTemp->set('sheetnum', $sheetnum);

  $wizTemp->set('message', "Import Complete.<br />Added $dudes records.");
}

$debug->line('End Step 5', 1);
}


  

$debug->line('Parsing wizTemp', 1);

    $wizTemp->set('form', $form);
    $body .= $wizTemp->parse();
    return;
    
}

function findPerson($data, $headerRow, $headerAction, $companyID) {
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
$ssnnodash = preg_replace('/-/', '', $ssn);
if ($ssn == $ssnnodash) {
  $sql = "select personID from people where ssn = '$ssn'";
} else {
 $sql = "select personID from people where ssn = '$ssn' OR ssn = '$ssnnodash'";
}
} elseif (in_array('employeeID')) {
$header = array_search('employeeID', $headerAction);
$cell = array_search($header, $headerRow);
$id = $data[$cell];
$sql = "select personID from companypeople where companyID = '$companyID' and employeeID = '$id'";
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
	       '#state#i' => "State",
	       '#^st$#i' => "State",
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
    '#effectivedate#i' => 'map2',
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
    '#premiumterm#i' => "Premium Term",
    '#premium#i' => "Premium",
    '#policy#i' => "Policy Number",
    '#effectivedate#i' => "Effective Date",
    '#benefit#i' => "Benefit Amount",
    '#smoker#i' => "Tobacco Use",
    '#discount#i' => "Discount",
    '#mnda#i' => "MNDA",
		 '#cola#i' => "COLA",
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


function getExcel($filename) {
  global $uploadPath;
  $result = NULL;
  $file = $uploadPath . "/" . $filename;
  $fh = @fopen ($file,'rb');
    if( !$fh ) $result = "Couldn't open filehandle for: $file";
  if( filesize($file)==0 ) $result="No file uploaded: $file";
  
  $fc = fread( $fh, filesize($file) );
  @fclose($fh);
    if( strlen($fc) < filesize($file) )
    $result = "Cannot read file: $file";	
  // Check excel file
  $exc = new ExcelFileParser;
  // We could use ParseFromFile and avoid the rigamarole above, but
  // string is faster and has more robust return codes.
  if (!$result) {
  $res = $exc->ParseFromString($fc);
  
  switch ($res) {
  case 0: break;
  case 1: $result="Can't open file";
  case 2: $result="File too small to be an Excel file";
  case 3: $result="Error reading file header";
  case 4: $result="Error reading file";
  case 5: $result="This is not an Excel file or file stored in Excel < 5.0";
  case 6: $result="File corrupted";
  case 7: $result="No Excel data found in file";
  case 8: $result="Unsupported file version";
  default:
    $result="Unknown error";
  }
  }
  return array($exc,$result);
}

function getData($xl, $sheetnum, $row, $col = NULL) {
$retval = array();
$ws = $xl->worksheet['data'][$sheetnum];
$max_col = $ws['max_col'];
$i = $row;
if (!$col) {
  if(isset($ws['cell'][$row]) && is_array($ws['cell'][$row]) ) {
		   for( $j=0; $j<=$max_col; $j++ ) {
			if( ( is_array($ws['cell'][$i]) ) &&
			    ( isset($ws['cell'][$i][$j]) )
			   ){

			 // print cell data
			 $data = $ws['cell'][$i][$j];
		   switch ($data['type']) {
			// string
			case 0:
				$ind = $data['data'];
				if( $xl->sst['unicode'][$ind] ) {
					$s = uc2html($xl->sst['data'][$ind]);
				} else
					$s = $xl->sst['data'][$ind];
				$retval[] = $s;
				break;
			// integer number
			case 1:
                                $retval[] = $data['data'];
				break;
			// float number
			case 2:
				$retval[] = $data['data'];
				break;
			// date
			case 3:
				$ret = $data[data];//str_replace ( " 00:00:00", "", gmdate("d-m-Y H:i:s",$xl->xls2tstamp($data[data])) );
				$retval[] = $ret ;
				break;
			case 4: //string
                                $retval[] = $data['data'];
			break;
			case 5: //hlink	
			$str = "<a href=\"" . uc2html($data['hlink']) ."\">".uc2html($data['data'])."</a>";
                        $retval[] = $str;
			break;
				
			default:
                                $retval[] = $data['data'];
				break;
		


}} else {
$retval[] = "&nbsp;";
}
}}} else {
$j = $col;
			if( ( is_array($ws['cell'][$i]) ) &&
			    ( isset($ws['cell'][$i][$j]) )
			   ){

			 // print cell data
			 $data = $ws['cell'][$i][$j];

		   switch ($data['type']) {
			// string
			case 0:
				$ind = $data['data'];
				if( $xl->sst['unicode'][$ind] ) {
					$s = uc2html($xl->sst['data'][$ind]);
				} else
					$s = $xl->sst['data'][$ind];
				$retval[] = $s;
				break;
			// integer number
			case 1:
                                $retval[] = $data['data'];
				break;
			// float number
			case 2:
				$retval[] = $data['data'];
				break;
			// date
			case 3:
				$ret = $data[data];//str_replace ( " 00:00:00", "", gmdate("d-m-Y H:i:s",$xl->xls2tstamp($data[data])) );
				$retval[] = $ret ;
				break;
			case 4: //string
			break;
			case 5: //hlink	
			$str = "<a href=\"" . uc2html($data['hlink']) ."\">".uc2html($data['data'])."</a>";
                        $retval[] = $str;
			break;
				
			default:
				$retval[] = "";
				break;
		

}}}

return $retval;
}

?>