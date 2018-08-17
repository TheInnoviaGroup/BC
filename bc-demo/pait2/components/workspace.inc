<?php
global $body, $form, $self, $javascripts, $styles, $login, $rootDir, $noDisplay;
$noDisplay = FALSE;
switch ($form->action) {
 case "addWorkspace":
   editWorkspace();
   break;
 case "editWorkspace":
   
   break;
 case "addSheet":
   addSheet();
   break;
 case "removeSheet":
   removeSheet();
   break;
 case "deleteWorkspace":
   deleteWorkspace();
   break;
case "sheetList":
	sheetList();
	break;

 default:

}

if (!$noDisplay) {
 	// here we just grab the initial workspace and option view
	$wsTemplate = new template('ws_InitialView.ihtml');
	$mySheetDB = createSheetDB();
	$sql = "Select * from sheets order by sheetID desc";
	$mySheetDB->query($sql, "sheetList");
	$sheets = $mySheetDB->allRecords("sheetList");
	$wsTemplate->set('sheets', $sheets);
	$wsTemplate->set('self', $self);
	$wsTemplate->set('rootDir', $rootDir);
	$wsTemplate->set('javascripts', $javascripts);
	$wsTemplate->set('styles', $styles);
	$wsTemplate->set('login', $login);
	$body = $wsTemplate->parse();
}

function sheetList() {
	global $body, $login, $javascripts, $styles, $self, $rootDir, $noDisplay;
	$wsTemplate = new template('sheetlistdiv.ihtml');
	$mySheetDB = createSheetDB();
	$sql = "select * from sheets order by sheetID desc";
	$mySheetDB->query($sql, "sheetList");
	$sheets = $mySheetDB->allRecords("sheetList");
	$wsTemplate->set('sheets', $sheets);
	$wsTemplate->set('self', $self);
	$wsTemplate->set('rootDir', $rootDir);
	$wsTemplate->set('javascripts', $javascripts);
	$wsTemplate->set('styles', $styles);
	$wsTemplate->set('login', $login);
	$body = $wsTemplate->parse();
	$noDisplay = TRUE;
}

function editWorkspace() {
global $form, $login, $self;

$mySheetDB = createSheetDB();
$today = time();
if($form->Submit == TRUE) {
$sql = "REPLACE INTO workspaces SET wsID='".$form->wsID."', name = '".$form->wsName."', public = '".$form->public."', created = '".$today."', owner = '".$login->user->user_idnum."'";

$mySheetDB->query($sql, "addWorkspaceSQL");
if($form->wsID > 0) {
	$wsID = $form->wsID;
} else {
	$wsID = $mySheetDB->insertID();
}
$array = array('success' => TRUE ,
			 'updateTree' => array('wsID' => $wsID,
						'treeNode' => 'source',
						'treeDiv' => 'tree-div') 
			 );
print json_encode($array);
exit;
} else {
global $self;
$array = array();
$array['success'] = TRUE;
$array['form']['options']['labelWidth'] = 100;
$array['form']['options']['url'] = $self.'?mode=Workspace&action=addWorkspace';
$array['form']['options']['waitMsgTarget'] = 'myDLGProgress';
$fieldset = array('type' => 'FieldSet',
		  'options' => array(
				     'legend' => 'Add A Workspace'
				     ));
$hiddenFields['refreshID'] = 'tree-div';
$hiddenFields['refreshURL'] = '';
$hiddenFields['creator'] = $user['user_idnum'];
$hiddenFields['Submit'] = TRUE;
$textbox1 = array(
		 'type' => 'TextField',
		 'options' => array(
				    'fieldLabel' => 'Workspace Name',
				    'name' => 'wsName',
				    'width' => 200,
				    'value' => $content->gn,
				    'allowBlank' => true
				    ));
$checkbox1 = array(
		'type' => 'Checkbox',
		'options' => array(
		'boxLabel' => 'Share This Workspace',
		'labelStyle' => 'display:none',
		'labelWidth' => 0,
		'name' => 'public',
		'inputValue' => '1', 
		'checked' => false
	));
$fieldset['children'] = array($textbox1, $checkbox1);

$array['form']['options']['baseParams'] = $hiddenFields;
$array['form']['children'][] = $fieldset;
$saveButton = array('name' => 'Save',
		    'handler' => 'Ext.ux.CascadingDialogs.handleDialogSubmit');
$cancelButton = array('name' => 'Reset',
		    'handler' => 'this.Reset');
$array['form']['buttons'] = array($cancelButton, $saveButton);
print json_encode($array);
exit;

}
}

function addSheet() {
global $form, $login, $self;

$mySheetDB = createSheetDB();
$tempArray = split("/", $form->wsID);
$wsID = $tempArray[1];

foreach($form->sheets as $key => $val) {
	// check to see if it's already there in wstoc...
	$sqlCheck = "SELECT * FROM wstoc WHERE sheetID=".$val." AND wsID=".$wsID;
	$mySheetDB->query($sqlCheck, "wstocCheckQuery");
	if($mySheetDB->sizeOf("wstocCheckQuery") == 0) {
		$sqlGet = "SELECT * FROM sheets WHERE sheetID=".$val;
		$mySheetDB->query($sqlGet, "getSheetQuery");
		$sheet = $mySheetDB->next_record("getSheetQuery");
	
		$sqlReplace = "INSERT INTO wstoc SET wsID='".$wsID."', sheetID = '".$sheet['sheetID']."', state='".$sheet['status']."', sheetName = '".$sheet['type'].":".$sheet['filename']."'";
		$mySheetDB->query($sqlReplace, "addSheetQuery");
	}
}

$array = array('success' => TRUE ,
			 'updateTree' => array('wsID' => $wsID,
						'treeNode' => 'source/'.$wsID,
						'sheetCount' => count($form->sheets),
						'sql1' => $sqlGet,
						'sql2' => $sqlReplace,
						'$sql3' => $sqlCheck,
						'form' => $form->sheets,
						'sheet' => $sheet,
						'treeDiv' => 'tree-div') 
			 );
print json_encode($array);
exit;
}


function deleteWorkspace() {
global $form, $login, $self;
$mySheetDB = createSheetDB();

// we get a node id that looks like 'source/some_id' so we parse it
$temp = split("/", $form->nodeID);
$wsID = $temp[1];
$sql = "DELETE FROM workspaces WHERE wsID='".$wsID."'";
$sql2 = "DELETE FROM wstoc WHERE wsID='".$wsID."'";

$mySheetDB->query($sql, "deleteWSQuery");
$mySheetDB->query($sql2, "deletewstocQuery");

$array = array('success' => TRUE ,
			 'updateTree' => array('wsID' => $wsID,
						'treeNode' => 'source/'.$wsID,
						'sheetCount' => count($form->sheets),
						'sql1' => $sql,
						'sql2' => $sql2,
						'wsID' => $wsID) 
			 );
print json_encode($array);
exit;
}


function removeSheet() {
global $form, $login, $self;

$mySheetDB = createSheetDB();
$tempArray = split("/", $form->nodeID);
$sheetID = $tempArray[2];
$wsID = $tempArray[1];

$sqlRemove = "DELETE FROM wstoc WHERE sheetID=".$sheetID." AND wsID=".$wsID;
$mySheetDB->query($sqlRemove, "wstocRemoveQuery");

$array = array('success' => TRUE ,
			 'updateNode' => array('wsID' => $wsID,
						'treeNode' => 'source/'.$wsID,
						'sheetCount' => count($form->sheets),
						'sql1' => $sqlGet,
						'sql2' => $sqlReplace,
						'$sql3' => $sqlCheck,
						'form' => $form->sheets,
						'sheet' => $sheet,
						'treeDiv' => 'tree-div') 
			 );
print json_encode($array);
exit;
}

?>