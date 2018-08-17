<?
// get values for the user
// ExistingBenefit
// OldBenefitPercent
// NewTotalBenefit
// NewTotalPercentage
// Tobacco Premium
// non-Tobacco Premium

// format as dollars

// money_format('%', $number);
global $user;
?>

<div id="welcome">
Welcome<br>
<?=ucfirst(strtolower($user->gn));?> <?=ucfirst(strtolower($user->sn));?>
</div>
<div class="numberLabel">
Current Monthly Disability Benefit*
</div>
<div class="numberAmount">
$<?=number_format(round($user->ExistingBenefit + $user->NewBenefitLOB2));?>
</div>

<div class="numberLabel">
% Monthly Base Salary Currently Covered
</div>
<div class="numberAmount">
<?=number_format(round($user->oldCoveredPercent+$user->oldERSIPPercent));?>%
</div>
<div class="numberLabel">
Eligible for Additional Monthly Benefit
</div>
<div class="numberAmount">
$<?=number_format(round($user->NewBenefitLOB1));?>
</div>
<div class="numberLabel">
% Monthly Base Salary Potentially Covered
</div>
<div class="numberAmount">
<?=number_format(round($user->newTotalPercentage));?>%
</div>
<div class="numberLabel">
Payroll Premium
</div>
<div class="numberAmount">
$<?=number_format($user->NonTobaccoPremium, 2);?> - <?=number_format($user->TobaccoPremium, 2);?>
</div>
<br />
<dive class="fine_print">*This assumes your participation in the  Group LTD plan</div>
