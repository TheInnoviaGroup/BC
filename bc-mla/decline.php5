<?php

include_once( 'benefitCoach.inc' );

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
      <div class="content">
      <h2>Decline Coverage:</h2>
      <form action="decline2.php5" name="decline" method="POST"><b>From:
<?=$user->gn;?> <?=$user->sn;?></b><br>

        <select name="reasonDeclined">
        <option>--Reason for declining--</option>
        <option>I already have additional coverage</option>
        <option>Cost</option>
        <option>I don't think I need it</option>
        </select>

        <br>
				<br>
        <label for="desc">Please enter any additional comments:</label><br>

        <textarea name="desc" rows="10" cols="50"></textarea><br><input name="Submit" value="Submit" style="width: 50%;" type="submit">
        <div class="moreInfo">FOR MORE INFORMATION:
			
			<p>If you have questions and would like to speak to a Disability Specialist, please contact our</p>
			
			<div style="display:block; text-align:center; color: #00a8d4;"><b>Disability Hotline</b> @ <b><span class="phoneNumber"><?=$user->QUESTION_PHONE;?></span></div</p>
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
