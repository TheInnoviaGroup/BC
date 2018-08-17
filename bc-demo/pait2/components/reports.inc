<?PHP
global $form;


switch ($form->action) {
case "missingData":
  missingData();
  break;
case "companyDemographics":
  companyDemographics();
  break;
case "changeReport":
  changeReport();
  break;
case "60DaysOut":
  daysOutReport();
  break;
 case "eligibleIncreases":
   eligibleIncreases();
   break;
}

function eligibleIncreases () {
  global $db, $form, $body, $self, $debug, $policyTypes, $login;
  $template = new template('eligibleIncreaseReport.ihtml');
  $template->set('action', $form->action);
  $template->set('self', $self);
  //Okey dokey. First let's get our plan design.
  $pid = $form->planDesignID;
  $gid = $form->groupDesignID;
  $year = $form->year;
  $companyID = $form->companyID;
  if ($form->create && !($pid > 0 && $gid > 0 && $year > 0)) {
    $template->set('picker', 1);
    $template->set('pid', $pid);
    $template->set('gid', $pid);
    $template->set('year', $year);
    $template->set('companyID', $companyID);
    $sql = "select * from plandesign where companyID = '".$form->companyID."' AND policyType = '2'";
    $db->query($sql);
    $template->set('individualDesigns', $db->allRecords());
    $sql = "select * from plandesign where companyID = '".$form->companyID."' AND policyType = '1'";
    $db->query($sql);
    $template->set('groupDesigns', $db->allRecords());
  } else if ($form->create && ($pid > 0 && $gid > 0 && $year > 0)) {
    $sql = "insert into reportqueue set report = 'eligibleIncreases', creator = '".$login->user->user_idnum."', time='".time()."', status = 0, config = '".serialize($form)."', companyID = '$companyID'";
    $db->query($sql);
    $template->set('picker', 3);
    $template->set('message', 'Your report request has been queued.');
    $sql = "select * from reportqueue where report = 'eligibleIncreases' AND companyID = '$companyID'  order by time DESC";
    $db->query($sql);
    if ($companyID) {
      $template->set('companyID', $companyID);
    }
    $template->set('reportList', $db->allRecords());
    $template->set('picker', 2);
  } else if ($form->delete) {
    $reportID = $form->reportID;
    $sql = "delete from eligibleincreases where reportID = '$reportID'";
    $db->query($sql);
    $sql = "delete from reportqueue where reportID = '$reportID'";
    $db->query($sql);
    $template->set('picker', 3);
    $template->set('message', 'Report Deleted');
    $sql = "select * from reportqueue where report = 'eligibleIncreases' AND companyID = '$companyID'  order by time DESC";
    $db->query($sql);
    if ($companyID) {
      $template->set('companyID', $companyID);
    }
    $template->set('reportList', $db->allRecords());
    $template->set('picker', 2);
  } else if ($form->view) {
    $reportID = $form->reportID;
    $sql = "select * from eligibleincreases where reportID = '$reportID' order by individualPercent DESC, individualTotalComp DESC";
    $db->query($sql);
    $results = $db->allRecords();
    $min = $results[0]['min'];
    $max = $results[0]['max'];
    if ($min > 0 && $max > 0) {
      $sql = "select count(*) as eligible from eligibleincreases where reportID = '$reportID' AND individualTotalComp > $min AND individualTotalComp < $max";
    } else if ($min > 0) {
      $sql = "select count(*) as eligible from eligibleincreases where reportID = '$reportID' AND individualTotalComp > $min";
    } else {
      $sql = "select count(*) as eligible from eligibleincreases where reportID = '$reportID' AND individualTotalComp < $max";
    }
    $db->query($sql);
    $res1 = $db->nextRecord();
    $template->set('eligible', $res1['eligible']);
    $template->set('reportID', $reportID);
    $template->set('results', $results);
    $sql = "select * from reportqueue where reportID = '$reportID'";
    $db->query($sql);
    $res = $db->nextRecord();
    $template->set('config', unserialize($res['config']));
    if ($form->format == "xls") {
      $template->file = "eligibleIncreases.ixls";
      $template->display();
      exit;
    }
  } else if ($form->create || $companyID) {
    $sql = "select * from reportqueue where report = 'eligibleIncreases' AND companyID = '$companyID' order by time DESC";
    $db->query($sql);
    if ($companyID) {
      $template->set('companyID', $companyID);
    }
    $template->set('reportList', $db->allRecords());
    $template->set('picker', 2);
  } else {
    showCompanyList();
    return;
  }
  $body .= $template->parse();

  //Nice to have: social, tobacco stat, policy num, benefit amount, premium

}



function changeReport () {
  global $db, $form, $body, $self, $debug;
  $template = new template('changeReport.ihtml');
  $template->set('action', $form->action);
  $template->set('self', $self);
  if ($form->importID) {
    if (! is_numeric($form->drilldown) && ! $form->drilldown) {
      $importSQL = "select r.*, c.name as companyName, p.name as carrierName from rawdatatoc as r, carriers as p, companies as c where r.companyID = c.companyID AND r.carrierID = p.carrierID AND importID = '".$form->importID."'";
      $db->query($importSQL);
      $import = $db->nextRecord();
      $statusSQL = "select statusChange, count(*) as total from changereport where importID = '".$form->importID."' group by statusChange order by statusChange ASC;";
      $db->query($statusSQL);
    $statuses = $db->allRecords();
    $compSQL = "select compensationAdded, count(*) as total from changereport where importID = '".$form->importID."' group by compensationAdded order by compensationAdded DESC;";
      $db->query($compSQL);
    $comps = $db->allRecords();
    $actionSQL = "select action, count(*) as total from changereport where importID = '".$form->importID."' group by action order by action ASC;";
      $db->query($actionSQL);
      $actions = $db->allRecords();
      $policySQL = "select policyAdded, count(*) as total from changereport where importID = '".$form->importID."' group by policyAdded order by policyAdded ASC;";
	$db->query($policySQL);
      $policy = $db->allRecords();
      $template->set('show', 'changeReport');
      $template->set('importID', $form->importID);
      $template->set('status', $statuses);
      $template->set('comp', $comps);
      $template->set('action', $actions);
      $template->set('policy', $policy);
      $template->set('importInfo', $import);
	} else {
	  $column = $form->drilltype;
	  $value = urldecode($form->drilldown);
      $importSQL = "select r.*, c.name as companyName, p.name as carrierName from rawdatatoc as r, carriers as p, companies as c where r.companyID = c.companyID AND r.carrierID = p.carrierID AND importID = '".$form->importID."'";
      $db->query($importSQL);
      $import = $db->nextRecord();
      $config = unserialize($import['config']);
      $tablename = $config['type'].'-'.$config['importID'].'-'.$config['companyID'].'-'.$config['carrierID'].'-'.(($config['worksheet'])?$config['worksheet']:0);
	  $sql = "select r.*, c.* from `$tablename` as r, changereport as c where r.rowID = c.rowID AND c.".$column." = '$value' AND importID = '".$form->importID."'";
	  $debug->line($sql, 1);
	  $db->query($sql);
	  $data = $db->allRecords();
	  $oldForm = $config['headerStuff'];
	  $headerRow = $oldForm->headerrow;
	 
	  $headerSQL = "select r.* from `$tablename` as r where r.rowID = '".$headerRow."'";
	  $db->query($headerSQL);
	  $debug->line($headerSQL, 1);
	  $header = $db->nextRecord();
	  $template->set('headers', $header);
	  $template->set('show', 'drilldown');
	  $template->set('importID', $form->importID);
	  $template->set('importInfo', $import);
	  $template->set('data', $data);
	  $template->set('column', $column);
	  $template->set('value', $value);
	}
  } else {
  }
  $body .= $template->parse();
}

function showCompanyList () {
  global $db, $form, $body, $self, $debug;

  $template = new template('reportCList.ihtml');
  $template->set('action', $form->action);
  $template->set('self', $self);
  $sql = "select name, companyID from companies order by name ASC";
  $db->query($sql);
  $array = array();
  while ($res = $db->nextRecord()) {
    $array[$res['companyID']] = array('companyID'=>$res['companyID'],'name'=>$res['name']);
  }
  $template->set('companies', $array);
  $body .= $template->parse();
}

function missingData () {
  global $db, $form, $body, $self, $debug;
  if (! $form->companyID) {
    showCompanyList();
    return;
  }
  $template = new template('missingDataReport.ihtml');
  $companyID = $form->companyID;
  //Okay. We have a lot of data to grab.

$sql = "select name from companies where companyID = $companyID";
$db->query($sql);
$res = $db->nextRecord();
$template->set('companyName', $res['name']);
// now last census:
$sql = "SELECT *
FROM `rawdatatoc`
WHERE companyID = $companyID
AND TYPE = 'census'
ORDER BY time DESC ;";
$db->query($sql);
$res = $db->nextRecord();
$template->set('lastCensus', $res['time']);

//Now we gotta grab our missing data. Add more here if necessary.
$requiredFields = array('Name' => 'gn,sn',
			'Social Security Number' => 'ssn',
			'Gender' => 'gender',
			'Date of Birth' => 'birthdate',
			'Address' => 'city,state,postal,address');
$missingData = array();
foreach ($requiredFields as $niceName => $field) {
  $where = ' where ';
  if (strstr($field, ',')) {
    $where .= "(";
    $fields = explode(',', $field);
    foreach ($fields as $field1) {
      $where .= "$field1 IS NULL OR $field1 = '' OR";
    }
    $where = substr($where, 0, -2);
    $where .= ")";
  } else {
    $where .= "($field IS NULL or $field = '')";
  }
  $where .= "AND companyID = $companyID";
  
  // Now we get a count. If the count is too high, we just include it. Otherwise, we'll include names.
  // by too high, I mean more than 200.
  $sql1 = "select count(*) as total from people $where";
  $db->query($sql1);

  $res = $db->nextRecord();
  if ($res['total'] > 200) {
    $missingData[$niceName] = $res['total'];
  } elseif ($res['total'] > 0) {
    // Let's get the nice data:
    $sql2 = "select gn, sn, personID from people $where";
    $db->query($sql2);
    $missingData[$niceName] = $db->allRecords();
  }
}
$template->set('self', $self);
$template->set('missingData', $missingData);

$body = $template->parse();

}

function companyDemographics () {
  global $db, $form, $body, $self, $debug;
  if (!$form->companyID) {
    showCompanyList();
    return;
  }
  $template = new template('demoReport.ihtml');
  //Okay. We have a lot of data to grab.
  //First... total by compensation. We grab this in chunks.
  $companyID = $form->companyID;
  $levels = array();
  $levels['More than $2,500,000'] = array('min' => 2500000,
		    'max' => 0);
  $levels['$2,250,000 - $2,500,000'] = array('min' => 2250000,
		    'max' => 2500000);
  $levels['$2,000,000 - $2,250,000'] = array('min' => 2000000,
		    'max' => 2250000);
  $levels['$1,750,000 - $2,000,000'] = array('min' => 1750000,
		    'max' => 2000000);
  $levels['$1,500,000 - $1,750,000'] = array('min' => 1500000,
		    'max' => 1750000);
  $levels['$1,250,000 - $1,500,000'] = array('min' => 1250000,
		    'max' => 1500000);
  $levels['$1,000,000 - $1,250,000'] = array('min' => 1000000,
		    'max' => 1250000);
  $levels['$900,000 - $1,000,000'] = array('min' => 900000,
		    'max' => 1000000);
  $levels['$800,000 - $900,000'] = array('min' => 800000,
		    'max' => 900000);
  $levels['$700,000 - $800,000'] = array('min' => 700000,
		    'max' => 800000);
  $levels['$600,000 - $700,000'] = array('min' => 600000,
		    'max' => 700000);
  $levels['$500,000 - $600,000'] = array('min' => 500000,
		    'max' => 600000);
  $levels['$400,000 - $500,000'] = array('min' => 400000,
		    'max' => 500000);
  $levels['$300,000 - $400,000'] = array('min' => 300000,
		    'max' => 400000);
  $levels['$200,000 - $300,000'] = array('min' => 200000,
		    'max' => 300000);
  $levels['$100,000 - $200,000'] = array('min' => 100000,
		    'max' => 200000);
  $levels['Less than $100,000'] = array('min' => 0,
		    'max' => 100000);
  $template->set('brackets', array_keys($levels));
$totals = array();
  foreach ($levels as $title => $minmax) {

        if ($minmax['min'] > 0 && $minmax['max'] > 0) {
      $sql = "SELECT count(*) AS total, h.year, h.companyID, AVG(p.birthdate) as age
FROM comphistory as h, people as p where h.companyID = $companyID AND h.personID = p.personID
GROUP BY h.year,
h.personID, h.companyID HAVING sum(amount) > ".$minmax['min']." AND sum(amount) < ".$minmax['max']." ORDER BY year DESC";
    } elseif ($minmax['min'] > 0) {
      $sql = "SELECT count(*) AS total, h.year, h.companyID, AVG(p.birthdate) as age
FROM comphistory as h, people as p where h.companyID = $companyID AND h.personID = p.personID
GROUP BY h.year, h.personID HAVING sum(amount) > ".$minmax['min']." ORDER BY year DESC";
    } else {
      $sql = "SELECT count(*) AS total, h.year, h.companyID, AVG(p.birthdate) as age
FROM comphistory as h, people as , where h.companyID = $companyID AND h.personID = p.personID 
GROUP BY h.year,
h.personID, h.companyID HAVING sum(amount) < ".$minmax['max']." ORDER BY year DESC";
} 


	//		pre($sql);
     $db->query($sql);
    while ( $res = $db->nextRecord() ) {
      $totals[$res['year']][$title]['totalage'] += $res['total'];
      $totals[$res['year']][$title]['ageexec'] += 1;
      $totals[$res['year']][$title]['total'] += $res['total'];
      $totals[$res['year']][$title]['age'] += $res['age'];
    }

    if ($minmax['min'] > 0 && $minmax['max'] > 0) {
      $sql = "SELECT count(*) AS total, h.year, h.companyID, SUM(d.hiredate) as tenure
FROM comphistory as h, companypeople as d where h.companyID = $companyID AND h.personID = d.personID
GROUP BY h.year,
h.personID, h.companyID HAVING sum(amount) > ".$minmax['min']." AND sum(amount) <= ".$minmax['max']." ORDER BY year DESC";
    } elseif ($minmax['min'] > 0) {
      $sql = "SELECT count(*) AS total, h.year, h.companyID, SUM(d.hiredate) as tenure
FROM comphistory as h, companypeople as d where h.companyID = $companyID AND h.personID = d.personID
GROUP BY h.year, h.personID HAVING sum(amount) > ".$minmax['min']." ORDER BY year DESC";
    } else {
      $sql = "SELECT count(*) AS total, h.year, h.companyID, SUM(d.hiredate) as tenure
FROM comphistory as h, companypeople as d where h.companyID = $companyID AND h.personID = d.personID
GROUP BY h.year,
h.personID, h.companyID HAVING sum(amount) <= ".$minmax['max']." ORDER BY year DESC";
}
       //  pre($sql);
    $db->query($sql);
    while ( $res = $db->nextRecord() ) {
      //$totals[$res['year']][$title]['totaltenure'] += $res['total'];
      $totals[$res['year']][$title]['totaltenure'] += 1;
      $totals[$res['year']][$title]['tenureexec'] += 1;
      $totals[$res['year']][$title]['tenure'] += ($res['tenure'] / $res['total']);
    }
  }
  //  exit;

    foreach ($totals as $year => $data) {
      foreach ($data as $title => $array) {
	
	
	$array['age'] = ($array['totalage'] > 0)?$array['age'] / $array['totalage']:'0';
	$array['tenure'] = ($array['totaltenure'] > 0)?$array['tenure'] / $array['totaltenure']:'0';
	$totals[$year][$title]['execs'] = ($array['ageexec'] > $array['tenureexec'])?$array['ageexec']:$array['tenureexec'];
	$totals[$year][$title]['age'] = $array['age'];
	$totals[$year][$title]['tenure'] = $array['tenure'];
	if (!($array['total'] > 0)) {
	  $array['total'] = $array['totaltenure'];
	} 
	$totals[$year][$title]['total'] = $array['total'];
      }
    }
    //  pre($totals);

$template->set('totals', $totals);  

 $sql = "select count(*) as total, ucase(state) as myState from people where companyID = $companyID group by ucase(state) order by ucase(state) ASC";
  //pre($sql);
  $db->query($sql);
 $template->set('states', $db->allRecords());

 $sql = "select AVG(amount) as avg, `type` as category, year from comphistory where companyID = $companyID GROUP BY type, year ORDER BY category DESC, year DESC";
  //pre($sql);
    
  $db->query($sql);
 $template->set('averages', $db->allRecords());

$sql = "select gender, min(birthdate) as maxAge, max(birthdate) as minAge, avg(birthdate) as avgAge from people where companyID = $companyID AND gender != 0 AND birthdate IS NOT NULL
AND birthdate != '' AND birthdate != 0 group by gender;";
 //pre($sql);
 $db->query($sql);
$age = array();
while ($res = $db->nextRecord()) {
  $age[$res['gender']] = $res;
}
$sql = "select gender, count(*) as total from people where companyID = $companyID and gender != 0 group by gender";
 //pre($sql);
 $db->query($sql);
while ($res = $db->nextRecord()) {
  $age[$res['gender']]['total'] = $res['total'];
}
$template->set('age', $age);

$sql = "SELECT p.gender, min(c.hiredate) AS maxTenure, max(c.hiredate) AS minTenure, avg(
c.hiredate
) AS avgTenure
FROM people AS p, companypeople AS c
WHERE p.companyID = $companyID
AND p.gender !=0
AND p.personID = c.personID
AND c.hiredate IS NOT NULL
and c.hiredate != 0
AND c.hiredate != ''
GROUP BY p.gender;";
 //pre($sql);
 $db->query($sql);
$tenure = array();
while ($res = $db->nextRecord()) {
  $tenure[$res['gender']] = $res;
}
$template->set('tenure', $tenure);
 $template->set('companyID', $companyID);
$sql = "select name from companies where companyID = $companyID";
 //pre($sql);
 $db->query($sql);
$res = $db->nextRecord();
$template->set('companyName', $res['name']);
 if ($form->format == 'xls') {
   $template->file = 'demoReport.ixls';
   $template->display();
   exit;
 } else {
$body = $template->parse();
 }
}


function daysOutReport () {
  global $db, $form, $body, $self, $debug, $policyTypes, $login, $genders;
  $template = new template('reconsiderationData.ihtml');
  $template->set('action', $form->action);
  $template->set('self', $self);
  //Okey dokey. First let's get our company.
  $companyID = $form->companyID;
  if (($companyID > 0 || $companyID == 'all') && $form->format != 'xls') {
    // grab the people from the company
    // who have a reconsideration date < 60 days from now
    $timeframe = time() + (60 * 86400);
    if ($companyID == 'all') {
    $sql = "SELECT p.gn, p.personID, p.sn, p.cn, p.workphone, p.homephone, i.reconsiderationDate, i.policyNumber FROM people as p, policyinfo as i WHERE i.reconsiderationDate <= '".$timeframe."' AND i.reconsiderationDate > 0 AND i.personID = p.personID ";
    
    $test = "SELECT count(*) as test FROM people as p, policyinfo as i WHERE i.reconsiderationDate <= '".$timeframe."' AND i.reconsiderationDate > 0 AND i.personID = p.personID;";
      
    } else {
    
    $sql = "SELECT p.gn, p.personID, p.sn, p.cn, p.workphone, p.homephone, i.reconsiderationDate, i.policyNumber FROM people as p, policyinfo as i WHERE i.reconsiderationDate <= '".$timeframe."' AND i.reconsiderationDate > 0 AND i.personID = p.personID AND p.companyID = ".$companyID;
    
    $test = "SELECT count(*) as test FROM people as p, policyinfo as i WHERE i.reconsiderationDate <= '".$timeframe."' AND i.reconsiderationDate > 0 AND i.personID = p.personID AND p.companyID = ".$companyID;
    }

  	$db->query($test);
  	
    $res = $db->nextRecord();
    
    $template->set('max', $res['test']);
    $template->set('test', $test);
    $start = $form->start;
    $end = $form->limit;
    //print "Start: $start  Limit: ".$res['test'];
			$limit = " LIMIT $start, $end";
			$template->set('start', $start);
		$sql .= $limit;
  
  	$db->query($sql);
  	$results = $db->allRecords();
  	$template->set('results', $results);
  	$template->set('sql', $sql);
  	
  //	$template->set('test', $test);
  	return $template->parse();
  } else if ($companyID > 0 && $form->format == 'xls') {
    // grab the people from the company
    // who have a reconsideration date < 60 days from now
    $timeframe = time() - (60 * 86400);
    
    $cSQL = "select name from companies where companyID = '$companyID'";
    $db->query($cSQL);
    $row = $db->nextRecord();
    
    $sql = "SELECT p.*, c.*, i.* FROM people as p, companypeople as c, policyinfo as i WHERE i.reconsiderationDate <= '".$timeframe."' AND i.reconsiderationDate > 0 AND i.personID = p.personID AND c.personID = p.personID AND p.companyID = ".$companyID;
    $db->query($sql);
    
    
    $name = "60 Day Reconsideration Report for ".$row['name'];
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
      $temprow[] = $genders[$row[$header]];
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
    
    
    
  }  else {
    showCompanyList();
    return;
  }

}


?>