<?php

class iuser extends anObject
{
    private $userData = array();
    private $changedData = array();
    private $isloaded = FALSE;
    
    /**
     * User ID For the auth cookie 
     */
    public $cookieUserID = 'aLibUser';
    /**
     * token for the auth cookie 
     */
    public $cookieUserToken = 'aLibToken';
    /**
     * An arbitrary string to hash the cookie auth token with.
     */
    public $userCookieSalt = "alibcookiesalt";
    /**
     * When the auth cookie expires (as always, in utime)
     */
    public $userCookieExpire; 
    /**
     * Path for the cookie.
     */
    public $userCookiePath = "/"; # See the setcookie() docs for these.
    /**
     * Domain for the cookie.
     */
    public $userCookieDomain = NULL;
    /**
     * Whether the cookie is HTTPS only or not.
     */
    public $userCookieSecure = 0;
    /**
     * Whether or not the user is logged in
     */
    public $loggedIn = FALSE;
    /**
     * Array of columns to search for the UID specified in the
     * constructor
     */
    public $uidColumns = array('userID', 'username', 'email');

    /** 
     * Field of the user table that indicates whether a user is active
     *  or not.
     */
    public $userStatusField = 'status';
    

   /**
     * Constructor
     *
     * @param string $uid User Identifier
     * @return user object
     */
    function __construct( $uid = NULL ) {
        if (!is_null($uid)) 
          {
            $this->userData['uid'] = $uid;
          }
        global $config;
        $this->cookieUserID = ($config->cookieUserID)?$cookie->cookieUserID:$this->cookieUserID;
        $this->cookieUserToken = ($config->cookieUserToken)?$cookie->cookieUserToken:$this->cookieUserToken;
        $this->cookieUserSalt = ($config->cookieUserSalt)?$cookie->cookieUserSalt:$this->cookieUserSalt;
        $this->cookieUserExpire = ($config->cookieUserExpire)?$cookie->cookieUserExpire:(time() +60*60*24*30); #30 days from now.
        $this->cookieUserPath = ($config->cookieUserPath)?$cookie->cookieUserPath:$this->cookieUserPath;
        $this->cookieUserDomain = ($config->cookieUserDomain)?$cookie->cookieUserDomain:$this->cookieUserDomain;
        $this->cookieUserSecure = ($config->cookieUserSecure)?$cookie->cookieUserSecure:$this->cookieUserSecure;
        parent::__construct($uid);
    }

    function __get($name) 
      {
          if ( property_exists(__CLASS__, $name) ) {
              return $this->{$name};
          }
          global $debug;
# make this populate the object if it hasn't been already
          if ((!$this->_isloaded) && isset($this->userData['uid'])) 
            {
                $debug->line("Loading User...", 9);
                $this->_loaduser();
                $debug->line("Wanted $name and got " . $this->userData[$name], 9);
            }
          return $this->userData[$name];
      }
    
    function __set($name, $value) 
      {
          if ( property_exists(__CLASS__, $name) ) {
              $this->{$name} = $value;
          }
          global $debug;
# then make this populate the object if it hasn't been already
          if ($debug->level >= 10) 
            {
                if (isset($this->{$name})) 
                  {
                      $debug->debug("Changing $name. From: %s To: %s", $this->{$name}, $this->{$value}, 10);
                  } else {
                      $debug->debug("Setting %s to %s", $name, $value, 10);
                  }
            }
          if (!$this->_isloaded && isset($this->userData['uid'])) 
            {
                $debug->debug("Loading User...", 10);
                $this->_loaduser();
            }
          $this->userData[$name] = $value;
          $this->changedData[$name] = 1;
      }

    private function _loaduser() 
      {
        global $debug;
        if ( $this->userData['uid'] == -1 ) {
            return false;        
        }
        $q = "select * from users where "; # if you change this query, change it below in commit();
        foreach( $this->uidColumns as $column ) {
            $q .= "`$column` = '".$this->userData['uid']."' OR ";
        }
        $q = substr( $q, 0, -4 );
                
        $debug->debug("Loading user %s with sql: %s", $this->userData['uid'], $q, 9);
        $myDB = $this->_getUserDB();
        
        
        $myResponse = $myDB->query($q);
        if ($myResponse->num_rows > 0) 
          {
            $this->userData = $myResponse->fetch_assoc();
            $this->_isloaded = TRUE;
            $debug->debug("Loaded %s", $this->userData, 9);
          }
        $debug->debug($myDB, 10);
      }


/**
  * Gets an instance of the userDB
  *
  * @return db object
  */
    private function _getUserDB() {
        global $config, $db, $userDB;
        $myDB = array();
        #If we don't have a configured user db, or if it's the same as
        #the main db, use the global $db object.
        if ( !$config->userDB || $config->userDB == $config->mainDB ) {
            if ( is_object($db) ) {
                $myDB = $db;
            } else {
                $userDB = new idb($config->mainDB);
                $myDB = $userDB;
            }
        } else {
            if ( is_object($userDB) ) {
                $myDB = $userDB;
            } else {
                $userDB = new idb($config->userDB);
                $myDB = $userDB;
            }
        }
        return $myDB;
    }

    /**
     * Commits changes made to the user object to the database.
     * This is very dangerous! Any variable you set via
     * <code>$user->somevariable = "somevalue"</code> will try to
     * insert itself into the db in the column "somevariable." The
     * exception is <code>$user->extraData[]</code> which should be
     * for purely temporary things.
     */
    public function commit() 
      {
        global $debug;
        $myDB = $this->_getUserDB();
        
        # make this make __set's go to the db.
        $temp = $this->userData;
        unset($temp['extraData']);
        $uid = $temp['uid'];
        unset($temp['uid']);
        $updateMe = array();
        foreach($temp as $key => $val) 
          {
            if ($this->changedData[$key]) 
              {
                $updateMe[$key] = $val;
              }
          }
        $sql1 = $this->_prepare("UPDATE", "users", $updateMe, "where username='" . $temp['username'] . "'");
        $sql = preg_replace("/\`passwd\` = '([^\']+)'/", "`passwd` = PASSWORD('$1')", $sql1);
        $debug->line("Converted \"$sql1\" to \"$sql\"", 3);

        $debug->line("Committing $sql", 9);
        $retval = $myDB->query($sql);
        $this->updateMe = array();
        return $retval;
      }
    public function addUser() 
      {
        global $debug;
        $myDB = $this->_getUserDB();
        # make this make __set's go to the db.
        $temp = $this->userData;
        unset($temp['extraData']);
        $uid = $temp['uid'];
        unset($temp['uid']);
        $sql1 = $this->_prepare("INSERT", "users", $temp);
        $sql = preg_replace("/`passwd` = '([^\']+)'/", "`passwd` = PASSWORD('$1')", $sql1);
        $debug->line("Committing $sql", 9);
        $myDB->query($sql);
        if ($myDB->insert_id > 0) 
          {
            $this->userID = $myDB->insert_id;
            $retval = TRUE;
          }
        else
          {
            $retval = FALSE;
          }
        return $retval;
      }
    private function _prepare() 
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
            # Remove the last ", " and add a " "
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
  

         /**
     * Authenticate the user.
     * @param string $u username
     * @param string $p password
     * @return bool zero for fail, one for succeed
     */
    
    function Auth($u, $p) 
      {
        global $debug, $config;
        $myDB = $this->_getUserDB();
        
        if ( $config->userStatusField ) {
            $sf = $config->userStatusField;
        } else {
            $sf = 'status';
        }
            $q = "select * from users where ("; # if you change this query, change it below in commit();
        foreach( $this->uidColumns as $column ) {
            $q .= "`$column` = '".$u."' OR ";
        }
        $q = substr( $q, 0, -4 );
                
        $q .= ") AND passwd=PASSWORD('$p') AND $sf > 0";
        $debug->line($q, 5);
        
        $res = $myDB->query($q);
        
        if ($res->num_rows > 0) 
          {
            return 1;
          }
        else
          {
            return 0;
          }
    }

         /**
     * Use the browser's HTTPAuth mechanism for authentication
     * @param string $html [optional] String of html for displaying if authentication fails.
     * @param string $realm [optional] Authentication realm
     * @return bool zero for fail, one for succeed
     */
    
    function HTTPAuth($html = "Authorization Required", $realm = "Private") 
      {
        global $AGLOBAL, $debug;
        $val = 0;
        global $PHP_AUTH_USER, $PHP_AUTH_PW;
        if (isset($PHP_AUTH_USER) && isset($PHP_AUTH_PW)) 
          {
            $val = $this->Auth($PHP_AUTH_USER, $PHP_AUTH_PW);
          }
        if ($val == 0) 
          {
            header('WWW-Authenticate: Basic realm="' . $realm . '"');
            header('HTTP/1.0 401 Unauthorized');
            echo $html;
            exit;
          }
        return $val;
      }
    /**
     * Logout someone who authenticated using cookies.
     */
    
    function CookieLogout() 
      {
        global $debug;
        $debug->line('Cookie Logout Called', 5);
        global $_COOKIE;
        $u = $_COOKIE[$this->cookieUserID];
        $t = $_COOKIE[$this->cookieUserToken];
        setcookie($this->cookieUserToken, "", time() -3600, $this->cookieUserPath, $this->cookieUserDomain, $this->cookieUserSecure);
        setcookie($this->cookieUserID, "", time() -3600, $this->cookieUserPath, $this->cookieUserDomain, $this->cookieUserSecure);
        unset($_COOKIE[$this->cookieUserID]);
        unset($_COOKIE[$this->cookieUserToken]);
      }
    /**
     * Authenticate someone using cookies
     * If called with no args, checks to see if there's a valid
     * cookie. If called with username and password, it attempts to
     * authenticate with {@link user::Auth()} and if successful sets a
     * cookie.
     * @param string $username [optional] Username
     * @param string $password [optional] Password
     * @return bool zero for failure, one for success. (On success,
     * <code>$this->uid</code> and <code>$this->loggedIn</code> are
     * set appropriately.
     */
    
    function CookieAuth() 
      {
        global $debug;
        $debug->line('Cookie Auth Called', 5);
        $myDB = $this->_getUserDB();
        
        #Called with no args, checks to see if there's a valid cookie.
        # Return 1 if valid cookie, otherwise, 0. Also sets $user->uid to cookie id and $user->loggedIn to 1;
        # Called with a username/password, does auth & sets cookie.
        global $_COOKIE, $config;
        $u = $_COOKIE[$this->cookieUserID];
        $t = $_COOKIE[$this->cookieUserToken];

        if ( $config->userStatusField != '' ) {
                  $sf = $config->userStatusField;
              } else {
                  $sf = 'status';
              }



        $debug->line("Got $u $t", 5);
        $debug->variable($_COOKIE, 5);
        if (func_num_args() > 0) 
          {

              $debug->line('func_num_args > 0', 5);
              $args = func_get_args();
              $debug->variable($args, 5);
              $debug->line("Got args", 5);
              $u = $args[0];
              $p = $args[1];
            if ($this->Auth($u, $p)) 
              {
                $q = "select passwd from users where ("; # if you change this query, change it below in commit();
                  foreach( $this->uidColumns as $column ) {
                      $q .= "`$column` = '".$u."' OR ";
                  }
                $q = substr( $q, 0, -4 );
                
                  $q .= ") AND $sf > 0";
       
                $debug->line('Succeeded auth, Query: ' . $q, 5);
                $res = $myDB->query($q);
                $row = $res->fetch_assoc();
                $h = $row['passwd'];
                $debug->line('h is ' . $h, 5);
                $token = md5($u . $h . $this->cookieUserSalt);
                $debug->line('token is ' . $token, 5);
                setcookie($this->cookieUserToken, $token, $this->USERCOOKIEEXPIRE, $this->cookieUserPath, $this->cookieUserDomain, $this->cookieUserSecure);
                setcookie($this->cookieUserID, $u, $this->USERCOOKIEEXPIRE, $this->cookieUserPath, $this->cookieUserDomain, $this->cookieUserSecure);
                $this->uid = $u;
                $this->loggedIn = 1;
                return 1;
              }
            else
              {
                $debug->line("Failed Cookie Auth, Query: " . $q, 5);
                $this->loggedIn = 0;
                return 0;
              }
          }
        else
          {
            if (isset($_COOKIE[$this->cookieUserID]) && isset($_COOKIE[$this->cookieUserToken])) 
              {
                $debug->line('Want username ' . $u, 5);
                $debug->line('Want token ' . $t, 5);
                
                $q = "select passwd from users where ("; # if you change this query, change it below in commit();
                  foreach( $this->uidColumns as $column ) {
                      $q .= "`$column` = '".$u."' OR ";
                  }
                $q = substr( $q, 0, -4 );
                
                  $q .= ") AND $sf > 0";
                #This is a hashed password. We don't care.
                
                $res = $myDB->query($q);
                
                $row = $res->fetch_assoc();
                $h = $row['passwd'];
                $debug->line('h is ' . $h, 5);
                $debug->line('want ' . md5($u . $h . $this->cookieUserSalt) , 5);
                if ($t == md5($u . $h . $this->cookieUserSalt)) 
                  {
                    $debug->line('Auth succeeded', 5);
                    $this->uid = $u;
                    $this->loggedIn = 1;
                    return 1;
                  }
                else
                  {
                    $debug->line('auth failed', 5);
                    $this->loggedIn = 0;
                    return 0;
                  }
              }
            else
              {
                $this->loggedIn = 0;
                return 0;
              }
          }
      }
    

};

?>
