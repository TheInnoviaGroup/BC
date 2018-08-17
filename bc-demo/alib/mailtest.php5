#!/usr/bin/php5/php
<?PHP

include_once('alib.inc');

$to = ($_GET['to'])?$_GET['to']:'bpjohnson@gmail.com';

$mail = new mail($to, $to, 'Test Message No Attachment');

$mail->body = "This is a test message.";
print "Message 1: <pre>\n".$mail->message."\n</pre>";;
$status = $mail->sendMessage();
print "Status1 : $status<br />";
print 

$mailA = new mail('bpjohnson@gmail.com', 'bpjohnson@gmail.com', 'Test Message With Attachment');

$mailA->body = "This is also a test message";
$mailA->X_Test_Header = "Test";
$mailA->addAttachment('saturn.jpg');
print "Message 2: <pre>\n".$mailA->message."\n</pre>";;
$status = $mailA->sendMessage();
print "Status2 : $status<br />";
?>