<?php

include_once( 'benefitCoach.inc' );

?>
<html>
<head><title></title>
<LINK REL=STYLESHEET TYPE='text/css' HREF='style.css'>
</head>
<body>
<table border="0" width="100%" cellspacing="0" cellpadding="0" align="center">
	<tr>
		<td width="204"><img src="images/todd_logo.jpg" width="204" height="76" hspace="0" vspace="0" 
align="left"></td>
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
      <h2>Frequently
Asked Questions:</h2>
<div class="question">Whom should I call with questions?</div>
<div class="answer">Please call your SIP Plan representative at <? if($user->NewBenefitLOB2 > 0): ?>888-640-8470<? else: ?>800-577-5457<? endif; ?></div>

<div class="questionHead">Policy Features:</div>

<? if($user->NewBenefitLOB2 <= 0): ?>
<div class="question">Can I participate in the SIP plan if I am not enrolled in the Group LTD plan? </div>
<div class="answer">YES. You may apply for the SIP plan if you aren't enrolled in the Group LTD plan.</div>
<? endif; ?>

<div class="question">What are the main advantages of applying for the coverage through this option rather than attempting to purchase it on my own?</div>
<div class="answer">
<ul type="disc">
	<li>Discounted Premium Rates because you are an affiliated-employee of the company.</li>
	<li>Guaranteed standard issue* underwriting allows you to apply for the SIP plan with no medical exam; however, you will be asked a few medical questions and benefits may be subject to a pre-existing condition limitation.</li>
	<li>No medical exam is required if you apply now.</li>
	<li>Convenient payroll deduction.</li>
</ul>
</div>

<div class="question">How much coverage can I purchase?</div>
<div class="answer">This depends on your income level and any other disability coverage you may have in force.</div>

<div class="question">I already own individual insurance, can I still participate?</div>
<div class="answer">YES. A comparison of the SIP plan with any existing coverage can be illustrated for you by calling <? if($user->NewBenefitLOB2 > 0): ?>888-640-8470<? else: ?>800-577-5457<? endif; ?> for additional information. </div>

<div class="question">What happens if I terminate employment?</div>
<div class="answer">Your coverage is individually owned which means you take it with you if you leave the company.  Your policy premium will not increase until age 65.</div>

<div class="question">If I use tobacco in any form will it affect my premium?</div>
<div class="answer">YES. Your personalized information provides premiums for both tobacco and non-tobacco use. If you are an occasional cigar smoker (less than two cigars per month) you should complete the form as a non-tobacco user. Please provide details of your cigar use in Section 2, after details 1-3.</div>

<div class="question">If I don't enroll now, can I participate in the future?</div>
<div class="answer">If you do not apply for coverage now, you may be required to provide full medical underwriting in the future if you choose to apply later.</div>

<div class="question">Can I discontinue coverage at any time?</div>
<div class="answer">YES. Premiums will be deducted from your pay each pay period. You may discontinue your participation in this program at any time by calling 877-286-2389.</div>

<div class="question">Can Unum cancel my policy?</div>
<div class="answer">Your policy cannot be cancelled or modified before age 65 without your permission as long as you continue to pay your premiums when due.</div>

<div class="question">If I become disabled, do I still have to pay premiums?</div>
<div class="answer">NO. After 90 days of covered disability, all future premiums will be waived while you continue to receive benefits under the SIP policy. Any premiums paid during the 90 days will be refunded.</div>

<div class="question">If I am receiving benefits under the Group LTD Plan, can I still receive benefits under the SIP Plan?</div>
<div class="answer">YES. Your SIP monthly benefit will not be offset or reduced by benefits received under the group plan.</div>


<div class="questionHead">Tax & Administrative Questions:</div>


<div class="question">Is my SIP benefit payment taxable?</div>
<div class="answer">Because you pay the premium for your SIP coverage with "after tax" dollars your SIP benefit will be "tax-free", under current tax laws.</div>

<div class="question">Where do I need to send the completed application?</div>
<div class="answer">Please complete the attached application via the web and submit.</div>

<div class="question">When is my coverage effective?</div>
<div class="answer">If accepted, coverage will be effective the first of the month in which payroll deductions begin, if, for the period of time commencing 180 days prior to, and including, the date of the application, you have been continuously at work on a full-time basis performing all the duties of your occupation without limitation due to injury or sickness and have not been hospitalized or homebound or hospitalized due to an injury or sickness.</div>

<div class="question">When do payroll deductions begin?</div>
<div class="answer">Your payroll deductions will begin the first pay date in July 2008. Premiums are deducted one month in advance of the policy date</div>

<div class="fine_print_blue">
<i>*Guaranteed standard issue underwriting is available to employees who for 180 days prior to, and including the application date must: a) not have missed one or more days of work or been homebound or admitted to a medical facility due to injury or sickness, and b) not have had any restrictions or limitations to the ability to work on a full time basis due to injury or sickness. You will be asked to meet additional requirements for long-term care exchange and catastrophic disability benefits.</i></div>

<div class="fine_print">
<p>
<i>The policy or its provisions may vary or be unavailable in some states.  The policy has exclusions and limitations that may affect any benefits payable.  See the actual policy or your Unum representative for specific provisions and details of availability.</i>
</p>


<p>
<i>Underwritten by:</i><br>
Provident Life and Accident Insurance Company<br>
1 Fountain Square, Chattanooga, TN 37402<br>
Long Term Care insurance will be underwritten by one of Unum Group's insuring affiliates.
unum.com
</p>


<p>
©2008 Unum Group. All rights reserved. Unum is a registered trademark and marketing brand of Unum Group and its insuring subsidiaries.
</p>


CU-9059 (3-08)</div>


			<div class="moreInfo">FOR MORE INFORMATION:
			
			<p>If you have questions and would like to speak to a Disability Specialist, please contact our</p>
			
			<? if($user->NewBenefitLOB2 > 0): ?>
		<div style="display:block; text-align:center; color: #00a8d4;"><b>Disability Hotline</b> @ <b><span class="phoneNumber">888-640-8470</span>
		<? else: ?>
		<div style="display:block; text-align:center; color: #00a8d4;"><b>Disability Hotline</b> @ <b><span class="phoneNumber">800-577-5457</span>
		</div>
		<? endif; ?>
      
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
