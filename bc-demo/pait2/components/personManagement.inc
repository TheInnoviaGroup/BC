<?PHP
$personTemplate = new template('personView.ihtml');
$altTemplates = array('comphistory' => 'personCompHistory.ihtml',
		      						'policyinfo' => 'personPolicyView.ihtml',
                      'editpolicy' => 'policyEdit.ihtml',
		      						'editperson' => 'personEdit.ihtml',
                      'details' => 'personDetails.ihtml',
                      'spouse' => 'spouseDetails.ihtml');
global $body, $form, $self, $javascripts, $styles, $login;

if ($form->module) {
    $personTemplate = new template($altTemplates[$form->module]);
}

$personTemplate->set("self", $self);
switch ($form->action) {
 case "editPerson":
   addPerson($personTemplate);
   break;
 case "addPerson":
   addPerson($personTemplate);
   break;
 case "editPolicy":
   addPolicy($personTemplate);
   break;
 case "addPolicy":
   addPolicy($personTemplate);
   break;
 case "showPerson":
   showPerson($personTemplate);
   break;
 case "deletePolicy":
   deletePolicy($personTemplate);
   break;
 case "delete":
   deletePerson($personTemplate);
   break;
 case "deleteSpouse":
   deleteSpouse($personTemplate);
   break;
 default:
 	print "Bad Action: ".$form->action;
}



function deletePerson($template) {
  global $db, $form, $body;
  $id = $form->personID;
  $personInfoSQL1 = "select * from people where personID = '$id'";
  $db->query($personInfoSQL1, 'check');
  $row = $db->nextRecord('check', MYSQL_ASSOC);
  audit('people', $row, array(), 'delete');
  $sql = "delete from people where personID = '$id'";
  $sql2 = "delete from companypeople where personID = '$id'";
  $sql3 = "delete from comphistory where personID = '$id'";
  $sql4 = "delete from log where personID = '$id'";
  $db->query($sql);
  $db->query($sql2);
  $db->query($sql3);
  $db->query($sql4);
  $template->set('action', 'delete');
  

  $body = $template->parse();

}

function deleteSpouse($template) {
  global $db, $form, $body;
  $id = $form->spouseID;
  $personInfoSQL1 = "select * from spouse where spouseID = '$id'";
  $db->query($personInfoSQL1, 'check');
  $row = $db->nextRecord('check', MYSQL_ASSOC);
  $personID = $row['personID'];
  $personInfoSQL2 = "select * from people where personID = '$personID'";
  $db->query($personInfoSQL2, 'check2');
  $from = $db->nextRecord('check2', MYSQL_ASSOC);
  $to = $from;
  $to['spouseID'] = 0;
  
  audit('spouse', $row, array(), 'delete');
  audit('people', $from, $to, 'edit');
  
  $sql = "delete from spouse where spouseID = '$id'";
  $sql2 = "update people set spouseID = 0 where personID = '$personID'";
  $db->query($sql);
  $db->query($sql2);
  $template->set('action', 'delete');
  print "{ success: true }";
  exit;

}

function deletePolicy($template) {
  global $form, $db;
  $checkSQL = "select * from policyinfo where uid = '".$form->uid."'";
  $db->query($checkSQL, 'check');
  $db->query($sql, 'check');
  $row = $db->nextRecord('check', MYSQL_ASSOC);
  audit('policyinfo', $row, array(), 'delete');
  $sql = "delete from policyinfo where uid = '".$form->uid."'";
  $db->query($sql);
  //showPerson($template, $form->personID);
  print json_encode(array('success'=>TRUE));
  exit;
}

function showPerson($template, $personID = NULL) {
  global $db, $form, $body, $login, $debug;
  $allowed = NULL;
  if ($login->user->access_lv > 1 && $login->user->access_lv != 5) {
    $allowed = TRUE;
  }
  $personID = ($personID)?$personID:$form->personID;
  $template->set('action', 'showPerson');
  # Now we have to get info from all over.
  if($form->module == "spouse") {
  	$sql = "select * from spouse where personID = '".$personID."'";
  	$db->query($sql, 'personRecord');
  	if ($personData = $db->nextRecord('personRecord')) {
    	$template->set('personID', $personID);
    	$template->set('personData', $personData);
    	$template->set('allowed', $allowed);
    	$login->template->set('isSubpage', 'yes');
    	$template->set('loginbox', $login->template->parse());
    	$body = $template->parse();
  	}
  } else {
  
  $sql = "select * from people where personID = '".$personID."'";
  $db->query($sql, 'personRecord');
  if ($personData = $db->nextRecord('personRecord')) {
    $companyID = $personData['companyID'];
    if ($companyID > 0) {
      $sql = "select p.employeeID, p.hiredate as hiredate, p.title as title, p.status as status, c.name as name, c.address as address, c.owner as owner, p.department as department from companies as c, companypeople as p where c.companyID = '".$companyID."' and p.personID = '".$personID."' and c.companyID = p.companyID";
      //      print $sql;
      $debug->line($sql,1);
      $db->query($sql, 'companyRecord');
      
      $companyData = $db->nextRecord('companyRecord');
      if ($login->user->access_lv != 5) {
      $sql2 = "select * from allowedcompanies where user_idnum = '".$login->user->user_idnum."' AND companyID = '".$companyID."'";
      $db->query($sql2, 'isAllowed');
      if ($db->Size('isAllowed') > 0) {
	$allowed = TRUE;
      }
      }
    } else {
      $companyData = array ('companyID' => 0,
			    'name' => "None");
    }
    if ($allowed && $login->user->access_lv > 2) {
      audit('people', array(), array(), 'view', 'user', array('username' => $login->user->username, 'personID' => $personID));
    }

    $compensationHistoryTemp = array();
    $log = array();
    $sql = "SELECT h.personID AS personID, h.type AS typeID, h.year AS year, h.amount AS amount, h.companyID AS companyID, h.updated as updated, c.name AS name, t.name AS
TYPE , t.compTypeID
FROM comphistory AS h, companies AS c, comptypes AS t
WHERE h.personID = '".$personID."'
AND h.companyID = c.companyID
AND h.type = t.name
ORDER BY h.year, h.type, c.name DESC";
    // print ($sql);
    $db->query($sql, 'compHistoryRecords');
    global $debug;
    $debug->line($sql, 1);
    while ($row = $db->nextRecord('compHistoryRecords')) {
      $compensationHistoryTemp[] = $row;
    }
    $sql = "SELECT l.time as time, l.notes as notes, l.category as category, u.gn as gn, u.sn as sn, u.email as email from log as l, users as u where l.id = '".$personID."' and l.type = 'person' and l.username = u.username order by l.time DESC";
    $db->query($sql, "logRecords");
    while ($row = $db->nextRecord('logRecords')) {
      $log[] = $row;
    }
    $sql = "show tables like 'census-%-".$companyID."-%'";
    $db->query($sql, "censusList");

    if ($db->Size("censusList") > 0) {
    $extraData = array();
      while ($res = $db->nextRecord('censusList')) {
	foreach ($res as $blah => $table) {
	  if (!preg_match('/^\d+$/', $blah)){
    $censusInfo = explode('-', $table);
    $sql3 = "select time from rawdatatoc where type = 'census' and importID = '".$censusInfo[3]."'";
    $db->query($sql3, 'censusInfo');
    $res = $db->nextRecord('censusInfo');
    $time = $res['time'];
    $title = "Census imported ".date("F j, Y, g:i a", $time);

	  $sql = "select * from `$table` where personID = '$personID'";
	  $db->query($sql, "censusData");
	  $sql2 = "show columns from `$table`";
	  $db->query($sql2, "tableInfo");
	  // print "SQL1: $sql<br />$sql2<br />";
	    $temp = array();
	    $temp2 = array();
	    while ($res3 = $db->nextRecord("tableInfo")) {
	      $temp2[] = $res3['Field'];
	      }

	    while ($res2 = $db->nextRecord("censusData")) {
	      $temp['data'][] = $res2;
	      $temp['info'] = $temp2;
	      $temp['title'] = $title;
	    }
	    $extraData[] = $temp;
	}
	      }
      }
    } else {
      $extraData = NULL;
    }


    $sql = "show tables like 'bid-%-".$companyID."-%'";
    $db->query($sql, "bidList");

    if ($db->Size("bidList") > 0) {
    $bidData = array();
      while ($res = $db->nextRecord('bidList')) {
	foreach ($res as $blah => $table) {
	  if (!preg_match('/^\d+$/', $blah)){
    $bidInfo = explode('-', $table);
    $sql3 = "select time from rawdatatoc where type = 'bid' and importID = '".$bidInfo[3]."'";
    $db->query($sql3, 'bidInfo');
    $res = $db->nextRecord('bidInfo');
    $time = $res['time'];
    $sql4 = "select name from carriers where carrierID = '".$bidInfo[1]."'";
    $db->query($sql4, 'bidInfo');
    $res = $db->nextRecord('bidInfo');
    $bidName = $res['name'];
    $title = "Bid from $bidName (imported ".date("F j, Y, g:i a", $time).")";

	  $sql = "select * from `$table` where personID = '$personID'";
	  $db->query($sql, "bidData");
	  $sql2 = "show columns from `$table`";
	  $db->query($sql2, "tableInfo");
	  // print "SQL1: $sql<br />$sql2<br />";
	    $temp = array();
	    $temp2 = array();
	    while ($res3 = $db->nextRecord("tableInfo")) {
	      $temp2[] = $res3['Field'];
	      }

	    while ($res2 = $db->nextRecord("bidData")) {
	      $temp['data'][] = $res2;
	      $temp['info'] = $temp2;
	      $temp['title'] = $title;
	    }
	    $bidData[] = $temp;
	}
	      }
      }
    } else {
      $bidData = NULL;
    }


$sql = "show tables like 'ratefile-%-".$companyID."-%'";
    $db->query($sql, "ratefileList");

    if ($db->Size("ratefileList") > 0) {
      $debug->line("Ratefiles exist.", 2);
    $ratefileData = array();
      while ($res = $db->nextRecord('ratefileList')) {
	foreach ($res as $blah => $table) {
	  if (!preg_match('/^\d+$/', $blah)){
    $ratefileInfo = explode('-', $table);
    $sql3 = "select time, importID from rawdatatoc where type = 'ratefile' and importID = '".$ratefileInfo[3]."'";
    $db->query($sql3, 'ratefileInfo');
    $debug->line($sql3, 2);
    $res = $db->nextRecord('ratefileInfo');
    $time = $res['time'];
    $policyUID = $ratefileInfo[3];
    $sql4 = "select name from carriers where carrierID = '".$ratefileInfo[1]."'";
    $db->query($sql4, 'ratefileInfo');
    $res = $db->nextRecord('ratefileInfo');
    $ratefileName = $res['name'];
    $title = "Additional Policy Info from $ratefileName";

	  $sql = "select * from `$table` where personID = '$personID'";
	  $debug->line($sql, 2);
	  $db->query($sql, "ratefileData");
	  $sql2 = "show columns from `$table`";
	  $debug->line($sql2, 2);
	  $db->query($sql2, "tableInfo");
	  // print "SQL1: $sql<br />$sql2<br />";
	    $temp = array();
	    $temp2 = array();
	    while ($res3 = $db->nextRecord("tableInfo")) {
		$temp2[] = $res3['Field'];
	    }
	    while ($res2 = $db->nextRecord("ratefileData")) {
	      $temp['data'][] = $res2;
	      $temp['info'] = $temp2;
	      $temp['title'] = $title;
	    }
	    $ratefileData[$policyUID] = $temp;
	}
	      }
      }
    } else {
      $ratefileData = NULL;
    }
    global $debug;
    $beneBreak = array();
    $sql11 = "select p.*, c.name as carrierName from policyinfo as p, carriers as c where personID = '$personID' AND c.carrierID = p.carrierID ORDER BY effectiveDate DESC";
    $debug->line("SQL11: $sql11",1 );
    $db->query($sql11, 'policyinfo');
    $policyinfo = array();
    while ($res11 = $db->nextRecord('policyinfo')) {
      $policyinfo[] = $res11;
      $paidBy = NULL;
      $myType = NULL;
      if ($res11['policyStatus'] == 0) {
	$paidBy = ($res11['paidBy'] == 1)?"Employer":"Employee";
	$myType = (preg_match('/[A-Za-z]+/', $res11['policyType']))?$res11['policyType']:"Supplemental Disability";
	$temp = $beneBreak['type'][$myType.'-'.$paidBy];
	//	$beneBreak['type'][$myType] = array( 'benefit' => $res11['benefitAmount']
	$temp['benefit'] += $res11['benefitAmount'];
	$temp['premium'] += $res11['premium'];
	$beneBreak['type'][$myType.'-'.$paidBy] = $temp;

       }
    }
    
    $sql12 = "select sum(benefitAmount) as totalBenefit from policyinfo where personID = '$personID' AND policyNumber IS NOT NULL AND (policyStatus = '0' OR policyStatus ='3' OR policyStatus = '4')";
    $db->query($sql12, 'policytotals');
    $res = $db->nextRecord('policytotals');
    $template->set('totalBenefit', $res['totalBenefit']);
    $sql14 = "select sum(premium) as totalPremium from policyinfo where personID = '$personID' AND policyNumber IS NOT NULL AND (policyStatus = '0' OR policyStatus = '4')";
    $db->query($sql14, 'policytotals');
    $res = $db->nextRecord('policytotals');

    $template->set('totalPremium', $res['totalPremium']);
    $template->set('beneBreak', $beneBreak);


    $compensationHistory = sussOutHistory($compensationHistoryTemp);
$debug->variable($ratefileData, 5);

// check for Spouse
if($personData['spouseID'] > 0) {
	$template->set('hasSpouse', TRUE);
} else {
	$template->set('hasSpouse', FALSE);
}

global $policyStatuses;
$template->set('policyStatuses', $policyStatuses);
    $template->set('allowed', $allowed);
    $template->set('extraData', $extraData);
    $template->set('bidData', $bidData);
    $template->set('personData', $personData);
    $template->set('companyData', $companyData);
    $template->set('compensationData', $compensationHistory);
    $template->set('policyinfo', $policyinfo);
    $template->set('ratefile', $ratefileData);
    $template->set('logData', $log);
    $template->set('personID', $personID);
    $login->template->set('isSubpage', 'yes');
    $template->set('loginbox', $login->template->parse());
    $body = $template->parse();
  }
 }
}

function sussOutHistory($array) {
  $retval = array();
  $totals = array();
  $tests = array();
  global $debug;
  $debug->line('thingy!',1);
  $debug->line($array[0],1);
  foreach ($array as $thingy) {
	  $debug->line("1 TypeID: ".$thingy['typeID']." amount: ".$thingy['amount']." year: ".$thingy['year']." updated: ".$thingy['updated'],1);
    $retval[] = $thingy;
    if ($totals[$thingy['year']]) { //if we have something for the year.
      if ($tests[$thingy['year'].$thingy['typeID']] > 1) { // if we've got the typeID already.
   if ($tests[$thingy['year']]['typeID'] < $thingy['updated']){ // We've got the most current.
	  $totals[$thingy['year']]['amount'] = $totals[$thingy['year']]['amount'] - $tests[$thingy['year'].$thingy['typeID']];
	  $totals[$thingy['year']]['amount'] = $totals[$thingy['year']]['amount'] + $thingy['amount'];
	  $debug->line(" Total: ".$totals[$thingy['year']]['amount'],1);;
	  $tests[$thingy['year'].$thingy['typeID']] = $thingy['amount']; 
	}
      } else {
	
	$totals[$thingy['year']]['amount'] = $totals[$thingy['year']]['amount'] + $thingy['amount'];
	  $tests[$thingy['year'].$thingy['typeID']] = $thingy['amount']; 
	  $debug->line(" Total: ".$totals[$thingy['year']]['amount'],1);;
	}
    } else {
      $totals[$thingy['year']] = $thingy;
      $totals[$thingy['year']]['type'] = "Total ".$thingy['year']." Compensation";
      $tests[$thingy['year'].$thingy['typeID']] = $thingy['amount']; #This is to catch duplicate entries. I serously doubt anyone will have the exact same amount of compensation twice. But hey, you never know.
						     }
  }

  $debug->variable($totals,1);
  $debug->variable($tests,1);

  foreach ($totals as $year => $rec) {
    $retval[] = $rec;
  }
  return $retval;

}


function addPerson($template) {
  global $db, $form, $debug, $body, $login, $personStatus, $self;
  $changes = array();
  
  if ($form->spouseID || ($form->isSpouse != 'FALSE' && $form->isSpouse)) {
  	if($form->spouseID == "add") {
			// someone submitted a new spouse
			$insertOrUpdate = "insert";
  		$spouseData = array();
  		$spouseData['spouseID'] = 'add';
  		$spouseData['personID'] = $form->personID;
  		$personID = $form->personID;
  	} else {
  	// we have an existing spouse
  	$spouseID = $form->spouseID;
  	$insertOrUpdate = "replace";
    $sql = "select * from spouse where spouseID = '".$spouseID."'";
    $spouseData = array();
    $db->query($sql, 'spouseRecord');
    $spouseData = $db->nextRecord('spouseRecord');
    }
    // did we submit or cancel?
    if ($form->cancel) {  showPerson($template); return ; }
    if ($form->submit) {
      $notes = "";
      $sql = "show columns from `spouse`";
      $db->query($sql, "shortcut");
      while ($sc = $db->nextRecord("shortcut")) {
	if (preg_match("/[A-Za-z]+/", $sc['Field'])) {
	$field = $sc['Field'];
	//	print "Field: $field<br>Form: ".$form->$field."<br />";
	$notes .= ($form->{$field} && ($form->{$field} != $spouseData[$field]) && !stristr($field, "date"))?"$field: ".$form->{$field}." was ".$spouseData[$field]."<br />\n":"";
	if (stristr($field, 'ssn') && $form->{$field}) {
	  $spouseData[$field] = intval(preg_replace('/\-/', '', $form->{$field}));
	} else if (stristr($field, "date") && $form->{$field}) {
		$utime = 0;
		$sep=(stristr($form->{$field}, "-"))?"-":"/";
		$cruft = explode($sep, $form->{$field});
		$utime = mktime(1,1,1,$cruft[0],$cruft[1], $cruft[2] );
		if ($utime != $spouseData[$field]) {
		  $notes .= "$field: ".gmdate(PAITDATE, $utime)." was ".gmdate(PAITDATE,$spouseData[$field])."<br />\n";
		}
		$spouseData[$field] = $utime;
		//$notes .= "1,1,1,".$cruft[0].",".$cruft[1].",".$cruft[2]." : $utime<br />\n";
	} else if ($form->{$field}) {
	  $spouseData[$field] = $form->{$field};
	}
	}
      }
      $sql = "show columns from `companypeople`";
      $db->query($sql, "shortcut");
      $sql = "select * from companypeople where personID = '".$personID."'";
      $db->query($sql, 'companypeople');
      $companyData = $db->nextRecord("companypeople");
      while ($sc = $db->nextRecord("shortcut")) {
	if ($sc['Field'] != 'status' && preg_match("/[A-Za-z]+/", $sc['Field'])) {
	$field = $sc['Field'];
	
	$notes .= ($form->{$field} && ($form->{$field} != $companyData[$field]) && !stristr($field, "date") && !stristr($field, "status"))?"$field: ".$form->{$field}." was ".$spouseData[$field]."<br />\n":"";

	if (stristr($field, "date") && $form->{$field}) {
		$utime = 0;
		$sep=(stristr($form->{$field}, "-"))?"-":"/";
		$cruft = explode($sep, $form->{$field});
		$utime = mktime(1,1,1,$cruft[0],$cruft[1], $cruft[2] );
		if ($utime != $companyData[$field]) {
		  $notes .= "$field: ".gmdate(PAITDATE, $utime)." was ".gmdate(PAITDATE,$companyData[$field])."<br />\n";
		}
		$companyData[$field] = $utime;
		
	} else if (!stristr($field,'status')) {
	$companyData[$field] = ($form->{$field})?$form->{$field}:$companyData[$field];
	}
	}
      }


      $sql = $db->prepare($insertOrUpdate, 'spouse', $spouseData);
      $db->query($sql);
      if($insertOrUpdate == 'insert') {
      	// make the update to the people table to add the spouse
      	$newSpouse = $db->insertID();
      	$sql = "UPDATE people SET spouseID = ".$newSpouse." WHERE personID = ".$spouseData['personID'];
      	$db->query($sql);
      }
      
      // print $sql;
      $debug->line($sql, 1);
      $changes['spouse']['to'] = $spouseData;

      unset($companyData['status']);
      $companyChanges = $companyData;
      unset($companyChanges['name']);
      unset($companyChanges['address']);
      unset($companyChanges['owner']);
      $changes['companypeople']['to'] = $companyChanges;

      $sql = $db->prepare($insertOrUpdate, 'companypeople', $companyData);
      $db->query($sql);
      $debug->line($sql, 1);
      
      $debug->line('logging', 2);
      $debug->variable($changes, 2);
      auditArray($changes);
      print "{success:true}";
      exit;
      //return;

    } // end of submit or cancel for spouse
    	
    
    
  
  } elseif ($form->personID || $form->companyID) {
    $personID = $form->personID;
    $sql = "select * from people where personID = '".$personID."'";
    $personData = array();
    $db->query($sql, 'personRecord');
    $personData = $db->nextRecord('personRecord');
    $changes['people'] = array('action' => 'edit',
			       'from' => $personData);
    $debug->line('persondata', 1);
    $debug->variable($personData,2);
    if ($personData['companyID'] > 0) {
      $insertOrUpdate = "replace";
      $changes['companypeople']['action'] = 'edit';
      $companyID = $personData['companyID'];
      $sql = "select p.employeeID, p.hiredate as hiredate, p.title as title, p.department as department, p.companyID as companyID, c.name as name, c.address as address, c.owner as owner from companies as c, companypeople as p where c.companyID = '".$companyID."' and p.personID = '".$personID."' and c.companyID = p.companyID";
    } else {
      $insertOrUpdate = "insert";
      $changes['people'] = array('action' => 'add',
				 'from' => array());

      $changes['companypeople']['action'] = 'add';
      $personData = array();
      $debug->line('companyID from form',1);
      $companyID = $form->companyID;
      $sql = "select c.name as name, c.address as address, c.owner as owner, c.companyID as companyID from companies as c where c.companyID = '".$companyID."'";
    }
    if ($companyID > 0) {
     $db->query($sql, 'companyRecord');
     $debug->line($sql,1);
     $companyData = $db->nextRecord('companyRecord');
     $companyData['personID'] = $personID;
      $companyChanges = $companyData;
      unset($companyChanges['name']);
      unset($companyChanges['address']);
      unset($companyChanges['owner']);
      $changes['companypeople']['from'] = $companyChanges;
     if ($companyData['owner'] == $login->user->username) {
       $allowed = TRUE;
     }
     $debug->variable($companyData, 1);
    } else {
     $companyData = array ('companyID' => 0,
			   'name' => "None");
    }
    
    if ($form->cancel) {  showPerson($template); return ; }
    if ($form->submit) {
      $notes = "";
      $sql = "show columns from `people`";
      $db->query($sql, "shortcut");
      while ($sc = $db->nextRecord("shortcut")) {
	if (preg_match("/[A-Za-z]+/", $sc['Field'])) {
	$field = $sc['Field'];
	//	print "Field: $field<br>Form: ".$form->$field."<br />";
	$notes .= ($form->{$field} && ($form->{$field} != $personData[$field]) && !stristr($field, "date"))?"$field: ".$form->{$field}." was ".$personData[$field]."<br />\n":"";
	if (stristr($field, 'ssn') && $form->{$field}) {
	  $personData[$field] = intval(preg_replace('/\-/', '', $form->{$field}));
	} else if (stristr($field, "date") && $form->{$field}) {
		$utime = 0;
		$sep=(stristr($form->{$field}, "-"))?"-":"/";
		$cruft = explode($sep, $form->{$field});
		$utime = mktime(1,1,1,$cruft[0],$cruft[1], $cruft[2] );
		if ($utime != $personData[$field]) {
		  $notes .= "$field: ".gmdate(PAITDATE, $utime)." was ".gmdate(PAITDATE,$personData[$field])."<br />\n";
		}
		$personData[$field] = $utime;
		//$notes .= "1,1,1,".$cruft[0].",".$cruft[1].",".$cruft[2]." : $utime<br />\n";
	} else if ($form->{$field}) {
	  $personData[$field] = $form->{$field};
	}
	}
      }
      $sql = "show columns from `companypeople`";
      $db->query($sql, "shortcut");
      $sql = "select * from companypeople where personID = '".$personID."'";
      $db->query($sql, 'companypeople');
      $companyData = $db->nextRecord("companypeople");
      while ($sc = $db->nextRecord("shortcut")) {
	if ($sc['Field'] != 'status' && preg_match("/[A-Za-z]+/", $sc['Field'])) {
	$field = $sc['Field'];
	//	print "Field: $field<br>Form: ".$form->$field."<br />";
	$notes .= ($form->{$field} && ($form->{$field} != $companyData[$field]) && !stristr($field, "date") && !stristr($field, "status"))?"$field: ".$form->{$field}." was ".$personData[$field]."<br />\n":"";

	if (stristr($field, "date") && $form->{$field}) {
		$utime = 0;
		$sep=(stristr($form->{$field}, "-"))?"-":"/";
		$cruft = explode($sep, $form->{$field});
		$utime = mktime(1,1,1,$cruft[0],$cruft[1], $cruft[2] );
		if ($utime != $companyData[$field]) {
		  $notes .= "$field: ".gmdate(PAITDATE, $utime)." was ".gmdate(PAITDATE,$companyData[$field])."<br />\n";
		}
		$companyData[$field] = $utime;
		
		//		$notes .= "1,1,1,".$cruft[0].",".$cruft[1].",".$cruft[2]." : $utime<br />\n";
	} else if (!stristr($field,'status')) {
	$companyData[$field] = ($form->{$field})?$form->{$field}:$companyData[$field];
	}
	}
      }


      $sql = $db->prepare($insertOrUpdate, 'people', $personData);
      $db->query($sql);
      $debug->line($sql, 1);
      if ($personID < 1) {
	$personID = $db->insertID();
	$companyData['personID'] = $personID;
	$personData['personID'] = $personID;
      }
      $changes['people']['to'] = $personData;

      unset($companyData['status']);
      $companyChanges = $companyData;
      unset($companyChanges['name']);
      unset($companyChanges['address']);
      unset($companyChanges['owner']);
      $changes['companypeople']['to'] = $companyChanges;

      $sql = $db->prepare($insertOrUpdate, 'companypeople', $companyData);
      $db->query($sql);
      $debug->line($sql, 1);
      //$logSQL = "insert into log set username='".$login->user->username."', type='person', time='".time()."', id='".$personID."', category='System', notes='Edited Person Data:<br />$notes'";
      //$db->query($logSQL);
      $debug->line('logging', 2);
      $debug->variable($changes, 2);
      auditArray($changes);
      //showPerson($template, $personID);
      print "{success:true}";
      exit;
      //return;

    }


  }
  
  $sql = "select name, companyID from companies";
  $db->query($sql);
  $array = array();
  while ($res = $db->nextRecord()) {
    $array[$res['companyID']] = $res['name'];
  }
  global $debug;
  $debug->line("companyID: $companyID",1);
  $template->set('companyID', $companyID);
  $debug->variable($companyData, 1);
  $template->set('statuses', $personStatus);
  $template->set('self', $self);
  $template->set('companyList', $array);
  
  $template->set('action', 'editPerson');
  if($form->spouseID) {
  	$template->set('personData', $spouseData);
  	$template->set('isSpouse', TRUE);
  	$template->set('spouseID', $form->spouseID);
  } else {
  	$template->set('personData', $personData);
  	$template->set('isSpouse', FALSE);
  	$template->set('spouseID', 'add');
  }
  
  $template->set('companyData', $companyData);
  $body = $template->parse();
  

  
}


function addPolicy($template) {
  global $db, $form, $body, $login, $personStatus, $self, $debug;
  if ($form->uid) {
  	// it's an edit
  	$debug->line('Got uid', 1);
  	if ($form->cancel) {  print "{success: true}\n"; exit; }
    if ($form->submit) { 
    	$debug->line('Policy Submitted', 1);
    	$policyData["uid"] = $form->uid;
    	$policyData["personID"] = $form->personID;
	$policyData["policyNumber"] = $form->policyNumber;
	$policyData["policyForm"] = $form->policyForm;
	$policyData["benefitAmount"] = $form->benefitAmount;
	$policyData["premium"] = $form->premium;
	$policyData["premiumTerm"] = $form->premiumTerm;
	$policyData["eliminationPeriod"] = $form->eliminationPeriod;
	$policyData["benefitPeriod"] = $form->benefitPeriod;
	$policyData["riskNumber"] = $form->riskNumber;
	$policyData["class"] = $form->class;
	$policyData["stateAppSigned"] = $form->stateAppSigned;
	$policyData["amendment"] = $form->amendment;
	$policyData["tobaccoUser"] = $form->tobaccoUser;
	$policyData["catastrophic"] = $form->catastrophic;
	$policyData["discount"] = $form->discount;
	$policyData["mnda"] = $form->mnda;
	$policyData["carrierID"] = $form->carrier;
	$policyData["policyStatus"] = $form->policyStatus;
	$policyData["policyType"] = $form->policyType;
	$policyData["paidBy"] = $form->paidBy;
   	$policyData["recovery"] = $form->recovery;
   	$policyData["wib"] = $form->wib;
   	$policyData["groupLTD"] = $form->groupLTD;
   	$policyData["extendedPartial"] = $form->extendedPartial;
   	$policyData["lc"] = $form->lc;
   	$policyData["fio"] = $form->fio;
		$utime = 0;
		$sep=(stristr($form->effectiveDate, "-"))?"-":"/";
		$cruft = explode($sep, $form->effectiveDate);
		$utime = mktime(1,1,1,$cruft[0],$cruft[1], $cruft[2] );
		$policyData['effectiveDate'] = $utime;
		if($form->reconsiderationDate != '') {
		  $utime = 0;
		  $sep=(stristr($form->reconsiderationDate, "-"))?"-":"/";
		  $cruft = explode($sep, $form->reconsiderationDate);
		  $utime = mktime(1,1,1,$cruft[0],$cruft[1], $cruft[2]);
		  $policyData['reconsiderationDate'] = $utime;
		} 
		$notes = "";
		$notes .= ($form->{$field} && $form->{$field} != $policyData[$field])?"$field: ".$form->{$field}."<br />\n":"";
		$logSQL = "insert into log set username='".$login->user->username."', type='person', time='".time()."', id='".$form->personID."', category='System', notes='Edited Policy Data.'";
		$db->query($logSQL);
		if ($policyData['uid'] > 0) {
		  $checkSQL = "select * from policyinfo where uid = '".$policyData['uid']."'";
		  $db->query($checkSQL, 'check');
		  $oldData = $db->nextRecord('check');
		  audit('policyinfo', $oldData, $policyData, 'edit');
		} else {
		  audit('policyinfo', array('personID' => $policyData["personID"]), $policyData, 'add');
		}
		
		$sql = $db->prepare('replace', 'policyinfo', $policyData);
		$debug->line('Query:'.$sql, 1);
		$db->query($sql);
                print "{\"success\": true }";
                exit;
		//showPerson($template);
		//$template->set('action', 'showPerson');
		//return;
		
    } else {
      $sql = "select carrierID, name from carriers where carrierID > 0";
      $db->query($sql, 'carrierList');
      $debug->line($sql, 1);
      while ($res = $db->nextRecord('carrierList')) {
	$carriers[$res['name']] = $res['carrierID'];
      }
    
      $template->set('carriers', $carriers);
    	// it's an edit form (not submitted)
    	$template->set('action', 'editPolicy');
		$uid = $form->uid;
		$sql = "select * from policyinfo where uid = '".$uid."'";
		$debug->line('Got here', 1);
	   $db->query($sql, 'policyRecord');
		if ($policyData = $db->nextRecord('policyRecord')) {
			  $template->set('policyData', $policyData);
		}
    }
   		$template->set('policyLegend', "Edit");
    
    } else {
$sql = "select carrierID, name from carriers where carrierID > 0";
      $db->query($sql, 'carrierList');
      $debug->line($sql, 1);
      while ($res = $db->nextRecord('carrierList')) {
	$carriers[$res['name']] = $res['carrierID'];
      }
    
      $template->set('carriers', $carriers);
    	
    	// it's an add
    	$template->set('action', 'editPolicy');
    	$policyData["uid"] = 0;
    	$template->set('policyData', $policyData);
    	$template->set('policyLegend', "Add");
    	if ($form->submit) { 
    	$debug->line('Policy Submitted', 1);
    	$policyData["uid"] = $form->uid;
    	$policyData["personID"] = $form->personID;
		$policyData["policyNumber"] = $form->policyNumber;
		$policyData["effectiveDate"] = $form->effectiveDate;
		$policyData["policyForm"] = $form->policyForm;
		$policyData["benefitAmount"] = $form->benefitAmount;
		$policyData["premium"] = $form->premium;
		$policyData["premiumTerm"] = $form->premiumTerm;
		$policyData["eliminationPeriod"] = $form->eliminationPeriod;
		$policyData["benefitPeriod"] = $form->benefitPeriod;
		$policyData["riskNumber"] = $form->riskNumber;
		$policyData["class"] = $form->class;
		$policyData["stateAppSigned"] = $form->stateAppSigned;
		$policyData["amendment"] = $form->amendment;
		$policyData["reconsiderationDate"] = $form->reconsiderationDate;
		$policyData["tobaccoUser"] = $form->tobaccoUser;
		$policyData["catastrophic"] = $form->catastrophic;
		$policyData["discount"] = $form->discount;
   	$policyData["mnda"] = $form->mnda;
   	$policyData["recovery"] = $form->recovery;
   	$policyData["wib"] = $form->wib;
   	$policyData["groupLTD"] = $form->groupLTD;
   	$policyData["extendedPartial"] = $form->extendedPartial;
   	$policyData["lc"] = $form->lc;
   	$policyData["fio"] = $form->fio;
		$policyData["policyStatus"] = $form->policyStatus;
		$policyData["policyType"] = $form->policyType;
		$policyData["paidBy"] = $form->paidBy;
 		$policyData["carrierID"] = $form->carrier;
			$utime = 0;
		$sep=(stristr($form->effectiveDate, "-"))?"-":"/";
		$cruft = explode($sep, $form->effectiveDate);
		$utime = mktime(1,1,1,$cruft[0],$cruft[1], $cruft[2] );
		$policyData['effectiveDate'] = $utime;
		
		if($form->reconsiderationDate != '') {
			$utime = 0;
			$sep=(stristr($form->reconsiderationDate, "-"))?"-":"/";
			$cruft = explode($sep, $form->reconsiderationDate);
			$utime = mktime(1,1,1,$cruft[0],$cruft[1], $cruft[2] );
			$policyData['reconsiderationDate'] = $utime;
		}
    	
    	//$notes = "";
    	//$notes .= ($form->{$field} && $form->{$field} != $policyData[$field])?"$field: ".$form->{$field}."<br />\n":"";
    	// $logSQL = "insert into log set username='".$login->user->username."', type='person', time='".time()."', id='".$form->personID."', category='System', notes='Added Person' Policy Data:<br />$notes'";
      	//$db->query($logSQL);
      	
      	
                
    	$sql = $db->prepare('insert', 'policyinfo', $policyData);
    	$debug->line('Query:'.$sql, 1);
      	$db->query($sql);
        $returnArray['success'] = TRUE;
        if ($policyData['uid'] > 0) {
            $returnArray['error'][] = 'uid > 0';
		  $checkSQL = "select * from policyinfo where uid = '".$policyData['uid']."'";
		  $db->query($checkSQL, 'check');
		  $oldData = $db->nextRecord('check');
		  audit('policyinfo', $oldData, $policyData, 'edit');
		} else {
                    $returnArray['error'][] = 'Added policy';
		  $returnArray['sql'] = audit('policyinfo', array('personID' => $policyData["personID"]), $policyData, 'add');
		}
        
         print json_encode($returnArray);
                exit;
      	//showPerson($template);
      	//$template->set('action', 'showPerson');
      	//return;
      
    }
    }
  $personID = $form->personID;
  $sql = "select * from people where personID = '".$personID."'";
  $db->query($sql, 'personRecord');
  $personData = $db->nextRecord('personRecord');
  $template->set('statuses', $personStatus);
  $template->set('self', $self);
  $template->set('companyList', $array);
  
  $template->set('personData', $personData);
  $template->set('companyData', $companyData);
  $body = $template->parse();

  }

?>