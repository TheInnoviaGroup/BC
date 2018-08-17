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
Current Monthly Disability Benefit
</div>
<div class="numberAmount">
$<?=number_format(round($user->ExistingBenefit));?>
</div>

<div class="numberLabel">
% Monthly <?=$user->CompDesc;?> Currently Covered
</div>
<div class="numberAmount">
<?=number_format(round($user->oldCoveredPercent));?>%
</div>
<div class="numberLabel">
Eligible for Additional Monthly Benefit
</div>
<div class="numberAmount">
$<?=number_format(round($user->NewBenefit));?>
</div>
<div class="numberLabel">
% Monthly <?=$user->CompDesc;?> Potentially Covered
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