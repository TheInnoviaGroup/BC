<?php

$debugLevel = 0;

$configFile = 'config.inc';
// include_once($configFile);

include_once("../alib/alib.inc");

addIncludePath('../alib');

$login = new login();

/*

   Here we turn off error reporting if debugging is 0 (we assume we're

   in production and don't want people seeing how we made the sausage.

   If debug level is > 8 we turn error reporting all the way on as well.

*/



if ( $debugLevel == 0 ) {

    error_reporting(0);    

} elseif ( $debugLevel > 8 ) {

    error_reporting(E_ALL);

}



if (! $login->loggedIn ) {

    $debug->debug('No user logged in: %s', $login, 3);
    
    $debug->debug('No user logged in: %s', $login, 3);

   	global $config, $enrollmentID, $companyID;
   	$db = new idb($config->mainDB);
   
    $sql = "SELECT dateOpen, dateClosed FROM enrollmentdefs WHERE enrollmentID = ".$config->enrollmentID." AND companyID = ".$config->companyID;
    
    $dateRes = $db->query($sql);
    $dateArray = $dateRes->fetch_assoc();
    $template = new template($config->loginTemplate);
		$template->set('dateOpen', $dateArray['dateOpen']);
		$template->set('dateClose', $dateArray['dateClosed']);
    $template->set('title', $config->defaultTitle);

    $template->set('config', $config);

    $template->display();

    exit;

} else {

    global $user;

    $user = $login->user;

}

?>

