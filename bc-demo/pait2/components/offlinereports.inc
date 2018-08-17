<?php /*>*/;

function eligibleIncreasesOffline ($reportID) {
  global $db, $form, $body, $self, $debug, $policyTypes;
  $sql = "select * from reportqueue where reportID = '$reportID' AND status = 0";
  $db->query($sql);
  if ($db->Size() == 0) {
    return; // someone's got it already.
  }
  $res = $db->nextRecord();
  $sql = "update reportqueue set status = 2 where reportID = '$reportID'"; // lock it.
  $db->query($sql);
  $form = unserialize($res['config']);

  $pid = $form->planDesignID;
  $gid = $form->groupDesignID;
  $year = $form->year;
  $companyID = $form->companyID;

  $sql = "select * from plandesign where planDesignID = '$pid'";
  $db->query($sql);
  $planDesign = $db->nextRecord();
  $sql = "select * from plandesign where planDesignID = '$gid'";
  $db->query($sql);
  $groupDesign = $db->nextRecord();
  $companyID = ($form->companyID)?$form->companyID:$planDesign['companyID'];
  $temp['percent'] = explode(";", $planDesign['percents']);
  $temp['max'] = explode(";", $planDesign['maximums']);
  $temp['min'] = explode(";", $planDesign['minimums']);
  $maxBenefit = $planDesign['maxBenefit'];
  $temp['gpercent'] = explode(";", $groupDesign['percents']);
  $temp['gmax'] = explode(";", $groupDesign['maximums']);
  $temp['gmin'] = explode(";", $groupDesign['minimums']);
  $gmaxBenefit = $groupDesign['maxBenefit'];
  $params = array();
  foreach ($temp as $key => $array) {
    foreach ($array as $string) {
      if (stristr($string, ':')) {
      $array = explode(':', $string);
      $params[$key][$array[0]] = $array[1]; 
    }
    }
  }
  //Need to fix this for all types.
  if (is_numeric($params['min']['Total Compensation'])) {
    $min = $params['min']['Total Compensation'];
  }
  if (is_numeric($params['max']['Total Compensation'])) {
    $max = $params['max']['Total Compensation'];
  }

  //  var_dump($params);
  $sql = "select p.*, c.* from people as p, companypeople as p where p.companyID = '$companyID' AND p.companyID = c.companyID and (status = 'Prospect'  or status = 'Insured')";
  // print "$sql\n";
  $db->query($sql, 'peopleList');
  $results = array();
  while ($person = $db->nextRecord('peopleList')) {
    //var_dump($person);
    $personID = $person['personID'];
    $results['name'] = $person['gn'].' '.$person['sn'];
    $results['name'] = ($results['name'] == "")?$person['ssn']:$results['name'];
    $results['name'] = ($results['name'] == "")?$person['employeeID']:$results['name'];
    //First get their current benefits.
    $sql = "select benefitAmount from policyinfo where personID = '$personID' AND policyType = 'Group LTD' and policyStatus = '0'";
    $db->query($sql, 'groupCurrent');
    while ($res = $db->nextRecord('groupCurrent')) {
      $results['groupCurrent'] += $res['benefitAmount'];
    }
    $sql = "select benefitAmount from policyinfo where personID = '$personID' AND (policyType = 'Individual LTD' OR policyType IS NULL) and policyStatus = '0'";
    $db->query($sql, 'iCurrent');
    while ($res = $db->nextRecord('iCurrent')) {
      $results['individualCurrent'] += $res['benefitAmount'];
    }

    $compSQL = "select * from comphistory where personID = '$personID' and year = '$year' and companyID = '$companyID'";
    //print ($compSQL);
    $db->query($compSQL, 'compensationList');
    //First figure group benes:
    while ($comp = $db->nextRecord('compensationList')) {
      //var_dump($comp);
      $results['compType'][$comp['type']] = $comp['amount'] / 12;
      //  print "Setting ".$comp['type']." to ". ($comp['amount'] / 12)."\n";;
    }

    if (count($results['compType']) > 0) {


    foreach ($params['gpercent'] as $type => $percent) {
      if ($type == "Total Compensation") {
	foreach ($results['compType'] as $type => $amount) {
	  $results['groupPercentTemp'] += $amount;
	}
	$results['groupPercent'] = $results['groupPercentTemp'] * $percent / 100;
	$results['groupTotalComp'] = $results['groupPercentTemp'];
      } else {
      $results['groupPercent'] += $results['compType'][$type] * $percent / 100;
      $results['groupTotalComp'] += $results['compType'][$type];
      }
    }
    if ($gmaxBenefit > 0 && ($results['groupPercent'] > $gmaxBenefit)) {
      //      print $results['groupPercent'].' > '.$gmaxBenefit.'\n';
      $results['groupPercent'] = $gmaxBenefit;
    }

    foreach ($params['percent'] as $type => $percent) {
      if ($type == "Total Compensation") {
	foreach ($results['compType'] as $type => $amount) {
	  $results['individualPercentTemp'] += $amount;
	}
	$results['individualPercent'] = $results['individualPercentTemp'] * $percent / 100;
	$results['individualTotalComp'] = $results['individualPercentTemp'];
      } else {
      $results['individualPercent'] += $results['compType'][$type] * $percent / 100;
      $results['individualTotalComp'] += $results['compType'][$type];
      }
    }
    
    if ($results['individualPercent'] - $results['groupPercent'] > $maxBenefit) {
      $results['individualPercent'] = $maxBenefit;
    } else {
      $results['individualPercent'] -= $results['groupPercent'];
    }

    $sql = "insert into eligibleincreases set reportID = '$reportID', personID = '$personID', name = '".$results['name']."',
            individualTotalComp = '".$results['individualTotalComp']."', individualPercent = '".$results['individualPercent']."', 
            individualCurrent = '".$results['individualCurrent']."', groupPercent = '".$results['groupPercent']."', 
            groupCurrent = '".$results['groupCurrent']."', min = '".$min."', max = '".$max."'"; 
    $db->query($sql);
    $debug->line('Results for: '.$results['name'], 6);
    $debug->variable($results ,6);
    unset($results);
    $results = array();
    //    if (($params['min']['Total Compensation'] > 0 && $results[$personID]['individualTotalComp'] < $params['min']['Total Compensation']) || 
    //	($params['max']['Total Compensation'] > 0 && $results[$personID]['individualTotalComp'] > $params['max']['Total Compensation'])
    //	) {
    //	unset($results[$personID]);
    //	}
    } else {
      unset($results[$personID]);
    }


  }


  $sql = "update reportqueue set status = 1 where reportID = '$reportID'"; // unlock it.
  $db->query($sql);
}




?>