<?PHP
global $body, $form;
switch ($form->action) {
case "addUser":
  addUser($form);
  break;
default:
  showAccount($form);
}

function showAccount($form) {
  $templateFile = "showAccount.ihtml";
  global $stylesheets, $javascripts, $AGLOBAL, $db, $self;;
  $debug = $AGLOBAL['DEBUG'];
  $stylesheets["createAccount.css"] = 'screen';
  $javascripts[] = "accountCreation.js";
  $a_levels = array ('Search All' => '',
			'user' => '5',
			'supervisor' => '6',
			'administrator' => '7');  
  $template = new template($templateFile);
  $debug->variable($form, 1);
  $debug->line($form->action, 1);
if($form->action == "detail") {
  $debug->line('account detail', 1);
	$title = "Account Detail";
	$curUser = new user($form->uid);
	if($curUser->line_lead == 1) {
		$ll_val = "yes";
	} else {
		$ll_val = "no";
	}
	
	if($curUser->access_lv == 7) {
		$a_level = "admin";
	} elseif($curUser->access_lv == 6) {
		$a_level = "supervisor";
	} else {
		$a_level = "user";
	}

	
	$template->set('first_name', $curUser->gn);
	$template->set('last_name', $curUser->sn);
	$template->set('username', $curUser->username);
	$template->set('access_lv', $a_level);
	$template->set('line_lead', $ll_val);
	$template->set('home_line', $curUser->home_line);
	$template->set('start_date', date("n/j/Y", $curUser->start_date));
	$template->set('url', $self);
	
} elseif($form->action == "edit") {
	$edituser = new user($form->uid);
	$template->set('first_name', $edituser->gn);
	$template->set('last_name', $edituser->sn);
	$template->set('username', $edituser->username);
	$template->set('access_lv', $edituser->access_lv);
	$template->set('line_lead', $edituser->line_lead);
	$template->set('home_line', $edituser->home_line);
	$template->set('start_date', date("n/j/Y", $edituser->start_date));
	$template->set('a_levels', $a_levels);
	$template->set('self', $self);
	
} elseif($form->action == "edit_update") {
	$user = new user($form->uid);
  	$user->gn = $form->first_name;
  	$user->sn = $form->last_name; #fixed
  	$user->account_status = 1;
  	$user->access_lv = $form->access_lv;
  	$user->home_line = $form->home_line;
  	$user->line_lead = $form->line_lead;
  	$user->start_date = parseTimeString($form->start_date);
  	$user->commit();
  	
  	
  	if($user->line_lead == 1) {
		$ll_val = "yes";
	} else {
		$ll_val = "no";
	}
	
	if($user->access_lv == 7) {
		$a_level = "admin";
	} elseif($user->access_lv == 6) {
		$a_level = "supervisor";
	} else {
		$a_level = "user";
	}
	
  	$template->set('first_name', $user->gn);
	$template->set('last_name', $user->sn);
	$template->set('username', $user->username);
	$template->set('access_lv', $a_level);
	$template->set('line_lead', $ll_val);
	$template->set('home_line', $user->home_line);
	$template->set('start_date', date("n/j/Y", $user->start_date));
	$template->set('a_levels', $a_levels);
	$template->set('self', $self);

} elseif($form->action == "reset") {
	$resetUser = new user($form->uid);
	
	if($resetUser->line_lead == 1) {
		$ll_val = "yes";
	} else {
		$ll_val = "no";
	}
	
	if($resetUser->access_lv == 7) {
		$a_level = "admin";
	} elseif($curUser->access_lv == 6) {
		$a_level = "supervisor";
	} else {
		$a_level = "user";
	}
	
	$template->set('first_name', $resetUser->gn);
	$template->set('last_name', $resetUser->sn);
	$template->set('username', $resetUser->username);
	$template->set('access_lv', $a_level);
	$template->set('line_lead', $ll_val);
	$template->set('home_line', $resetUser->home_line);
	$template->set('start_date', date("n/j/Y", $resetUser->start_date));
	$template->set('a_levels', $a_levels);
	$template->set('self', $self);
	
	
} elseif($form->action == "reset_update") {
	$user = new user($form->uid);
  	$user->passwd = $form->password;
  	$user->commit();
  	$template->set('url', $self);
  	
} elseif($form->action == "delete_confirm") {
	$user = new user($form->uid);
  	$template->set('fullname', $user->gn." ".$user->sn);
  	$template->set('username', $user->username);
  	
} elseif($form->action == "delete_user") {
	$user = new user($form->uid);
	$sql = "DELETE FROM users WHERE username = '".$form->uid."'";
	$results = $db->query($sql);
	$debug->variable($sql, 1);
  	$template->set('url', $self);

} elseif($form->action == "search_user") {
  $debug->line('Search User!', 1);
	$template->set('a_levels', $a_levels);
	$template->set('self', $self);
	$template->set('url', $self);

} elseif($form->action == "assign_credit_search") {
  $debug->line('Assign Credit User!', 1);
	$template->set('a_levels', $a_levels);
	$template->set('self', $self);
	$template->set('url', $self);
	$template->set('assigncredit', 1);


} elseif($form->action == "display_search_results" || $form->action == "display_search_assign_credit") {

$query="SELECT * FROM users WHERE ";

if($form->first_name!="") {
$query=$query."gn LIKE '%$form->first_name%' AND ";
}

if($form->last_name!="") {
$query=$query."sn LIKE '%$form->last_name%' AND ";
}

if($form->username!="") {
$query=$query."username LIKE '%$form->username%' AND ";
}

if($form->access_lv!="") {
$query=$query."access_lv='$form->access_lv' AND ";
}

if($form->line_lead!="") {
$query=$query."line_lead = '$form->line_lead' AND ";
}

if($form->home_line!="") {
$query=$query."home_line='$form->home_line' AND ";
}

if($form->start_date!="") {
$start_date_stamp=parseTimeString($form->start_date);
$query=$query."start_date = '$start_date_stamp' AND ";
}

$debug->variable(date("n/j/Y", $start_date_stamp), 1);
$query= substr($query, 0, (count($query)-5));
$query= $query." ORDER BY sn";
$debug->variable($query, 1);

$results = $db->query($query);
$template->set('userList', $db->allrecords());
// $template->set('results', $results);
$debug->variable($results, 1);
} elseif ($form->action == "assign_credit_choose_unit") {
  $template->set('chooseunit', 1);
  $template->set('users', $form->selectUser);
  $template->set('topicList', buildTopicList());
} elseif ($form->action == "assign_credit_confirm") {
  $template->set('users', $form->users);
  $template->set('units', $form->selectUnit);

} elseif ($form->action == "assign_credit_final") {
  $users = $form->users;
  $units = $form->units;
  $now = time();
  $message = "";
  global $db;
  foreach ($users as $uid => $cruft) {
    $user = new user($uid);
    foreach ($units as $unit => $cruft) {
    $array = array('userID' => $user->user_idnum,
		   'unitID' => $unit,
		   'status' => 2,
		   'dateCompleted' => $now);
    $sql = $db->prepare('replace', 'trainingprogress', $array);
    //    $message .= $sql . "<br />\n";
    $db->query($sql);
    }
  }
  $message .= "Training Credit has been assigned.<br />";

  $template->set('message', $message);

} elseif ($form->action == "assign_training_choose_unit") {
  $template->set('chooseunit', 1);
  $template->set('users', $form->selectUser);
  $template->set('topicList', buildTopicList());
} elseif ($form->action == "assign_training_confirm") {
  $template->set('users', $form->users);
  $template->set('units', $form->selectUnit);

} elseif ($form->action == "assign_training_final") {
  $users = $form->users;
  $units = $form->units;
  $now = time();
  $message = "";
  global $db;
  foreach ($users as $uid => $cruft) {
    $user = new user($uid);
    foreach ($units as $unit => $cruft) {
    $array = array('userID' => $user->user_idnum,
		   'unitID' => $unit,
		   'status' => 1,
		   'dateStarted' => $now);
    $sql = $db->prepare('replace', 'trainingprogress', $array);
    //    $message .= $sql . "<br />\n";
    $db->query($sql);
    }
  }
  $message .= "Training has been assigned.<br />";

  $template->set('message', $message);

} else {

	$title = "Account List";
	$sql = "SELECT * FROM users ORDER BY gn";
	$results = $db->query($sql);
	$template->set('userList', $db->allrecords());
	// $template->set('results', $results);
	$debug->variable($results, 1);
}
global $self, $body;
$template->set('self', $self);
$template->set('debug', $debug);
$template->set('action', $form->action);
$template->set('stylesheet', $stylesheet);
$template->set('ajaxjavascript', $ajaxJavascript);
$template->set('additionaljavascript', $additionalJavascript);
$template->set('title', $title);
$debug->variable($template, 1);
$body = $template->parse();


}


function addUser($form) {
  $templateFile = "createAccount.ihtml";
  global $stylesheets, $javascripts, $AGLOBAL;
  $stylesheets["createAccount.css"] = 'screen';
  $javascripts[] = "accountCreation.js";
  $a_levels = array ('Select Access Level' => '0',
		     'user' => '5',
		     'supervisor' => '6',
		     'administrator' => '7');

  // This is a list of form variables and regular expressions to match on.
$formValidation = array('email' => '/[A-Za-z0-9._%-]+@[A-Za-z0-9._%-]+\.[A-Za-z]{2,5}/', 'confirm_email' => '/[A-Za-z0-9._%-]+@[A-Za-z0-9._%-]+\.[A-Za-z]{2,5}/');

// Here we go.
$template = new template($templateFile);
$badvars = $form->validate($formValidation);
$AGLOBAL['DEBUG']->variable($form, 1);
$step = ($form->step)?$form->step:0;
if ($form->back) { 
  $step = 1;
 }


if ($step == 1) {
  if (!$form->back) {
  $step++;
  }
  $AGLOBAL['DEBUG']->line('checking variables', 1);
  // Did we enter a username
  if ($form->username) {
    $AGLOBAL['DEBUG']->line('does the username exist', 1);
    // If so, does it already exist?
    $testuser = new user($form->username);
    if ($testuser->create_date) {
      $AGLOBAL['DEBUG']->line('yes.', 1);
      // user already exists
      $badvars[] = "username";
      $errors['username'] = "That username is already in use. Please choose another.";
    $step = 1;
    }
  } else {
    $AGLOBAL['DEBUG']->line('No username entered',1);
    $badvars[] = "username";
    $errors['username'] = "You must choose a username.";
    $step = 1;
  }
  // Is the password at least 4 chars long?
  if (strlen($form->password) < 4) {
    $AGLOBAL['DEBUG']->line('password shorter than 4 chars', 1);
    $badvars[] = "password";
    $errors['password'] = "Your password must be at least 4 characters long.";
    $step = 1;
  }
  // Is the username also?
  if (strlen($form->username) < 4 || strlen($form->username) > 20) {
    $AGLOBAL['DEBUG']->line('username shorter than 4 or longer than 20: '.$form->username, 1);
    $badvars[] = "username";
    $errors['username'] = "Your username must be between 4 and 20 characters long.";
    $step = 1;
  }
  if (preg_match('/[^A-Za-z0-9\_\-\.]/', $form->username)){
    $AGLOBAL['DEBUG']->line('username has illegal characters: '.$form->username, 1);
    $badvars[] = "username";
    $errors['username'] = "Your username can only consist of letters, numbers, underscores (_) and dashes (-).";
    $step = 1;
  }
  $AGLOBAL['DEBUG']->line("back? " . $form->back, 1);
  // Match the confirmations?
  if ($form->password != $form->confirm_password && !$form->back) {
    $errors['confirm_password'] = "Must match the password you entered above.";
    $badvars[] = "confirm_password";
    $step = 1;
  }
}

if ($step == 2) {
  $AGLOBAL['DEBUG']->line('Doing this thing because step = '.$step,1);
  // Let's do this thing.
  $user = new user($form->username);
  if ($user->create_date) {
    // somehow we've got a real user. Go back to the beginning.
    $step = 1;
  } else {
  $AGLOBAL['DEBUG']->variable($user, 1);
  $user->username = $form->username;
  $user->gn = $form->first_name;
  $user->sn = $form->last_name;
  $user->account_status = 1;
  $user->access_lv = $form->access_lv;
  $user->home_line = $form->home_line;
  $user->line_lead = $form->line_lead;
  $user->passwd = $form->password;
  $user->start_date = parseTimeString($form->start_date);
  $user->addUser();
  $AGLOBAL['DEBUG']->variable($user, 1);
  $step = 3;
  }
}

if ($step == 0) { $step = 1; }
global $self;
// First we do a little validation.
$template->set('goneback', $form->back);
$template->set('errors', $errors);
$template->set('step', $step);
$template->set('self', $self);
$template->set('returnto', $form->returnto);
$template->set('debug', $AGLOBAL['DEBUG']);
$template->set('stylesheet', $stylesheet);
$template->set('ajaxjavascript', $ajaxJavascript);
$template->set('additionaljavascript', $additionalJavascript);
$template->set('form', $form);
$template->set('badvars', $badvars);
$template->set('a_levels', $a_levels);
$template->set('title', $title);

global $body;
$body = $template->parse();

}




?>