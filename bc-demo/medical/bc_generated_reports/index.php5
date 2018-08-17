<!DOCTYPE html PUBLIC "-//W3C//DTD XHTML 1.0 Strict//EN"
        "http://www.w3.org/TR/xhtml1/DTD/xhtml1-strict.dtd">
<html xmlns="http://www.w3.org/1999/xhtml">
<head>
	<title>File List</title>
</head>
<body>
<?
$dir = ".";
$dh  = opendir($dir);
$files = array();
$directories = array();
while (false !== ($filename = readdir($dh))) {
if (is_dir($filename)) {
if ($filename != '.' && $filename != '..') {
$directories[] = $filename;
}
} else {
if ($filename != __FILE__ && $filename != 'index.php5') {
   $files[] = $filename;
}
}
}?><br>

<h2>Files</h2>
<? foreach ($files as $dir): ?>
<a href="<?=$dir;?>"><?=$dir;?></a><br />
<? endforeach;?>
</body>
</html>
