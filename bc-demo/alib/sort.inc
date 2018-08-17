<?PHP
/**
 * Sorting functions.
 * Since this isn't an object, you have to include it with include_once('sort.inc');
 * @package aLib
 * @subpackage Utility
 */
/**
 * sortAssoc sorts an array of associative arrays by the fields specified in an accompanying array.
 *
 * <code>$subarray1 = array('field1' => 'a', 'field2' => 'b', 'field3' => 'c');
 * $subarray2 = array('field1' => 'd', 'field2' => 'b', 'field3' => 'a');
 * $subarray3 = array('field1' => 'a', 'field2' => 'c', 'field3' => 'b');
 * $array = array($subarray1, $subarray2, $subarray3);
 * $fields = array ('field2', 'field3', 'field1');
 * $sorted = sortAssoc($array, $fields);
 * </code>
 *
 * $sorted would put the component arrays into the order: $subarray2, $subarray1, $subarray3.
 *
 * @param mixed $array An array of associative arrays to sort.
 * @param array $fields a simple array of fields to sort on.
 * @param bool $casesensitive If the sort should be case sensitive
 * @param bool $natural If the sort order should be naturalistic (eg. one in which 9 comes before 10).
 * @param mixed $sorted The array sorted.
 */

function sortAssoc($array, $fields, $casesensitive = FALSE, $natural = TRUE) 
  {
    global $AGLOBAL;
    $AGLOBAL['sortAssocParams']['Fields'] = $fields;
    $AGLOBAL['sortAssocParams']['caseSensitive'] = $casesensitive;
    $AGLOBAL['sortAssocParams']['natural'] = $natural;
    $sorted = $array;
    if (uasort($sorted, '__sortAssocCallback')) 
      {
        return $sorted;
      }
    else
      {
        return $array;
      }
  }

function __sortAssocCallback($a, $b) 
  {
    global $AGLOBAL;
    $fields = $AGLOBAL['sortAssocParams']['Fields'];
    $casesensitive = $AGLOBAL['sortAssocParams']['caseSensitive'];
    $natural = $AGLOBAL['sortAssocParams']['natural'];
    foreach($fields as $field) 
      {
        if ($casesensitive && $natural) 
          {
            $cmp = strnatcasecmp($a[$field], $b[$field]);
            if ($cmp != 0) 
              {
                return $cmp;
              }
          }
        elseif($casesensitive) 
          {
            $cmp = strcasecmp($a[$field], $b[$field]);
            if ($cmp != 0) 
              {
                return $cmp;
              }
          }
        elseif($natural) 
          {
            $cmp = strnatcmp($a[$field], $b[$field]);
            if ($cmp != 0) 
              {
                return $cmp;
              }
          }
        else
          {
            $cmp = strcmp($a[$field], $b[$field]);
            if ($cmp != 0) 
              {
                return $cmp;
              }
          }
      }
    return 0;
  }
/**
 * sortAssocSecondIndex sorts an array of associative arrays by the fields specified in an accompanying array.
 *
 * Works like sortAssoc but will do it one index deep. Instead of sorting on $array['field'] it'll sort on
 * $array['index']['field'] where 'index' is one key to look under in all arrays.
 *
 *
 * @param mixed $array An array of associative arrays to sort.
 * @param array $fields a simple array of fields to sort on.
 * @param string $index Index under which to sort.
 * @param bool $casesensitive If the sort should be case sensitive
 * @param bool $natural If the sort order should be naturalistic (eg. one in which 9 comes before 10).
 * @param mixed $sorted The array sorted.
 */

function sortAssocSecondIndex($array, $fields, $index, $casesensitive = FALSE, $natural = TRUE) 
  {
    global $AGLOBAL;
    $AGLOBAL['sortAssocSecondIndexParams']['Index'] = $index;
    $AGLOBAL['sortAssocSecondIndexParams']['Fields'] = $fields;
    $AGLOBAL['sortAssocSecondIndexParams']['caseSensitive'] = $casesensitive;
    $AGLOBAL['sortAssocSecondIndexParams']['natural'] = $natural;
    $sorted = $array;
    if (uasort($sorted, '__sortAssocSecondIndexCallback')) 
      {
        return $sorted;
      }
    else
      {
        return $array;
      }
  }

function __sortAssocSecondIndexCallback($a, $b) 
  {
    global $AGLOBAL, $debug;
    $index = $AGLOBAL['sortAssocSecondIndexParams']['Index'];
    $fields = $AGLOBAL['sortAssocSecondIndexParams']['Fields'];
    $casesensitive = $AGLOBAL['sortAssocSecondIndexParams']['caseSensitive'];
    $natural = $AGLOBAL['sortAssocSecondIndexParams']['natural'];
    foreach($fields as $field) 
      {
        if ($casesensitive && $natural) 
          {
            $cmp = strnatcasecmp($a[$index][$field], $b[$index][$field]);
            if ($cmp != 0) 
              {
                return $cmp;
              }
          }
        elseif($casesensitive) 
          {
            $cmp = strcasecmp($a[$index][$field], $b[$index][$field]);
            if ($cmp != 0) 
              {
                return $cmp;
              }
          }
        elseif($natural) 
          {
            $cmp = strnatcmp($a[$index][$field], $b[$index][$field]);
            if ($cmp != 0) 
              {
                return $cmp;
              }
          }
        else
          {
            $cmp = strcmp($a[$index][$field], $b[$index][$field]);
            if ($cmp != 0) 
              {
                return $cmp;
              }
          }
      }
    return 0;
  }
/**
 * Sort linked list.
 * @param mixed $toSort assoc array to sort, where ID is the key.
 * @param array $sortOrder array of ID(1) => ID(2) where ID 1 comes before ID2.
 * @return array $sorted An array of of the values from $toSort sorted according to $sortOrder.
 */

function sortLinkedList($toSort, $sortOrder) 
  {
    global $AGLOBAL;
    $AGLOBAL['DEBUG']->variable($sortOrder, 11);
    $AGLOBAL['DEBUG']->variable($toSort, 11);
    $sorted = array();
    $ID = $sortOrder[0];
    $AGLOBAL['DEBUG']->line('ID: ' . $ID, 11);
    $sorted[] = $toSort[$ID];
    $next = $ID;
    unset($toSort[$ID]);
    while (count($toSort) > 0) 
      {
        $ID = $sortOrder[$next];
        $AGLOBAL['DEBUG']->line('ID: ' . $ID, 11);
        $sorted[] = $toSort[$ID];
        $next = $ID;
        unset($toSort[$ID]);
      }
    return $sorted;
  }
?>