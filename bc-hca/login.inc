<?PHP



/**

 * There are two ways one won't get a true login. One is if you don't attempt to log in, and one is if you do and fail.

 * If $login->loggedIn is false, you're in one of these error states. If $login->error == BADLOGIN, you failed auth.

 */



define("BADLOGIN", 1);





class login extends anObject {

  public $user;

  public $loggedIn;

  public $error;

  public $mmddYYYY = "/(0[1-9]|1[012])[-.\/](0[1-9]|[12][0-9]|3[01])[-.\/]((19|20)\d\d)/";



  

  

  function __construct () {

    global $debug, $arguments, $AGLOBAL, $config;



    $this->user = new bcUser();    



#    $form = new formhandler(2, array('username', 'password'));

    $form = new iForm(2); #Strict Form

    $form->addPost(); #We only want posted variables.

    $form->addCookie();

    

    

    $form->filter->lastname( FILTER_SANITIZE_STRING, FILTER_FLAG_ENCODE_AMP );

    $form->filter->firstname( FILTER_SANITIZE_STRING, FILTER_FLAG_ENCODE_AMP );

    $form->filter->last4ssn( FILTER_SANITIZE_STRING, FILTER_FLAG_ENCODE_AMP);

    $form->filter->enrollcode( FILTER_SANITIZE_STRING, FILTER_FLAG_ENCODE_AMP );


    $form->filter->submit( FILTER_SANITIZE_STRING, FILTER_FLAG_ENCODE_AMP );

    $form->filter->logout( FILTER_VALIDATE_BOOLEAN );




    $invalid = array();

    if ( $form->lastname === false ) {

        $invalid[] = 'lastname';

    }

    if ( $form->firstname === false ) {

        $invalid[] = 'firstname';

    }

    if ( $form->enrollcode === false ) {

        $invalid[] = 'enrollcode';

    }


    if ( $form->last4ssn === false ) {

        $invalid[] = 'last4ssn';

    }



    



    

    



    

    

    $badlogin = 0;

    $this->loggedIn = FALSE;

    if ($form->logout == 1  || !(stristr($_SERVER['REQUEST_URI'], "logout") === FALSE)) {

      $this->user->CookieLogout();

      $debug->level = 0;

    $template = new template($config->logoutTemplate);

    $template->set('title', $config->defaultTitle);

    $template->display();

    exit;

    }

    if ($this->user->CookieAuth()) {

      $debug->line("Passed cookieauth from cookie", 8);

      $this->user->loggedIn = TRUE;

      $this->user->_loaduser();

      $this->trackUser();

      $this->error = FALSE;

      $this->loggedIn = TRUE;

    } elseif ( $form->submit == 'Log In' ) {

        if ( count($invalid) > 0 ) {

                $debug->debug('Invalid Login Info: %s %s', $invalid, $form, 3);

                $template = new template($config->loginTemplate);

                $template->set('title', $config->defaultTitle);

                $template->set('config', $config);

                $template->set('invalid', $invalid);

                $template->display();

                exit;

        }

        $debug->debug("Checking cookieauth for %s",$form, 8);

	if (! $this->user->CookieAuth($form)) {

            $debug->debug("FAILED cookieauth for %s",$form, 8);

            $this->error = BADLOGIN;

            $template = new template($config->loginTemplate);

            $template->set('title', $config->defaultTitle);

            $template->set('config', $config);

            $template->set('user', $this->user);

            $template->set('form', $form);

            $template->set('badlogin', TRUE);

            $template->display();

            exit;



	} else {

	  $debug->line("Passed cookieauth from form", 8);

	  $this->error = FALSE;

          $this->user->_loaduser();

          $this->trackUser();

	  $this->user->loggedIn = TRUE;

	  $this->loggedIn = TRUE;

	  // this is where we check for our re-direct to apply
	  if($this->user->NewBenefitLOB2 > 0) {
	  	header("Location: apply.php5");
	  	exit;
	  }

	}

    }

    parent::__construct();

  }





/**

  * Track the user's whereabouts

  *

  * @param string $extraData [optional] Extra data to store

  * @param string $module [optional] Module the user has hit (optionally automatically parsed from the url)

  * @return void

  */

     public function trackUser( $extraData = NULL, $module = NULL ) {

         global $config, $debug;

         if ( !$module ) {

             list($uri, $cruft) = explode('?', $_SERVER['REQUEST_URI']);

             $fileInfo = explode('/', $uri);

             list($module,$cruft) = explode('.',$fileInfo[count($fileInfo)-1]);

             $module = ($module == '')?'index':$module;

         }

         $sql = "insert into tracking set userID = '".$this->user->rowID."', enrollmentID = '".$config->enrollmentID."',

                 time = '".time()."', module = '".$module."', extraData = '".$extraData."'";

         $this->user->db->query($sql, 'trackUser');

     }

  

}







?>
