<?php



$ini = ini_set("soap.wsdl_cache_enabled", 0);



global $user, $config;





try {

    $exchangeConnection = new SoapClient($config->wsdlFile, array( 'trace' => TRUE )

                                         );

} catch (Exception $e){

    $debug->level = -1;

    print "Exception caught:<br />\n";

    pre($e);

    exit;

}

try {

$class = 1;
if($user->EE_PAY_FREQUENCY == "Monthly") {
	$class = 2;
}

$dashedSSN = join('-', array(substr($user->ssn,0,3), substr($user->ssn,3,2), substr($user->ssn,5,4)));

    $LogonEmployee = new SoapVar(array('IDKey' => 'SSN'),

                             SOAP_ENC_OBJECT,

                            'LogonEmployee',

                            'http://www.plane.biz/exchange/20060125/logoninfoemployee');

    $Employee = new SoapVar(array('SSN' => $dashedSSN,

                                  'Class' => $class,

                                  'Location' => 0   ),

                            SOAP_ENC_OBJECT,

                            'Employee',     

                            'http://www.plane.biz/exchange/20060125/employeeinfo');

    $LogonEnroller = new SoapParam(array( 'nil' => TRUE ),

                                 'LogonEnroller'

                                 );

    $Dependent = new SoapVar(array( 'nil' => TRUE ),

                                 SOAP_ENC_OBJECT,

                                 'Dependents',

                                 'http://www.plane.biz/exchange/20060125/dependentinfo');

    $HostInfo = new SoapVar(array('ReturnURL' => 'https://mla.benefitcoach.net/bc_return.php5',
                                  'ExceptionURL' => 'https://mla.benefitcoach.net/',
                                  'TimeoutURL' => 'https://mla.benefitcoach.net/'),
                             SOAP_ENC_OBJECT,
                            'HostURL',
                            'http://www.plane.biz/exchange/20060125/hostinfo');

                                 

    



    $exchangeConnection->HeaderInfoValue->EmployerID = 'cf7df883-636c-47cc-a29f-d310a802933b'; # Our GUID.

    $retval = $exchangeConnection->__soapCall('GetExchangeStatus', array(

                                                               $Employee, array(),array(),

                                                               $LogonEmployee, $HostInfo
                                                               ),

                                    array(),

                                    new SoapHeader('http://www.plane.biz/exchange/20060125/headerinfo',

                                            'HeaderInfo',

                                            new SoapVar($exchangeConnection->HeaderInfoValue, SOAP_ENC_OBJECT, "HeaderInfo", "http://www.plane.biz/exchange/20060125/headerinfo")

                                            )

                             );

                                                 
    } catch (Exception $e){

    $debug->level = -1;

    print "Exception caught:<br />\n";

    print "Last Request: <br />\n";

    pre(highlight_string(preg_replace('/\>\s*\</', ">\n<", $exchangeConnection->__getLastRequest()), TRUE));

    print "Last Response: <br />\n";

    pre(highlight_string(preg_replace('/\>\s*\</', ">\n<", $exchangeConnection->__getLastResponse()), TRUE));



    pre($e);

    exit;

}

?>

<form action="<?=$retval->ExchangeURL;?>" method="POST" id="form1">

<input type="hidden" name="ExchangeSessionID" length=50 value="<?=$retval->SessionID;?>" />

</form>

<script type="text/javascript">  



function doForm( formID ) {

    var form = document.getElementById(formID);

    form.submit();

    return false;

}

</script>
