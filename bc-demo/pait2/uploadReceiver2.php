<?php
$include_path = "./templates;./components";
set_include_path(get_include_path() . ";" . $include_path . ";");
global $AGLOBAL;
//$AGLOBAL['include_path'] = get_include_path();
include_once('../alib/alib.inc');
include_once('../alib/sort.inc');
include_once('../alib/time.inc');
include_once('../alib/misc.inc');
include_once('config.inc'); //configs.

include_once('./components/general.inc'); // general functions and objects
global $debug, $AGLOBAL, $form, $db;
$debug->level = 0; // 0 to turn off, higher for more output. alib
		   // objects do a lot of chatter at level 11 and higher.
$debug->output = 'pretty';
$debug->line('aLib init complete', 1);

include_once('audit.inc');

// Must create a global db. I should make the components smart about this.
$db = new db($AGLOBAL['DEMODBURL']);
// This is to hand our formdata. All of our components use the global $form.


$acceptable_types = array();
//$formHTML = unhtmlize(preString($form), TRUE);

$upload = new upload();
if (is_array($upload->errors) && count($upload->errors) > 0) {
     $message = "The upload encountered the following error(s):<br />\n";
     foreach ($upload->errors as $error) {
       $message .= $error . "<br />\n";
     }
     
     
   } else {
     $message ="There was an error with the upload. Please contact the system administrator.";
   }
   $retval = array();
$retval['success'] = FALSE;
$retval['message'] = $message."<br />";
global $_POST;
global $_FILES;
$retval['form'] = $_POST;
$retval['files'] = $_FILES;
//$retval['upload'] = unhtmlize(preString($upload), TRUE);
print json_encode($retval);

?>