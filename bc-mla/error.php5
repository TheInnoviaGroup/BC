<?php

include_once( 'benefitCoach.inc' );

?>
<html>
<head><title></title>
<style type="text/css">

body {
 margin: 0px;
 margin-left: 10px;
	font-size: small;
	font-family: arial, helvetica;
	color: #555;
}

.topNav {
background: url(images/row_bg.jpg);
height: 76px;
}

.mainStage {
	padding: 10px;
}

.theNumbers {
	padding: 10px;
	text-align:center;
}

.mainTable {
	border-left: 1px solid #555555;
	border-top: 1px solid #555555;
	border-bottom: 1px solid #555555;
	margin-left: 10px;
}

.mainStageTable {
	margin-right: auto;
	margin-left: auto;
}

.sideTop {
background: url(images/side_top.jpg);
background-repeat: repeat-x;
}

.sideBottom {
background: url(images/side_bottom.jpg);
background-repeat: repeat-x;
}

.sideLeft {
background: url(images/side_left.jpg);
background-repeat: repeat-y;
}

.sideRight {
background: url(images/side_right.jpg);
background-repeat: repeat-y;
}

a.buttonLink:link {
	font-size: small;
	font-family: arial, helvetica;
	color: #555555;
	text-decoration: none;
}

a.buttonLink:visited {
	font-size: small;
	font-family: arial, helvetica;
	color: #555555;
	text-decoration: none;
}

a.buttonLink:hover {
	color: #00a8d4;
}

a.buttonLink:active {
	color: #FFFFFF;
}

#welcome {
	font-size: medium;
	font-family: arial, helvetica;
	color: #00a8d4;
	padding-left: 20px; 
	padding-top: 10px; 
}
.numberLabel {
	font-size: small;
	font-family: arial, helvetica;
	color: #555555;
	padding-left: 20px; 
	padding-top: 10px; 
}
.numberAmount {
	font-size: small;
	font-family: arial, helvetica;
	color: #84cc32;
	padding-left: 20px;
}
label {
 font-size: .8em;
 }
</style>

<script language="JavaScript">
function validate() {
var myf = document.forms.fauxlogin;
if (myf.lastname.value=='Smith' && myf.firstname.value=='John' && myf.birthdate.value=='01-08-1960' && myf.enrollcode.value==myf.reenrollcode.value) {
myf.action="index.php5";
myf.Submit();
} else {

myf.action="error.php5";
myf.Submit();
}

}
  </script>
</head>
<body>
<table border="0" width="100%" cellspacing="0" cellpadding="0" align="center">
	<tr>
		<td width="204"><img src="images/mlaLogo.gif" width="172" height="49" hspace="0" vspace="0" align="left"></td>
		<td>
		<table border="0" width="100%" cellspacing="0" cellpadding="0" align="center" class="topNav">
	<tr>
		<td width="160">&nbsp;</td>
	</tr>
</table>
		</td>
	</tr>
	<tr>
		<td colspan="2" align="center">
		<div style="color:#660000">Invalid Login</div>
		<table border="0" width="80%" cellspacing="0" cellpadding="0" align="center" class="mainStageTable">
	<tr>
		<td width="18"><img src="images/corner_top_left.jpg" width="18" height="18" hspace="0" vspace="0"></td>
		<td class="sideTop"></td>
		<td width="18"><img src="images/corner_top_right.jpg" width="18" height="18" hspace="0" vspace="0"></td>
	</tr>
	<tr>
		<td class="sideLeft"></td>
		<td><center>
      <form action="" name="fauxlogin" onsubmit="validate();">
    	
        <table bgcolor="#ffffff" border="0" cellpadding="3" cellspacing="0">

	<tbody>
            <tr>

		<td align="right"><label for="lastname">Last
Name</label></td>

		<td><input style="width: 300px;" tabindex="1" name="lastname"></td>

	</tr>

	<tr>

		<td align="right"><label for="firstname">First Name</label> </td>

		<td><input style="width: 300px;" tabindex="2" name="firstname"></td>

	</tr>

	<tr>

		<td align="right"><label for="birthdate">Birth Date (mm-dd-yyyy)</label></td>

		<td><input style="width: 300px;" tabindex="3" name="birthdate"></td>

	</tr>

	<tr>

		<td align="right"> <label for="enrollcode">Enrollment Code (from your
secure e-mail)</label></td>

		<td><input tabindex="4" name="enrollcode" style="width: 300px;" type="password"></td>

	</tr>

	<tr>

		<td align="right"><label for="reenrollcode">Re-enter Enrollment Code</label></td>

		<td><input tabindex="5" name="reenrollcode" style="width: 300px;" type="password"></td>

	</tr>

	<tr>

		<td colspan="2">
		<input tabindex="6" name="submit" style="width: 100%;" value="Log In" onclick="validate();" type="submit">
		</td>

	</tr>

          </tbody>
        </table>
      </center></td>
		<td class="sideRight"></td>
	</tr>
	<tr>
		<td width="18"><img src="images/corner_bottom_left.jpg" width="18" height="18" hspace="0" vspace="0"></td>
		<td class="sideBottom"></td>
		<td width="18"><img src="images/corner_bottom_right.jpg" width="18" height="18" hspace="0" vspace="0"></td>
	</tr>
</table>
		</td>
	</tr>
</table>
		
</body>
