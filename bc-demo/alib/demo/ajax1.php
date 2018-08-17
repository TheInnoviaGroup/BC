<?PHP
include_once("../alib.inc");
class testObject {
  function testMethod ($arg) {
    return "You're inside PHP::testObject::testMethod and you gave the argument: ".$arg;
  }
}

class testObjectTwo {
  function testMethodOne ($arg) {
    return "You're inside PHP::testObjectTwo::testMethodOne and you gave the argument: ".$arg;
  }
  function testMethodTwo ($arg) {
    return "You're inside PHP::testObjectTwo::testMethodTwo and you gave the argument: ".$arg;
  }
  function testMethodThree ($arg) {
    return "You're inside PHP::testObjectTwo::testMethodThree and you gave the argument: ".$arg;
  }
}

function testFunction ($arg) {
   return "You're inside PHP::testFunction and you gave the argument: ".$arg;
}

$ajax = new Ajax();
$ajax->addObject("testObject");
$allowedMethods = array("testMethodOne", "testMethodThree");
$ajax->addObject("testObjectTwo", $allowedMethods);
$ajax->addFunction("testFunction");
//dynamicAjaxFunctionObject();


$ajax->start();

?>