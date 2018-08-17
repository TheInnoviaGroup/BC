<?PHP
$lobTemplate = new template('lobMain.ihtml');
$altTemplates = array('editlob' => 'lobEdit.ihtml',
                      'details' => 'lobDetails.ihtml');
global $body, $form, $self, $javascripts, $styles, $login;

if ($form->action) {
    $lobTemplate = new template($altTemplates[$form->action]);
}

$lobTemplate->set("self", $self);
switch ($form->action) {
 case "editlob":
   addlob($lobTemplate);
   break;
 case "addlob":
   addlob($lobTemplate);
   break;
 case "showlob":
   showlob($lobTemplate);
   break;
 case "delete":
   deletelob($lobTemplate);
   break;
default:
    lobMain($lobTemplate);
    break;
}

function showlob($template) {
    global $body, $self, $login, $form, $db, $debug;
    $template->set('action', 'showlob');
    if (! $form->companyID) {
        $template = new template('lobCList.ihtml');
        $template->set('self', $self);
        $template->set('action', 'showlob');
        $sql = "select name, companyID from companies order by name ASC";
        $db->query($sql);
        $array = array();
        while ($res = $db->nextRecord()) {
          $array[$res['companyID']] = array('companyID'=>$res['companyID'],'name'=>$res['name']);
        }
        $template->set('title', 'View LOBs');
        $template->set('companies', $array);
    } else {
        $template = new template('lobList.ihtml');
        $sql = "select * from lobdefs where companyID = '".$form->companyID."'";
        $db->query($sql);
        $template->set('lobArray', $db->allRecords());
        $sql = "select name from companies where companyID = '".$form->companyID."'";
        $db->query($sql);
        $template->set('self', $self);
        $row = $db->nextRecord();
        $template->set('companyName', $row['name']);
    }
     $body = $template->parse();
}

function lobMain($temp) {
    global $body, $login, $self;
    
    
    
    $login->template->set('isSubpage', 'yes');
    $temp->set('self', $self);
    $temp->set('loginbox', $login->template->parse());
    $body = $temp->parse();
}

function addlob($template) {
    global $body, $self, $login, $form, $db, $debug;
    $template->set('action', 'editlob');
    if (! $form->companyID && ! $form->lobID) {
        $template = new template('lobCList.ihtml');
        $template->set('self', $self);
        $template->set('action', 'editlob');
        $sql = "select name, companyID from companies order by name ASC";
        $db->query($sql);
        $array = array();
        while ($res = $db->nextRecord()) {
          $array[$res['companyID']] = array('companyID'=>$res['companyID'],'name'=>$res['name']);
        }
        $template->set('title', 'Add LOB');
        $template->set('companies', $array);
    } else if ($form->shortname) {
      if ($form->lobID) {
        $sql = "update lobdefs set companyID = '".$form->companyID."', shortName = '".$form->shortname."',
                description = '".$form->description."', criteria = '".$form->criteria."' where lobID = '".$form->lobID."'";
      } else {
        $sql = "insert into lobdefs set companyID = '".$form->companyID."', shortName = '".$form->shortname."',
                description = '".$form->description."', criteria = '".$form->criteria."'";
      }
      $db->query($sql);
      print "{success: true}\n";
      exit;
    } else {
        if ($form->companyID) {
        $template->set('companyID', $form->companyID);
        $sql = "select name from companies where companyID = '".$form->companyID."'";
        $db->query($sql);
        $template->set('self', $self);
        $row = $db->nextRecord();
        $template->set('companyName', $row['name']);
        }
        if ($form->lobID) {
            $sql = "select * from lobdefs where lobID = '".$form->lobID."'";
            $db->query($sql);
            
            $row1 = $db->nextRecord();
            $template->set('shortname', $row1['shortName']);
            $template->set('description', $row1['description']);
            $template->set('companyID', $row1['companyID']);
            $template->set('criteria', $row1['criteria']);
            $template->set('lobID', $row1['lobID']);
            $sql = "select name from companies where companyID = '".$row1['companyID']."'";
        $db->query($sql);   
        $template->set('self', $self);
        $row = $db->nextRecord();
        $template->set('companyName', $row['name']);
        } else {
            
        }
        
    }
    
    $body = $template->parse();
}


?>