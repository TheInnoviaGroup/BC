<?PHP
// Config options
// Set up our user db url which is the same as our data db url.
global $AGLOBAL;
$AGLOBAL['USERDBURL'] = 'mysql://paituser:jhj7l45g@127.0.0.1/demo_paitDB';
$AGLOBAL['DEMODBURL'] = $AGLOBAL['USERDBURL'];
$AGLOBAL['AUDITDBURL'] = 'mysql://paituser:jhj7l45g@127.0.0.1/demo_paitAudit';
$AGLOBAL['SHEETDBURL'] = 'mysql://paituser:jhj7l45g@127.0.0.1/demo_paitSheets';

// Can specify stylesheets here. 
global $stylesheets, $javascripts, $pageTitle, $extendedTitle;
$stylesheets = array("styles.css" => "screen");
$javascripts = array();
$pageTitle = "PAIT";
$extendedTitle = ": Policy Administration and Information Tracking"; 

// path for uploaded files:
global $uploadPath;
$uploadPath = "c:/temp/";
//$uploadPath = "/tmp/";

// Any additional stuff to put on every single page...
global $body;
$body = "";


// Name of the main file (admin.php or admin.php5
global $self;
$self = "index.php";
$rootDir = '/pait2/';


if ($debug->level > 0) {
  $stylesheets['debug.css'] = "screen";
  $javascripts[] = "debug.js";
}





?>
