<?PHP
$welcomeTemplate = new template('index.ihtml');
global $body, $login, $self, $db;
$sql = "select name, companyID from companies";
$db->query($sql);
$array = array();
while ($res = $db->nextRecord()) {
  $array[$res['companyID']] = $res['name'];
}



$sql2 = "select name, saveID from savedsearches";
$db->query($sql2);
$array2 = array();
while ($res = $db->nextRecord()) {
  $array2[$res['saveID']] = $res['name'];
}
$sql3 = "select p.*, u.gn as gn, u.sn as sn, u.email as email from pendinguploads as p, users as u where p.uploadedBy = u.username order by p.time DESC";
$db->query($sql3);
$array3 = array();
while ($res = $db->nextRecord()) {
  $array3[$res['uploadID']] = $res;
}

$sql4 = "select r.*, c.name as companyName, p.name as carrierName from rawdatatoc as r, carriers as p, companies as c where r.status > -1 AND r.companyID = c.companyID AND r.carrierID = p.carrierID ORDER BY r.time DESC";
$db->query($sql4);
$array4 = array();
while ($res = $db->nextRecord()) {
  $array4[$res['importID']] = $res;
}



global $personStatus, $uploadPath, $uploadStatuses;
if ($form->searchtype == "compensation") {
  $welcomeTemplate->set('searchtype', "compensation");
  $sql4 = "select * from comptypes";
  $db->query($sql4);
  $array4 = array();
  while ($res = $db->nextRecord()) {
    $array4[$res['compTypeID']]=$res['name'];
  }
  $welcomeTemplate->set('compList', $array4);
 } else {
  $welcomeTemplate->set('searchtype', $form->searchtype);
 }

$welcomeTemplate->set('companyList', $array);
$welcomeTemplate->set('saveList', $array2);
$welcomeTemplate->set('uploadPath', $uploadPath);
$welcomeTemplate->set('uploadList', $array3);
$welcomeTemplate->set('importList', $array4);
$welcomeTemplate->set('uploadStatus', $uploadStatuses);
$welcomeTemplate->set('statusList', $personStatus);
$welcomeTemplate->set('user', $login->user);
$welcomeTemplate->set('self', $self);
$welcomeTemplate->set('loginbox', $login->template->parse());
$body = $welcomeTemplate->parse();
?>