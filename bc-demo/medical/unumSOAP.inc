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

    $LogonEmployee = new SoapVar(array('IDKey' => 'SSN'),
                             SOAP_ENC_OBJECT,
                            'LogonEmployee',
                            'http://www.plane.biz/exchange/20060125/logoninfoemployee');
    $Employee = new SoapVar(array('SSN' => $user->ssn,
                                  'Class' => (stristr($user->payrollCycle,'week'))?1:2,
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
    $HostInfo = new SoapVar(array('ReturnURL' => 'https://hca.benefitcoach.net/bc_return.php5',
                                  'ExceptionURL' => 'https://hca.benefitcoach.net/',
                                  'TimeoutURL' => 'https://hca.benefitcoach.net/'),
                             SOAP_ENC_OBJECT,
                            'HostURL',
                            'http://www.plane.biz/exchange/20060125/hostinfo');
                                 
    

#    $exchangeConnection->HeaderInfoValue->EmployerID = '114d6821-240c-4bc2-bfb7-33fc550f3c4d'; # Our GUID.
    $exchangeConnection->HeaderInfoValue->EmployerID = '4df69ef5-9fc6-4f59-ae4c-c465ca8ce507'; # Our GUID.
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
