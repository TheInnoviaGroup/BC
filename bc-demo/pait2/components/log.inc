<?PHP
  global $login, $db, $form, $self, $javascripts;
$javascripts[] = "cpaint2.inc.compressed.js";
$javascripts[] = "spell_checker.js";

$template = new template('addlog.ihtml');
$template->set('self', $self);
if ($form->Submit) {
  $username = $login->user->username;
  $array = array('username' => $username,
		 'type' => $form->type,
		 'id' => $form->id,
		 'notes' => $form->notes,
		 'category' => $form->category,
		 'time' => time());
  audit('log', array(), $array, 'add');
 // $sql = $db->prepare('insert', 'log', $array);
 // $db->query($sql);
  $template->set('done', TRUE);
  $template->set('type', $form->type);
  $template->set('id', $form->id);
print "{success: true}";
exit;
} else {
  if ($form->action == "addCompany") {
    $type = "company";
    $id = $form->companyID;
  } elseif ($form->action == "addCarrier") {
    $type = "carrier";
    $id = $form->carrierID;
  } else {
    $type = "person";
    $id = $form->personID;
  }
  $template->set('type', $type);
  $template->set('id', $id);
}
  global $body;
  $body = $template->parse();


?>