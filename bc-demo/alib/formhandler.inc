<?PHP
define("NIGHTMAREFORM", -1); // Don't ask.

/**
 * @constant Uses session variables and shared secrets to validate the form entry. You must use $form->generateSecret(), also includes requirements of less strict safety levels.
 */
define("PARANOIDFORM", 0);
/**
 * @constant requires that the form be submitted via SSL, also includes requirements of less strict safety levels.
 */
define("STRICTFORM", 1);
/**
 * @constant uses $form->requiredVars and $form->unsafeVars, also includes requirements of less strict safety levels.
 */
define("MODERATEFORM", 2);
/**
 * @constant strips slashes and html encodes all variables.
 */
define("LAZYFORM", 3);
/**
 * Safe handling of form variables. Well, as safe as anything can be.
 * Data from form submissions is accessed via $form->variable_name
 * where variable_name was the value of the "name" parameter in the
 * HTML form element.
 * @package aLib
 * @subpackage HTML
 * @author BrYan P. Johnson
 * @version 0.4
 * @example demo/formtest.php Shows MODERATEFORM level security in use.
 */
class formHandler extends anObject
  {
    public $formVars;
    private $unsafeVars;
    private $safeVars;
    private $safetyLevel;
    private $wantVars;
    private $unwantedVars;
    public $error;
    /**
     * Constructor. Called when $form = new FormHandler() is called.
     * @param int $safetyLevel How "safe" to ensure the data is. Several constants are defined: PARANOIDFORM, STRICTFORM, MODERATEFORM and LAZYFORM
     * @param mixed $safeVars [optional] Array of variable names we know should be included. Any other form data submitted via POST or GET will be ignored.
     * @param mixed $unsafeVars [optional] Array of variable names to specifically not include.
     * @return bool Returns true if the form data submitted passed safety checks, false if otherwise.
     */
    
    function __construct($safetyLevel = 2, $safeVars = NULL, $unsafeVars = NULL) 
      {
        global $debug, $_GET, $_POST;
        $this->safetyLevel = $safetyLevel;
        $this->wantVars = $safeVars;
        $this->unwantedVars = $unsafeVars;
        switch ($this->safetyLevel) 
          {
        case -1:
            $this->error = "No form processing available in Nightmare mode.";
            $debug->line($this->error, 10);
            return 0; // Haha.
            
        break;
        case 0:
            return $this->paranoid();
        break;
        case 1:
            return $this->strict();
        break;
        case 2:
            return $this->moderate();
        break;
        default:
            return $this->lazy();
          }
      }
    /**
     * Returns an html hidden input string with a shared secret to be
     * used with PARANOIDFORM level security. Use this when creating the
     * form via html, before submission. Upon submission it will be
     * checked against the session variable of the same name.
     * @return string String of the form <input type="hidden" name="formsecret" value="some shared secret string">
     */
    
    function generateSecret() 
      {
        $secret = md5(uniqid(rand() , true));
        $_SESSION['formsecret'] = $secret;
        $string = sprintf("<input type=\"hidden\" name=\"formsecret\" value=\"%s\" />", $secret);
        return $string;
      }
    private 
    function paranoid() 
      {
        global $_SESSION, $_POST, $_GET;
        $want = (isset($_POST['formsecret'])) ? $_POST_['formsecret'] : $_GET['formsecret'];
        if ($want == $_SESSION['formsecret']) 
          {
            return $this->strict();
          }
        global $debug;
        $this->error = "Secrets did not match.";
        $debug->line($this->error, 10);
        $dumpme = array(
            'want' => $want,
            'session' => $_SESSION
        );
        $debug->variable($dumpme, 10);
        return 0;
      }
    private 
    function strict() 
      {
        global $_SERVER;
        if ($_SERVER["HTTPS"] == 'on') 
          {
            return $this->moderate();
          }
        global $debug;
        $this->error = "Not communicating via SSL, no form data accepted.";
        $debug->line($this->error, 10);
        return 0;
      }
    private 
    function moderate() 
      {
        global $_POST, $_GET;
        if (is_array($this->unwantedVars)) 
          {
            foreach($this->unwantedVars as $var) 
              {
                unset($_POST[$var], $_GET[$var]);
              }
          }
        if (is_array($this->wantVars)) 
          {
            foreach($this->wantVars as $var) 
              {
                if (isset($_POST[$var])) 
                  {
                    $this->safeVars[$var] = $this->wash($_POST[$var]);
                  }
                elseif(isset($_GET[$var])) 
                  {
                    $this->safeVars[$var] = $this->wash($_GET[$var]);
                  }
              }
          }
        return $this->lazy();
      }
    private 
    function lazy() 
      {
        global $_POST, $_GET;
        foreach($_POST as $key => $val) 
          {
            $this->formVars[$key] = $this->wash($val);
          }
        foreach($_GET as $key => $val) 
          {
            $this->formVars[$key] = $this->wash($val);
          }
        return 1;
      }
    private 
    function wash($input) 
      {
        $newval = $this->removeSlashes($input);
        return $this->htmlize($newval);
      }
    private 
    function removeSlashes($input) 
      {
        if (!get_magic_quotes_gpc() || (!is_string($input) && !is_array($input))) 
          {
            return $input;
          }
        if (is_string($input)) 
          {
            $output = stripslashes($input);
          }
        elseif(is_array($input)) 
          {
            $output = array();
            foreach($input as $key => $val) 
              {
                $newval = $this->removeSlashes($val);
                $output[$key] = $newval;
              }
          }
        return $output;
      }
    private 
    function htmlize($input) 
      {
        if (is_string($input)) 
          {
            $output = htmlspecialchars($input, ENT_QUOTES);
          }
        elseif(is_array($input)) 
          {
            $output = array();
            foreach($input as $key => $val) 
              {
                $newval = $this->htmlize($val);
                $output[$key] = $newval;
              }
          }
        return $output;
      }
    /**
     * Validate the data.
     * @param mixed $validations an associative array of the form:
     * <code>$validations = array('formvariablename' => '#someregularexpression#', #checks against a regexp
     *                      'formvariablename2' => '> 0', # checks that the value is > 0
     *                      'formvariablename3' => '< 1',); # checks that the value is less than 1.
     * $invalid_variables = $form->validate($validations);</code>
     * @return mixed $invalid_variables A list of variable names that failed validation.
     */
    public 
    function validate($validations) 
      {
        $checkthis = ($this->safetyLevel < 3) ? $this->safeVars : $this->formVars;
        $naughty = array();
        foreach($validations as $key => $type) 
          {
            if (preg_match("/^[\#\/]/", $type) && isset($checkthis[$key]) && !preg_match($type, $checkthis[$key])) 
              {
                //type is Regex match, key exists and the regex doesn't match the value
                $naughty[] = $key;
              }
            elseif(preg_match("/^[\>] (\d+)/", $type, $matches) && isset($checkthis[$key]) && ($checkthis[$key] <= $matches[1])) 
              {
                // type is greater than, key exists and value is less than or equal to...
                $naughty[] = $key;
              }
            elseif(preg_match("/^[\<] (\d+)/", $type, $matches) && isset($checkthis[$key]) && ($checkthis[$key] >= $matches[1])) 
              {
                // type is less than, key exists and value is less than or equal to...
                $naughty[] = $key;
              }
          }
        return $naughty;
      }
    public 
    function __get($name) 
      {
        if ($this->safetyLevel < 3) 
          {
            return (isset($name)) ? $this->safeVars[$name] : $this->safeVars;;
          }
        else
          {
            return (isset($name)) ? $this->formVars[$name] : $this->formVars;
          }
      }
    public 
    function __set($name, $value) 
      {
        return 1;
        #This is a read only object. Bite me.
        
      }
    /**
     * Reparse the form data based on new safe and unsafe vars. Note,
     * any variables already declared unsafe remain unsafe, and any
     * declared safe remain safe. This function only adds new "safe" data.
     * @param mixed $safeVars [optional] Array of variable names we know should be included. Any other form data submitted via POST or GET will be ignored.
     * @param mixed $unsafeVars [optional] Array of variable names to specifically not include.
     * @return bool Returns true if the form data submitted passed safety checks, false if otherwise.
     */
    public 
    function reparse($newSafe = NULL, $newUnsafe = NULL) 
      {
        global $debug;
        $safeVars = $this->wantVars;
        $unsafeVars = $this->unwantedVars;
        $this->wantVars = array_merge($safeVars, $newSafe);
        $this->unwantedVars = array_merge($unsafeVars, $newUnsafe);
        switch ($this->safetyLevel) 
          {
        case -1:
            $this->error = "No form processing available in Nightmare mode.";
            $debug->line($this->error, 10);
            return 0; // Haha.
            
        break;
        case 0:
            return $this->paranoid();
        break;
        case 1:
            return $this->strict();
        break;
        case 2:
            return $this->moderate();
        break;
        default:
            return $this->lazy();
          }
      }
  }
?>