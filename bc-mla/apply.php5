<?php

include_once( 'benefitCoach.inc' );
include_once( 'unumSOAP.inc' );

?>
<html>
<head><title></title>
<LINK REL=STYLESHEET TYPE='text/css' HREF='style.css'>
</head>
<body>

		<?
		include_once("navigation.php5");
		?>
<table border="0" width="100%" cellspacing="0" cellpadding="0" align="center" class="mainTable">
	<tr>
		<td width="200" class="theNumbers" valign="top">
		<?
		include_once("numbersColumn.php5");
		?>
		</td>
		<td class="mainStage" valign="top"><div style="display:block; text-align:center; color: #00a8d4;"><b>Disability Hotline</b> @ <b><span class="phoneNumber"><?=$user->QUESTION_PHONE;?></span></div>
		<table border="0" width="100%" cellspacing="0" cellpadding="0" align="center" class="mainStageTable">
	<tr>
		<td><img src="images/corner_top_left.jpg" width="18" height="18" hspace="0" vspace="0"></td>
		<td class="sideTop"></td>
		<td><img src="images/corner_top_right.jpg" width="18" height="18" hspace="0" vspace="0"></td>
	</tr>
	<tr>
		<td class="sideLeft"></td>
		<td>      
      <table border="0" cellspacing="0" cellpadding="2" align="center" bgcolor="#FFFFFF">
	<tr>
		<td align="center" class="apply" valign="top"><a href="javascript:doForm('form1');"><img src="images/button_apply.jpg" alt="" width="200" height="60" hspace="5" border="0"></a><p>
When you click the <b>Apply Now</b> button, you will be transferred to the UNUM website to begin the enrollment process.  The online enrollment will allow you to quickly apply for coverage up to the "Guaranteed Issue" amount of $6,000 per month.  If you have additional monthly benefit available, you will be contacted within two business days to complete the enrollment process.
</p>

</td>
	</tr>
</table>

      </td>
		<td class="sideRight"></td>
	</tr>
	<tr>
		<td><img src="images/corner_bottom_left.jpg" width="18" height="18" hspace="0" vspace="0"></td>
		<td class="sideBottom"></td>
		<td><img src="images/corner_bottom_right.jpg" width="18" height="18" hspace="0" vspace="0"></td>
	</tr>
</table>
		</td>
	</tr>
</table>
		
</body>
</html>