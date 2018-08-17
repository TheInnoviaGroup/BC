<?php
/**
 * Alib Master File -- just include_once and forget.
 * This file inits several variables. So better check
 * them. (They're documented below). It also defines the anObject
 * object, the one from which all other alib objects should be
 * defined.
 * @package aLib
 * @author BrYan P. Johnson
 * @version 3.0
 */
/**
 * function alib_init -- initializes the alib. Don't worry. Just ignore it.
 */

function alibInit() 
  {
    addIncludePath('.');
    include_once ('debug.inc');
    global $debug, $debugLevel;
    $debug = Debug::singleton();
    $debug->level = ($debugLevel) ? $debugLevel : 0;
    include_once ('configHandler.inc');
    # include_once('db.inc');
    
  }
/**
 * Add a path to the include path, independent of system
 *
 * @param string or array $directory Directory(s) to add to the include path
 * @param bool $recurse Add subdirectories to the include path
 * @return none
 */

function addIncludePath($directory, $recurse = FALSE) 
  {
    if (is_array($directory)) 
      {
        foreach($directory as $dir) 
          {
            addIncludePath($dir, $recurse);
          }
      }
    else
      {
          
        if (stristr(php_uname('s') , 'win') && !stristr(php_uname('s') , 'dar')) 
          {
            /* We're on Windows */
            $delim = ";";
          }
        else
          {
            $delim = ":";
            /* Pretty much everyone else. */
          }
        set_include_path(get_include_path() . $delim . $directory);
        if ($recurse) 
          {
              
            if ($handle = opendir($directory)) 
              {
                          
                while (false !== ($filename = readdir($handle))) 
                  {
                     
                      
                    if ($filename != '.' && $filename != '..') {
                         if (is_dir($directory.'/'.$filename)) 
                      {
                         
                        addIncludePath($directory.'/'.$filename, $recurse);
                      }
                         }
                  }
              }
          }
      }
  } # End addIncludePath

/**
 * We autoload any object someone tries to load. If there is a file by
 * the same name as the object, we load it. Since object class names
 * are case insensitive, we downcase the filename. No more including!
 */

function __autoload($class_name) 
  {
    if (!include_once (strtolower($class_name) . ".inc")) 
      {
        include_once ($class_name . ".inc");
      }
  }
/** 
 * The aLib abstract standard object.
 *
 * This way we ensure that all of our objects have the same
 * interfaces.
 * @package aLib
 * @subpackage Utility
 * @author BrYan P. Johnson
 * @version 3.0
 */
abstract class anObject
  {
    public 
    function __construct() 
      {
      }
    public 
    function __get($name) 
      {
        global $debug;
        if ($debug->level >= 10) 
          {
            $debug->line("Object (" . get_class($this) . ") __get $name = ", 10);
            $debug->variable($this->$name, 10);
          }
        return $this->$name;
      }
    /**
     * This catches any calls to undefined functions.
     *
     * Yum. With debuglevel of 0, it's silent.
     */
    public 
    function __call($f, $a) 
      {
        global $debug;
        if ($debug->level >= 1) 
          {
            $c = get_class($this);
            $m = get_class_methods($this);
            $debug->line("Undefined function call: " . $f . " from class " . $c . " with args:", 1);
            $debug->variable($a, 1);
            $debug->line("Actual allowed methods for " . $c . ":", 1);
            $debug->variable($m, 1);
          }
      }
    public 
    function __set($name, $value) 
      {
        global $debug;
        if ($debug->level >= 10) 
          {
            if (isset($this->$name)) 
              {
                $debug->line("Object (" . get_class($this) . ") changing $name. Was:", 10);
                $debug->variable($this->$name, 10);
                $debug->line("Object (" . get_class($this) . ") changing $name. To:", 10);
                $debug->variable($value);
              }
            else
              {
                $debug->line("Object (" . get_class($this) . ") setting " . $name, 10);
                $debug->variable($value, 10);
              }
          }
        $this->$name = $value;
      }
  }
/* Now I do the alibInit because why wouldn't you? And why should I make
you? */
alibInit();
?>
