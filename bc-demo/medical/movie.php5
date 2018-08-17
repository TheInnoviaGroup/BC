<?php

include_once( 'benefitCoach.inc' );
$today = time();
if($today < $user->dateOpen) {
	// it's before the enrollment
	$isOpen = 0;
} elseif($today > $user->dateClosed) {
	// it's after the enrollment
	$isOpen = -1;
} else {
	// it's during the enrollment
	$isOpen = 1;
}

?>
<html xmlns="https://www.w3.org/1999/xhtml" xml:lang="en" lang="en">
<head>
	<meta http-equiv="Content-Type" content="text/html; charset=iso-8859-1" />
	<title>Your Personal Benefit Coach</title>
	<style>
		body {
			background-color:#ffffff;
			font-family:arial;
			font-size:9pt;
		}
		
		td {
			font-family:arial;
			font-size:8pt;
		}
		
		input {
			font-family:arial;
			font-size:8pt;
		}
		
		#flashdiv {
			width:586px;
			text-align:center;
			border:0px;
			margin:0px;
			padding:0px
		}
		
		#formdiv {
			float:right;
			width:250px
			border:0px;
			margin:12px;
			background-color:yellow;
		}
		
		.vartype {
			padding-left:6px;
			font-style:italic;
			color:#purple;
		}
	</style>
	<script language="javascript">AC_FL_RunContent = 0;</script>
	<script src="AC_RunActiveContent.js" language="javascript"></script>
</head>
<body>
<!-- FLASH DIV -->
<div id="flashdiv">
			<script language="javascript">
			if (AC_FL_RunContent == 0) {
				alert("This page requires AC_RunActiveContent.js. In Flash, run \"Apply Active Content Update\" in the Commands menu to copy AC_RunActiveContent.js to the HTML output folder.");
			} else {
				AC_FL_RunContent(
					'codebase', 'https://download.macromedia.com/pub/shockwave/cabs/flash/swflash.cab#version=8,0,0,0',
					'width', '586',
					'height', '469',
					'src', 'in001_player?er_enrollment_date=<?=date("F j, Y", $user->dateClosed);?>&er_enrollment_open=<?=$isOpen;?>&cl_compensation_base=<?=round($user->SalaryMonthly);?>&cl_compensation_bonus=<?=round($user-> BonusMonthly);?>&cl_compensation_commission=<?=round($user->Commission);?>&cl_compensation_other=<?=round($user->Other);?>&cl_compensation_total=<?=round($user->TotalMonthly);?>&er_old_compensation_covered_base = <?=$user->oldCompBase;?>&er_old_compensation_covered_bonus=<?=$user->oldCompBonus;?>&er_old_compensation_covered_commission=<?=$user->oldCompCommission;?>&er_old_compensation_covered_other=<?=$user->oldCompOther;?>&er_old_compensation_covered_selected=<?=$user->oldCompSelected;?>&er_old_disability_benefit=<?=round($user->oldBenefitPercent);?>&er_old_benefit_cap=<?=round($user->oldBenefitCap);?>&er_old_benefit_omit_tax=<?=$user->taxesVoiceFlag;?>&cl_old_compensation_covered_total=<?=round($user->oldCompensationCovered);?>&cl_old_compensation_covered_at_risk=<?=round($user->oldAtRisk);?>&cl_old_benefit=<?=round($user->ExistingBenefit);?>&cl_old_lost=<?=round($user->oldLost);?>&cl_old_benefit_percent=<?=round($user->oldPersonalBenefitPercent);?>&cl_old_lost_percent=<?=round($user->oldLostPercent);?>&er_benefit_percentage=<?=round($user->newBenefitPercent);?>&er_benefit_cap=<?=round($user->newBenefitCap);?>&er_benefit_total_cap=<?=round($user->newTotalBenefitCap);?>&er_other_cap=<?=$user->otherCapVoiceFlag;?>&er_compensation_covered_base=<?=$user->newCompBase;?>&er_compensation_covered_bonus=<?=$user->newCompBonus;?>&er_compensation_covered_commission=<?=$user->newCompCommission;?>&er_compensation_covered_other=<?=$user->newCompOther;?>&er_compensation_covered_selected=<?=$user->newCompSelected;?>&cl_compensation_covered_additional_total=<?=round($user->coveredAdditionalTotal);?>&cl_additional_benefit=<?=round($user->NewBenefit);?>&cl_total_benefit=<?=round($user->newTotalBenefit);?>&cl_total_percentage=<?=round($user->newTotalPercentage);?>&cl_notobacco_premium=<?=round($user->NonTobaccoPremium, 2);?>&cl_tobacco_premium=<?=round($user->TobaccoPremium, 2);?>',
					'quality', 'high',
					'pluginspage', 'https://www.macromedia.com/go/getflashplayer',
					'align', 'middle',
					'play', 'true',
					'loop', 'false',
					'scale', 'showall',
					'wmode', 'window',
					'devicefont', 'false',
					'id', 'in001_player',
					'bgcolor', '#ffffff',
					'name', 'in001_player',
					'menu', 'false',
					'allowScriptAccess','sameDomain',
					'movie', 'in001_player?er_enrollment_date=<?=date("F j, Y", $user->dateClosed);?>&er_enrollment_open=<?=$isOpen;?>&cl_compensation_base=<?=round($user->SalaryMonthly);?>&cl_compensation_bonus=<?=round($user-> BonusMonthly);?>&cl_compensation_commission=<?=round($user->Commission);?>&cl_compensation_other=<?=round($user->Other);?>&cl_compensation_total=<?=round($user->TotalMonthly);?>&er_old_compensation_covered_base = <?=$user->oldCompBase;?>&er_old_compensation_covered_bonus=<?=$user->oldCompBonus;?>&er_old_compensation_covered_commission=<?=$user->oldCompCommission;?>&er_old_compensation_covered_other=<?=$user->oldCompOther;?>&er_old_compensation_covered_selected=<?=$user->oldCompSelected;?>&er_old_disability_benefit=<?=round($user->oldBenefitPercent);?>&er_old_benefit_cap=<?=round($user->oldBenefitCap);?>&er_old_benefit_omit_tax=<?=$user->taxesVoiceFlag;?>&cl_old_compensation_covered_total=<?=round($user->oldCompensationCovered);?>&cl_old_compensation_covered_at_risk=<?=round($user->oldAtRisk);?>&cl_old_benefit=<?=round($user->ExistingBenefit);?>&cl_old_lost=<?=round($user->oldLost);?>&cl_old_benefit_percent=<?=round($user->oldPersonalBenefitPercent);?>&cl_old_lost_percent=<?=round($user->oldLostPercent);?>&er_benefit_percentage=<?=round($user->newBenefitPercent);?>&er_benefit_cap=<?=round($user->newBenefitCap);?>&er_benefit_total_cap=<?=round($user->newTotalBenefitCap);?>&er_other_cap=<?=$user->otherCapVoiceFlag;?>&er_compensation_covered_base=<?=$user->newCompBase;?>&er_compensation_covered_bonus=<?=$user->newCompBonus;?>&er_compensation_covered_commission=<?=$user->newCompCommission;?>&er_compensation_covered_other=<?=$user->newCompOther;?>&er_compensation_covered_selected=<?=$user->newCompSelected;?>&cl_compensation_covered_additional_total=<?=round($user->coveredAdditionalTotal);?>&cl_additional_benefit=<?=round($user->NewBenefit);?>&cl_total_benefit=<?=round($user->newTotalBenefit);?>&cl_total_percentage=<?=round($user->newTotalPercentage);?>&cl_notobacco_premium=<?=round($user->NonTobaccoPremium, 2);?>&cl_tobacco_premium=<?=round($user->TobaccoPremium, 2);?>',
					'salign', ''
					); //end AC code
			}
		</script>
		<noscript>
			<object classid="clsid:d27cdb6e-ae6d-11cf-96b8-444553540000" codebase="https://download.macromedia.com/pub/shockwave/cabs/flash/swflash.cab#version=8,0,0,0" width="586" height="469" id="in001_player" align="middle">
			<param name="allowScriptAccess" value="sameDomain" />
			<param name="movie" value="in001_player.swf" /><param name="loop" value="false" /><param name="menu" value="false" /><param name="quality" value="high" /><param name="bgcolor" value="#ffffff" />	<embed src="in001_player.swf" loop="false" menu="false" quality="high" bgcolor="#ffffff" width="586" height="469" name="in001_player" align="middle" allowScriptAccess="sameDomain" type="application/x-shockwave-flash" pluginspage="https://www.macromedia.com/go/getflashplayer" />
			</object>
		</noscript>
</div>

</body>
</html>
