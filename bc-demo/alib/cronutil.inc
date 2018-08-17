<?PHP

/* sql:
CREATE TABLE `cronLock` (
`pid` INT( 32 ) NOT NULL ,
`procName` VARCHAR( 255 ) NOT NULL ,
`time` INT( 32 ) NOT NULL
)
*/

/* Returns true if no other processes with $name have the a lock
 registered in the table. */

function cronCheck($name, $db) {
  $sql = "select * from cronlock where procName = '$name'";
  $db->query($sql, 'cronCheck');
  if ($db->Size() > 0) {
    return FALSE;
  } else {
    return TRUE;
  }
}

/* Attempts to place a lock in the cronlock table. Fails if there is
 a) an mysql error OR if there is already a lock with that name. */

function cronLock($name, $db) {
  if (cronCheck($name, $db)) {
    $pid = posix_getpid();
    $sql = "insert into cronlock set pid = '$pid', procName = '$name', time = '".time()."'";
    return $db->query($sql, 'cronLock');
  } else {
    return FALSE;
  }
}

/* Attempts to remove a lock in the cronlock table. Fails if there is
 a) an mysql error OR if there is no lock with that name. */

function cronUnLock($name, $db) {
  if (! cronCheck($name, $db)) {
    $pid = posix_getpid();
    $sql = "delete from cronlock where pid = '$pid' AND procName = '$name'";
    return $db->query($sql, 'cronUnLock');
  } else {
    return FALSE;
  }
}

?>