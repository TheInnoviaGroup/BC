<?PHP

$uploadTemplate = new template('upload.ihtml');
$uploadTemplate->set('uploadInterval', 100);
$upload = new upload();
global $form, $db, $body, $javascripts, $AGLOBAL, $debug;
//$javascripts[] = 'LoadVars.js';
//$javascripts[] = 'BytesUploaded.js';
//$debug->level = 0;
$debug->variable($form, 1);
 if ($upload->verify('mediaFile')) {
 global $uploadPath;
 if ( $upload->save_file($uploadPath, 2) ) {
   doFinished();
 } else {
   print "There has been an error!<br><pre>\n";
   var_dump($upload);
   print "</pre>";
   exit;
 }



 } elseif ($form->upload) {
   if (is_array($upload->errors) && count($upload->errors) > 0) {
     $message = "The upload encountered the following error(s):<br />\n";
     foreach ($upload->errors as $error) {
       $message .= $error . "<br />\n";
     }
     $uploadTemplate->set('message', $message);
     
   } else {
     $message ="There was an error with the upload. Please contact the system administrator.";
   }
 } else if ($form->next) {
   $debug->line('inserting', 1);
   doInsert();
 } else {
   // I think I can safely remove most of this stuff. For some reason,
   // I left it in when I copied this from the Taylormade site.
   $uploadTemplate->set('type', $form->type);
   $uploadTemplate->set('id', $form->id);
   if ($form->type == "questionMedia" || $form->type == "explanationMedia") {
     $sql = "select questionText as title from quizquestions where questionID ='". $form->id . "'";
     $uploadTemplate->set('typeText', ($form->type == "questionMedia")?"question":"explanation");
   } elseif ($form->type == "answerMedia") {
     $sql = "select answerText as title from quizanswers where answerID = '". $form->id ."'";
     $uploadTemplate->set('typeText', "answer");
   } else {
     $sql = "select name as title from trainingunits where unitID = '". $form->id ."'";
     $uploadTemplate->set('typeText', 'training unit');
   }
   $db->query($sql);
   $AGLOBAL['DEBUG']->line($sql, 1);
   $res = $db->nextRecord();
   $uploadTemplate->set('longtitle', $res['title']);
   

 }

$body .= $uploadTemplate->parse();

function doInsert() {
  global $upload, $uploadTemplate, $db, $login, $debug, $form;

 $filename = $form->file;
 $company = $form->company;
 $carrier = $form->carrier;
 $policyType = $form->policyType;
 $policyStatus = $form->policyStatus;
 $updateOnly = $form->updateOnly;
 $paidBy = $form->paidBy;
 $type = ($carrier == 0)?'census':'ratefile'; // need to re-add back the bid support.
 $carrier = ($carrier == 0)?9:$carrier; // Our special carrier. Ignore this.
 $config = array('filename' => $filename,
		 'policyType' => $policyType,
		 'policyStatus' => $policyStatus,
		 'updateOnly' => $updateOnly,
		 'paidBy' => $paidBy);
 $debug->variable($config, 1);
 $debug->line(serialize($config), 1);
 $debug->variable(unserialize(serialize($config)), 1);
 if ($form->worksheet) {
   //Multiple.
   $array = $form->worksheet;
   foreach ($array as $sheetNum => $cruft) {
     $config['worksheet'] = $sheetNum;
     $sql = "insert into rawdatatoc set companyID = '$company', carrierID = '$carrier', type = '$type', time = '".time()."', status = 0, owner = '".$login->user->user_idnum."', config = '".serialize($config)."'";
     $debug->line('sql: '.$sql,1);
     $db->query($sql);
   }
 } else {
     $sql = "insert into rawdatatoc set companyID = '$company', carrierID = '$carrier', type = '$type', time = '".time()."', status = 0, owner = '".$login->user->user_idnum."', config = '".serialize($config)."'";
     $db->query($sql);
     $debug->line('sql2: '.$sql,1);
 }
 $uploadTemplate->set('success', TRUE);
 $uploadTemplate->set('finished', TRUE);
}

function doFinished() {
  global $upload, $uploadTemplate, $db;
if (include_once('excelparser.php')) {
global $debug;
$debug->line('included excelparser.php', 1);
}
include_once('exceldate.php');

 $filename = $upload->file['name'];
 global $uploadPath;
 $uploadTemplate->set('filename', $filename);
 $uploadTemplate->set('finished', TRUE);
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
$uploadTemplate->set('policyStats', $policyStatuses);
global $policyTypes;
$uploadTemplate->set('policyTypes', $policyTypes);
$debug->line('set policyStats and policyTypes', 1);
       $uploadTemplate->set('companies', $companies);
       $uploadTemplate->set('carriers', $carriers);
$debug->line('here '.$uploadPath.$filename, 1);
$xl = new ExcelFileParser('excel.log', ABC_DEBUG);
$error = $xl->ParseFromFile( $uploadPath.$filename );
       $debug->variable($error,1);
$debug->line('here1', 1);
	$ws_number = count($xl->worksheet['name']);
	$uploadTemplate->set('numSheets', $ws_number);
	if ($ws_number > 1) {
	  $debug->line('more than 1 sheet.', 1);
	  	  $uploadTemplate->set('xl', $xl);
	}

}
?>