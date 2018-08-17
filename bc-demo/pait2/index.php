<?PHP
/* un comment for error reporting
ini_set("display_errors","2");
ERROR_REPORTING(E_ALL);
*/

$include_path = "./templates:./components";
set_include_path(get_include_path() . ":" . $include_path . ":");
global $AGLOBAL;
//$AGLOBAL['include_path'] = get_include_path();

include_once('../alib/alib.inc');
include_once('../alib/sort.inc');
include_once('../alib/time.inc');
include_once('../alib/misc.inc');
include_once('config.inc'); //configs.
$AGLOBAL['DEBUG'] = $debug;
    
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
$form = new formhandler(LAZYFORM);


$mainTemplate = new template("container.ihtml");

$login = new login("login.ihtml");

 if ($login->loggedIn) {
   if ($login->user->access_lv == 3) {
     //     uploadOnly
     header("Location: https://".$_SERVER['HTTP_HOST']."/dropsite/"); /* Redirect browser */
     /* Make sure that code below does not get executed when we redirect. */
     exit;
   } else {

   $login->template->set('self', $self);
switch ($form->mode) {
case "Admin":
  include_once('admin.inc');
  $mainTemplate->set('loginbox', $login->template->parse());
  break;
case "Log":
  include_once('log.inc');
  $mainTemplate->set('loginbox', $login->template->parse());
  break;
case "Search":
  include_once('search.inc');
  $mainTemplate->set('loginbox', $login->template->parse());
  break;
case "Sheets":
  include_once('sheetManagement.inc');
  $mainTemplate->set('loginbox', $login->template->parse());
  break;
case "Person":
  include_once('personManagement.inc');
  $mainTemplate->set('loginbox', $login->template->parse());
  break;
case "Company":
  include_once('cManagement.inc');
  $mainTemplate->set('loginbox', $login->template->parse());
  break;
case "Carrier":
  include_once('cManagement.inc');
  $mainTemplate->set('loginbox', $login->template->parse());
  break;
case "Workspace":
  include_once('workspace.inc');
  $mainTemplate->set('loginbox', $login->template->parse());
  break;
case "Upload":
  include_once('fileUpload.inc');
  $mainTemplate->set('loginbox', $login->template->parse());
  break;
case "Importer":
  include_once('importHandler.inc');
  $mainTemplate->set('loginbox', $login->template->parse());
  break;
case "Import":
  include_once('importXLS.inc');
  $mainTemplate->set('loginbox', $login->template->parse());
  break;
case "Report":
  include_once('reports.inc');
  $mainTemplate->set('loginbox', $login->template->parse());
  break;
case "LOB":
    include_once('lobManagement.inc');
    $mainTemplate->set('loginbox', $login->template->parse());
    break;
 case "ajax":
   include_once('ajax.inc');
   exit;
default:
  include_once('welcomeScreen.inc');
  $pageTitle .= $extendedTitle;
  $mainTemplate->set('loginbox', $login->template->parse());
}
   }
 } else {
   $mainTemplate->set('nodisplay', TRUE);
  $body = $login->template->parse();
}

$mainTemplate->set('title', $pageTitle);
$mainTemplate->set('stylesheets', $stylesheets);
$mainTemplate->set('javascripts', $javascripts);
$mainTemplate->set('self', $self);
$mainTemplate->set('body', $body);
$mainTemplate->display();
 
?>
