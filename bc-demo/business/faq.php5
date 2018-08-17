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
      <h2>Frequently
Asked Questions:</h2>

      <div class="question">Why
do I need supplemental coverage? Don&rsquo;t I already have group
coverage?</div>

      <div class="answer">Your
company provides you with a long-term disability group plan that
replaces 60% of your base salary to a maximum monthly benefit of
$10,000. However, this benefit is company paid, so any benefits that
you receive are taxable. Further, your current group disability plan
does not cover other types of compensation such as incentive and
bonus compensation, so the benefit that you receive may be
significantly less than your actual income.</div>

      <div class="question">What
is the advantage of buying this policy through your company&rsquo;s
endorsement?</div>

<div class="answer">
      <ul>

        <li>You can apply for an additional monthly
benefit of up to $15,000 with limited medical or financial
underwriting. The benefit amount available to you is based on your
total earned income amount, covering all forms of compensation to
include your base salary, bonus, commissions, and other forms of
incentive compensation.</li>

        <li>You will receive a discount from the carrier&rsquo;s individual policy rates.</li>

        <li>
          The policy is individually-owned by<i> Client Name </i>you. Should you decide to leave your company for any reason, you can take the coverage with you at the same discounted rate.

        </li>

        <li>Premiums are fixed up to age 65.
        </li>
        
        <li>
          Premiums can be conveniently paid through pay-roll deductions and because they are paid with after-tax dollars, benefits are tax-free and are in addition to benefits available under company-provided group coverage.

        </li>

      </ul>
			</div>
			
      <div class="question">I already own my own policy. What should I do?</div>

      <div class="answer">You will have to evaluate the supplemental policy being offered in comparison to the policy you currently own. Either your financial advisor or one of our Executive Disability Specialists can help you with that comparison. During this evaluation, you may even discover that there is an opportunity to purchase this coverage in addition to your existing protection.</div>

			<div class="question">Can I buy enough coverage to insure all of my compensation?</div>
			
			<div class="answer">Combined with the monthly maximum of $10,000 under your Group Long Term Disability plan, this supplemental individual policy allows you up to $15,000 of additional monthly benefits, protecting up to $25,000 of your total monthly income. This additional coverage will help cover a much higher percentage of your current total income.</div>
			
			<div class="question">How do I apply for this coverage? </div>
			
			<div class="answer">You may apply through the convenience of online enrollment by simply clicking on &ldquo;Apply Now&rdquo; on the main page and answer a few simple questions.</div>
			
			<div class="question">How do I know this site is secure and my personal information is protected? </div>
			
			<div class="answer">Your privacy and the security of your data is a major goal. The site utilizes the latest in web security practices including SSL (Secure Socket Layer) for the highest levels of encryption. This technology is standard in the e-commerce industry, and encodes and decodes any information going in and out of the server. This is the same type of technology utilized on e-commerce sites by major companies needing the highest level of security. </div>
			
			<div class="question">If I apply online do I need to provide a &ldquo;wet signature&rdquo;?</div>
			
			<div class="answer">The online tool utilizes e-signature technology. Once you click the &ldquo;Submit&rdquo; button on the application page, you have officially submitted your application. There is no &ldquo;wet signature&rdquo; or paper application required.</div>
			
			<div class="question">Will I have to take a physical to apply for coverage?</div>
			
			<div class="answer">If you apply for a monthly benefit greater than this guaranteed issue amount, you will have to provide further evidence of insurability which may include a physical exam.</div>
			
			<div class="question">Can I increase my benefit amount in the future?</div>
			
			<div class="answer">Yes. On an annual basis, you will have the opportunity to make adjustments to your coverage based upon your changing needs. This includes coverage increases warranted by increases in your income.</div>
			
			<div class="question">What happens with my coverage when I return from a disability?</div>
			
			<div class="answer">Your coverage will continue. If you have a recurrence of a prior disability within twelve months of returning to work, your waiting period is waived. If you have an unrelated disability within 30 days after returning to work, your waiting period is waived.</div>
			
			<div class="question">While I am disabled, will I have to pay premiums?</div>
			
			<div class="answer">No, after you satisfy the waiting period, your future premiums are waived while you are receiving benefits under the policy.</div>



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
