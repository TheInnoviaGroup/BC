<?PHP
global $login;
$template = new template('admin.ihtml');
$template->set('userObj', $login->user);
switch($form->action) {
case "User":
  editUser($template);
  break;
case "addUser":
  editUser($template, TRUE);
  break;
case "assignCompany":
  assignCompany($template);
  break;
case "unassignCompany":
  assignCompany($template, TRUE);
} 

function assignCompany($template, $unassign = FALSE) {
  global $db, $form, $body, $login, $self, $debug;
  if ($form->companyID > 0 && $login->user->access_lv == 2) {
    if ($unassign) {
      $sql = "delete from allowedcompanies where companyID = '".$form->companyID."' AND user_idnum = '".$form->user_idnum."'";
    } else {
      $sql = "insert into allowedcompanies set companyID = '".$form->companyID."',  user_idnum = '".$form->user_idnum."'";
    }
    $debug->line($sql, 1);
    $db->query($sql, 'assignCompany');
  } 
  $allowedCompanies = allowedCompanies($form->user_idnum);
  $sql = "select * from companies";
  $db->query($sql, 'companyList');
  $companies = $db->allRecords('companyList');
  $db->query("select * from users where user_idnum = '".$form->user_idnum."'", 'userInfo');
  $userData = $db->nextRecord('userInfo');
  $template->set('allowedCompanies', $allowedCompanies);
  $template->set('companies', $companies);
  $template->set('user_idnum', $form->user_idnum);
  $template->set('action', 'assignCompanies');
  $template->set('userData', $userData);
  $template->set('self', $self);
  $body .= $template->parse();
}


function editUser($mainTemplate, $add = NULL) {
  global $db, $form, $debug;
    $mainTemplate->set('action', 'user');
  if ($form->userID) {
    $debug->line('userID '.$form->userID, 1);
    if ($form->delete) {
      $sql = "delete from users where user_idnum = '".$form->userID."';";
      $db->query($sql);
      $mainTemplate->set('message', "User Deleted.");
  global $body;
  $body .= $mainTemplate->parse();

      return;
    } else {
      $user = new user($form->userID);
      $debug->line("got user ".$user->username, 1);
      if ($form->submit) {
	$user->gn = ($form->gn)?$form->gn:$user->gn;
	$user->sn = ($form->sn)?$form->sn:$user->sn;
	$user->email = ($form->email)?$form->email:$user->email;
	$user->access_lv = ($form->access_lv)?$form->access_lv:$user->access_lv;
	$mainTemplate->set('message', 'User Info Updated.');
	if ($form->password && ($form->password == $form->confirmpassword)) {
	  $user->passwd = $form->password;
	} elseif ($form->password) {
	  $mainTemplate->set('message', "Passwords do not match. Please try again.");
	}
	$user->commit();
      }
      $mainTemplate->set('user', $user);
    }
  } elseif ($form->submit) {
	if ($form->password && ($form->password == $form->confirmpassword)) {
	  $sql = "insert into users set username='".$form->username."', gn='".$form->gn."', sn='".$form->sn."', email='".$form->email."', access_lv='".$form->access_lv."', passwd=PASSWORD('".$form->password."');";
	  $debug->line($sql, 1);
	  $db->query($sql);
	  $user = new user($db->insertID());
	  $mainTemplate->set('user', $user);
	  $mainTemplate->set('message', "User added.");
	} elseif ($form->password) {
	  $mainTemplate->set('message', "Passwords do not match. Please try again.");
	  $mainTemplate->set('add', TRUE);
	  $mainTemplate->set('user', $form);
	}
  } elseif ($add) {
    $mainTemplate->set('user', NULL);

  } else {
    $mainTemplate->set('user', NULL);
    $sql = "select * from users";
    $db->query($sql);
    $array = $db->allRecords();
    $mainTemplate->set('action', 'userlist');
    $mainTemplate->set('records', $array);
  }
  global $body;
  $body .= $mainTemplate->parse();

}

?>