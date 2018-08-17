<?php

include_once( 'benefitCoach.inc' );
global $user;
?>
<html>
<head><title></title>
<LINK REL=STYLESHEET TYPE='text/css' HREF='style.css'>
</head>
<body>
<table border="0" width="100%" cellspacing="0" cellpadding="0" align="center">
	<tr>
		<td width="204"><img src="images/todd_logo.jpg" width="204" height="76" hspace="0" vspace="0" align="left"></td>
		<td>
		<?
		include_once("navigation.php5");
		?>
		</td>
	</tr>
</table>
<table border="0" width="100%" cellspacing="0" cellpadding="0" align="center" class="mainTable">
	<tr>
		<td width="200" class="theNumbers" valign="top">
		<?
		include_once("numbersColumn.php5");
		?>
		</td>
		<td class="mainStage" valign="top"><? if($user->NewBenefitLOB2 > 0): ?>
		<div style="display:block; text-align:center; color: #00a8d4;"><b>Disability Hotline</b> @ <b><span class="phoneNumber">888-640-8470</span>
		<? else: ?>
		<div style="display:block; text-align:center; color: #00a8d4;"><b>Disability Hotline</b> @ <b><span class="phoneNumber">800-577-5457</span>
		</div>
		<? endif; ?>
		<table border="0" width="100%" cellspacing="0" cellpadding="0" align="center" class="mainStageTable">
	<tr>
		<td><img src="images/corner_top_left.jpg" width="18" height="18" hspace="0" vspace="0"></td>
		<td class="sideTop"></td>
		<td><img src="images/corner_top_right.jpg" width="18" height="18" hspace="0" vspace="0"></td>
	</tr>
	<tr>
		<td class="sideLeft"></td>
		<td>      
      <h2>Contact Us</h2>

     
      <div class="content">
      <form action="contact2.php5" name="contactus" method="POST"><b>From:
<?=$user->gn;?> <?=$user->sn;?></b><br>

        <select name="contactMethod">
        <option>--Choose a contact method--</option>
        <option>Please Call Me</option>
        <option>Please E-mail Me</option>
        </select>

        <br>
				<br>
        <label for="desc">Please enter your message:</label><br>

        <textarea name="desc" rows="10" cols="50"></textarea><br><input name="Submit" value="Submit" style="width: 50%;" type="submit">
        <div class="moreInfo">FOR MORE INFORMATION:
			
			<p>If you have questions and would like to speak to a Disability Specialist, please contact our</p>
			
			<p><b>Disability Hotline</b> @ <b>
			<? if($user->NewBenefitLOB2 > 0): ?>
			<span class="phoneNumber">888-640-8470</span>
			<? else: ?>
			<span class="phoneNumber">800-577-5457</span>
		<? endif; ?>
			</p>
			</div>

      </form>

      </div>

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
