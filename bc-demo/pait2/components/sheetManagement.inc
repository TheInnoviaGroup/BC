<?PHP
$sheetTemplate = new template('sheetMain.ihtml');
$altTemplates = array('editsheet' => 'sheetEdit.ihtml',
                      'details' => 'sheetDetails.ihtml'); //examples
global $body, $form, $self, $javascripts, $styles, $login, $sheetDB;



if ($form->action) {
    $sheetTemplate = new template($altTemplates[$form->action]);
}

$sheetTemplate->set("self", $self);
switch ($form->action) {
    case "report":
        reportBroker($sheetTemplate);
        break;
case "createTwoSheetJob":
    createTwoSheetJob($sheetTemplate);
    break;
case "createOneSheetJob":
    createOneSheetJob($sheetTemplate);
    break;
case "addColumn":
    addColumn($sheetTemplate);
    break;
case "assignHeaders":
    assignHeaders($sheetTemplate);
    break;
case "filterSheets":
    filterSheets($sheetTemplate);
    break;
case "mergeSheets":
    mergeSheets($sheetTemplate);
    break;
case "dialog":
    doDialog($sheetTemplate);
}

function reportBroker($temp) {
    global $form, $db;
    $sheetDB = createSheetDB();
    $nodeArray = split('/', $form->node);
    $reportID = $nodeArray[count($nodeArray)-1];
    $sql = "select * from reports where reportID = '$reportID'";
    $sheetDB->query($sql);
    $reportInfo = $sheetDB->nextRecord();
    switch($reportInfo['type']) {
        case "missingData":
            case "unmatchedPeople":
                case "unmatchedSpouses":
                    case "duplicateRows":
                    case "import":
                    case "importTest":
                    showReportTable($reportInfo);
                    break;
        case "compareTwo":
            showCompareTwoReport($reportInfo);
        break;
    }
}

function showCompareTwoReport($reportInfo) {
    global $self, $form, $db;
    $reportTemplate = new template('sheetDifferences.ihtml');
    //pre($reportInfo);
    $tables = explode('-', $reportInfo['config']);
    $sql1 = 'describe `sheet-'.$tables[0].'`';
    $sql2 = 'describe `sheet-'.$tables[1].'`';
    $sheetDB = createSheetDB();
    $sheetDB->query($sql1);
    //pre($sql1);
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
    $table1Unique = array_diff($table1Fields, $table2Fields);
    $table2Unique = array_diff($table2Fields, $table1Fields);
    //pre($table1Desc);
    $reportTemplate->set('sheet1Cols', $table1Unique);
    $reportTemplate->set('sheet2Cols', $table2Unique);
    
    $sql3 = 'select sheetName from wstoc where sheetID = '.$tables[0];
    $sql4 = 'select sheetName from wstoc where sheetID = '.$tables[1];
    $sheetDB->query($sql3);
    $row1 = $sheetDB->nextRecord();
    $sheetDB->query($sql4);
    $row2 = $sheetDB->nextRecord();
    
    $reportTemplate->set('sheet1', $row1['sheetName']);
    $reportTemplate->set('sheet2', $row2['sheetName']);
    
     $tableName = $reportInfo['config'];
    $template = new template('reportTableViewer.ihtml');
    $template->set('reportType', $reportInfo['type']);
    $template->set('table', $tableName);
    $template->set('self', $self);
    $template->set('size', 15); // we default to 50, but it's changeable.
    $template->set('prettyNames', TRUE); //Whether or not to uppercase the first letter of column names.
    $reportTemplate->set('diffGrid', $template->parse());
    
    
    
    $reportTemplate->display();
    exit;
}

function showReportTable($info) {
    global $self, $form;
    $tableName = 'sheet-'.$info['sheetID'];
    $template = new template('reportTableViewer.ihtml');
    $template->set('reportType', $info['type']);
    $template->set('node', $form->node);
    $template->set('table', $tableName);
    $template->set('self', $self);
    $template->set('size', 15); // we default to 15, but it's changeable.
    $template->set('prettyNames', TRUE); //Whether or not to uppercase the first letter of column names.
    $template->display();
    exit;
}



function doDialog($temp) {
    global $form, $db;
    switch($form->module) {
        case "mergeTwo":
            mergeTwoDialog();
            break;
        case "filterSecond":
            filterSecondDialog();
            break;
        case "addColumn":
            addColumnDialog();
            break;
    		case "unmatchedSpouses":
            createOneSheetJob($sheetTemplate);
            $returnValue = array();
            $returnValue['success'] = TRUE;   
     				$returnValue['openDialog']['options'] = array(
            'type' => 'alert',
            'title' => 'Spouse Report Queued.',
            'msg' => 'The Spouse Report has been queued for processing.<br>You will be notified by email when it is complete.'
        		);
    				print json_encode($returnValue);
    				exit;
            break;
        case "compareTwo":
            createTwoSheetJob($sheetTemplate);
            $returnValue = array();
            $returnValue['success'] = TRUE;   
     				$returnValue['openDialog']['options'] = array(
            'type' => 'alert',
            'title' => 'Comparison of Sheets Queued.',
            'msg' => 'The Comparison Report has been queued for processing.<br>You will be notified by email when it is complete.'
        		);
    				print json_encode($returnValue);
    				exit;
            break;
       case "unmatchedPeople":
            createOneSheetJob($sheetTemplate);
            $returnValue = array();
            $returnValue['success'] = TRUE;   
     				$returnValue['openDialog']['options'] = array(
            'type' => 'alert',
            'title' => 'Unmatched People Report Queued.',
            'msg' => 'The Unmatched People has been queued for processing.<br>You will be notified by email when it is complete.'
        		);
    				print json_encode($returnValue);
    				exit;
            break;
            
      case "comparePAIT":
            createOneSheetJob($sheetTemplate);
            $returnValue = array();
            $returnValue['success'] = TRUE;   
     				$returnValue['openDialog']['options'] = array(
            'type' => 'alert',
            'title' => 'PAIT Comparision Queued.',
            'msg' => 'The PAIT Comparision has been queued for processing.<br>You will be notified by email when it is complete.'
        		);
    				print json_encode($returnValue);
    				exit;
            break;
            
      case "import":
            createOneSheetJob($sheetTemplate);
            $returnValue = array();
            $returnValue['success'] = TRUE;   
     				$returnValue['openDialog']['options'] = array(
            'type' => 'alert',
            'title' => 'Import Queued.',
            'msg' => 'The Import has been queued for processing.<br>You will be notified by email when it is complete.'
        		);
    				print json_encode($returnValue);
    				exit;
            break;
        case "importTest":
            createOneSheetJob($sheetTemplate);
            $returnValue = array();
            $returnValue['success'] = TRUE;   
     				$returnValue['openDialog']['options'] = array(
            'type' => 'alert',
            'title' => 'Import Queued.',
            'msg' => 'The Import has been queued for testing.<br>You will be notified by email when it is complete.'
        		);
    				print json_encode($returnValue);
    				exit;
            break;
    }
}

function addColumnDialog() {
    global $form, $db, $self;
    $array = array();
    $array['success'] = TRUE;
    $array['console'][] = 'Creating Form...';
    $array['form']['options']['labelWidth'] = '50';
    $array['form']['options']['url'] = $self."?mode=Sheets&action=addColumn";
    $array['form']['options']['waitMsgTarget'] = 'myDLGProgress';
    $nodeID = split('/', $form->nodeID);
    $array['console'][] = $nodeID;
    $array['form']['options']['baseParams'] = array('table' => "sheet-".$nodeID[2]);
    
    $array['form']['children'][] = array(
		 'type' => 'TextField',
		 'options' => array(
				    'fieldLabel' => 'Column Name',
				    'name' => 'column',
				    'width' => 250,
				    'allowBlank' => false,
				    ));
    $array['form']['children'][] = array(
		 'type' => 'TextField',
		 'options' => array(
				    'fieldLabel' => 'Column Value',
				    'name' => 'value',
				    'width' => 250,
				    'allowBlank' => false,
				    ));
    $saveButton = array('name' => 'Save',
		    'handler' => 'Ext.ux.CascadingDialogs.handleDialogSubmit');
$cancelButton = array('name' => 'Reset',
		    'handler' => 'this.Reset');
$array['form']['buttons'] = array($cancelButton, $saveButton);
print json_encode($array);
exit;
}

function mergeTwoDialog() {
    global $form, $db, $self;
    $array = array();
    $array['success'] = TRUE;
    $array['console'][] = 'Creating Form...';
    $array['form']['options']['labelWidth'] = '50';
    $array['form']['options']['url'] = $self."?mode=Sheets&action=mergeSheets";
    $array['form']['options']['waitMsgTarget'] = 'myDLGProgress';
    $nodeID1 = split('/', $form->nodeID1);
    $nodeID2 = split('/', $form->nodeID2);
    $array['console'][] = $nodeID;
    $array['form']['options']['baseParams'] = array('sheet1' => $form->nodeID1,
                                                    'sheet2' => $form->nodeID2,
                                                    'module' => $form->module);
    $array['form']['children'][] = array(
		 'type' => 'Radio',
		 'options' => array(
				    
                                    'boxLabel' => $form->nodeName1,
				    'name' => 'primary',
                                    'inputValue' => 'sheet1',
				    'width' => 250,
				    'allowBlank' => false,
                                    'itemCls' => 'hide-label'
				    ));
    $array['form']['children'][] = array(
		 'type' => 'Radio',
		 'options' => array(
				    'boxLabel' => $form->nodeName2,
				    'name' => 'primary',
                                    'inputValue' => 'sheet2',
				    'width' => 250,
				    'allowBlank' => false,
                                    'itemCls' => 'hide-label'
				    ));
    
    $saveButton = array('name' => 'Save',
		    'handler' => 'Ext.ux.CascadingDialogs.handleDialogSubmit');
$cancelButton = array('name' => 'Reset',
		    'handler' => 'this.Reset');
$array['form']['buttons'] = array($cancelButton, $saveButton);
print json_encode($array);
exit;
}

function filterSecondDialog() {
    global $form, $db, $self;
    $array = array();
    $array['success'] = TRUE;
    $array['console'][] = 'Creating Form...';
    $array['form']['options']['labelWidth'] = '50';
    $array['form']['options']['url'] = $self."?mode=Sheets&action=filterSheets";
    $array['form']['options']['waitMsgTarget'] = 'myDLGProgress';
    $nodeID1 = split('/', $form->nodeID1);
    $nodeID2 = split('/', $form->nodeID2);
    $array['console'][] = $nodeID;
    $array['form']['options']['baseParams'] = array('sheet1' => $form->nodeID1,
                                                    'sheet2' => $form->nodeID2,
                                                    'module' => $form->module);
    $array['form']['children'][] = array(
		 'type' => 'Radio',
		 'options' => array(
				    
                                    'boxLabel' => $form->nodeName1,
				    'name' => 'primary',
                                    'value' => 'sheet1',
				    'width' => 250,
				    'allowBlank' => false,
                                    'itemCls' => 'hide-label'
				    ));
    $array['form']['children'][] = array(
		 'type' => 'Radio',
		 'options' => array(
				    'boxLabel' => $form->nodeName2,
				    'name' => 'primary',
                                    'value' => 'sheet2',
				    'width' => 250,
				    'allowBlank' => false,
                                    'itemCls' => 'hide-label'
				    ));
    
    $saveButton = array('name' => 'Save',
		    'handler' => 'Ext.ux.CascadingDialogs.handleDialogSubmit');
$cancelButton = array('name' => 'Reset',
		    'handler' => 'this.Reset');
$array['form']['buttons'] = array($cancelButton, $saveButton);
print json_encode($array);
exit;
}

/* Untested. */

function assignHeaders($temp) {
    global $form, $db;
    $returnValue = array();
    $uniqueCol = FALSE;
    $myDB = createSheetDB();
    $sheet1 = $form->sheet1;
    $headerRow = $form->headerrow;
    $headerAction = $form->headerAction;
    $headerSub1Action = $form->headerSub1Action;
    $headerSub2Action = $form->headerSub2Action;
    
    $headerRowSQL = "select * from `$sheet1` where rowID = '".$headerRow."'";
    $myDB->query($headerRowSQL, 'headerRow');
    $headerRow = $myDB->nextRecord('headerRow', MYSQL_ASSOC);
    $ignoreCount = 0;
    $errors = array();
    $sqlT = ' ALTER TABLE `sheet-%s` CHANGE `%s` `%s` %s ';
    foreach ($headerRow as $column => $header) {
        $temph = preg_replace('/[\s]+/', '', $header);
        $newColumn = $headerAction[$temph];
        print "$header:$temph:$newColumn<br />";
        if (is_numeric($newColumn)) {
        if ($newColumn == 6) { // policy info
            $newColumn = $headerSub2Action[$temph];
        } else if ($newColumn == 1) {
            $newColumn = $headerSub1Action[$temph];
        } else if ($newColumn == 0) {
            $newColumn = 'ignore'.$ignoreCount;
            $ignoreCount++;
        } else if ($newColumn == 2) {
            $newColumn = $temph;  
        }
        }
        if ($uniqueCol != 'ssn' && ($newColumn == 'ssn' || $newColumn == 'employeeID')) {
            $uniqueCol = $newColumn;
        }
        
        // For duplicate finding later.
        
        //We can't do dates as integers yet since we aren't doing our date conversions yet.
        $typeMap = array(
                         'cn' => 'TEXT',
                         'title' => 'TEXT',
                         'amendment' => 'TEXT',
                         );
        $type = 'VARCHAR ( 255 )'; 
        if (array_key_exists($newColumn, $typeMap)) {
            $type = $typeMap[$newColumn];
        }
        $myDB->query(sprintf($sqlT, $sheet1, $column, $newColumn, $type), 'alterTable');
        if ($myDB->isError()) {
            $errors[] = array('from' => $column,
                              'header' => $header,
                              'to' => $newColumn,
                              'error' => $myDB->getError());
        }
    
    }
    if (count(array_keys($errors)) > 0) {
        $returnValue['success'] = FALSE;
        $returnValue['errors'] = $errors;
    } else {
        global $user;
        $returnValue['success'] = TRUE;
        $updateStatusSQL = "update sheets set status = 2 where sheetID = 'sheet-".$sheet1."'";
        $myDB->query($updateStatusSQL);
        if ($uniqueCol) {
        #Duplicate checking... If we have no unique column, we can't check for duplicates
        $alterSQLD = "ALTER TABLE `sheet-".$sheet1."` ADD `duplicates` INT( 64 ) NOT NULL DEFAULT '0' AFTER `rowID` ;'";
        $returnValue['sql'][] = $alterSQLD;
        $myDB->query($alterSQLD);
        $duplicateSQL = "UPDATE `sheet-".$sheet1."` LEFT JOIN (
                            SELECT `".$uniqueID."`, count( `".$uniqueID."` ) AS total
                            FROM `sheet-".$sheet1."`
                            GROUP BY `".$uniqueID."`
                            HAVING total >1
                            ) AS dupTot
                        USING ( `".$uniqueID."` )
                        SET duplicates = total;";
                        #Wow, that's gnarly.
        $myDB->query($duplicateSQL);
        $returnValue['sql'][] = $duplicateSQL;
        $addDupeReport = "update reports set type = 'duplicateRows', time = '".time()."', status=2, owner='".$user->user_idnum."', name='Duplicate Entries'";
        $myDB->query($addDupeReport);
        $returnValue['sql'][] = $addDupeReport;
        }
    }
    print json_encode($returnValue);
    exit;
}

function mergeSheets($temp) {
    global $form, $login;
    $returnValue = array();
    $myDB = createSheetDB();
    $nodeID1 = split('/',$form->sheet1);
    $sheet1 = $nodeID1[2];
    $nodeID2 = split('/',$form->sheet2);
    $sheet2 = $nodeID2[2];
    $now = time();
    $jobFunction = $form->module;
    $jobConfig = array('primary' => $form->primary,
                       'workspace1' => $nodeID1[1],
                       'workspace2' => $nodeID2[1]);
    $status = 0; //Creating.
    $sql = "insert into jobqueue set status = '$status', owner = '".$login->user->user_idnum."', created = '$now', sheet1 = '$sheet1', sheet2 = '$sheet2', jobFunction = '$jobFunction', jobConfig = '".addslashes(serialize($jobConfig))."'";
    $myDB->query($sql, 'create2sheetjob');
    if ($myDB->isError()) {
        $returnValue['success'] = FALSE;
        $returnValue['errorText'] = 'MYSQL Error: '.$myDb->getError();
    } else {
    // $returnValue['success'] = TRUE;   
        $returnValue = array();
            $returnValue['success'] = TRUE;   
     				$returnValue['openDialog']['options'] = array(
            'type' => 'alert',
            'title' => 'Sheet Merger Queued.',
            'msg' => 'The Sheet Merge has been queued for processing.<br>You will be notified by email when it is complete.'
        		);
    				print json_encode($returnValue);
    				exit;
    
    //Do your display stuff here.
    //print json_encode($returnValue);
    } 
}

function filterSheets($temp) {
    global $form, $login;
    $returnValue = array();
    $myDB = createSheetDB();
    $nodeID1 = split('/',$form->sheet1);
    $sheet1 = $nodeID1[2];
    $nodeID2 = split('/',$form->sheet2);
    $sheet2 = $nodeID2[2];
    $now = time();
    $jobFunction = $form->module;
    $jobConfig = array('primary' => $form->primary,
                       'workspace1' => $nodeID1[1],
                       'workspace2' => $nodeID2[1]);
    $status = 0; //Creating.
    $sql = "insert into jobqueue set status = '$status', owner = '".$login->user->user_idnum."', created = '$now', sheet1 = '$sheet1', sheet2 = '$sheet2', jobFunction = '$jobFunction', jobConfig = '".addslashes(serialize($jobConfig))."'";
    $myDB->query($sql, 'create2sheetjob');
    if ($myDB->isError()) {
        $returnValue['success'] = FALSE;
        $returnValue['errorText'] = 'MYSQL Error: '.$myDb->getError();
    } else {
    // $returnValue['success'] = TRUE;   
        $returnValue = array();
            $returnValue['success'] = TRUE;   
     				$returnValue['openDialog']['options'] = array(
            'type' => 'alert',
            'title' => 'Sheet Filtering Queued.',
            'msg' => 'The Filter Sheets job has been queued for processing.<br>You will be notified by email when it is complete.'
        		);
    				print json_encode($returnValue);
    				exit;
    
    //Do your display stuff here.
    //print json_encode($returnValue);
    } 
}

function createTwoSheetJob($temp) {
    global $form, $login;
    $returnValue = array();
    $myDB = createSheetDB();
    $nodeID1 = split('/',$form->nodeID1);
    $sheet1 = $nodeID1[2];
    $nodeID2 = split('/',$form->nodeID2);
    $sheet2 = $nodeID2[2];
    $now = time();
    $jobFunction = $form->module;
    $jobConfig = $form->jobConfig; //Should be an array.
    $status = 0; //Creating.
    $sql = "insert into jobqueue set status = '$status', owner = '".$login->user->user_idnum."', created = '$now', sheet1 = '$sheet1', sheet2 = '$sheet2', jobFunction = '$jobFunction', jobConfig = '".addslashes(serialize($jobConfig))."'";
    $myDB->query($sql, 'create2sheetjob');
    if ($myDB->isError()) {
        $returnValue['success'] = FALSE;
        $returnValue['errorText'] = 'MYSQL Error: '.$myDb->getError();
    } else {
    // $returnValue['success'] = TRUE;   
        
    
    //Do your display stuff here.
    //print json_encode($returnValue);
    }
    
}


function createOneSheetJob($temp) {
    global $form, $login;
    $returnValue = array();
    $myDB = createSheetDB();
    $nodeID = split('/',$form->nodeID);
    $sheet1 = $nodeID[2];
    $sheet2 = '-1';
    $now = time();
    $jobFunction = $form->module;
    $jobConfig = $form->jobConfig; //Should be an array.
    $status = 0; //Creating.
    $sql = "insert into jobqueue set status = '$status', owner = '".$login->user->user_idnum."', created = '$now', sheet1 = '$sheet1', sheet2 = '$sheet2', jobFunction = '$jobFunction', jobConfig = '".addslashes(serialize($jobConfig))."'";
    $myDB->query($sql, 'create1sheetjob');
    if ($myDB->isError()) {
        $returnValue['success'] = FALSE;
        $returnValue['errorText'] = 'MYSQL Error: '.$myDb->getError();
    } else {
        
    // $returnValue['success'] = TRUE;   
        
    
    //Do your display stuff here.
    //print json_encode($returnValue);
    }
    
}


function addColumn($temp) {
    global $form;
    $returnValue = array();
    $myDB = createSheetDB();
    $table = $form->table;
    $column = $form->column;
    $value = $form->value;
    $column = preg_replace('/[^A-Za-z0-9]*/', '', $column);
    $type = (strlen($value) > 255)?"TEXT":"VARCHAR( 255 )";
    $sql = 'ALTER TABLE `'.$table.'` ADD `'.$column.'` '.$type.' NOT NULL DEFAULT '."'$value'";
    $myDB->query($sql, 'addColumn');
    if ($myDB->isError()) {
        $returnValue['success'] = FALSE;
        $returnValue['errorText'] = 'MYSQL Error: '.$myDB->getError();
        $returnValue['errorSQL'] = $sql;
    } else {
     $returnValue['success'] = TRUE;   
     $returnValue['openDialog']['options'] = array(
            'type' => 'alert',
            'title' => 'Column added.',
            'msg' => 'Column "'.$column.'" added with a value of "'.$value.'"'
        );
     $returnValue['openDialog']['url'] = '';
    }
    
    //Do your display stuff here.
    print json_encode($returnValue);
    
}


function sheetMain($temp) {
    global $body, $login, $self;
    
    
    
    $login->template->set('isSubpage', 'yes');
    $temp->set('self', $self);
    $temp->set('loginbox', $login->template->parse());
    $body = $temp->parse();
}

function addsheet($template) {
    global $body, $self, $login, $form, $db, $debug;
    $template->set('action', 'editsheet');
    
    $body = $template->parse();
}


?>