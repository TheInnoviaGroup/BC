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
$debug->level = 0; // 0 to turn off, higher for more output. alib
		   // objects do a lot of chatter at level 11 and higher.
$debug->output = 'pretty';
$debug->line('aLib init complete', 1);

$login = new login("login.ihtml");

 if ($login->loggedIn) {
include_once('audit.inc');
$form = new formhandler(LAZYFORM);
// Must create a global db. I should make the components smart about this.
$db = new db($AGLOBAL['DEMODBURL']);
// This is to hand our formdata. All of our components use the global $form.
$sheetDB = createSheetDB();

//$formHTML = unhtmlize(preString($form), TRUE);
global $_FILES, $_POST, $_REQUEST;
$retval['upload'][] = array_keys($_FILES);
$retval['upload'][] = array_keys($_POST);
$retval['upload'][] = array_keys($_REQUEST);
$retval['form'] = $form->formVars;
$upload = new upload();
if (is_array($upload->errors) && count($upload->errors) > 0) {
    $message = "The upload encountered the following error(s):\n";
    foreach ($upload->errors as $error) {
        $message .= $error . "\n";
    }
    $retval = array();
    $retval['success'] = FALSE;
    $retval['error'] = $message;
} else {
    if ($upload->verify('file')) {
        global $uploadPath;
        if ( $upload->save_file($uploadPath, 2) ) {
            $sql = "insert into sheets set time = ".time().", status = 0, owner = '".$login->user->user_idnum."', filename = '".$upload->file['name']."'";
            $sheetDB->query($sql);
            if (! $sheetDB->isError()) {
            $retval['filename'] = $upload->file['name'];    
            $retval['success'] = TRUE;
            } else {
                $retval['success'] = FALSE;
                $retval['error'] = $sheetDB->getError();
                $retval['sql'] = $sql;
            }
        } else {
            $retval['success'] = FALSE;
            $retval['error'] = "There has been an error!";
        }
    } else {
        $message = "Unable to verify upload:\n";
        foreach ($upload->errors as $error) {
            $message .= $error . "\n";
        }
        $retval = array();
        $retval['success'] = FALSE;
        $retval['error'] = $message;    
    }
}


print json_encode($retval);
 }
?>