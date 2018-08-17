<?php
$include_path = dirname(__FILE__) ."/alib/;". dirname(__FILE__) ."/components/;". dirname(__FILE__) ."/templates/;";
set_include_path(get_include_path() . ";" . $include_path . ";");
include_once('alib/alib.inc');
include_once('alib/sort.inc');
include_once('alib/time.inc');
include_once('alib/misc.inc');
include_once('components/search.inc');
include_once('config.inc'); //configs.
include_once('general.inc'); // general functions
global $debug, $AGLOBAL, $form, $db;
$debug->level = 0; // 0 to turn off, higher for more output. alib
		   // objects do a lot of chatter at level 11 and higher.
$debug->output = 'pretty';
$debug->line('aLib init complete', 1);


// Can specify stylesheets here. 
$stylesheets = array("styles.css" => "screen");
$javascripts = array("prototype.js", "scriptaculous.js", "sortablemenu.js", "sorttable.js", "windowOps.js");
$pageTitle = "Ethnography";

if ($debug->level > 0) {
  $stylesheets['debug.css'] = "screen";
  $javascripts[] = "debug.js";
}
global $login;
$login = new login("login.ihtml");

 if ($login->loggedIn) {
   if ($login->user->access_lv == 3) {
     //     uploadOnly
     header("Location: https://".$_SERVER['HTTP_HOST']."/dropsite/"); /* Redirect browser */
     /* Make sure that code below does not get executed when we redirect. */
     exit;
   } else {

// Must create a global db. I should make the components smart about this.
$db = new db($AGLOBAL['DEMODBURL']);
// This is to hand our formdata. All of our components use the global $form.
$form = new formhandler(LAZYFORM);

if($form->logo) {

	// they chose a logo
	$dbResults = searchPerson($form->saveID, NULL, TRUE, FALSE, TRUE);

	$name = $dbResults->name;
	// $testrow = $dbResults->nextRecord();
  if($form->logo == "Todd") {
		$logoDiv = "<div class=\"content\"><div class=\"logoTodd\"><center>
		<img src=\"toddLogo.jpg\" width=\"263\" height=\"116\" hspace=\"0\" vspace=\"0\" />
		</center></div>
		<div class=\"return\"><b>Ashlea Stanfill, LUTCF, CLTC</b><br />
	Atlanta Plaza Suite 2300<br />
	950 E. Paces Ferry Rd. &bull; Atlanta, GA 30326<br />
	<div class=\"phone\">(404) 846 3075 Phone &bull; (404) 846 3153 (Fax)</div>
	</div>
	";
	} else {
		$logoDiv = "<div class=\"content\"><div class=\"logoInnovia\"><center>
	<img src=\"innoviaLogo.jpg\" width=\"225\" height=\"62\" hspace=\"0\" vspace=\"0\" />
	</center></div>
	<div class=\"return\"><b>Ashlea Stanfill, LUTCF, CLTC</b><br /><br />
	
						 Atlanta Plaza Suite 2300<br />
	950 E. Paces Ferry Rd. &bull; Atlanta, GA 30326
			 <div class=\"phone\">(404) 846 3075 Phone  &bull;  (404) 846 3153 (Fax)</div>
	</div>";
	}
} else {
	// they didn't choose a logo
	// show the form


}
	
?>
<!DOCTYPE html PUBLIC "-//W3C//DTD XHTML 1.0 Strict//EN"
        "http://www.w3.org/TR/xhtml1/DTD/xhtml1-strict.dtd">
<html xmlns="http://www.w3.org/1999/xhtml">
<head>
	<title>Mailing Labels for <?=$name;?></title>
	<LINK REL=STYLESHEET TYPE='text/css' HREF='style.css'>
<style type="text/css" title="printStyle.css">

.note {
	font-family: verdana, arial, sans-serif;
	font-size: 14px;
	vertical-align: top;
	width: 390px;
	height: 320px;
}

.lastRow {
	font-family: verdana, arial, sans-serif;
	font-size: 14px;
	vertical-align: top;
	width: 390px;
	height: 240px;
}

html {
margin: 0;
}

body {
margin: 0;
}

div.content {
	width: 263px;
}

div.to {
	font-family: verdana, arial, sans-serif;
	font-size: 14px;
	text-align: center;
	position: relative;
	top: 15px;
}
div.toName {
	font-weight: bold;
	font-size: 14px;
}
div.toAddress {
	font-size: 14px;
}
div.toCityStateZip {
	font-size: 14px;
}

div.phone {
	font-size: 8px;
}

div.return {
	font-family: verdana, arial, sans-serif;
	font-size: 10px;
	text-align: center;
	position: relative;
	top: 5px;
	padding-bottom: 5px;
}
div.logoInnovia {
	margin-top: 30px;
}

div.logoTodd {
	margin-top: 5px;
}

@page {
  margin: 2cm;
}

table
{ 
page-break-after: always;
}

</style>
</head>
<body>
<?


  
  
if($form->logo) {

	$col = 1;
	$rows = 1;
	$rowStyle = "note";
	
print "<table border=\"0\" cellspacing=\"0\" cellpadding=\"0\" align=\"center\" bgcolor=\"#FFFFFF\">
	";

	while($activeToken = $dbResults->next_record()) {
			if($col == 2) {
			print "<td class=\"".$rowStyle."\" align=\"right\">".$logoDiv."
			<hr width=\"250\">
			<div class=\"to\">
			<div class=\"toName\">".$activeToken['gn']." ".$activeToken['sn']."</div> 
			<div class=\"toAddress\">".$activeToken['homeaddress']."</div>
			<div class=\"toCityStateZip\">".$activeToken['city'].", ".$activeToken['state']." ".$activeToken['postal']."</div>
			</div>
			</div></td>
			</tr>";
			$col=1;
			$rows=$rows+1;
		} else {
			print "<tr>
			<td class=\"".$rowStyle."\" align=\"left\">".$logoDiv."
			<hr width=\"250\">
			<div class=\"to\">
			<div class=\"toName\">".$activeToken['gn']." ".$activeToken['sn']."</div> 
			<div class=\"toAddress\">".$activeToken['homeaddress']."</div>
			<div class=\"toCityStateZip\">".$activeToken['city'].", ".$activeToken['state']." ".$activeToken['postal']."</div>
			</div>
			</div></td>";
			$col=$col+1;
		}
		if($rows == 4) {
		print "
	</table>
	<table border=\"0\" cellspacing=\"0\" cellpadding=\"0\" align=\"center\" bgcolor=\"#FFFFFF\">
	";
		$rows = 1;
		$rowStyle = "note";
		} elseif($rows == 3) {
		$rowStyle = "lastRow";
		}
		
	}
	// finish out the rows
	while($rows < 4) {
			if($col == 2) {
			print "<td class=\"".$rowStyle."\" align=\"right\">".$logoDiv."
			<hr width=\"250\">
			<div class=\"to\">
			<div class=\"toName\"></div> 
			<div class=\"toAddress\"></div>
			<div class=\"toCityStateZip\"></div>
			</div>
			</div></td>
			</tr>";
			$col=1;
			$rows=$rows+1;
		} else {
			print "<tr>
			<td class=\"".$rowStyle."\" align=\"left\">".$logoDiv."
			<hr width=\"250\">
			<div class=\"to\">
			<div class=\"toName\"></div> 
			<div class=\"toAddress\"></div>
			<div class=\"toCityStateZip\"></div>
			</div>
			</div></td>";
			$col=$col+1;
		}
		if($rows == 4) {
		print "
	</table>
	<table border=\"0\" cellspacing=\"0\" cellpadding=\"0\" align=\"center\" bgcolor=\"#FFFFFF\">
	";
		} elseif($rows == 3) {
		$rowStyle = "lastRow";
		}
	}
	
} else {
	// they didn't choose a logo
	// show the form
?>	
<form method="post" action="labelPrint.php">
<table border="0" cellspacing="0" cellpadding="5" align="center">
	<tr>
		<td align="center"><img src="toddLogo.jpg" width="263" height="116" hspace="0" vspace="0" /></td>
		<td align="center"><img src="innoviaLogo.jpg" width="225" height="62" hspace="0" vspace="0" /></td>
	</tr>
	<tr>
		<td align="center">Todd Logo<br /><input name="logo" id="logo" type="radio" value="Todd" /></td>
		<td align="center">Innovia Logo<br /><input name="logo" id="logo" type="radio" value="Innovia" checked="checked" /></td>
	</tr>
	<tr>
		<td align="center" colspan="2"><input name="saveID" id="saveID" type="hidden" value="<?=$form->saveID;?>" /><input name="Submit" id="Submit" type="submit" value="Submit" /></td>
	</tr>
</table>
</form>

<?	
}	
?>
</body>
</html>
<?
    }} ?>