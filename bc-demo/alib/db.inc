<?php
/**
 * mysql object
 * Use this or the db object. They're the same.
 * @package aLib
 * @subpackage Database
 * @version 0.4
 * @todo Make db know about serialize and unserialize.
 */
class mysql extends anObject
  {
    public $user;
    public $pass;
    public $db;
    public $host;
    public $link;
    public $results = array();
    public $nameRow = array();
    /**
     * Whether or not to use the cache.
     * @var bool $useCache default is FALSE.
     */
    public $useCache = FALSE;
    /**
     * Directory for caching. Set BEFORE useCache.
     * @var string $cacheDir default is "alibcache/".
     */
    public $cacheDir = "alibcache/";
    /**
     * Cache expiry. Set BEFORE useCache.
     * @var bool $cacheExpire default is 3600.
     */
    public $cacheExpire = 3600;
    public $cache;
    /**
     * Constructor method
     * Params may be either an array or a dsn.
     * @param mixed $args an array of [$user, $pass, $db, $host]
     * @param string $dsn alternate param, a valid dsn is: mysql://user:pass@host/db
     * @return mixed
     */
    
    function __construct() 
      {
        $nargs = func_num_args();
        if ($nargs > 0) 
          {
            $args = func_get_args();
            if ($nargs == 1) 
              {
                $this->setup($args[0]);
              }
            else
              {
                $this->setup($args[0], $args[1], $args[2], $args[3]);
              }
          }
        parent::__construct();
      }
    /**
     * __set method set up to catch if someone sets the variable
     * "login". If so, we run setup. Otherwise, we just set the variable.
     */
    
    function __set($name, $value) 
      {
        if ($name == "login") 
          {
            global $debug;
            $debug->line("db.inc: variable \"login\" set. Running setup", 10);
            if (is_array($value)) 
              {
                $this->setup($value[0], $value[1], $value[2], $value[3]);
              }
            else
              {
                $this->setup($value);
              }
          }
        elseif($name == "useCache") 
          {
            if ($value) 
              {
                $this->cache = new dbCache($container = 'file', $container_options = array(
                    'cache_dir' => $this->cacheDir,
                    'filename_prefix' => 'dbcache_'
                ) , $this->cacheExpire);
              }
          }
        parent::__set($name, $value);
      }
    /**
     * Constructor method
     * Params may be either an array or a dsn. need to update for assoc array
     * @param mixed @args an array of [$user, $pass, $db, $host]
     * @param string $dsn alternate param, a valid dsn is: mysql://user:pass@host/db
     * @return mixed
     * @deprecated deprecated since version 0.1
     * @see mysql::__construct()
     */
    public 
    function setup() 
      {
        $nargs = func_num_args();
        $args = func_get_args();
        if ($nargs == 1) 
          {
            if (is_array($args[0])) 
              {
                $this->user = $args[0]['user'];
                $this->pass = $args[0]['password'];
                $this->db = $args[0]['db'];
                $this->host = $args[0]['host'];
              }
            else
              {
                if (preg_match("#mysql:\/\/([^:]+):([^@]+)\@([^\/]+)\/(.*)#", $args[0], $matches)) 
                  {
                    $this->user = $matches[1];
                    $this->pass = $matches[2];
                    $this->db = $matches[4];
                    $this->host = $matches[3];
                  }
              }
          }
        else
          {
            $this->user = $args[0];
            $this->pass = $args[1];
            $this->db = $args[3];
            $this->host = $args[2];
            //      var_dump($this);
            
          }
      }
    /**
     * Establish a link to the db.
     */
    public 
    function connect() 
      {
        //$res = result code; 1 = connected, 0 = not
        $res = 0;
        global $debug;
        $debug->line("Attempting...", 10);
        $this->link = mysql_connect($this->host, $this->user, $this->pass);
        $this->_error();
        if ($this->link) 
          {
            $debug->line("Selecting DB", 10);
            mysql_select_db($this->db);
            $this->_error();
            $res = 1;
          }
        else
          {
            $this->_error();
          }
        return $res;
      }
    /**
     * Disconnect from the db.
     * @see mysql::__destruct()
     */
    public 
    function disconnect() 
      {
        // if($this->link){ mysql_close($this->link); }
        
      }
    /**
     * Automatically called when the object is destroyed.
     * @see mysql::disconnect()
     */
    
    function __destruct() 
      {
        $this->disconnect();
      }
    /** Query the db.  
     *
     * This is the big bad. This is how you execute sql queries. It
     * returns the results to you, as well as leaving them accessible
     * via {@link mysql::next_record()}.
     *
     * if $db->useCache is TRUE, this function caches the sql query and
     * returns the cached result if it hasn't expired. Force a cache
     * flush with $this->cache->flush(). To remove a single sql query
     * (like after you do an update), do $this->cache->remove($query);
     *
     * @param string $q the sql query to execute
     * @param string $name [optional] a name for the query so that multiple result
     * sets are accessible via {@link mysql::next_record()}.
     * @return mixed $results the associative array of results.
     */
    public 
    function query($q, $name = "Default", $unbuffered = FALSE) 
      {
        if ($q == "") 
          {
            return 0;
          }
        global $debug;
        $debug->line("Got query $q", 10);
        $doQuery = TRUE;
        $cache_id = md5($q);
        if ($this->useCache) 
          {
            /* See if we've got it in the cache */
            if (stristr($q, 'SELECT')) 
              {
                $debug->line("Checking for cache.", 10);
                // Compute unique cache identifier for this query
                $tempRes = $this->cache->get($cache_id, 'alib_db_query_cache');
                ($tempRes == NULL) ? $doQuery = TRUE : $doQuery = FALSE;
              }
          }
        if ($doQuery) 
          {
            if (!$this->link) 
              {
                $debug->line("Connecting", 10);
                $this->connect();
              }
            $this->changeDb($this->db);
            if ($unbuffered) 
              {
                $tempRes = mysql_unbuffered_query($q, $this->link);
              }
            else
              {
                $tempRes = mysql_query($q, $this->link);
              }
            if ($this->useCache) 
              {
                $this->cache->save($cache_id, $tempRes, $this->cacheExpire, 'alib_db_query_cache');
              }
          }
        $debug->variable($tempRes, 10);
        $this->results["$name"] = $tempRes;
        return $tempRes;
      }
    /**
     * Frees results from memory.
     * @param string $name [optional] the name of the query to free
     * @todo make this free the object memory as well as the mysql memory
     */
    public 
    function free($name = "Default") 
      {
        mysql_free_result($this->results[$name]);
      }
    /**
     * Return an associative array for the next row of the result set.
     * @param string $name [optional] the name of the query
     * @param const $result_type [optional] the type of result returned
     * The optional second argument result_type in mysql_fetch_array()
     * is a constant and can take the following values: MYSQL_ASSOC,
     * MYSQL_NUM, and MYSQL_BOTH. MYSQL_BOTH is the default for this
     * argument.
     * @return mixed $row The next row of results
     * @deprecated use nextRecord()
     */
    public 
    function next_record($name = "Default", $result_type = MYSQL_BOTH) 
      {
        if ($this->results["$name"]) 
          {
            return $this->nameRow[$name] = mysql_fetch_array($this->results[$name], $result_type);
          }
      }
    /**
     * Return an associative array for the next row of the result set.
     * @param string $name [optional] the name of the query
     * @param const $result_type [optional] the type of result returned
     * The optional second argument result_type in mysql_fetch_array()
     * is a constant and can take the following values: MYSQL_ASSOC,
     * MYSQL_NUM, and MYSQL_BOTH. MYSQL_BOTH is the default for this
     * argument.
     * @return mixed $row The next row of results
     */
    public 
    function nextRecord($name = "Default", $result_type = MYSQL_BOTH) 
      {
        if ($this->results["$name"]) 
          {
            return $this->nameRow[$name] = mysql_fetch_array($this->results[$name], $result_type);
          }
      }
    /**
     * Return all records from a result set.
     * @param string $name [optional] the name of the query
     * @return mixed $array An array containing all of the results.
     */
    public 
    function allRecords($name = "Default", $result_type = MYSQL_BOTH) 
      {
        $array = array();
        while ($res = $this->nextRecord($name, $result_type)) 
          {
            $array[] = $res;
          }
        return $array;
      }
    /**
     * Return the number of rows affected by the last query executed.
     */
    public 
    function affected_rows() 
      {
        return mysql_affected_rows($this->link);
      }
    public 
    function affectedRows() 
      {
        return mysql_affected_rows($this->link);
      }
    /**
     * Return one value from one field from the current row of a query.
     * @param string $column_name Name of the column whose data to return.
     * @param string $name [optional] name of the query
     * @return mixed $result
     */
    public 
    function getField($f, $name = "Default") 
      {
        return $this->nameRow[$name][$f];
      }
    public 
    function ChangeDb($db) 
      {
        $this->db = $db;
        mysql_select_db($db);
      }
    /**
     * Returns the size of the result set of the last query.
     * @param string $name [optional] name of the query
     * @return int
     */
    public 
    function Size($name = "Default") 
      {
        $rows = 0;
        if ($this->results["$name"]) 
          {
            $rows = mysql_num_rows($this->results["$name"]);
            //} else {
            //pre(debug_backtrace());
            
          }
        return $rows;
      }
    public 
    function sizeOf($name = "Default") 
      {
        return mysql_num_rows($this->results["$name"]);
      }
    protected 
    function _error() 
      {
        $error = mysql_error($this->link);
        global $debug;
        $debug->line("DB error: " . $error, 9);
        $debug->variable($this, 10);
      }
    public 
    function isError() 
      {
        return (mysql_errno($this->link) > 0) ? TRUE : FALSE;
      }
    public 
    function getError() 
      {
        $error = mysql_error($this->link);
        return $error;
      }
    public 
    function insertID() 
      {
        return mysql_insert_id($this->link);
      }
    /**
     * Prepare an sql statement from mixed variables.
     * Oh this is wacky. I hate writing sql. I have data I like
     * already, I don't want to reformat it. This takes my data and
     * makes a pretty sql statement for me. Why would you use this? If
     * you have a really large array to insert and you're lazy. Like
     * me.
     *
     * Args are type, table, mixedvars1 and where.
     * Follow along:
     * If type is insert, mixedvars1 is what to insert
     * If type is update or replace, mixedvars1 is what to
     * update/replace and where is your where statement
     *
     * mixedvars1 may be an array or a string. If
     * insert/update/replace an associative array is assumed to be
     * column=>value. A string is a string.
     *
     * <i>Where statements at this time must be strings.</i>
     * @see user::commit() for an example.
     */
    public 
    function prepare() 
      {
        $args = func_get_args();
        $type = $args[0];
        $table = $args[1];
        if (stristr($type, "update")) 
          {
            $buildme = sprintf("%s %s SET ", $type, $table);
          }
        else
          {
            $buildme = sprintf("%s INTO `%s` SET ", $type, $table);
          }
        $insertMe = $args[2];
        if (is_array($insertMe)) 
          {
            foreach($insertMe as $col => $val) 
              {
                if (!preg_match("/^[\d]+$/", $col)) 
                  {
                    $temp = sprintf("`%s` = '%s', ", $col, $val);
                    $buildme.= $temp;
                  }
              }
            // Remove the last ", " and add a " "
            $buildme = substr($buildme, 0, -2);
            $buildme.= " ";
          }
        else
          {
            $buildme.= $insertMe . " ";
          }
        if (strtolower($type) != "insert") 
          {
            if (count($args) > 3) 
              {
                $where = $args[3];
              }
            $buildme.= $where;
          }
        return $buildme;
      }
  }
/**
 * A class just for convenience. Saves you three whole letters.
 * @package aLib
 * @subpackage Database
 */
class db extends mysql
  {
    /**
     * The greatest object method in the world!
     */
    
    function f() 
      {
        print_d("f", 1);
      }
  }
?>
