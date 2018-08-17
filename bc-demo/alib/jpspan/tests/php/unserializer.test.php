<?php
/**
* @version $Id: unserializer.test.php,v 1.1 2005/06/28 07:26:10 bryan Exp $
* @package JPSpan
* @subpackage Tests
*/

/**
* Includes
*/
require_once('../config.php');
require_once JPSPAN . 'Unserializer.php';

/**
* @package JPSpan
* @subpackage Tests
*/
class TestOfJPSpan_Unserializer extends UnitTestCase {

    function TestOfJPSpan_Unserializer() {
        $this->UnitTestCase('TestOfJPSpan_Unserializer');
    }
    
    function testUnserializeXML() {
        $var = 'foo';
        $s = '<?xml version="1.0" encoding="UTF-8"?><r><s>foo</s></r>';
        $this->assertEqual(JPSpan_Unserializer::unserialize($s,'xml'),$var);
    }
    
    function testUnserializePHP() {
        $var = 'foo';
        $s = serialize($var);
        $this->assertEqual(JPSpan_Unserializer::unserialize($s,'php'),$var);
    }
    

}

/**
* Conditional test runner
*/
if (!defined('TEST_RUNNING')) {
    define('TEST_RUNNING', true);
    $test = &new TestOfJPSpan_Unserializer();
    $test->run(new HtmlReporter());
}
?>
