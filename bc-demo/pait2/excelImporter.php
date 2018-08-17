<?PHP

error_reporting(E_ALL);
ini_set('display_errors','On');

$include_path = "./templates;./components";
set_include_path(get_include_path() . ";" . $include_path . ";");
include_once('../alib/alib.inc');
include_once('../alib/sort.inc');
include_once('../alib/time.inc');
include_once('../alib/misc.inc');
//include_once('../alib/cronutil.inc');
include_once('config.inc'); //configs.
include_once('general.inc'); // general functions and objects
global $debug, $AGLOBAL, $form, $db;
$debug->level = 6; // 0 to turn off, higher for more output. alib
		   // objects do a lot of chatter at level 11 and higher.
$debug->output = 'plain';
$debug->line('aLib init complete', 1);

$processName = "excelImporter";

// Must create a global db. I should make the components smart about this.
$db = createSheetDB();

//if (cronLock($processName, $db)) {
  $sql = "select * from sheets where status = '0'";
  $db->query($sql, 'dataTOC');
  $debug->variable($db,1);
  if ($db->Size('dataTOC') > 0) {
    $sql2 = "update sheets set status = '20' where status = '0'";
    $db->query($sql2, 'dataTOClock');
   
    while ($config = $db->nextRecord('dataTOC')) {
    //  pre($config);
      $debug->line('importing...',1);
      $debug->variable($config,1);
      importXLSFromFile($config);
    }
  }
// cronUnLock($processName, $db);
//} else {
//  exit;
//}

function importXLSFromFile($config) {
  global $db, $debug, $uploadPath;
  print "Including parser...\n";
  require_once('excelparser.php');
  //if (include_once('excelparser.php')) {
    global $debug;
    $debug->line('included excelparser.php', 1);
 // }
  print "Including crappy date functions.\n";
  include_once('exceldate.php');
  $debug->variable($config, 3);
  $xl = new ExcelFileParser('excel.log', ABC_DEBUG);
  $error = $xl->ParseFromFile( $uploadPath.$config['filename'] );
  $debug->variable($error,1);
  if ($config['worksheet']) {
    $ws_n = $config['worksheet'];
  } else {
    $ws_n = 0;
  }
  $ws = $xl->worksheet['data'][$ws_n]; // Get worksheet data
  $config['worksheetName'] = $ws['name'];
  $db_table = 'sheet-'.$config['sheetID'];
  $debug->variable($ws, 6);
  $max_row = $ws['max_row'];
  $max_col = $ws['max_col'];
  $debug->line('max_row: '.$max_row.' max_col: '.$max_col, 3);
	$tbl_SQL .= 'CREATE TABLE IF NOT EXISTS `'.$db_table.'` ( rowID int(64) NOT NULL auto_increment,';
	$fieldcheck = array();
	$fieldname = array();
	for ( $j = 0; $j <= $ws['max_col']; $j++ ) {
	  	$tbl_SQL .= "field".$j. " text NOT NULL,";
		$fieldcheck[$j] = TRUE;
		$fieldname[$j] = "field".$j;
	}

	
	$tbl_SQL .= "PRIMARY KEY  (`rowID`)) TYPE=MyISAM AUTO_INCREMENT=0";
	if ( $max_row > 0 && $max_col >= 0 ) {
			$SQL = prepareTableData ( $xl, $ws, $fieldcheck, $fieldname );
	}
		else {
		  // fatal("Empty worksheet");
		  $config['error'] = "Empty Worksheet. Processing Stopped.";
		  $statusSQL = "update sheets set status = -1,errors = '".serialize($config)."' where sheetID = '".$config['sheetID']."'";

	}
	
	if (empty ( $SQL )) {
	  //fatal("Output table error");
		  $config['error'] = "Table Error. Processing Stopped.";
		  $statusSQL = "update sheets set status = -1,errors = '".serialize($config)."' where sheetID = '".$config['sheetID']."'";

	}
	//Okay.
	$debug->line($tbl_SQL, 1);
	$db->query($tbl_SQL); // Create the table.
		$sql_pref = 'INSERT INTO `' . $db_table . '` SET ';
	
	$err = "";	
	$nmb = 0; // Number of inserted rows
	
	foreach ( $SQL as $sql ) {
	
		$sql = $sql_pref . $sql;
		//print "<pre>$sql\n</pre>\n";
		$debug->line($sql, 2);
		if ( !$db->query($sql) ) {
		$err .= "<b>SQL error in</b> :<br>$sql <br>";
			
		}
		else $nmb++;
			
	}

	if ( empty ($err) ) {
	  $config['numberRowsImported'] = $nmb;
	  $statusSQL = "update sheets set status = 1 where sheetID = '".$config['sheetID']."';";
          $db->query("insert into reports set sheetID = '".$config['sheetID']."', status=2, type='missingData', name='Missing Data', owner='".$config['owner']."', time='".time()."';", "report");
	}
	$db->query($statusSQL);

}


// The below are all from the zakkis sample files for the excel
// importer. I haven't even looked at them to see what I need and what
// I don't.


function print_error( $msg )
	{
		print <<<END
		<tr>
			<td colspan=5><font color=red><b>Error: </b></font>$msg</td>
			<td><font color=red><b>Rejected</b></font></td>
		</tr>

END;
	}

function getHeader( $exc, $data )
{
		// string
	
		$ind = $data['data'];
		if( $exc->sst[unicode][$ind] )
			return convertUnicodeString ($exc->sst['data'][$ind]);
		else
			return $exc->sst['data'][$ind];

}


function convertUnicodeString( $str )
{
	for( $i=0; $i<strlen($str)/2; $i++ )
	{
		$no = $i*2;
		$hi = ord( $str[$no+1] );
		$lo = $str[$no];
		
		if( $hi != 0 )
			continue;
		elseif( ! ctype_alnum( $lo ) )
			continue;
		else
			$result .= $lo;
	}
	
	return $result;
}

function uc2html($str) {
	$ret = '';
	for( $i=0; $i<strlen($str)/2; $i++ ) {
		$charcode = ord($str[$i*2])+256*ord($str[$i*2+1]);
		$ret .= '&#'.$charcode;
	}
	return $ret;
}



function get( $exc, $data )
{
  //  print "Type: ".$data['type']." for: ".$data['data']." <br />";
	switch( $data['type'] )
	{
		// string
	case 0:
		$ind = $data['data'];
		if( $exc->sst[unicode][$ind] )
			return uc2html($exc->sst['data'][$ind]);
		else
			return $exc->sst['data'][$ind];
break;
		// integer
	case 1:
		return $data['data'];
break;
		// float
	case 2:
		return $data['data'];
            break;
        case 3:
	  return $data['data'];
        break;
	  // return str_replace ( " 00:00:00", "", gmdate("d-m-Y H:i:s",$exc->xls2tstamp($data[data])) );

	default:
		return $data['data'];
	}
}



function fatal($msg = '') {
	echo '[Fatal error]';
	if( strlen($msg) > 0 )
		echo ": $msg";
	echo "<br>\nScript terminated<br>\n";
	if( $f_opened) @fclose($fh);
	exit();
}



function getTableData ( &$ws, &$exc ) {
		
	global $excel_file, $db_table;
	global $db_host, $db_name, $db_user, $db_pass;
	
	if ( !isset ( $_POST['useheads'] ) )
		$_POST['useheads'] = "";
	
	$data = $ws['cell'];
	
echo <<<FORM

	<form action="" method="POST" name="db_export">
	<table border="0" cellspacing="1" cellpadding="2" align="center" bgcolor="#666666">
	<tr bgcolor="#f1f1f1">

FORM;

// Form fieldnames
	
if ( !$_POST['useheaders'] ) {
	for ( $j = 0; $j <= $ws['max_col']; $j++ ) {

		$field = "field" . $j;
						
		echo <<<HEADER

		<td>
		<input type="checkbox" name="fieldcheck[$j]" value="$j" checked title="Check to proceed this field">
		<input type="text" name="fieldname[$j]" value="$field" title="Field name">
		</td>

HEADER;
	}
}
else {
	for ( $j = 0; $j <= $ws['max_col']; $j++ ) {
		
		$field = getHeader ( $exc, $data[0][$j] );
			
		$field = ereg_replace ( "^[0-9]+", "", $field );
		
		if (empty ($field) )
			$field = "field" . $j;
		
		echo <<<HEADER

		<td>
		<input type="checkbox" name="fieldcheck[$j]" value="$j" checked title="Check to proceed this field">
		<input type="text" name="fieldname[$j]" value="$field" title="Field name">
		</td>

HEADER;
	}
}

	
	echo "</tr>";
	
	foreach( $data as $i => $row ) { // Output data and prepare SQL instructions
		
				
		if ( $i == 0 && $_POST['useheaders'] )
		continue;
		
		echo "<tr bgcolor=\"#ffffff\">";
		
		for ( $j = 0; $j <= $ws['max_col']; $j++ ) {
		
			$cell = get ( $exc, $row[$j] );
			echo "<td>$cell</td>";
					
		}
		
		echo "</tr>";
		$i++;
	}
	
	if ( empty ( $db_table ) )
		$db_table = "Table1";
					
echo <<<FORM2
	
	</table><br>
	<table align="center" width="390">
	<tr><td>Table name:</td><td>&nbsp;<input type="text" name="db_table" value="$db_table"></td></tr>
	<tr><td>Drop table if exists:</td><td><input type="checkbox" name="db_drop" checked></td></tr>
	<tr><td colspan="2">
	<i>Uncheck this option to add data into the existing table.<br><font color="red">
	Note that if you have mismatch in fieldnames in database and fieldnames in outputting data will be errors!</td></tr>
	<tr><td>Database host:</td><td>&nbsp;<input type="text" size=30 name="db_host" value="$db_host"></td></tr>
	<tr><td>Database name:</td><td>&nbsp;<input type="text" size=30 name="db_name" value="$db_name"></td></tr>
	<tr><td>Database user:</td><td>&nbsp;<input type="text" size=30 name="db_user" value="$db_user"></td></tr>
	<tr><td>Database password:</td><td>&nbsp;<input type="password" size=30 name="db_pass" value="$db_pass"></td></tr>
	<tr><td></td><td><input type="hidden" name="excel_file" value="$excel_file">
	<input type="hidden" name="useheaders" value="$_POST[useheaders]">
	<input type="hidden" name="step" value="2">
	&nbsp;<input type="submit" name="submit" value="Output"></td></tr>
	</form>
	</table>
	<br>&nbsp;
<div align="right">
<a href="http://www.zakkis.ca" style="font-size: 9px; text-decoration: none; font-family: Verdana, Geneva, Arial, Helvetica, sans-serif;">ZAKKIS Tech. All Rights Reserved.</a>&nbsp;&nbsp;
</div>
FORM2;

} 



function prepareTableData ( &$exc, &$ws, $fieldcheck, $fieldname ) {
	
			
	$data = $ws['cell'];
	
	foreach( $data as $i => $row ) { // Output data and prepare SQL instructions
		
				
		if ( $i == 0 && $_POST['useheaders'] )
			continue;
		
		$SQL[$i] = "";
		
		for ( $j = 0; $j <= $ws['max_col']; $j++ ) {
		
			if ( isset($fieldcheck[$j]) ) {
			
								
				$SQL[$i] .= $fieldname[$j];
				$SQL[$i] .= "=\"";
				$SQL[$i] .= addslashes ( get ( $exc, $row[$j] ) );
				$SQL[$i] .= "\"";
				
				$SQL[$i] .= ",";
			}
		
				
		}
		
		$SQL[$i] = rtrim($SQL[$i], ',');
		
		$i++;
	}
	
	return $SQL;	
			
} 


?>
