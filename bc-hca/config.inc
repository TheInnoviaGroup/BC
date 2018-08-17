<?php

/* DB Setups */
$userDB = "mysql://paituser:jhj7l45g@127.0.0.1/benefitCoach";
$mainDB = "mysql://paituser:jhj7l45g@127.0.0.1/benefitCoach";


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
$self = '/';

/* Company ID */
$companyID = 6;

/* Enrollment ID */
$enrollmentID = 3;

/* Enrollement Data Table */
$enrollmentData = 'enrollmentdata';

/* Enrollement Defs Table */
$enrollmentDefs = 'enrollmentdefs';

/* Timezone Adjustment */
$timeZone = 2;

/* File to connect to Unum  */
$wsdlFile = 'exchangeconnect.wsdl';


?>
