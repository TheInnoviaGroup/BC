<?PHP
$cTemplate = new template('cManagement.ihtml');
global $body, $form, $self;
$cTemplate->set("self", $self);
switch ($form->action) {
 case "editCompany":
   addCompany($cTemplate);
   break;
 case "addCompany":
   addCompany($cTemplate);
   break;
 case "addCarrier":
   addCarrier($cTemplate);
   break;
 case "showCompany":
   showCompany($cTemplate);
   break;
 case "showCarrier":
   showCarrier($cTemplate);
   break;
 case "addContact":
   addContact($cTemplate);
   break;
 case "editContact":
   addContact($cTemplate);
   break;
 case "deleteContact":
   deleteContact($cTemplate);
 }


function deleteContact($template) {
  global $db, $form, $body;
  $cn = $form->cn;
  $idname = strtolower($form->mode) . "ID";
  $mode = strtolower($form->mode);
  $company = $form->$idname;
  $sql = "delete from ".strtolower($mode)."contacts where cn = '$cn' and $idname = '$company'";
  $db->query($sql);
    if ($form->mode == "Company") {
      showCompany($template, $id);
      return;
    } else {
      showCarrier($template, $id);
      return;
    }

  
}

function addContact($template) {
  global $db, $form, $body;
  if ($form->type) {
    $type = $form->type;
  } else {
    $type = strtolower($form->mode);
  }
    $id = $type . "ID";
  if ($form->Submit) {
    $array = array('cn' => $form->name,
	      'title' => $form->title,
	      'workphone' => preg_replace('/[^\d]+/', '', $form->work),
	      'cellphone' => preg_replace('/[^\d]+/', '', $form->cell),
		   'dob' => $form->dob,
		   'hiredate' => $form->hiredate,
	      'email' => $form->email,
	      'address' => $form->address,
	      'city' => $form->city,
	      'state' => $form->state,
	      'postal' => $form->postal,
	      'fax' => $form->fax,
	      $id => $form->$id);
    if ($form->contactID) {
      $array['contactID'] = $form->contactID;
}
    $sql = $db->prepare('replace', $type . "contacts", $array);
    $db->query($sql);
    if ($type == "company") {
      showCompany($template, $form->$id);
      return;
    } else {
      showCarrier($template, $form->$id);
      return;
    }



  } else {
    global $debug;
    $template->set('action', 'addContact');
    if ($form->action == "editContact") {
      $sql = "select * from ".$type."contacts where contactID = '".$form->contactID."' AND $id = '".$form->$id."'";
      $debug->line($sql, 1);
      $db->query($sql);
      $row = $db->nextRecord();
      $template->set('contactInfo', $row);
    }
    if ($form->mode == "Company") {
      $template->set('companyID', $form->companyID);
    } else {
      $template->set('carrierID', $form->carrierID);
    }
  }
  $body = $template->parse();
}


function showCompany($template, $id = NULL) {
  global $db, $form, $body, $login;
  $allowed = NULL;
  if ($login->user->access_lv > 1) { $allowed = TRUE; }
  $companyID = ($id)?$id:$form->companyID;
  $template->set('action', 'showCompany');
  # Now we have to get info from all over.
  $sql = "select * from companies where companyID = '".$companyID."'";
  $db->query($sql, 'companyRecord');
  if ($companyData = $db->nextRecord('companyRecord')) {
    if ($companyData['owner'] == $login->user->username) { $allowed = TRUE; }
    $rulesSQL = "select * from companyrules where companyID = '".$companyID."'";
    $db->query($rulesSQL, 'rules');
    if ($db->Size('rules') > 0) {
      $rules = $db->nextRecord('rules', MYSQL_ASSOC);
      $template->set('rules', $rules);
    }
    $sql = "select * from companycontacts where companyID = '".$companyID."'";
    $db->query($sql, 'companyContacts');
    if ($db->Size('companyContacts') > 0) {
      while ($res = $db->nextRecord('companyContacts')) {
      $contactData[] = $res;
      }
    } else {
      $contactData[0] = array ('contactID' => 0,
			    'cn' => "None");
    }
    $log = array();
    $sql = "SELECT l.time as time, l.notes as notes, l.category as category, u.gn as gn, u.sn as sn, u.email as email from log as l, users as u where l.id = '".$companyID."' and l.type = 'company' and l.username = u.username order by l.time DESC";
    $db->query($sql, "logRecords");
    while ($row = $db->nextRecord('logRecords')) {
      $log[] = $row;
    }
    $sql = "SELECT count( * ) AS total, companyID, status
FROM people
where companyID = '".$companyID."'
GROUP BY companyID, status;";
    $db->query($sql, 'companyStats');
    $res = $db->allRecords('companyStats');
    //print "<pre>";
    //var_dump($res);
    //print "\n$sql\n</pre>";
    $template->set('stats', $res);
    $template->set('allowed', $allowed);
    $template->set('companyData', $companyData);
    $template->set('contactData', $contactData);
    $template->set('logData', $log);
    $body = $template->parse();
  }
}


function addCompany($template) {
  global $db, $form, $body, $login;
  if ($form->Submit) {
    $array = array('name' => $form->name,
		   'address' => $form->address,
		   'phone1' => preg_replace('/[^\d]+/', '', $form->phone1),
		   'phone2' => preg_replace('/[^\d]+/', '', $form->phone2),
		   'planDesign' => $form->planDesign);
    if ($form->id) {
      $array['companyID'] = $form->id;
      $logSQL = "insert into log set username='".$login->user->username."', type='company', time='".time()."', id='".$form->id."', category='System', notes='Edited Company data.'";
      $db->query($logSQL);
      $sql = $db->prepare('replace', 'companies', $array);
    } else {
    $sql = $db->prepare('insert', 'companies', $array);
$logSQL = "insert into log set username='".$login->user->username."', type='company', time='".time()."', id='".$form->id."', category='System', notes='Company Created.'";
      $db->query($logSQL);
    }
    $db->query($sql);
    if ($form->rules) {
      $rules = $form->rules;
      $count = 0;
      foreach ($rules as $type => $rule) {
	if ($rule != "") {
	  $count++;
	}
      }
      if ($count > 0) {
	$rules['companyID'] = $form->id;
	$rulesSQL = $db->prepare('replace', 'companyrules', $rules);
	$db->query($rulesSQL);
      }
    }
   // showCompany($template, ($form->id)?$form->id:$db->insertID());
   $array = array('success' => TRUE,
		  'id' => ($form->id)?$form->id:$db->insertID());
   print json_encode($array);
   exit;
    //return;


  } else {
    if ($form->id) {
      $sql = "select * from companies where companyID = '".$form->id."'";
      $db->query($sql);
      $res = $db->nextRecord();
      $template->set('companyData', $res);
      $rulesSQL = "select * from companyrules where companyID = '".$form->id."'";
    $db->query($rulesSQL, 'rules');
    if ($db->Size('rules') > 0) {
      $rules = $db->nextRecord('rules', MYSQL_ASSOC);
      $template->set('rules', $rules);
    }
    }
    $template->set('action', 'addCompany');
  }
  $body = $template->parse();
  return;
}

function showCarrier($template, $id = NULL) {
  global $db, $form, $body, $login;
  $allowed = NULL;
  if ($login->user->access_lv > 1) { $allowed = TRUE; }

  $carrierID = ($id)?$id:$form->carrierID;
  $template->set('action', 'showCarrier');
  # Now we have to get info from all over.
  $sql = "select * from carriers where carrierID = '".$carrierID."'";
  $db->query($sql, 'carrierRecord');
  if ($companyData = $db->nextRecord('carrierRecord')) {
    if ($companyData['owner'] == $login->user->username) { $allowed = TRUE; }

    $sql = "select * from carriercontacts where carrierID = '".$carrierID."'";
    $db->query($sql, 'carrierContacts');
    if ($db->Size('carrierContacts') > 0) {
      while ($res = $db->nextRecord('carrierContacts')) {
      $contactData[] = $res;
      }
    } else {
      $contactData = NULL;
    }
    $log = array();
    $sql = "SELECT l.time as time, l.notes as notes, l.category as category, u.gn as gn, u.sn as sn, u.email as email from log as l, users as u where l.id = '".$carrierID."' and l.type = 'carrier' and l.username = u.username order by l.time DESC";
    $db->query($sql, "logRecords");
    if ($db->Size('logRecords') > 0) {
    while ($row = $db->nextRecord('logRecords')) {
      $log[] = $row;
    }
    } else {
      $log = NULL;
    }
    $template->set('allowed', $allowed);
    $template->set('companyData', $companyData);
    $template->set('contactData', $contactData);
    $template->set('logData', $log);
    $body = $template->parse();
  }
}

function addCarrier ($template) {
  global $db, $form, $body, $login;
  if ($form->Submit) {
    $array = array('name' => $form->name,
		   'address' => $form->address,
		   'phone1' => preg_replace('/[^\d]+/', '', $form->phone1),
		   'phone2' => preg_replace('/[^\d]+/', '', $form->phone2));
    if ($form->id) {
      $array['carrierID'] = $form->id;
      $logSQL = "insert into log set username='".$login->user->username."', type='carrier', time='".time()."', id='".$form->id."', category='System', notes='Edited Carrier data.'";
      $db->query($logSQL);
      $sql = $db->prepare('replace', 'carriers', $array);
    } else {
      $logSQL = "insert into log set username='".$login->user->username."', type='carrier', time='".time()."', id='".$form->id."', category='System', notes='Created Carrier.'";
      $db->query($logSQL);
    $sql = $db->prepare('insert', 'carriers', $array);
    }
    $db->query($sql);
    showCarrier($template, ($form->id)?$form->id:$db->insertID());
    return;


  } else {
    if ($form->id) {
      $sql = "select * from carriers where carrierID = '".$form->id."'";
      $db->query($sql);
      $res = $db->nextRecord();
      $template->set('companyData', $res);
    }
    $template->set('action', 'addCarrier');
  }
  $body = $template->parse();
  return;



}

?>