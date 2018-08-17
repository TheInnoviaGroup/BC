<?PHP

// Call this once with no args in the url. Then call with
// formtest.php?var1=foo&var2=bar&var3=baz for some wacky crazy fun.

include_once('../alib.inc');
global $debug, $AGLOBAL;
$debug->level = 11; // 0 to turn off, higher for more output. alib
		   // objects do a lot of chatter at level 11 and higher.
$debug->output = 'pretty';
$debug->line('aLib init complete', 1);

$allowed = array('var1', 'var3');
$verboten = array('var2');

$form = new formHandler(MODERATEFORM, $allowed, $verboten);

print "Var1: " . $form->var1 . "<br>";
print "Var2: " . $form->var2 . "<br>";
print "Var3: " . $form->var3 . "<br>";

print_v($form, 1);

?>