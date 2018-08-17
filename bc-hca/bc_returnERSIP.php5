<?php
global $redirect; 
$redirect = 'Not';
include_once( 'benefitCoach.inc' );
global $user;
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

</head>
<body>
<table border="0" width="100%" cellspacing="0" cellpadding="0" align="center">
	<tr>
		<td width="204"><img src="images/HCALogo2007.gif" width="172" height="49" hspace="0" vspace="0" align="left"></td>
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
		<table border="0" width="80%" cellspacing="0" cellpadding="0" align="center" class="mainStageTable">
	<tr>
		<td width="18"><img src="images/corner_top_left.jpg" width="18" height="18" hspace="0" vspace="0"></td>
		<td class="sideTop"></td>
		<td width="18"><img src="images/corner_top_right.jpg" width="18" height="18" hspace="0" vspace="0"></td>
	</tr>
	<tr>
		<td class="sideLeft"></td>
		<td><center>
      <h3>What to expect next...</h3>
<p>
Your application is currently being reviewed.
If we have any questions about your application, we will contact you.
</p>
 
<p>
<b>Once your application is approved:</b><br>
A policy will be mailed to your home in the months of August and September. 
If your application is modified or declined in any way:
Don Hurwitz, a plan consultant, will call you directly.
</p>


<p>
<b>If your application is not approved:</b><br>
A letter will be sent your home explaining why it was not approved and your rights to appeal that decision.
</p>

<p>
If you have any questions, please contact a plan consultant at either 800-289-7724 (Don Hurwitz) or 888-640-8470 (Dan Wisted).
</p>
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
</body></html>
