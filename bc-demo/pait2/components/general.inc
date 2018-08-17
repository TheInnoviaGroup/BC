<?PHP

function formatPhone($number) {
  $number = preg_replace("/[^0-9]/","",$number);
  $npa = substr($number, 0,3);
  $nxx = substr($number, 3,3);
  if (strlen($number) == 10) {
  $rest = substr($number, 6);
  $string = "$npa-$nxx-$rest";
  } else {
    $rest = substr($number,6,4);
    $ext = substr($number,10);
    $string = "$npa-$nxx-$rest x$ext";
  }
  return $string;

}

function companyName($id) {
  global $db;
  $sql = "select name from companies where companyID = '$id'";
  $db->query($sql, 'companyNameFunction');
  $res = $db->nextRecord('companyNameFunction');
  return $res['name'];
}

function gimmeUserFullName($user_idnum) {
global $db;
	$sql = "SELECT gn, sn FROM users WHERE user_idnum = '".$user_idnum."'";

	$db->query($sql, 'getUserName');
    if ($db->Size('getUserName') > 0) {
    	$theUser = $db->nextRecord('getUserName');
      return $theUser['gn']." ".$theUser['sn'];
    } else {
      return false;
    }

}

//Even numbers cannot be acted on by the user.
global $uploadStatuses;
$uploadStatuses = array( 0 => 'Pending Excel Import',
			 1 => 'Ready for Data Assignment',
			 2 => 'Pending Final Data Reconciliation',
			 3 => 'Complete, Pending Review',
			 7 => 'Data Reconciliation Aborted, Errors Detected',
			 5 => 'Complete',
			 20 => 'Processing',
			 21 => 'Processing Error');


global $genders;
$genders = array( 0 => 'Unknown',
		  1 => 'Female',
		  2 => 'Male');

define(PAITDATE, "m-d-Y");
function multiline($text) {
  $retval = preg_replace("/\n/", "<br />\n", $text);
  return $retval;
}

function getCompensationTypes() {
  global $db;
  $sql = "select * from comptypes;";
  $db->query($sql, "comptypes");
  while ($res = $db->nextRecord("comptypes")) {
    $retval[$res['name']] = $res['compTypeID'];
  }
  return $retval;
}



global $policyStatuses;
$policyStatuses = array( 0 => "Active",
		       1 => "Proposed",
			 2 => "Inactive",
			 3 => "OEC",
			 4 => "Substandard");
#When we make reports, only Active are eligible for increase.

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
		       14 => "Lapsed",
		       15 => "App Withdrawn",
		       16 => "App Expired",
		       17 => "Proposal",
		       18 => "Previously Solicited",
		       19 => "App Received",
		       20 => "HR Revoked");

global $policyTypes;
$policyTypes = array("Group LTD", "Individual LTD", "LTC", "Life", "Key Person", "Spouse: Group LTD", "Spouse: Individual LTD", "Spouse: LTC", "Spouse: Life", "Spouse: Key Person");

function formatMoney($text) {
  $retval = "$".number_format($text,2);
  return $retval;
}

function formatSocial($text) {
  $temp = preg_replace("/[^0-9]*/", "", $text);
  $temp = str_pad($temp, 9, "0", STR_PAD_LEFT);
  $one = substr($temp, 0, 3);
  $two = substr($temp, 3, 2);
  $three = substr($temp, 5);
  $retval = $one ."-". $two ."-". $three;
  return $retval;

}

// Unhtmlize a saved form variable. Use this if you want to display
// the raw html that the form object sanitized for storage in the
// DB. Second arg is boolean of whether or not to make it safe for
// javascript.

function unhtmlize($string, $jssafe = FALSE) {
  $trans_tbl = get_html_translation_table (HTML_ENTITIES );
  $trans_tbl = array_flip ($trans_tbl );
  $s = strtr ($string ,$trans_tbl );
  if ($jssafe) {
    $s = preg_replace('/\r\n/',"\\n",$s);
  }
  return $s;
}

// turn plan design ID into text.

function planDesignID2Text($id) {
  global $db, $policyTypes;
  $sql = "select * from plandesign where planDesignID = '$id'";
  $db->query($sql);
  $res = $db->nextRecord();
  $sql2 = "select name from companies where companyID = '".$res['companyID']."'";
  $db->query($sql2);
  $res2 = $db->nextRecord();
  $text = $res['title'].": (".$policyTypes[$res['policyType'] - 1].") ";
  $percents = explode(";", $res['percents']);
  $maximums = explode(";", $res['maximums']);
  $minimums = explode(";", $res['minimums']);
  $config = unserialize($res['formSerial']);
  $max = array();
  $min = array();
  
  foreach ($percents as $percent) {
    if (strstr($percent, ':')) {
    $type = "";
    $percentage = "";
    list($type, $percentage) = explode(':', $percent);
    $type = ($type == "total")?"Total Compensation":$type;
    $text .= " $percentage% of ".strtolower($type)." and";
    }
  }
  $text = substr($text,0,-4);
  $text .= " to a maximum of ".formatMoney($res['maxBenefit'])." ";
  if (strstr($res['maximums'], ':') || strstr($res['minimums'], ':')) {
    $text .= "limited to persons with a ";
    foreach ($maximums as $maxi) {
      if (strstr($maxi, ':')) {
      list($type, $amount) = explode(':', $maxi);
      $text .= "maximum ".strtolower($type)." of ".formatMoney($amount)." and a";
      }
    }
  
    foreach ($minimums as $mini) {
      if (strstr($mini, ':')) {
      list($type, $amount) = explode(':', $mini);
      $text .= "minimum ".strtolower($type)." of ".formatMoney($amount)." and a";
      }
    }
  $text = substr($text,0,-5);
  }
  $text .= ".";
			$exclusions = count($config->eRiskNumber);
			$inclusions = count($config->iRiskNumber);
			$gtArray = array('gt' => ' benefit more than ',
				    'lt' => ' benefit less than ',
				    'eq' => ' benefit of ');
			if (count($config->eRiskNumber) > 0 && count($config->iRiskNumber) > 0) {
			  if ($exclusions > 1) {
			  $text .= " This is excluding anyone holding policies with risk numbers of ";
			  } else {
			  $text .= " This is excluding anyone holding a policy with a risk number of ";
			  }
			  for ($i = 1; $i <= $exclusions; $i++) {
			    $text .= $config->eRiskNumber[$i];
			    if ($config->eRiskValue[$i] != "") {
			      $text .= "(".$gtArray[$config->eRiskRelative[$i]].formatMoney($config->eRiskValue[$i]).")";
			    }
			    $text .= ", ";
			  }
			  $text = substr($text, 0, -2);
			  $text .= ". ";

			  if ($inclusions > 1) {
			  $text .= " This also includes only those holding policies with risk numbers of ";
			  } else {
			  $text .= " This also includes only those holding a policy with a risk number of ";
			  }
			  for ($i = 1; $i <= $inclusions; $i++) {
			    $text .= $config->iRiskNumber[$i];
			    if ($config->iRiskValue[$i] != "") {
			      $text .= "(".$gtArray[$config->iRiskRelative[$i]].formatMoney($config->iRiskValue[$i]).")";
			    }
			    $text .= ", ";
			  }
			  $text = substr($text, 0, -2);
			  $text .= ". ";
			  
			} else if ($exclusions > 0) {
			  if ($exclusions > 1) {
			    $text .= " This is excluding anyone holding policies with risk numbers of ";
			  } else {
			    $text .= " This is excluding anyone holding a policy with a risk number of ";
			  }
			  for ($i = 1; $i <= $exclusions; $i++) {
			    $text .= $config->eRiskNumber[$i];
			    if ($config->eRiskValue[$i] != "") {
			      $text .= "(".$gtArray[$config->eRiskRelative[$i]].formatMoney($config->eRiskValue[$i]).")";
			    }
			    $text .= ", ";
			  }
			  $text = substr($text, 0, -2);
			  $text .= ". ";

			} else if ($inclusions > 0) {
			  if ($inclusions > 1) {
			  $text .= " This includes only those holding policies with risk numbers of ";
			  } else {
			  $text .= " This includes only those holding a policy with a risk number of ";
			  }
			  for ($i = 1; $i <= $inclusions; $i++) {
			    $text .= $config->iRiskNumber[$i];
			    if ($config->iRiskValue[$i] != "") {
			      $text .= "(".$gtArray[$config->iRiskRelative[$i]].formatMoney($config->iRiskValue[$i]).")";
			    }
			    $text .= ", ";
			  }
			  $text = substr($text, 0, -2);
			  $text .= ". ";

			}
  return $text;
  
}

function isAllowedCompany($user, $companyID) {
  global $db, $debug;
  $sql = "select companyID from allowedcompanies where user_idnum = '".$user->user_idnum."' AND companyID = $companyID";
  $debug->line($sql, 2);
  $db->query($sql, 'isAllowed');
  $allowed = FALSE;
  if ($db->Size('isAllowed') > 0) {
    $allowed = TRUE;
  } else if ($user->access_lv == 2) {
    $allowed = TRUE;
  }
  
  return $allowed;
}

function allowedCompanies($user) {
  global $db;
  if (is_numeric($user)) {
    $userID = $user;
  } else {
    $userID = $user->user_idnum;
  }
  $sql = "select companyID from allowedcompanies where user_idnum = '".$userID."'";
  $db->query($sql, 'allowedCompanies');
  $retval = array();
  while ($res = $db->nextRecord('allowedCompanies')) {
    $retval[] = $res['companyID'];
   }
  return $retval;
}


function paitSerialize($string) {
$retval = serialize($string);
$retval = preg_replace("#;#msi",'0x3B',$retval);
$retval = preg_replace("/(\n)|(\r)/"," ",$retval);
$retval = addcslashes($retval, "\0..\37!@\@\177..\377");
$retval = addslashes($retval);

 return $retval;
}

function paitUnserialize($string) {
  $retval = stripslashes($string);
  $retval = stripcslashes($retval);
  $retval = preg_replace("#0x3B#msi",';',$retval);
  $retval = unserialize($retval);
  return $retval;
}


function createSheetDB() {
    global $AGLOBAL, $sheetDB;
    if ($sheetDB instanceof db) {
        return $sheetDB;
    } else {
        $sheetDB = new db($AGLOBAL['SHEETDBURL']);
        return $sheetDB;
    }
    
}

?>