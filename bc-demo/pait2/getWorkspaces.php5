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
global $debug, $AGLOBAL, $form, $db, $login, $rootDir;
$debug->level = 0; // 0 to turn off, higher for more output. alib
		   // objects do a lot of chatter at level 11 and higher.
$debug->output = 'pretty';
$debug->line('aLib init complete', 1);

// Must create a global Sheet db. 

$mySheetDB = createSheetDB();

// This is to hand our formdata. All of our components use the global $form.
$form = new formhandler(LAZYFORM);




$login = new login("login.ihtml");

 if ($login->loggedIn) {
   if ($login->user->access_lv == 3) {
     //     uploadOnly
     header("Location: https://".$_SERVER['HTTP_HOST']."/dropsite/"); /* Redirect browser */
     /* Make sure that code below does not get executed when we redirect. */
     exit;
   } else {
   
if($form->node == "source" || $form->node == "") {
	// grab all the user's workspaces
	$sql = "SELECT * FROM workspaces WHERE owner = '".$login->user->user_idnum."' OR public = '1'";
	
	$mySheetDB->query($sql, "workspaceList");
	
	$array = array();
	// $array['sql'] = $sql;
	while($wsItem = $mySheetDB->nextRecord("workspaceList")) {
		array_push($array, array('text' => $wsItem['name'],
		 'id' => 'source/'.$wsItem['wsID'],
			'cls' => 'folder', 'type' => 'workspace'));
	
	}
	// and make happy json out of it
	
	print json_encode($array);
	exit;
} else {
	// it's a node grab the related stuff
	$nodeArray = split("/", $form->node);
	if(count($nodeArray) == 2) {
		// its something in the workspace
		// grab all the user's workspaces
		$sql = "SELECT * FROM wstoc WHERE wsID = '".$nodeArray[1]."'";
		
		$mySheetDB->query($sql, "workspaceList");
		
		$array = array();
		// $array['sql'] = $sql;
		while($wsItem = $mySheetDB->nextRecord("workspaceList")) {
			// set up switch for status flags
		switch($wsItem['state']) {
			case "-1":
				$css_color = "file_red";
				$qtText = "This file has problems";
				break;
			case "0":
				$css_color = "file_grey";
				$qtText = "This file has not finished importing";
				break;
			case "1":
				$css_color = "file_yellow";
				$qtText = "This file does not have headers assigned";
				break;
			case "2":
				$css_color = "file_green";
				$qtText = "This file has headers assigned";
				break;
			case "3":
				$css_color = "file_blue";
				$qtText = "This file has been imported into PAIT";
				break;
		
		}
		
			array_push($array, array('text' => ucfirst($wsItem['sheetName']),
			 'id' => 'source/'.$wsItem['wsID'].'/'.$wsItem['sheetID'],
				'qtip' => $qtText,
				'icon' => $rootDir.'images/excel.gif',
		    'cls' => $css_color, 'type' => 'sheet', 'status' => $wsItem['state']));
		
		}
		// and make happy json out of it
		
		print json_encode($array);
		exit;
} elseif(count($nodeArray) == 3) {
		// its a report for the sheet
		// grab all the sheet's report
		$sql = "SELECT * FROM reports WHERE sheetID = '".$nodeArray[2]."' AND status = 2";
		
		$mySheetDB->query($sql, "reportList");
		
		$array = array();
		// $array['sql'] = $sql;
		while($wsItem = $mySheetDB->nextRecord("reportList")) {
						// set up switch for status flags
		switch($wsItem['state']) {
			case "-1":
				$css_color = "file_red";
				$qtText = "This report has problems";
				break;
			case "0":
				$css_color = "file_grey";
				$qtText = "This report has been queued";
				break;
			case "1":
				$css_color = "file_yellow";
				$qtText = "This report is currently processing ";
				break;
			case "2":
				$css_color = "file_green";
				$qtText = "This report has been processed";
				break;
		}
		
		
			array_push($array, array('text' => ucfirst($wsItem['name']),
			 'id' => 'source/'.$nodeArray[1].'/'.$nodeArray[2].'/'.$wsItem['reportID'],
				'leaf' => TRUE,
				'qtip' => $qtText,
				'icon' => $rootDir.'images/report.gif',
		    'cls' => $css_color, 'type' => 'report'));
		}
		// and make happy json out of it
		
		print json_encode($array);
		exit;
}
	

}

 }
}  else {
 	$mainTemplate = new template("container.ihtml");
  $mainTemplate->set('nodisplay', TRUE);
  $body = $login->template->parse();
}


?>