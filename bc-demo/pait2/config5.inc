<?PHP
// Config options
// Set up our user db url which is the same as our data db url.
global $AGLOBAL;
$AGLOBAL['USERDBURL'] = 'mysql://pait_dbuser:jhj7l45g@localhost/demo_paitDB';
$AGLOBAL['DEMODBURL'] = $AGLOBAL['USERDBURL'];
$AGLOBAL['AUDITDBURL'] = 'mysql://pait_audit:jhj7l45g@localhost/demo_paitAudit';
$AGLOBAL['SHEETSDBURL'] = 'mysql://pait_audit:jhj7l45g@localhost/demo_paitSheets';

// Can specify stylesheets here. 
global $stylesheets, $javascripts, $pageTitle, $extendedTitle;
$stylesheets = array("styles.css" => "screen");
$javascripts = array();
$pageTitle = "PAIT";
$extendedTitle = ": Policy Administration and Information Tracking"; 

// path for uploaded files:
global $uploadPath;
//$uploadPath = "e:/temp/";
$uploadPath = "../dropsite/drops/";

// Any additional stuff to put on every single page...
global $body;
$body = "";


// Name of the main file (admin.php or admin.php5
global $self;
$self = "index.php5";
$rootDir = '/beta/';


if ($debug->level > 0) {
  $stylesheets['debug.css'] = "screen";
  $javascripts[] = "debug.js";
}



// Person status types. A smart programmer would've made these editable by the administrator users.

global $personStatus;
$personStatus = array( 0 => "Prospect",
		       1 => "Did Not Respond",
		       2 => "Declined",
		       3 => "UW Declined",
		       4 => "Not Taken",
		       5 => "Pending",
		       6 => "Insured",
		       7 => "On Claim",
		       8 => "On Leave",
		       9 => "Deceased",
		       10 => "Resigned and Port Pending",
		       11 => "Resigned and Ported",
		       12 => "Resigned and Dropped",
		       13 => "Cancelled",
		       14 => "Lapsed");




?>
