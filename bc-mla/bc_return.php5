<?php

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

p {
margin-bottom: 5px;
margin-top: 20px;
}
ul {
margin-top: 0px;
}

</style>

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
		<table border="0" width="80%" cellspacing="0" cellpadding="0" align="center" class="mainStageTable">
	<tr>
		<td width="18"><img src="images/corner_top_left.jpg" width="18" height="18" hspace="0" vspace="0"></td>
		<td class="sideTop"></td>
		<td width="18"><img src="images/corner_top_right.jpg" width="18" height="18" hspace="0" vspace="0"></td>
	</tr>
	<tr>
		<td class="sideLeft"></td>
		<td align="left">
		<center>
      <h3>What to expect next...</h3>

<p>
Your application is currently being reviewed.  If we have any questions about your application, we will contact you. 
</p>

<p>
If you have additional coverage available over the Guaranteed Issue amount:
<ul type="disc">
	<li>You will be contacted in the next 48 hours</li>
</ul>

</p>

<p>
Once your application is approved:
<ul type="disc">
	<li>Your policy will be mailed to you in the next 6-8 weeks</li>
</ul>

</p>


<p>
If your application is not approved:
<ul type="disc">
	<li>A letter will be sent your home explaining why it was not approved and your rights to appeal that 
decision.</li>
</ul>

</p>


<p>
If you have declined the opportunity to participate in the SIP Plan:
<ul type="disc">
	<li>No further action is necessary. If you later decide to participate, you must provide whatever evidence of 
insurability is required at that time.</li>
</ul>

</p>

<p>
If you still have questions, please contact us at:
<div style="display:block; text-align:center; color: #00a8d4;"><b>Disability Hotline</b> @ <b><span 
class="phoneNumber"><?=$user->QUESTION_PHONE;?></span></div>
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
