<?php/*>*/ ;
// Our Ajax API -- Mostly this is used for paging through result sets.
global $form, $debug;
$debug->level = 0;
switch ( $form->action) {
    case "eligibles":
        eligibles( );
        break;
    case "changes":
        changes( );
        break;
    case "missingPersons":
        missingPersons( );
        break;

    default:
        formView( );
        break;
}

function missingPersons(){
global $db , $debug, $form;
    $importID = $form->importID;
    $companyID = $form->companyID;
    $importSQL = "select r.*, c.name as companyName, p.name as carrierName from rawdatatoc as r, carriers as p, companies as c where r.companyID = c.companyID AND r.carrierID = p.carrierID AND importID = '" . $form->importID . "' AND r.companyID = '$companyID'";
    $db->query( $importSQL);
    $import = $db->nextRecord( );
    $config = unserialize( $import['config']);
    $tablename = $config['type'] . '-' . $config['importID'] . '-' . $config['companyID'] . '-' . $config['carrierID'] . '-' . ( ( $config['worksheet'])?$config['worksheet']:0);
    $oldForm = $config['headerStuff'];
    $headerRow = $oldForm->headerrow;
    $headerSQL = "select r.* from `$tablename` as r where r.rowID = '" . $headerRow . "'";
    $db->query( $headerSQL);
    $debug->line( $headerSQL, 1);
    $headers = $db->nextRecord( );

    $page = $form->page;
    $size = $form->pageSize;
    $limit = "LIMIT " . ( ( $page - 1) * $size) . ", $size";
    $sql = "select p.personID as myID, p.* from people p left join changereport c on p.personID=c.personID  AND c.importID = '$importID' where p.companyID = '$companyID' AND c.personID is null  ORDER BY p.status, p.sn, p.gn " . $limit;

    //$sql = "select r.*, c.* from `$tablename` as r, changereport as c where r.rowID = c.rowID AND c." . $column . " = '$value' AND importID = '" . $form->importID . "' " . $limit;
    $debug->line( $sql, 1);

    $db->query( $sql);
    header( 'Content-type: application/xml');

    print "<ResultSet>\n";
    if ($db->Size() == 0) {
      print "<Item>\n<state>No Records Found</state>\n</Item>\n";
    }
    while ( $row = $db->nextRecord( )) {
        print "<Item>\n";
	print "<name>".htmlentities($row['gn']." ".$row['sn'])."</name>\n";
	print "<city>".htmlentities($row['city'])."</city>\n";
	print "<state>".htmlentities($row['state'])."</state>\n";
	print "<dob>".gmdate(PAITDATE, $row['birthdate'])."</dob>\n";
	print "<status>".htmlentities($row['status'])."</status>\n";
	print "<personID>".htmlentities($row['myID'])."</personID>\n";
        print "</Item>\n";
    }
    $countSQL = "select count(*) as total from people p left join changereport c on p.personID=c.personID  AND c.importID = '$importID' where c.personID is null AND p.companyID = '$companyID'";
    $db->query( $countSQL);
    $res = $db->nextRecord( );
    //    print "<sql>$sql</sql>\n<countSQL>$countSQL</countSQL>\n";
    print "<TotalCount>" . $res['total'] . "</TotalCount>\n";
    
    print "</ResultSet>\n";
    exit;

}


function changes( )
{
    global $db , $debug, $form;
    $importID = $form->importID;
    $column = $form->column;
    $value = $form->value;
    $importSQL = "select r.*, c.name as companyName, p.name as carrierName from rawdatatoc as r, carriers as p, companies as c where r.companyID = c.companyID AND r.carrierID = p.carrierID AND importID = '" . $form->importID . "'";
    $db->query( $importSQL);
    $import = $db->nextRecord( );
    $config = unserialize( $import['config']);
    $tablename = $config['type'] . '-' . $config['importID'] . '-' . ($config['companyID'] - 1) . '-' . $config['carrierID'] . '-' . ( ( $config['worksheet'])?$config['worksheet']:0);
    $oldForm = $config['headerStuff'];
    $headerRow = $oldForm->headerrow;
    $headerSQL = "select r.* from `$tablename` as r where r.rowID = '" . $headerRow . "'";
    $db->query( $headerSQL);
    $debug->line( $headerSQL, 1);
    $headers = $db->nextRecord( );

    $page = $form->page;
    $size = $form->pageSize;
    $limit = "LIMIT " . ( ( $page - 1) * $size) . ", $size";
    $sql = "select r.*, c.* from `$tablename` as r, changereport as c where r.rowID = c.rowID AND c." . $column . " = '".unhtmlize(unhtmlize($value))."' AND importID = '" . $form->importID . "' " . $limit;
    $debug->line( $sql, 1);

    $db->query( $sql);
    header( 'Content-type: application/xml');

    print "<ResultSet>\n";
    // print "<SQLQuery>$sql</SQLQuery>\n";
    while ( $row = $db->nextRecord( )) {
        // pre($row);
        print "<Item>\n";
        foreach( $headers as $cruft => $header) {
            // pre($cruft);
            // pre($header);
            if ( ! is_numeric( $cruft)) {
                if ( is_numeric( $header)) {
                    $header = "rowID";
                }
                $header = preg_replace( '/\s/', '', $header);
                $header = preg_replace( '/\-/', '', $header);
                $header = preg_replace( "/\//", '', $header);
                $header = preg_replace( '/^(\d+)/', 'PAIT$1', $header);
                print "<PAIT" . $header . ">" . htmlentities($row[$cruft]) . "</PAIT" . $header . ">\n";
            }
        }
        print "</Item>\n";
    }
    $countSQL = "select count(*) as total from `$tablename` as r, changereport as c where r.rowID = c.rowID AND c." . $column . " = '".unhtmlize(unhtmlize($value))."' AND importID = '" . $form->importID . "'";
    $db->query( $countSQL);
    print "<SQLStatement>$sql</SQLStatement>\n";
    $res = $db->nextRecord( );
    print "<TotalCount>" . $res['total'] . "</TotalCount>\n";
    print "</ResultSet>\n";
    exit;
}

function eligibles( )
{
    exit;
}

function formView( )
{
    global $form;
    pre( $form);
    exit;
}

?>