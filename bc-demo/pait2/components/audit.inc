<?PHP

function auditArray($array) {
  global $login, $debug;
  $debug->line('auditArray', 1);
  $debug->variable($array, 2);
  $user = $login->user->username;
  foreach ($array as $table => $data) {
    $debug->line('auditing: '.$table, 1);
    audit($table, $data['from'], $data['to'], $data['action'], ($data['source'])?$data['source']:'user', ($data['details'])?$data['details']:$user);
  }
  }

function audit($table, $from, $to, $action = 'add', $source = 'user', $details = array()) {
  //First we check to see if from and to are the same. If so, we don't log crap.
  if (is_array($from) && is_array($to)) {
  $diff = array_diff_assoc($from, $to);
  if ((count($from) > 0 && count($diff) == 0) && $action != 'add') {
    return -1;
    }
  }
  static $logDB;
  /* We only create this puppy once. And only if we need it. */
  if (!isset($logDB)) {
    global $AGLOBAL;
    $logDB = new db($AGLOBAL['AUDITDBURL']);
  }
  $now = microtime(TRUE); //Float time in microseconds.

  //By default, we assume the currently logged in user did something.
  if (count($details) == 0 && $source == 'user') {
    global $login;
    $details['username'] = $login->user->username;
  }
  $insert = array('time' => $now,
		  'source' => $source,
		  'sourceDetails' => addslashes(serialize($details)),
		  'action' => $action,
		  'table' => $table,
		  'from' => addslashes(serialize($from)),
		  'to' => addslashes(serialize($to)));
  $sql = $logDB->prepare('insert', 'rawtransactions', $insert);
 //pre($sql);
  global $debug;
  $debug->line("logging: $sql", 2);
  $logDB->query($sql);
  if ($logDB->affectedRows() > 0) {
    return TRUE;
  } else {
    return FALSE;
  }
}


?>