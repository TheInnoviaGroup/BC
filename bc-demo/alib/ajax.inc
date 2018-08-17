<?PHP
/**
 * aLib AJAX object.
 *
 * This object is simply a wrapper for the JPSPAN library
 * (http://jpspan.sourceforge.net/wiki/doku.php?id=start). I as yet
 * haven't done anything with the javascript side of things. To use:
 *
 * <code>
 * $ajax = new Ajax();
 * $ajax->addObject("myObjectName"); // Adds the object to the ajax registry.
 * $allowedMethods = array("method1", "method3");
 * $ajax->addObject("object2", $allowedMethods); 
 * // Adds object2 to the ajax registry, but only allows the methods
 * // "method1" and "method3", not "method2"
 * $ajax->addFunction("functionName"); 
 * // Adds the function "functionName" accessable through the javascript object 
 * // method ajaxfunction.functionname
 * $ajax->start(); // Does the actual ajax work.
 * </code>
 *
 * @package aLib
 * @subpackage AJAX
 *
 */



class Ajax extends anObject {
  private $po;
  private $addedFunctions = FALSE;
  private $funcs = array();
  

  /**
   * Creation of the Ajax object require's that JPSpan be in your include path.
   */

  function __construct () {
    $path = dirname(__FILE__) . '/jpspan/';
    set_include_path(get_include_path() . ";" . $path);

    // Including this sets up the JPSPAN constants
    require_once 'JPSpan.php';
    // Load the PostOffice server
    require_once JPSPAN . 'Server/PostOffice.php';
    $this->po = & new JPSpan_Server_PostOffice();
    parent::__construct();
    }

  /**
   * Add an object to the Ajax registry.
   * @param string $obj The class name to add to the registry.
   * @param array $methods A list of methods allow. If omitted, all public methods not starting with an underscore will be added.
   */
  function addObject ($obj, $methods = NULL) {
    if ($methods) {
      $desc = new JPSpan_HandleDescription();
      $desc->Class = $obj;
      $desc->methods = $methods;
    } else {
      $desc = NULL;
    }
    $this->po->addHandler(new $obj, $desc);
  }

  /**
   * Add a function to the ajax registry. Accessed via the javascript object method ajaxfunction.functionname
   * @param string $function Name of the function to add.
   */
  function addFunction ($function) {
    $this->funcs[] = $function;
  }

  /**
   * Returns a string containing the javascript necessary to
   * encapsulate the PHP objects in Javascript. Shouldn't need this, as
   * the start method does it for you.
   * @param bool $compress If true, removes extra whitespace from the javascript.
   */
  function getJavascript ($compress=FALSE) {
    if (!$this->addedFunctions && count($this->funcs) > 0) {
      //    dynamicAjaxFunctionObject($this->funcs);
      //$this->addObject(new ajaxFunction);
    }

    define('JPSPAN_INCLUDE_COMPRESS',$compress);
    $G = & $this->po->getGenerator();
    require_once JPSPAN . 'Include.php';
    $I = & JPSpan_Include::instance();
    // HACK - this needs to change
    $I->loadString(__FILE__,$G->getClient());
    $string = $I->getCode();
    return $string;
  }

  /**
   * This does it all. Determines if we're the client or server (in JPSPAN parlance) and does the right thing.
   */
  function start () {
    if (!$this->addedFunctions && count($this->funcs) > 0) {
      dynamicAjaxFunctionObject($this->funcs);
      $this->addObject(new ajaxFunction);
    }
    global $_SERVER;
    if (isset($_SERVER['QUERY_STRING']) &&
        strcasecmp($_SERVER['QUERY_STRING'], 'client')==0) {
      // Compress the output Javascript (e.g. strip whitespace)
      define('JPSPAN_INCLUDE_COMPRESS',TRUE);
      
      // Display the Javascript client
      $this->po->displayClient();
      
    } else {
      
      // This is where the real serving happens...
      // Include error handler
      // PHP errors, warnings and notices serialized to JS
      require_once JPSPAN . 'ErrorHandler.php';
      
      // Start serving requests...
      $this->po->serve();
      
    }
    
    
  }
  
}

function dynamicAjaxFunctionObject ($functions = NULL) {
  if (!$functions) {
    $functions = array();
  }
  $evalThis = "class ajaxFunction extends anObject {



  function __construct () {

    parent::__construct();
   }
  ";
  
  foreach ($functions as $f) {
    
    $evalThis .= "function ".$f." () {
  \$a = func_get_args();
  \$eval = \"".$f." (\";
      \$count = -1;
      foreach (\$a as \$b) {
	\$count++;
	\$eval .= \"\\\$a[\" . \$count . \"],\";
      }
      if (\$count > -1) {
	\$eval = substr(\$eval, 0, -1);
      }
      \$eval .= \");\";
      \$retval = eval(\$eval);
      return \$retval;
   }
";
  }

  $evalThis .= "}";
    eval($evalThis);
}


?>