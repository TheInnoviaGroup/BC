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
		<td width="204"><img src="images/HCALogo2007.gif" width="172" height="49" hspace="0" vspace="0" align="left"></td>
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
      <h2>Coverage Declined</h2>

<?
		$form = new iform(2);
		$form->addPost();
		$reasonDeclined = $form->filter->reasonDeclined(FILTER_SANITIZE_STRING, FILTER_FLAG_ENCODE_AMP);
		$desc = $form->filter->desc(FILTER_SANITIZE_STRING, FILTER_FLAG_ENCODE_AMP);
		
		$messageString = "<html><body>".$user->gn." ".$user->sn." has sent the following decline message:\n<br><b>Reason Declined: ".$reasonDeclined."</b><br><br>".$desc."<br><br></body><html>";
		
		$address = "enrollment@innoviagroup.com";
		
// prep the mail
		$mail = new mail;
		$mail->from = "systems@benefitcoach.net";
		$mail->Reply_To = "systems@benefitcoach.net";
		$mail->to = $address;
		$mail->subject = "Benefit Coach Message From ".$user->gn." ".$user->sn;
		$mail->body = $messageString;
		$mail->Content_Type = "text/html";
		$mail->Content_Transfer_Encoding = "quoted-printable";
		$mail->MIME_Version = "1.0";	
		
		//send
		$mail->sendMessage();
		
         
?>
      <div class="content">
      <p>Your message has been sent.</p>
      
			<p>If you have questions and would like to speak to a Disability Specialist, please contact our</p>
			
			<p><b>Disability Hotline</b> @ <b>
			<? if($user->NewBenefitLOB2 > 0): ?>
			<span class="phoneNumber">888-640-8470</span>
			<? else: ?>
			<span class="phoneNumber">800-577-5457</span>
		<? endif; ?>
			</p>

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
