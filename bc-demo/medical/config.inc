<?php

/* DB Setups */
/* local  
$userDB = "mysql://ftguru_contactsu:t35lac01l@127.0.0.1/bcDemo";
$mainDB = "mysql://ftguru_contactsu:t35lac01l@127.0.0.1/bcDemo";
*/

/* live */
$userDB = "mysql://paituser:jhj7l45g@127.0.0.1/bcDemo";
$mainDB = "mysql://paituser:jhj7l45g@127.0.0.1/bcDemo";


/* Global Templates */
$loginTemplate = "login.php5";

/* The template we show when they log out or come back from the carrier. */
$logoutTemplate = "";

/* User Login Expiration Time */
$loginExpires = 60*60*2; #2 hours

/* Default long page title */
$defaultTitle = 'Benefit Coach';

/* Application name (AlphaNumeric Only) */
$appName = 'BenefitCoach';

/* What's our root directory? */
$self = '/medical/';

/* Company ID */
$companyID = 47;

/* Enrollment ID */
$enrollmentID = 1;

/* Enrollement Data Table */
$enrollmentData = 'medicalEnrollment';

/* Enrollement Defs Table */
$enrollmentDefs = 'enrollmentdefs';

/* Timezone Adjustment */
$timeZone = 2;

/* File to connect to Unum  */
$wsdlFile = 'exchangeconnect.wsdl';


?>
