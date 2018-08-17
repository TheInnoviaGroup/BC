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
      <h2>Frequently
Asked Questions:</h2>

<? if($user->EE_PAY_FREQUENCY == "Semi-Monthly"): ?>

<div class="question">What is supplemental Individual Income Protection insurance?
</div>
<div class="answer">Income Protection insurance &ndash; also called disability insurance &ndash; replaces a portion of your income when you can't work because of an illness or injury that is covered by your policy. Supplemental Income Protection (SIP) insurance can help provide you with additional financial protection if you become disabled.</div>

<div class="question">I have group disability insurance... why do I need more coverage?</div>

<div class="answer"><p>
You have a basic level of disability insurance through a group plan, but many people find the benefit payments may not be enough to cover all their expenses while they are disabled, especially since the benefit payments you may receive would be taxable under current tax laws.
</p>

<p>
This is why you may want to consider applying for SIP insurance. If you become disabled, you may be eligible to receive benefits from both your group disability policy and your supplemental SIP policy, which can help protect more of your income.
</p></div>


<div class="question">How would Workers' Compensation or Social Security disability payments affect my need for additional income protection?</div>

<div class="answer">Workers' Compensation only covers injuries that happen on the job. In contrast, SIP insurance provides protection when you get sick or hurt on or off the job. Social Security disability coverage only pays for the most serious disabilities&ndash;those expected to last at least one year. Even if you were to qualify for benefit payments from either or both of these sources, the amount you received could reduce the benefits from your group plan. Your SIP benefit, however, would not be impacted by any payments from Workers' Compensation or Social Security disability.</div>

<div class="question">What makes Unum's SIP insurance plan such a valuable purchase through my workplace?</div>

<div class="answer"><p>
You could research and compare individual insurance products on your own, but your firm has already selected a great one for you! Our SIP insurance policy includes a wealth of important features available to you through your workplace. Here are just a few:
</p>


<ul>
<li>	You own and can keep the SIP policy if you ever leave your job.
	
</li>
<li>	This insurance coverage is available at a discount because you are buying as part of a group.
	
</li>
<li>	Premiums are paid through the convenience of payroll deduction.

</li>
<li>	Because you pay the premiums, any disability benefits that you receive are tax-free, under current law.

</li>
<li>	Simplified underwriting* allows you to obtain a SIP policy with no medical exam (only a few medical questions will be asked; benefits may be subject to a pre-existing condition limitation).

</li>
<li>	The base policy pays benefits if you are totally disabled in your own occupation, which means you are unable to work in your own occupation, not working in another occupation, and are under the care of a doctor. 

</li>
<li>	Benefits will begin to accrue after a waiting period of 180 Days.

</li>
<li>	Benefits can be payable up to age 65. This benefit period is the maximum time you could receive benefits if you are disabled.
</li>
</ul>


<p>
*	Simplified underwriting available to employees who for 180 days prior to and including the application date must: a) have been continuously at work on a full time basis performing all the duties of their occupation without limitation due to injuries or sickness, and b) have not been hospitalized or home bound due to injury or sickness and c) are under the age 65. You will be asked to meet additional requirements for Lifetime Continuation and Catastrophic Disability benefits.
</p>
</div>

<div class="question">What other unique features are included in the SIP plan?</div>

<div class="answer"><p>
This insurance plan is not just a basic income protection policy. Unum's SIP plan has been packaged to offer you a valuable, flexible solution to help you protect more of the income you've worked so hard to earn. Some of the additional features of this policy are listed below, but refer to the policy for complete details on available benefits.
</p>


<ul>
<li>Your policy includes the Catastrophic Disability Benefit, which provides an additional 25% income replacement up to $10,000 per month if you suffer a catastrophic disability, which could increase your living expenses. The Catastrophic Disability Benefit, combined with your base individual income protection coverage, can provide for up to 100% replacement of your prior earnings and includes coverage for cognitive impairment, ADL disabilities (defined as the loss of two or more Activities of Daily Living, including bathing, dressing, eating, toileting, continence and transferring), and presumptive disability (the total and permanent loss of hearing, sight, speech or use of two limbs).*

</li>
<li>The Lifetime Continuation Option allows you to exchange your income protection policy to an individual long term care policy when you are between the ages of 60 and 70, without having to provide evidence of insurability. Your base policy LTC benefit will be $3,000 per month (or $100 per day, if state required) with a benefit period of six years.

</li>
<li>Residual Benefits are paid monthly for less-than-total disability. The amount paid depends on how much income you are able to earn.
	
</li>
<li>Work Incentive Benefits are available when you initially return to work while disabled to help make up for the difference between your previous income and what you are currently earning. These short-term incentive benefits are equal to the difference between your prior income and your current earnings, for up to 100% income replacement, subject to your maximum benefit amount.
	
</li>
<li>Recovery Benefits are paid (proportionally) while you rebuild your earnings after a disability, if your loss of earnings continues after you are fully recovered and have returned to work in your own occupation full-time.
	
</li>
<li>We can help coordinate the rehabilitation services, when appropriate, for you while you are totally or residually disabled, including coordination of physical therapy, vocation testing, retraining, career counseling, placement services, worksite modifications and more that can help you return to work.
	
</li>
<li>Until you are age 65, your SIP policy can't be cancelled, provisions can't be changed and your cost can't be increased as long as premiums are paid on time.
</li>
</ul></div>


<div class="question">How much does the SIP insurance cost and how do I apply for coverage?
</div>

<div class="answer">
Now that you're more familiar with how this insurance policy works and the features and benefits included, let's take a look to see how it will affect your personal situation. On the enrollment web site, you'll find a personalized example illustrating your current group disability insurance coverage, how the SIP policy can provide you with a higher benefit amount, and the premium associated with this supplemental coverage. If you decide this coverage is right for you, click on "Apply Now" at the top of this page to complete your online application.</div>

<? else: ?>


<div class="question">What is supplemental Individual Income Protection insurance?</div>
<div class="answer">
Income Protection insurance &ndash; also called disability insurance &ndash; replaces a portion of your income when you can't work because of an illness or injury that is covered by your policy. Supplemental Income Protection (SIP) insurance can help provide you with additional financial protection if you become disabled.</div>

<div class="question">I have group disability insurance... why do I need more coverage?</div>

<div class="answer"><p>
You have a basic level of disability insurance through a group plan, but many people find the benefit payments may not be enough to cover all their expenses while they are disabled.
</p>

<p>
This is why you may want to consider applying for SIP insurance. If you become disabled, you may be eligible to receive benefits from both your group disability policy and your supplemental SIP policy, which can help protect more of your income.
</p>
</div>

<div class="question">How would Workers' Compensation or Social Security disability payments affect my need for additional income protection?</div>

<div class="answer">Workers' Compensation only covers injuries that happen on the job. In contrast, SIP insurance provides protection when you get sick or hurt on or off the job. Social Security disability coverage only pays for the most serious disabilities&ndash;those expected to last at least one year. Even if you were to qualify for benefit payments from either or both of these sources, the amount you received could reduce the benefits from your group plan. Your SIP benefit, however, would not be impacted by any payments from Workers' Compensation or Social Security disability.</div>

<div class="question">What makes Unum's SIP insurance plan such a valuable purchase through my workplace?</div>

<div class="answer">You could research and compare individual insurance products on your own, but your firm has already selected a great one for you! Our SIP insurance policy includes a wealth of important features available to you through your workplace. Here are just a few:

<ul>
<li>You own and can keep the SIP policy if you ever leave your job.
	
</li>
<li>This insurance coverage is available at a discount because you are buying as part of a group.
	
</li>
<li>Premiums are paid through the convenience of payroll deduction.

</li>
<li>Because you pay the premiums, any disability benefits that you receive are tax-free, under current law.

</li>
<li>Simplified underwriting* allows you to obtain a SIP policy with no medical exam (only a few medical questions will be asked; benefits may be subject to a pre-existing condition limitation).

</li>
<li>The base policy pays benefits if you are totally disabled in your own occupation, which means you are unable to work in your own occupation, not working in another occupation, and are under the care of a doctor. 

</li>
<li>Benefits will begin to accrue after a waiting period of 180 Days.

</li>
<li>Benefits can be payable up to age 65. This benefit period is the maximum time you could receive benefits if you are disabled.
</ul>

<p>
*	Simplified underwriting available to employees who for 180 days prior to and including the application date must: a) have been continuously at work on a full time basis performing all the duties of their occupation without limitation due to injuries or sickness, and b) have not been hospitalized or home bound due to injury or sickness and c) are under the age 65. You will be asked to meet additional requirements for Lifetime Continuation and Catastrophic Disability benefits.
</p>

</div>

<div class="question">What other unique features are included in the SIP plan?</div>

<div class="answer">This insurance plan is not just a basic income protection policy. Unum's SIP plan has been packaged to offer you a valuable, flexible solution to help you protect more of the income you've worked so hard to earn. Some of the additional features of this policy are listed below, but refer to the policy for complete details on available benefits.

<ul>
<li>Your policy includes the Catastrophic Disability Benefit, which provides an additional 25% income replacement up to $10,000 per month if you suffer a catastrophic disability, which could increase your living expenses. The Catastrophic Disability Benefit, combined with your base individual income protection coverage, can provide for up to 100% replacement of your prior earnings and includes coverage for cognitive impairment, ADL disabilities (defined as the loss of two or more Activities of Daily Living, including bathing, dressing, eating, toileting, continence and transferring), and presumptive disability (the total and permanent loss of hearing, sight, speech or use of two limbs).*

</li>
<li>The Lifetime Continuation Option allows you to exchange your income protection policy to an individual long term care policy° when you are between the ages of 60 and 70, without having to provide evidence of insurability. Your base policy LTC benefit will be $3,000 per month (or $100 per day, if state required) with a benefit period of six years.

</li>
<li>Residual Benefits are paid monthly for less-than-total disability. The amount paid depends on how much income you are able to earn.
	
</li>
<li>Work Incentive Benefits are available when you initially return to work while disabled to help make up for the difference between your previous income and what you are currently earning. These short-term incentive benefits are equal to the difference between your prior income and your current earnings, for up to 100% income replacement, subject to your maximum benefit amount.
	
</li>
<li>Recovery Benefits are paid (proportionally) while you rebuild your earnings after a disability, if your loss of earnings continues after you are fully recovered and have returned to work in your own occupation full-time.
	
</li>
<li>We can help coordinate the rehabilitation services, when appropriate, for you while you are totally or residually disabled, including coordination of physical therapy, vocation testing, retraining, career counseling, placement services, worksite modifications and more that can help you return to work.
	
</li>
<li>Until you are age 65, your SIP policy can't be cancelled, provisions can't be changed and your cost can't be increased as long as premiums are paid on time.
</ul>
</div>

<div class="question">How much does the SIP insurance cost and how do I apply for coverage?</div>

<div class="answer">Now that you're more familiar with how this insurance policy works and the features and benefits included, let's take a look to see how it will affect your personal situation. On the enrollment web site, you'll find a personalized example illustrating your current group disability insurance coverage, how the SIP policy can provide you with a higher benefit amount, and the premium associated with this supplemental coverage. If you decide this coverage is right for you, click on "Apply Now" at the top of this page to complete your online application.</div>


<? endif; ?>
			<div class="moreInfo">FOR MORE INFORMATION:
			
			<p>If you have questions and would like to speak to a Disability Specialist, please contact our</p>
			
			<div style="display:block; text-align:center; color: #00a8d4;"><b>Disability Hotline</b> @ <b><span class="phoneNumber"><?=$user->QUESTION_PHONE;?></span>
      
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
