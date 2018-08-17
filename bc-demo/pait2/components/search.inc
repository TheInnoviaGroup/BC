<?PHP
global $form;


switch ($form->action) {
case "exportXLS":
  exportSaved($form->saveID, FALSE);
  break;
case "exportMailMerge":
  exportSaved($form->saveID, TRUE);
  break;
case "searchCompensation":
  searchCompensation();
  break;
case "searchPerson":
  searchPerson();
  break;
case "searchCompany":
  searchCompany();
  break;
case "searchCarrier":
  searchCarrier();
  break;
case "showCompanyPeople":
  showCompanyPeople();
  break;
case "saveSearch":
  saveSearch();
  break;
case "deleteSearch":
  deleteSearch();
  break;
case "showSaveSearch":
  searchPerson($form->saveID);
  break;

}

function exportSaved($save, $mailmerge = FALSE) {
  global $genders;
  $db = searchPerson($save, NULL, TRUE, TRUE, TRUE);
  $name = $db->name;
  $testrow = $db->nextRecord();
  $name = preg_replace("/\s+/", '', $name);
  header('HTTP/1.1 200 OK');
  header('Date: ' . date("D M j G:i:s T Y"));
  header('Last-Modified: ' . date("D M j G:i:s T Y"));
  header("Content-type: application/octet-stream");
  //    header("Content-type: application/vnd.ms-excel; name=’Excel’");
  // header("Content-Length: " . (string)(strlen($data)) );
  header("Content-Disposition: attachment; filename=\"".$name.".csv\"");
  header("Cache-Control: no-store, no-cache, no-transform, must-revalidate, private");
  //    header("Pragma: no-cache");
  header("Expires: 0");
  $headers = array_keys($testrow);
  $results = array();
  foreach ($headers as $header) {
    if (preg_match('/[A-Za-z]/', $header)) {
      $results[0][] = $header;
    }
  }
  $line = "";
  foreach($results[0] as $value) { 
    if ((!isset($value)) OR ($value == "")) {
      $value = "\t";
    } else {
      $value = str_replace('"', '""', $value);
      $value = '"' . $value . '"' . ",";
    }
    $line .= $value;
  }
  print str_replace("\r","",rtrim($line,"\n\t\r\0 ,")."\n");
  $temprow = array();
  $line = "";
  foreach ($results[0] as $header) {
    //    print "Header: $header, value: $testrow[$header]\n";
    if (stristr($header,'date')) {
      $temprow[] = gmdate(PAITDATE,$testrow[$header]);
     } elseif (stristr($header,'gender')) {
      $temprow[] = $genders[$testrow[$header]];
    } elseif (stristr($header,'amount')) {
      $temprow[] = formatMoney($testrow[$header]);
    } elseif (stristr($header,'phone')) {
      $temprow[] = formatPhone($testrow[$header]);
    } elseif (stristr($header,'ssn')) {
      $temprow[] = formatSocial($testrow[$header]);
      
    } else {
      $temprow[] = ($testrow[$header])?$testrow[$header]:" ";
    }
  }
  
  if (count($temprow) >1){
    foreach($temprow as $value) {      
      
      if ((!isset($value)) OR ($value == "")) {
	$value = "\t";
	  } else {
	$value = str_replace('"', '""', $value);
	$value = '"' . $value . '"' . ",";
      }
      $line .= $value;
    }
    print str_replace("\r","",rtrim($line,"\n\t\r\0 ,")."\n");
  }
      
  while ($row = $db->nextRecord()) {
    $temprow = "";
    foreach ($results[0] as $header) {
      if (stristr($header,'date')) {
      $temprow[] = gmdate(PAITDATE,$row[$header]);
} elseif (stristr($header,'gender')) {
      $temprow[] = $genders[$testrow[$header]];
      } elseif (stristr($header,'amount')) {
      $temprow[] = formatMoney($row[$header]);
      } elseif (stristr($header,'phone')) {
      $temprow[] = formatPhone($row[$header]);
      } elseif (stristr($header,'ssn')) {
      $temprow[] = formatSocial($row[$header]);

      } else {
      $temprow[] = ($row[$header])?$row[$header]:" ";
      }
    }
    //    $results[] = $temprow;
	  $line = "";
if (count($temprow) >1){
	foreach($temprow as $value) {      

	  if ((!isset($value)) OR ($value == "")) {
            $value = "\t";
	  } else {
            $value = str_replace('"', '""', $value);
            $value = '"' . $value . '"' . ",";
	  }
	  $line .= $value;
	}
	print str_replace("\r","",rtrim($line,"\n\t\r\0 ,")."\n");
      }
  }
    global $debug;
   $debug->level = 0; 

    exit;
}


function saveSearch() {
  global $db, $form, $body, $self, $debug;
  $array = array("saveit" => unhtmlize($form->saveit),
	    "name" => $form->savesearchname);
  $sql = $db->prepare('replace', 'savedsearches', $array);
  $debug->line("savesql: $sql", 1);
  $db->query($sql);
  $sql2 = "select * from savedsearches where name = '".$form->savesearchname."'";
  $db->query($sql2, "getID");
  $row = $db->nextRecord("getID");
  searchPerson($row['saveID'], "Search ".$form->savesearchname." successfully saved.");
}

function deleteSearch() {
  global $db, $form, $body, $self, $debug;
  $sql = "delete from savedsearches where saveID = '".$form->saveID."'";
  $db->query($sql);
  include_once('welcomeScreen.inc');
}



function searchCompensation ($save = NULL, $message = NULL, $resultsonly = NULL) {
  global $db, $form, $body, $self, $debug;
  $template = new template('searchcompensation.ihtml');
  $template->set('self', $self);
  if ($save) {
    $sql = "select saveit, name from savedsearches where saveID = '".$save."'";

    $db->query($sql);
    $row = $db->nextRecord();
    $name= $row['name'];
    list($saveit, $sp, $comparison, $limitcomptype, $limitcompany, $limitstatus, $sortby, $limitresults) = explode("###", $row['saveit']);
    if ($saveit != "comp") {
      if ($resultsonly) {
      $results = searchPerson($save, NULL, TRUE);
      return $results;
      } else {
      searchPerson($save);
      return;
      }
    }
    if ($limitcompany == "-") {
      $limitcompany = NULL;
    }  
  if ($limitstatus == "-") {
      $limitstatus = NULL;
    }   
 if ($limitresults == "-") {
      $limitresults = NULL;
    }
  } else {
  $sp = $form->searchcomp;
  if ($form->compcomparer == "More than") {
    $comparison = ">=";
  } elseif ($form->compcomparer = "Less than") {
    $comparison = "<=";
  } else {
    $comparison = "=";
  }
  $limitcomptype = $form->limitcomptype;
  $limitcompany = $form->limitcompany;
  $limitstatus = $form->limitstatus;
  $sortby = $form->sortby;
  $limitresults = $form->limitresults;
  }
  $saveit = "comp###";
  $saveit .= $sp;
  $saveit .= "###".$comparison;

  if ($limitcomptype == -1) {
    $title = "Total Annual Compensation";
  } elseif ($limitcomptype == 0) {
    $title = "Any Compensation Type";
} else {
$types = array_flip(getCompensationTypes());
$title = $types[$limitcomptype];
}  
  $template->set('complabel', $title);
  $template->set('for', $title." ".$comparison." ".formatMoney($sp));
  
  if (preg_match('/[A-Za-z0-9.\$]/', $sp)) {
   $sc = preg_replace('/\$/','',$sp);
   if ($resultsonly) {
     $pdata = "p.*, c.name as companyName, c.planDesign, e.* from people as p, companies as c, companypeople as e,";
     $wdata = "c.companyID = p.companyID and h.personID = p.personID and c.companyID = h.companyID and h.type = t.name and e.personID = p.personID";
   } else {
     $pdata = "p.personID as personID, p.gn as gn, p.sn as sn, p.cn as cn, p.city as city, p.state as state, p.birthdate as birthdate, p.status as status, c.companyID as companyID, c.name as companyName from people as p, companies as c,";
     $wdata = "c.companyID = p.companyID and h.personID = p.personID and c.companyID = h.companyID and h.type = t.name";
   }
  if ($limitcomptype != -1) {
  $sql = "select h.*, t.*, $pdata comphistory as h, comptypes as t where h.amount $comparison $sc AND $wdata ";
  } else {
    $sql = "select h.personID, h.type, SUM(h.amount) as amount, h.companyID, h.year, t.*, $pdata  comphistory as h, comptypes as t where $wdata";

  }
  $highlight = FALSE;
  } else {
$sql = "select p.personID as personID, p.gn as gn, p.sn as sn, p.cn as cn, p.city as city, p.state as state, p.birthdate as birthdate, p.status as status, c.companyID as companyID, c.name as companyName from people as p, companies as c where c.companyID = p.companyID";
$highlight = FALSE;
  }
  if ($limitcomptype > 0) {
    $sql .= " AND t.compTypeID = '".$limitcomptype."'";
  }
    $saveit .= "###".$limitcomptype;
  
  if ($limitcompany > 0) {
    $sql .= " AND c.companyID = '".$limitcompany."'";
    $saveit .= "###".$limitcompany;
  }else{
    $saveit .= "###-";
  }
  if ($limitstatus > 0) {
    global $personStatus;
    $sql .= " AND p.status = '".$personStatus[$limitstatus]."'";
    $saveit .= "###".$limitstatus;
  }else{
    $saveit .= "###-";
  }

  if ($limitcomptype == -1) {
    $sql .= " GROUP BY h.year, h.personID having amount $comparison $sc";
  }

  $sortOrderMap = array( "Last Name" => "cn, sn",
			 "State" => "state, city, postal",
			 "Date of Birth" => "birthdate",
			 "Status" => "status",
			 "Compensation Amount" => "amount");
  $sql .= " ORDER BY status, ".$sortOrderMap[$sortby]." ASC";
  $saveit .= "###".$sortby;


  if ($limitresults > 0) {
    $sql .= " LIMIT 0, ".$limitresults;
    $saveit .="###".$limitresults;
  }else{
    $saveit .= "###-";
  }
  $debug->line($sql, 1);
  $db->query($sql);
  $array = $db->allRecords();
  $template->set('saveit', $saveit);
  $template->set('results', $array);
  if ($resultsonly) {
    array_unshift($array, $name);
    return $array;
  }
  $body .= $template->parse();
  if ($highlight) {
  $args = explode(' ', $sp);
  foreach ($args as $arg) {
  $arg = preg_replace("/[\*\+\-]/", "", $arg);
  $body = preg_replace("/($arg)/i", "<span class=\"highlightme\">$1</span>", $body);
  }
  $body = preg_replace("/<caption>Search results for: .*<\/caption>/", "<caption>Search results for: $sp</caption>", $body);
  $body = preg_replace("/name=\"saveit\" value\=\"comp###\<span class\=\"highlightme\"\>.*\<\/span\>/", "name=\"saveit\" value=\"comp###".urlencode(preg_replace("/[\*\+\-]/","",$sp)), $body);
  }
}

//Holy cow. I'll never remember what all those switches were for.
// John says: Yes it would be nice to know :-)
function searchPerson ($save = NULL, $message = NULL, $resultsonly = NULL, $extended = NULL, $export = NULL) {
  global $db, $form, $body, $self, $debug, $login;
  $template = new template('searchperson.ihtml');
       if ($form->limitname == 'on') {
      $nameOnly = TRUE;
    } else {
      $nameOnly = FALSE;
    }
    $template->set('limitname', $nameOnly);
  if ($save || $form->urlish) {
    if ($save) {
    $sql = "select saveit, name from savedsearches where saveID = '".$save."'";
    $db->query($sql);
    $row = $db->nextRecord();
    $name= $row['name'];
    
    list($sp, $limitcompany, $limitstatus, $sortby, $limitresults,$limitname) = explode("###", $row['saveit']);
    } else {
      list($sp, $limitcompany, $limitstatus, $sortby, $limitresults,$limitname) = explode("###", $form->urlish);
    }
    if (preg_match('/pol-\d+/', $sp)) {
      $temp = explode('-', $sp);
      $sp = $temp[1];
      $searchPolicy = TRUE;
    }
    if ($sp == "comp") {
      if ($resultsonly) {
      $results = searchCompensation($save, NULL, TRUE);
      return $results;
	} else {
	  searchCompensation($save);
	  return;
	}
    }
    if ($limitcompany == "-") {
      $limitcompany = NULL;
    }  
  if ($limitstatus == "-") {
      $limitstatus = NULL;
    }   
 if ($limitresults == "-") {
      $limitresults = NULL;
    }
    if ($limitname != "-" && $limitname) {
      $nameOnly = TRUE;
      $template->set('limitname', $limitname);
    }
  } else {
    if ($form->searchpolicy == 'on') {
      $sp = $form->searchperson;
      $searchPolicy = TRUE;
    } else {
      $sp = $form->searchperson;
      $searchPolicy = FALSE;
    }
    
  if (preg_match('/\d*-\d*\-\d*/', $sp)) {
    $sp = intval(preg_replace('/\-/', '', $sp));
  }
  if (preg_match('/\'/', $sp)) {
    $sp = preg_replace('/\'/', '&#039;', $sp);
  }
  $limitcompany = $form->limitcompany;
  $limitstatus = $form->limitstatus;
  $sortby = $form->sortby;
  $limitresults = $form->limitresults;
  }

  $template->set('for', $form->searchperson);
  $template->set('self', $self);

  $saveit = "";
  if ($searchPolicy) {
    $saveit = 'pol-'.$sp;
  } else {
  $saveit = $sp;
  }
  if ($resultsonly) {
    if($extended) {
      //Put policy info join here.
      $pdata = "*, p.address as homeaddress, p.address2 as homeaddress2 FROM people AS p
LEFT JOIN companypeople AS e ON ( p.personID = e.personID ) 
LEFT JOIN companies AS c ON ( p.companyID = c.companyID ) 
LEFT JOIN policyinfo AS d ON ( p.personID = d.personID ) 
LEFT JOIN carriers AS f ON ( d.carrierID = f.carrierID ) 
";
      $wdata = " 1 ";
      $test = "select count(*) as test from people as p, companies as c, companypeople as e, policyinfo as d, carriers as f"; 
    } else {
      $pdata = "p.*,p.address as homeaddress, p.address2 as homeaddress2,  c.name as companyName, c.planDesign, e.* from people as p, companies as c, companypeople as e";
      $wdata = "c.companyID = p.companyID AND e.personID = p.personID";
      $test = "select count(*) as test from people as p, companies as c, companypeople as e"; 
    }
  } elseif ($searchPolicy) {
    $pdata = "p.personID as personID, p.gn as gn, p.sn as sn, p.cn as cn, p.city as city, p.state as state, p.birthdate as birthdate, p.status as status, c.companyID as companyID, c.name as companyName, i.policyNumber, i.personID from people as p, companies as c, policyinfo as i";
    $wdata = "c.companyID = p.companyID AND i.personID = p.personID group by p.personID";
    $test = "select count(*) as test from people as p, companies as c, policyinfo as i"; 
  } else {
    $pdata = "p.personID as personID, p.gn as gn, p.sn as sn, p.cn as cn, p.city as city, p.state as state, p.birthdate as birthdate, p.status as status, c.companyID as companyID, c.name as companyName from people as p, companies as c";
    $wdata = "c.companyID = p.companyID";
    $test = "select count(*) as test from people as p, companies as c "; 
    
  }
  if (strlen($sp) < 4 && preg_match('/[A-Za-z0-9]/', $sp)) {
    $sql = "select $pdata where (p.sn = '$sp' OR  p.gn = '$sp') AND $wdata";
    $test .= "where p.sn = '$sp' OR  p.gn = '$sp' AND $wdata";
    $highlight = TRUE;
  } else if ($nameOnly && preg_match('/[A-Za-z0-9]/', $sp)) {
    $sql = "select $pdata where (p.gn LIKE '%$sp%' OR p.sn LIKE '%$sp%' OR p.cn LIKE '%$sp%') AND $wdata";
    $test .= "where MATCH (
p.gn, p.mn, p.sn, p.suffix, p.salutation, p.cn, p.ssn, p.city, p.state, p.postal, p.address, p.workphone, p.homephone, p.cellphone, p.status) AGAINST ('$sp' IN BOOLEAN MODE) AND $wdata";
    $highlight = TRUE;
  } elseif ($searchPolicy) {
    $sql = "select $pdata where i.policyNumber LIKE '%$sp%' AND $wdata";
    $test .= "where i.policyNumber LIKE '%$sp%' AND $wdata";
    $highlight = FALSE;
    
  } else if (preg_match('/[A-Za-z0-9]/', $sp)){
    $sql = "select $pdata where MATCH (
p.gn, p.mn, p.sn, p.suffix, p.salutation, p.cn, p.ssn, p.city, p.state, p.postal, p.address, p.workphone, p.homephone, p.cellphone, p.status) AGAINST ('$sp' IN BOOLEAN MODE) AND $wdata";
    $test .= "where MATCH (
p.gn, p.mn, p.sn, p.suffix, p.salutation, p.cn, p.ssn, p.city, p.state, p.postal, p.address, p.workphone, p.homephone, p.cellphone, p.status) AGAINST ('$sp' IN BOOLEAN MODE) AND $wdata";
    $highlight = TRUE;
    
    
  } else {
    $sql = "select $pdata where $wdata";
    $test .= " where $wdata";
    $highlight = FALSE;
  }
  global $login;
  if ($login->user->access_lv == 1 || $login->user->access_lv == 5) {
    $sql .= " AND (";
    $array = allowedCompanies($login->user);
    foreach ($array as $id) {
      $sql .= "p.companyID = '$id' OR ";
    }
    $sql = substr($sql, 0, -3);
    $sql .= ")";
  if ($limitcompany > 0) {
    //    print "1 Limit Company!<br />";
    $sql .= " AND p.companyID = '".$limitcompany."'";
    $test .= " AND p.companyID = '".$limitcompany."'";
    $saveit .= "###".$limitcompany;
  }else{

    $saveit .= "###-";
  }
  
  } else if ($login->user->access_lv == 2){
    if ($limitcompany > 0) {
      //  print "2 Limit Company!<br />";
    $sql .= " AND p.companyID = '".$limitcompany."'";
    $test .= " AND p.companyID = '".$limitcompany."'";
    $saveit .= "###".$limitcompany;
    }else{
      $saveit .= "###-";
    }
  } 
  if (is_numeric($limitstatus)) {
    global $personStatus;
    $sql .= " AND p.status = '".$personStatus[$limitstatus]."'";
    $test .= " AND p.status = '".$personStatus[$limitstatus]."'";
    $saveit .= "###".$limitstatus;
  }else{
    $saveit .= "###-";
  }
  if ($form->sort) {
    $sortOrderMap = array('name' => 'p.sn, p.gn',
			  'city' => 'p.city',
			  'state' => 'p.state',
			  'status' => 'p.status',
			  'dob' => 'p.birthdate');
  $sql .= " ORDER BY ".$sortOrderMap[$form->sort]." ".$form->dir;
  } else {
  $sortby = ($sortby)?$sortby:"Status";
  $sortOrderMap = array( "Last Name" => "sn, gn",
			 "State" => "state, city, postal",
			 "Date of Birth" => "birthdate",
			 "Status" => "status");
  $sql .= " ORDER BY p.status, ".$sortOrderMap[$sortby]." ASC";
  }
  $saveit .= "###".$sortby;
  //$template->set('companyID', $sp);
    if ($resultsOnly || $extended) {
      #do nothing

  } elseif ($limitresults > 0 && !$extended ) {
      //      print "<pre>$test</pre>";
  	$db->query($test);
    $res = $db->nextRecord();

    $sql .= " LIMIT 0, ".$limitresults;
    $saveit .="###".$limitresults;
  }else{
      
  $db->query($test);
    $res = $db->nextRecord();
    $saveit .= "###-";
    if ($res['test'] > 50 && !$extended && !$form->labels) {
      $limit = " LIMIT 0, 50";
    }
    $template->set('max', $res['test']);
    $start = $form->start;
    //print "Start: $start  Limit: ".$res['test'];
  if ($start > 0 && !$extended) {
    $limit = " LIMIT $start, 50";
    $template->set('start', $start);
  } else {
    $template->set('start', 0);
  }
  $sql .= $limit;

  }
  if ($nameOnly) {
    $saveit .= "#on";
  } else {
    $saveit .= "#-";
  }
  $debug->line($sql, 1);
  $start = $form->start;
  $template->set('urlIsh', $saveit);
  $template->set('sql', $sql);
  if (!$form->grid) {
    $array = array('success' => 'true',
		   'openSearchGrid' => array(
					     'title' => 'Search Results for: '.$sp,
					     'url' => $self."?mode=Search&action=searchPerson&grid=1&limitname=".$form->limitname."&urlish=".urlencode($saveit)
					     ));
    print json_encode($array);
    exit;
  }
  //   print "<pre>$test</pre>";
  //    var_dump($res);
  // print "<pre>$sql</pre>";
  $db->query($sql);
  if ($export) {
    $db->name = $name;
    return $db;
  }
  $array = $db->allRecords();
  $template->set('saveit', $saveit);
  $template->set('results', $array);
if ($resultsonly) {
    array_unshift($array, $name);
    return $array;
  }
  $body .= $template->parse();
  if ($highlight) {
  $args = explode(' ', $sp);
  foreach ($args as $arg) {
  $arg = preg_replace("/[\*\+\-]/", "", $arg);
  $body = preg_replace("/($arg)/i", "<span class=\"highlightme\">$1</span>", $body);
  }
  $body = preg_replace("/<caption>Search results for: .*<\/caption>/", "<caption>Search results for: $sp</caption>", $body);
  $body = preg_replace("/name=\"saveit\" value\=\"\<span class\=\"highlightme\"\>.*\<\/span\>/", "name=\"saveit\" value=\"".urlencode(preg_replace("/[\*\+\-]/","",$sp)), $body);
  }
}




function showCompanyPeople () {
  global $db, $form, $body, $self, $login;
  $template = new template('searchperson.ihtml');
  $template->set('self', $self);
  $sp = $form->companyID;
  $start = $form->start;
  $sn = $form->sn;
  if ($form->sort) {
    $orders = array('dob' => 'p.birthdate',
                    'name' => 'p.sn',
                    'city' => 'p.city',
                    'status' => 'p.status, p.sn',
                    'state' => 'p.state');
  $orderBy = $orders[$form->sort].' '.$form->dir;
  } else {
    $orderBy = 'p.status, p.sn, p.gn ASC';
  }
  $template->set('companyID', $sp);
    $test = "select count(*) as test from people where companyID ='$sp' ORDER BY status, sn ASC";
    $db->query($test);
    $res = $db->nextRecord();
    if ($res['test'] > 50) {
      $limit = " LIMIT 0, 50"; 
    }
    $template->set('max', $res['test']);

  if ($start > 0) {
    $limit = " LIMIT $start, 50";
    $template->set('start', $start);
  } else {
    $template->set('start', 0);
  }
  if ($sn) {
    $where = " AND sn LIKE '$sn%'";
    $template->set('sn', $sn);
  }
  $where .= " ORDER BY " .$orderBy.' ';
  $sql = "select p.personID as personID, p.gn as gn, p.sn as sn, p.cn as cn, p.city as city, p.state as state, p.status as status, p.birthdate as birthdate, c.companyID as companyID, c.name as companyName from people as p, companies as c where p.companyID = '$sp' AND c.companyID = p.companyID".$where.$limit;
  //  print "Sql: $sql<br />";
  $db->query($sql);
  $array = $db->allRecords();
  $template->set('sql', $sql);
  $template->set('for', "Employees of ".$array[0]['companyName']);
  $template->set('results', $array);
  if ($login->user->access_lv == 2) {
  $body = $template->parse();
  } else if (($login->user->access_lv == 1 || $login->user->access_lv == 5) && isAllowedCompany($login->user, $sp)) {
    $body = $template->parse();
  }
}

function searchCompany () {
  global $db, $form, $body, $self, $login;
  $template = new template('searchc.ihtml');
  $template->set('type', "Company");
  $template->set('self', $self);
  
  $query = "select * from companies order by name";
  $db->query($query);
  if ($login->user->access_lv == 2) {
  $array = $db->allRecords();
  } else if ($login->user->access_lv == 1  || $login->user->access_lv == 5) {
    $array = array();
    $allowed = allowedCompanies($login->user);
    while($res = $db->nextRecord()) {
      if (in_array($res['companyID'], $allowed)) {
	$array[] = $res;
      }
    }
  }
  $query = "SELECT count( * ) AS insureds, companyID
FROM people
WHERE STATUS = 'Insured'
GROUP BY companyID;";
  $counts = array();
  $db->query($query, 'counting');
  while ($res = $db->nextRecord('counting')) {
    $counts[$res['companyID']] = $res['insureds'];
  }
  $template->set('counts', $counts);
  $template->set('results', $array);
  $body = $template->parse();

}


function searchCarrier () {
  global $db, $form, $body, $self;
  $template = new template('searchc.ihtml');
  $template->set('type', "Carrier");
  $template->set('self', $self);
  $query = "select * from carriers where carrierID != 0";
  $db->query($query);
  $array = $db->allRecords();
  $template->set('results', $array);
  $body = $template->parse();

}
?>