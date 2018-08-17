<?php
global $form, $login, $db, $debug;
$debug->level = 0;
switch($form->module) {
    case "searchPersonForm":
        searchPersonForm();
    break;
    case "undoPerson":
	undoPerson();
    break;
    case "policySummary":
        policySummary();
    break;
    case "personAcctHistory":
        personAcctHistory();
    break;
    case "tableViewer":
	tableViewer();
    break;
    case "showTableViewer":
	showTableViewer();
    break;
    case "showSheetViewer":
        showTableViewer(TRUE);
        break;
    case "reportViewer":
        reportViewer();
        break;
    case "sheetViewer":
        tableViewer(createSheetDB());
        break;
}

function showTableViewer($sheets = FALSE) {
    global $form, $self;
    $template = new template('tableViewer.ihtml');
    
    $template->set('useSheets', $sheets);        
    if ($sheets) {
        $array = split('/', $form->node);
        $table = "sheet-".$array[2];
    } else {
        $table = $form->table;
    }

    $template->set('table', $table);
    $template->set('self', $self);
    $template->set('size', 15); // we default to 50, but it's changeable.
    $template->set('prettyNames', TRUE); //Whether or not to uppercase the first letter of column names.
    $template->display();
    exit;
}


/* Pass me an array of column names and I'll view a table where those columns have missing data. */

function tableViewerColsMissingData($table, $newDB = FALSE) {
    $colsSQL = 'describe `'.$table.'`';
    if ($newDB) {
        $newDB->query($colsSQL);
        $results = $newDB->allRecords();
    } else {
        global $db;
        $db->query($colsSQL);
        $results = $newDB->allRecords();
    }
    $where = '';
    foreach($results as $res) {
        $col = $res['Field'];
        $where .= '(`'.$col.'` is NULL OR `'.$col.'` = '."'') OR ";
    }
    $where = substr($where, 0, -3);
    tableViewer($newDB, $where);
}

function reportViewer() {
    global $form, $self;
    $sheetDB = createSheetDB();
    switch($form->report) {
     case "missingData":
        tableViewerColsMissingData($form->table, $sheetDB);
        break;
    case "unmatchedPeople":
    tableViewer($sheetDB, 'personID IS NULL OR personID = -1');
    break;
    case "unmatchedSpouses":
    tableViewer($sheetDB, 'spouseID IS NULL OR spouseID = -1');
    break;
    case "duplicateRows":
        tableViewer($sheetDB, 'duplicates > 1');
        break;
    case "compareTwo":
        $array = explode('-', $form->table);
        tableViewer($sheetDB, "`sheet1`='".$array[0]."' AND `sheet2`='".$array[1]."'", 'sheetdiffs', array('sheet1','sheet2'), array('rowID1' => '#ffcccc','value1' => '#ffcccc','value2' => '#ccccff','rowID2' => '#ccccff'));
        break;
    case "import":
        $array = explode('-', $form->table);
        $nodes = explode('/', $form->node);
        tableViewer($sheetDB, "`sheetID`='".$array[1]."' AND reportID = '".$nodes[3]."'", 'importreport', array('reportID', 'sheetID'));
        break;
    case "importTest":
        $array = explode('-', $form->table);
        $nodes = explode('/', $form->node);
        tableViewer($sheetDB, "`sheetID`='".$array[1]." AND reportID = '".$nodes[3]."'", 'importtestreport', array('reportID', 'sheetID'));
        break;
    }    
}

/* To view tables in other dbs, create a new function like:
  
  function newDBTableViewer() {
  global $dbSetup;
  $newDB = new db($dbSetup);
  tableViewer($newDB);
  }
  
  tableViewer will handle the rest.
*/



function tableViewer($newDB = FALSE, $where = FALSE, $table = FALSE, $hideFields = array(), $columnColors = array()) {
    global $form, $login, $db, $debug;
    $array = array('success'=>TRUE);
    
    $noMeta = FALSE;
    $table = ($table)?$table:$form->table;
    $sql = 'describe `'.$table.'`';
    $start = ($form->start)?$form->start:0;
    $limit = ($form->limit)?$form->limit:50;
    if ($where) {
        $where = ' WHERE '.$where;
    } else {
        $where = ' ';
    }
    if ($form->filters) {
        $noMeta = TRUE;
        $where = (stristr($where, 'where'))?$where.' AND ':' WHERE ';
        $array = split(';', $form->filters);
        foreach ($array as $statement) {
         $stride = split(':', $statement);
         if ($stride[1] != "") {
         if ($stride[1] == "gt") {
            $stride[1] = ">";
         } elseif ($stride[1] == "lt") {
            $stride[1] = "<";
         } elseif ($stride[1] == "eq") {
            $stride[1] = "=";
         }
         if (strtolower($stride[1]) == "like") {
            $sep = "%";
         } else {
            $sep = '';
         }
         $where .= '`'.$stride[0].'` '.$stride[1]." '".$sep.$stride[2].$sep."' AND";
        }
        }
        $where = substr($where, 0, -4);
    }
    if ($form->sort) {
	$orderBy = ' ORDER BY `'.$form->sort.'` '.$form->dir.' ';
    } else {
	$orderBy = ' ';
    }
    if ($form->exportXLS) {
    $sql2 = 'select * from `'.$table.'` '.$where.$orderBy.';';
    $sql3 = 'select count(*) as totalCount from `'.$table.'` '.$where.$orderBy.';';
    $debug->line($sql2,1);
    $debug->line($sql3,1);
    } else {
    $sql2 = 'select * from `'.$table.'` '.$where.$orderBy.'limit '.$start.','.$limit.';';
    $sql3 = 'select count(*) as totalCount from `'.$table.'` '.$where.$orderBy.';';
    }
    if ($newDB) {
        $debug->line('newDB',1);
	$newDB->query($sql);
	$res = $newDB->allRecords();
	$newDB->query($sql2);
	$res2 = $newDB->allRecords('Default',MYSQL_ASSOC);
	$newDB->query($sql3);
	$res3 = $newDB->nextRecord();
	$totalCount = $res3['totalCount'];
    } else {
        $debug->line('old DB',1);
	$db->query($sql);
	$res = $db->allRecords();
	$db->query($sql2);
	$res2 = $db->allRecords('Default',MYSQL_ASSOC);
	$db->query($sql3);
	$res3 = $db->nextRecord();
	$totalCount = $res3['totalCount'];
    }
    if ($form->exportXLS) {
        $debug->variable($res2,2);
        $debug->variable($res,2);
        exportXLS($res2, $res);
        
    }
    
    $mysqlDataTypeMap = array('int' => 'int',
			      'varchar' => 'string',
			      'char' => 'string',
			      'bool' => 'boolean',
			      'char' => 'string',
			      'text' => 'string',
			      'float' => 'float',
			      'date' => 'date');
    $gridFilterMap = array('int' => 'numeric',
			      'varchar' => 'string',
			      'char' => 'string',
			      'bool' => 'numeric',
			      'char' => 'string',
			      'text' => 'string',
			      'float' => 'numeric',
			      'date' => 'date');
    
    $array['sql1'] = $sql;
    $array['sql2'] = $sql2;
    $array['sql3'] = $sql3;
    $count = 0;
    $id = FALSE;
    if ($form->meta && !$noMeta) {
    foreach ($res as $row) {
        if (!in_array($row['Field'], $hideFields)) {
	$array['metaData']['fields'][$count]['name'] = $row['Field'];
	if ($form->prettyNames) {
	    $array['metaData']['fields'][$count]['header'] = ucfirst($row['Field']);
	} else {
	    $array['metaData']['fields'][$count]['header'] = $row['Field'];
	}
        if (strtolower($row['Field']) == 'personid') {
            $array['metaData']['fields'][$count]['renderer'] = 'Ext.PAIT.personIDRenderer';
        }
	$array['metaData']['fields'][$count]['locked'] = FALSE;
	$array['metaData']['fields'][$count]['sortable'] = TRUE;
	$tempType = preg_replace('/\(.*\)/', '', $row['Type']);
        $array['metaData']['fields'][$count]['type'] = (array_key_exists(strtolower($tempType), $mysqlDataTypeMap))?$mysqlDataTypeMap[$tempType]:'auto';
        $array['metaData']['fields'][$count]['filter'] = (array_key_exists(strtolower($tempType), $gridFilterMap))?$gridFilterMap[$tempType]:'null';
        if ($row['Key'] == 'PRI') {
	    $id = $row['Field'];
	}
        if (count($columnColors) > 0 && in_array($row['Field'], array_keys($columnColors))) {
            $array['metaData']['fields'][$count]['css'] = "background-color: ".$columnColors[$row['Field']].";";
        }
	$count++;
    }
    }
    $array['metaData']['root'] = 'dataRows';
    $array['metaData']['totalProperty'] = 'totalCount';
    if ($id) {
	$array['metaData']['id'] = $id;
    }
    }
    $array['totalCount'] = $totalCount;
    foreach ($res2 as $row) {
	$array['dataRows'][] = $row;
    }
    print json_encode($array);
    exit;
}

function undoPerson() {
    global $form, $login, $db, $debug;
    
    $uniqueID = $form->uniqueID;
    $temp = explode('-', $uniqueID);
    $personID = $temp[0];
    $username = $temp[1];
    $time = $temp[2];
    $field = $temp[3];
    $result = array();
    if ($field == "") {
	$array['success'] = FALSE;
	$array['errorText'] = 'Unable to undo that record.';
	print json_encode($array);
	exit;
    }
     $allowed = NULL;
    if ($login->user->access_lv > 1 && $login->user->access_lv != 5) {
      $allowed = TRUE;
    }
    global $AGLOBAL;
    $logDB = new db($AGLOBAL['AUDITDBURL']);
    #First we verify our field, our from and our to:
    $verifySQL = "select * from personaccounthistory where personID = '$personID' AND time = '$time' AND field = '$field' AND username = '$username'";
    $logDB->query($verifySQL);
    if ($res = $logDB->nextRecord()) {
	#This is a bit backward. But we can work with it. the "to" is what we're changing from and the "from" is what we're changing to.
	# So first we check that the current pait entry has the same information we're trying to undo.
	$temp = explode('.', $res['field']);
	$table = $temp[0];
	$column = $temp[1];
	#Update this for policyinfo to include policy uid.
	$checkSQL = "select $column, companyID from $table where personID = '$personID'";
	$result['checkSQL'] = $checkSQL;
	$db->query($checkSQL);
	$row = $db->nextRecord();
	if (!$allowed || !(isAllowedCompany($login->user, $row['companyID']) && $login->user->access_lv != 5)) {
	    $array['success'] = FALSE;
	$array['errorText'] = 'You do not have permission to undo that record.';
	print json_encode($array);
	exit;
	}
	if ($row[$column] == $res['to']) {
	    #Rock on. Let's do this thing!
	    $updateSQL = "update $table set $column = '".$res['from']."' where personID = '$personID'";
	    $result['updateSQL'] = $updateSQL;
	    $db->query($updateSQL);
	    if ($db->affectedRows() == 1) {
		audit($table, array('personID' => $personID, $column => $res['to']), array($column => $res['from']), 'undo'); #Yep. We re-log.
		$result['success'] = TRUE;
	    } else {
		$result['success'] = FALSE;
		$result['errorText'] = "There has been an unexpected database error: ".$db->getError();
	    }
	} else {
	    $result['success'] = FALSE;
	    $result['errorText'] = "The current value for ".ucfirst($column)." does not match the value you're trying to undo.";
	    $result['extendedError'] = "Column: $column From: '".$res['from']."' To: '".$res['to']."' Current: '".$row[$column]."'";
	}
    } else {
	$result['success'] = FALSE;
	$result['errorText'] = "Could not find the correct undo information. Please refresh the person's account history.";
    }
    print json_encode($result);
    exit;
}

function personAcctHistory() {
    global $form, $login, $db, $debug;
    $personID = $form->personID;
    $order = ($form->sort)?$form->sort.' '.$form->dir:'time desc';
    $array = array('success' => TRUE);
    global $AGLOBAL;
    $logDB = new db($AGLOBAL['AUDITDBURL']);
    $sql1 = "select count(*) as total from personaccounthistory where personID = '".$personID."'";
    $logDB->query($sql1);
    $res = $logDB->nextRecord();
    $array['totalCount'] = $res['total'];
    $sql = "select * from personaccounthistory where personID = '".$personID."' ORDER BY $order LIMIT ".$form->start.", ".$form->limit;
    $logDB->query($sql);
    while ($res = $logDB->nextRecord('Default', MYSQL_ASSOC)) {
	$res['uniqueID'] = $personID.'-'.$res['username'].'-'.$res['time'].'-'.$res['field'];
	$res['time'] = date('g:i:s m/d/y', $res['time']);
	$temp = explode('.', $res['field']);
	$res['field'] = ucfirst($temp[1]);
	$res['action'] = ucfirst($res['action']);
	$res['username'] = ucfirst($res['username']);
        $array['results'][] = $res;
    }
    print json_encode($array);
    exit;
}

function policySummary() {
    global $form, $login, $db, $debug;
    $beneBreak = array();
    $personID = $form->personID;
    $array = array('success' => TRUE);
    $sql11 = "select p.*, c.name as carrierName from policyinfo as p, carriers as c where personID = '$personID' AND c.carrierID = p.carrierID ORDER BY effectiveDate DESC";
    $array['sql1'] = $sql11;
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
        $myTerm = (preg_match('/[A-Za-z]+/', $res11['premiumTerm']))?$res11['premiumTerm']:"Unknown";
	$temp = $beneBreak['type'][$myType.'-'.$paidBy.'-'.$myTerm];
	//	$beneBreak['type'][$myType] = array( 'benefit' => $res11['benefitAmount']
	$temp['benefit'] += $res11['benefitAmount'];
	$temp['premium'] += $res11['premium'];
	$beneBreak['type'][$myType.'-'.$paidBy.'-'.$myTerm] = $temp;

       }
    }
    //$array['beneBreak'] = $beneBreak;
    
    $sql12 = "select sum(benefitAmount) as totalBenefit from policyinfo where personID = '$personID' AND policyNumber IS NOT NULL AND (policyStatus = '0' OR policyStatus ='3' OR policyStatus = '4')";
    $db->query($sql12, 'policytotals');
    $res = $db->nextRecord('policytotals');
    $array['totalBenefit'] = $res['totalBenefit'];
    $sql14 = "select sum(premium) as totalPremium from policyinfo where personID = '$personID' AND policyNumber IS NOT NULL AND (policyStatus = '0' OR policyStatus = '4')";
    $db->query($sql14, 'policytotals');
    $res = $db->nextRecord('policytotals');
    $array['totalPremium'] = $res['totalPremium'];
    foreach ($beneBreak['type'] as $types => $data) {
        $array2 = split('-', $types);
        $array['policyTypes'][] = array('type' => $array2[0],
                                        'term' => $array2[2],
                                        'paidBy' => $array2[1],
                                        'benefit' => $data['benefit'],
                                        'premium' => $data['premium']);
    }
    print json_encode($array);
    exit;
    
}

function searchPersonForm() {
global $self;
$array = array();
$array['success'] = TRUE;
$array['form']['options']['labelWidth'] = '75';
$array['form']['options']['url'] = $self;
$array['form']['options']['baseParams'] = array('action' => 'searchPerson',
						 'mode' => 'Search');

$fieldset1 = array('type' => 'FieldSet',
		  'options' => array(
				     'legend' => 'Search',
				     ));
$textbox = array(
		 'type' => 'TextField',
		 'options' => array(
				    'fieldLabel' => 'Find a Person',
				    'name' => 'searchperson',
				    'width' => 175,
				    'allowBlank' => false,
				    ));
$checkbox = array(
		 'type' => 'Checkbox',
		 'options' => array(
				    'fieldLabel' => 'Search by Name Only',
				    'name' => 'limitname',
				    'width' => 175,
				    'allowBlank' => FALSE,
				    'checked' => TRUE,
				    'cls' => 'paitCheckbox'
				    ));
$checkbox2 = array(
		 'type' => 'Checkbox',
		 'options' => array(
				    'fieldLabel' => 'Search by Policy Number Only',
				    'name' => 'searchpolicy',
				    'width' => 175,
				    'checked' => FALSE,
				    'cls' => 'paitCheckbox'
				    ));
 $fieldset1['children'] = array($textbox, $checkbox, $checkbox2);
 $fieldset2 = array('type' => 'FieldSet',
		    'options' => array(
				       'legend' => 'Limit Your Results',
				       ));
$combobox = array(
		  'type' => 'ComboBox',
		  'options' => array(
				     'fieldLabel' => 'To a Specific Company',
				     'hiddenName' => 'limitcompany',
				     'displayField' => 'companyName',
				     'valueField' => 'id',
				     'typeAhead' => true,
				     'mode' => 'local',
				     'triggerAction' => 'all',
				     'emptyText' => 'Select a company...',
				     'selectOnFocus' => true,
				     'width' => 190
				     ),
		  'simplestore' => array(
				   'fields' => array('id', 'companyName'),
				   'data' => 'Ext.PAIT.companyList' // from states.js
				   ),

                  );
$combobox2 = array(
		  'type' => 'ComboBox',
		  'options' => array(
				     'fieldLabel' => 'To a Specific Status',
				     'hiddenName' => 'limitstatus',
				     'displayField' => 'status',
				     'valueField' => 'id',
				     'typeAhead' => true,
				     'mode' => 'local',
				     'triggerAction' => 'all',
				     'emptyText' => 'Select a status...',
				     'selectOnFocus' => true,
				     'width' => 190
				     ),
		  'simplestore' => array(
				   'fields' => array('id', 'status'),
				   'data' => 'Ext.PAIT.statusList' // from states.js
				   ),

                  );


 $fieldset2['children'] = array($combobox, $combobox2);
$array['form']['children'][] = $fieldset1;
$array['form']['children'][] = $fieldset2;
$saveButton = array('name' => 'Search',
		    'handler' => 'Ext.PAIT.handleSearchSubmit');
$cancelButton = array('name' => 'Reset',
		    'handler' => 'this.Reset');
 $array['form']['buttons'] = array($cancelButton, $saveButton);
print json_encode($array);
}

function exportXLS($data, $tableInfo) {
  global $form;
  //pre($data);
  //pre($tableInfo);
  //exit;
  $name = "ExportedSheet";
  header('HTTP/1.1 200 OK');
  header('Date: ' . date("D M j G:i:s T Y"));
  header('Last-Modified: ' . date("D M j G:i:s T Y"));
  header("Content-type: application/octet-stream");
  //    header("Content-type: application/vnd.ms-excel; name=’Excel’");
  // header("Content-Length: " . (string)(strlen($data)) );
  header("Content-Disposition: attachment; filename=\"".$name.".csv\"");
  header("Cache-Control: no-store, no-cache, no-transform, must-revalidate, private");
  //    header("Pragma: no-cache");
  header("Expires: 0");
  $headers = array();
  //pre($tableInfo);
  foreach ($tableInfo as $row) {
        $headers[] = $row['Field'];
  }
  
  //var_dump($headers);
  //var_dump($tableInfo);
  //var_dump($data);
  //print "\n";
  //$headers = array_keys($testrow);
  $results = array();
  foreach ($headers as $header) {
    if (preg_match('/[A-Za-z]/', $header)) {
      $results[0][] = $header;
    }
  }
  $line = "";
  //pre($results);
  //exit;
  foreach($results[0] as $value) { 
    if ((!isset($value)) OR ($value == "")) {
      $value = "\t";
    } else {
      $value = str_replace('"', '""', $value);
      $value = '"' . $value . '"' . ",";
    }
    $line .= $value;
  }
  print str_replace("\r","",rtrim($line,"\n\t\r\0 ,")."\n");
  $temprow = array();
  $line = "";
      
  foreach ($data as $row) {
    $temprow = "";
    foreach ($results[0] as $header) {
      if (stristr($header,'date') && (is_int($row[$header])) ) {
      $temprow[] = (@gmdate(PAITDATE,$row[$header]))?(@gmdate(PAITDATE,$row[$header])):"";
      } elseif (stristr($header,'gender')) {
      $temprow[] = $genders[$testrow[$header]];
      } elseif (stristr($header,'amount')) {
      $temprow[] = formatMoney($row[$header]);
      } elseif (stristr($header,'phone')) {
      $temprow[] = formatPhone($row[$header]);
      } elseif (stristr($header,'ssn')) {
      $temprow[] = formatSocial($row[$header]);

      } else {
      $temprow[] = ($row[$header])?$row[$header]:" ";
      }
    }
    //    $results[] = $temprow;
	  $line = "";
if (count($temprow) >1){
	foreach($temprow as $value) {      

	  if ((!isset($value)) OR ($value == "")) {
            $value = "\t";
	  } else {
            $value = str_replace('"', '""', $value);
            $value = '"' . $value . '"' . ",";
	  }
	  $line .= $value;
	}
	print str_replace("\r","",rtrim($line,"\n\t\r\0 ,")."\n");
      }
  }


    exit;
}

    


?>