<?php
include_once("../html/components/general.inc");
$ini = ini_set("soap.wsdl_cache_enabled", 0);

global $user, $config;

class CrappyUnumSoapClient extends SoapClient {
public $myLastReq;

  public function __construct($wsdl, $options = array()) {
     return parent::__construct($wsdl, $options);
  }

 

  public function __doRequest($request, $location, $action, $version) {
  global $user, $config;
 //   $newReq = preg_replace('/ xsi:type=\"[A-Za-z0-9:\-\.\_]+\"/', '', $request);
//	$newReq = preg_replace('/<ns[0-9]*:/', '<', $newReq);
//	$newReq = preg_replace('/<\/ns[0-9]*:/', '</', $newReq);

	$reqTmp = new template('soapTemplate.xml');
	$reqTmp->set('unumKey', 'e6648cf8-0ecb-479b-8575-c462ef7f08d6');
	$reqTmp->set('unumClass', '1');
	$reqTmp->set('unumLocation', '0');
	$reqTmp->set('bcReturnURL', 'https://hca.benefitcoach.net/bc_returnERSIPVOLSIP.php5');
	$reqTmp->set('ssn', formatSocial($user->ssn));
	$reqTmp->set('user', $user);
	$newReq = $reqTmp->parse();
	
	$this->myLastReq = $newReq;
    return parent::__doRequest($newReq, $location, $action, $version);
  } 
} 

try {
    $exchangeConnection = new CrappyUnumSoapClient($config->wsdlFile, array( 'trace' => TRUE ));
  //  $exchangeConnection = new SoapClient($config->wsdlFile, array( 'trace' => TRUE ));
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
    $Employee = new SoapVar(array('SSN' => formatSocial($user->ssn),
                                  'Class' => 1,
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
    $HostInfo = new SoapVar(array('ReturnURL' => 'https://hca.benefitcoach.net/bc_returnERSIP.php5',
                                  'ExceptionURL' => 'https://hca.benefitcoach.net/',
                                  'TimeoutURL' => 'https://hca.benefitcoach.net/'),
                             SOAP_ENC_OBJECT,
                            'HostURL',
                            'http://www.plane.biz/exchange/20060125/hostinfo');
    $ExchangeStatusInput = new SoapParam(array('Employee' => $Employee, 'HostURL' => $HostInfo, 'LogonEmployee' => $LogonEmployee, ), 'ExchangeStatusInput');
    $exchangeConnection->HeaderInfoValue->EmployerID = 'e6648cf8-0ecb-479b-8575-c462ef7f08d6'; // Our GUID.
    $retval = $exchangeConnection->__soapCall('GetExchangeStatus', array(
//                                                               $Employee, array(),array(),
//                                                               $LogonEmployee, $HostInfo
$ExchangeStatusInput
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
    pre(highlight_string(preg_replace('/\>\s*\</', ">\n<", $exchangeConnection->myLastReq), TRUE));
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
