<?PHP
global $form;


switch (strtolower($form->action)) {
case "save":
  savePD();
  break;
case "delete":
  deletePD();
  break;
case "edit":
  addPD(TRUE);
  break;
case "show":
  showPD();
  break;
case "list":
  listPD();
  break;
default:
  addPD();
  break;
}

function deletePD() {
  global $db, $form, $body, $self, $debug;
  $id = $form->planDesignID;
  $sql = "delete from plandesign where planDesignID = '$id'";
  $db->query($sql);
  addPD(FALSE, TRUE);

}

function savePD() {
  global $db, $form, $body, $self, $debug;
  $id = $form->planDesignID;
  $sql = "select active from plandesign where planDesignID = '$id'";
  $db->query($sql);
  $res = $db->nextRecord();
  if ($res['active'] < 0) {
    $active = $res['active'] + 5;
  $sql = "update plandesign set active = '$active' where planDesignID = '$id'";
  $db->query($sql);
  }
  showPD('Plan Design Successfully Saved.');
}

function showPD($message = NULL) {
  global $db, $form, $body, $self, $debug;
  $template = new template('planDesigner.ihtml');
  $template->set('self', $self);
  $companyID = $form->companyID;
  $template->set('companyID', $companyID);
  $myID = $form->planDesignID;
  $text = planDesignID2Text($myID);
  $template->set('designText', $text);
  $template->set('planDesignID', $myID);
  $template->set('noAction', 'TRUE');
  if ($message) {
    $template->set('message', $message);
  }
  $body = $template->parse();
}

function listPD() {
  global $db, $form, $body, $self, $debug;
if (! $form->companyID) {

  $template = new template('planDCList.ihtml');
  $template->set('action', $form->action);
  $template->set('self', $self);
  $sql = "select name, companyID from companies order by name ASC";
  $db->query($sql);
  $array = array();
  while ($res = $db->nextRecord()) {
    $array[$res['companyID']] = array('companyID'=>$res['companyID'],'name'=>$res['name']);
  }
  $template->set('companies', $array);
  $template->set('list', TRUE);
  $body = $template->parse();
} else {
  $template = new template('planDesignList.ihtml');

  $sql = "select * from plandesign where companyID = $form->companyID";
  $db->query($sql);
  $array = $db->allRecords();
  $template->set('list', $array);
  $template->set('companyID', $form->companyID);
  $sql = "select name from companies where companyID = $companyID";
  $db->query($sql);
  $res = $db->nextRecord();
  $template->set('companyName', $res['name']);
  $template->set('self', $self);
  $body = $template->parse();

}
}

function addPD($edit = FALSE, $startOver = FALSE) {
  global $db, $form, $body, $self, $debug;
if (! $form->companyID || $startOver) {

  $template = new template('planDCList.ihtml');
  $template->set('action', $form->action);
  $template->set('self', $self);
  $sql = "select name, companyID from companies order by name ASC";
  $db->query($sql);
  $array = array();
  while ($res = $db->nextRecord()) {
    $array[$res['companyID']] = array('companyID'=>$res['companyID'],'name'=>$res['name']);
  }
  $template->set('companies', $array);
  $body = $template->parse();

} else {

  $template = new template('planDesigner.ihtml');
  $template->set('self', $self);
  $companyID = $form->companyID;
  $template->set('companyID', $companyID);
  $sql = "select name from companies where companyID = $companyID";
  $db->query($sql);
  $res = $db->nextRecord();
  $template->set('companyName', $res['name']);
  if (!$form->Submit || $edit) {
    
  $template->set('compTypes', getCompensationTypes());
  if ($edit) {
    $id = $form->planDesignID;
    $sql = "select * from plandesign where planDesignID = '$id'";
    $db->query($sql);
    $res = $db->nextRecord();
    $template->set('edits', $res);
    $percents1 = explode(';', $res['percents']);
    $percents = array();
    $count = 0;
    foreach ($percents1 as $cruft) {
      if (strstr($cruft, ':')) {
      list($type, $amount) = explode(':', $cruft);
      $percents[$type] = $amount;
      $count++;
      }
    }
    if ($count == 1) {
      $template->set('showAdvanced', FALSE);
      $template->set('showMinMax', $type);
      $percents['total'] = $amount;
    } else {
      $template->set('showAdvanced', TRUE);

    }
    $template->set('config', unserialize($res['formSerial']));
    $template->set('percents', $percents);
    $maximums1 = explode(';', $res['maximums']);
    $maximums = array();
    foreach ($maximums1 as $cruft) {
      if (strstr($cruft, ':')) {
      list($type, $amount) = explode(':', $cruft);
      $maximums[$type] = $amount;
    }
	  }
    $template->set('maximums', $maximums);
    $minimums1 = explode(';', $res['minimums']);

    $minimums = array();
    foreach ($minimums1 as $cruft) {
      if (strstr($cruft, ':')) {
      list($type, $amount) = explode(':', $cruft);
      $minimums[$type] = $amount;
      }}
    $template->set('minimums', $minimums);
    $template->set('planDesignID', $id);
    $template->set('edit', TRUE);
  }


  } else {

    // pre($form);
    //exit;
    $template->set('edit', FALSE);
    $percent = "";
    $max = "";
    $min = "";
    foreach ($form->percent as $id => $val) {
      if ($id == "total" && $val != "") {
	$percent = $id.":".$val;
	break;
      } else if ($val != "") {
      $percent .= "$id:$val;";
      }
    }
    foreach ($form->max as $id => $val) {
      if ($val != "") {
	if (!strstr($val, '.')) {
	  $val .= ".00";
	}
      $max .= "$id:$val;";
      }
    }
    foreach ($form->min as $id => $val) {
      if ($val != "") {
	if (!strstr($val, '.')) {
	  $val .= ".00";
	}
      $min .= "$id:$val;";
      }
    }
    $active = $form->active;
    $active -= 5;
    if (!$form->planDesignID) {
      $sql = "INSERT into plandesign set companyID = '".$companyID."', title = '".$form->title."', maxBenefit = '".$form->cap."', active = '".$active."', policyType = '".$form->policyType."', percents = '".$percent."', maximums = '$max', minimums = '$min', formSerial = '".serialize($form)."'";
    $db->query($sql);
    $myID = $db->insertID();
    } else {
$sql = "REPLACE into plandesign set companyID = '".$companyID."', title = '".$form->title."', maxBenefit = '".$form->cap."', active = '".$active."', policyType = '".$form->policyType."', percents = '".$percent."', maximums = '$max', minimums = '$min',planDesignID='".$form->planDesignID."', formSerial = '".serialize($form)."'";
    $db->query($sql);

    $myID = $form->planDesignID;
    }
    $text = planDesignID2Text($myID);
    $template->set('designText', $text);
    $template->set('planDesignID', $myID);

  }

  $body = $template->parse();
}
}
?>