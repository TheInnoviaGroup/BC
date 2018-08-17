<?PHP
include_once ('Cache.php');
/**
 * Db Cache for use by the db object. You shouldn't use this, just set
 * $db->useCache = TRUE; and the db object does all the work. Note, it
 * uses file caching.
 *
 * There *are* two variables you can set. $db->cacheDir is the directory
 * in which to store our query cache. Default is alibcache/
 * $db->cacheExpire is the number of seconds to store a query
 * for. Default is 3600. You must set those before setting
 * $db->useCache to TRUE.
 *
 * This requires the Pear Cache object to be available.
 * @package aLib
 * @subpackage Database
 * @version 0.4
 */
class dbCache extends Cache
  {
    var $expires = 3600;
    var $dir = "alibcache";
    var $result = array();
    
    function __construct($container = 'file', $container_options = array(
        'cache_dir' => '.',
        'filename_prefix' => 'dbcache_'
    ) , $expires = 3600) 
      {
        $this->Cache($container, $container_options);
        $this->expires = $expires;
      }
    
    function __destruct() 
      {
        $this->_Cache();
      }
    
    function remove($query) 
      {
        $id = md5($query);
        parent::remove($id, 'alib_db_query_cache');
      }
  }
?>