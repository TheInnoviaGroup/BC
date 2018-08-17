<?
global $debug;
$debugLevel = 0;
$configFile = '../config.inc';
// include_once($configFile);
include_once("../../html/alib/alib.inc");
addIncludePath('../../html/alib');
$debug->output = "firebug";

$db = new idb($config->mainDB);
$sql = "SELECT count(*) as count, module FROM `tracking` WHERE enrollmentID = $config->enrollmentID group by module";

$thisEnrollmentRes = $db->query($sql);

$month = date('m');
$day =  date('d');
$year = date('Y');


$firstThingThisMorning = mktime(0,0,0,$month,$day,$year);

$sql = "SELECT count(*) as count, module FROM `tracking` WHERE `time` >= $firstThingThisMorning AND enrollmentID = $config->enrollmentID group by module";

$todaysEnrollmentRes = $db->query($sql);
?>

<html>
<head><title>Reports</title>
<style type="text/css">
#displayArea {
	margin: 10px;
	border: 1px solid black;
	padding: 5px;
}
</style>
<script src="../ext-min-BC.js" type="text/javascript" language="javascript">
</script>

<script type="text/javascript">
Ext.onReady(function() {
   
   Ext.QuickTips.init();
   // message target    Ext.form.Field.prototype.msgTarget = "side";
      
   var myField = new Ext.form.TextField({
		 id:'searchName',
			fieldLabel:'By First Or Last Name',
			name: 'searchFor',
			width:275,
			allowBlank:false,
			blankText:'Please enter a name'
 	});
 	
 	var myForm = new Ext.FormPanel({
      title:'',
      labelWidth: 1,
      frame:true,
      id: 'searchForm',
      items:[myField]
 	}); 
 	
 	var myButton = new Ext.Button({
         text: 'Search',
         id: 'searchButton',
         handler: function(button,event) {
					doneFunction()
				}
   	});
  
  var myDisplay = new Ext.Panel({
  	id: 'displayArea',
  	html: '<b>Search Results:</b>'
  });

  var mySearchPanel = new Ext.Panel({  	
		renderTo:'myDiv',
		id: 'searchPanel',
  	items: [
			myForm,
			myButton,
			myDisplay
  	]
  });  
  
 var doneFunction = function(form,action) {
 var textFieldCmp = Ext.getCmp('searchName');
 var searchTerm = textFieldCmp.getValue();
 
  var cmp = Ext.getCmp('displayArea');
  cmp.load({
  	url: 'searchName.php5',
    params: {searchFor: searchTerm}, // or a URL encoded string
    nocache: true,
    text: "Loading...",
    timeout: 30,
    scripts: false
  });
 }

});


</script>

</head>
<body>
<table width=100%><tr><td width=75% valign="top">
<h2>Stats: </h2><table width="100%" cellspacing="5" cellpadding="10" align="center" bgcolor="#FFFFFF">
	<tr>
		<td valign="top" style="border:1px solid #99CCFF"><h2>So far today: <?=$month;?>/<?=$day;?>/<?=$year;?></h2>
<table width="300" cellspacing="5" cellpadding="0" align="center" bgcolor="#FFFFFF">
	<tr>
		<td><b>Page</b></td>
		<td><b>Hits</b></td>
	</tr>
<?
$total = 0;
while ($reportRow = $todaysEnrollmentRes->fetch_assoc()): ?>
<tr>
		<td><?=$reportRow['module'];?></td>
		<td><?=$reportRow['count'];?></td>
	</tr>
<? 
$total = $total+$reportRow['count']; 
endwhile ?>
<tr>
		<td align="right"><b>Total</b></td>
		<td><b><?=$total;?></b></td>
	</tr>
	<tr>
		<td colspan="2" align="center"><a href="dailySoFar.php5" title="Download data for today -- from midnight to now">Download As Spreadsheet</a></td>
	</tr>
	
</table></td>
		<td valign="top" style="border:1px solid #99CCFF"><h2>So far this Enrollment</h2>
<table width="300" cellspacing="5" cellpadding="0" align="center" bgcolor="#FFFFFF">
	<tr>
		<td><b>Page</b></td>
		<td><b>Hits</b></td>
	</tr>
<?
$total = 0;
while ($reportRow = $thisEnrollmentRes->fetch_assoc()): ?>
<tr>
		<td><?=$reportRow['module'];?></td>
		<td><?=$reportRow['count'];?></td>
	</tr>
<? 
$total = $total+$reportRow['count']; 
endwhile ?>
<tr>
		<td align="right"><b>Total</b></td>
		<td><b><?=$total;?></b></td>
	</tr>
</table></td>
	</tr>
	<tr>
		<td colspan="2"><h2>Find A User</h2>
		<div id="myDiv"></div>
</td>
	</tr>
</table>
</td><td width=25% valign="top"><h2>CSV Reports: </h2>
<?
$debug->debug("Here");
$dir = ".";
$dh  = opendir($dir);
$files = array();
$directories = array();
while (false !== ($filename = readdir($dh))) {
if (is_dir($filename)) {
if ($filename != '.' && $filename != '..') {
$directories[] = $filename;
}
} else {
if ($filename != __FILE__) {
   $files[] = $filename;
}
}
}
sort($files);
?>
<? foreach ($files as $file): ?>
<? if (stristr($file, '.csv')): ?>
<a href="<?=$file;?>"><?=$file;?></a><br />
<? endif; ?>
<? endforeach;?>
