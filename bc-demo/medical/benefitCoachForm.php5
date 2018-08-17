<?php

include_once( 'benefitCoach.inc' );

?>
<html xmlns="https://www.w3.org/1999/xhtml" xml:lang="en" lang="en">
<head>
	<meta http-equiv="Content-Type" content="text/html; charset=iso-8859-1" />
	<title>in001_player_testpage_070324</title>
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
		
<!-- FORM DIV -->

<div id="formdiv">
	<form method="post" action="index.php5">
		<input type="hidden" name="mode" value="submit">
				<table border="0" cellspacing="0" cellpadding="1" width="340">
		<tr>
			<td colspan="3" style="color:white;background-color:purple">KEY: er=company; cl=employee</td>
		</tr>
		<tr>
			<td colspan="3" style="color:white;background-color:red">NOTE: all fields required with appropriate values; no commas in integers!</td>
		</tr>
		<tr>
			<td colspan="3" >&nbsp;</td>
		</tr>
		
		<tr><td colspan="3" style="padding:4px;color:white;background-color:grey">SETUP DATA</td></tr>
		
		<tr>
			<td width="150">er_enrollment_date</td>
			<td width="40" class="vartype">str</td>
			<td width="*"><input type="text" name="er_enrollment_date" value="July 1, 2007"></td>
		</tr>
		
		<tr>
			<td colspan="3" class="vartype">Open enrollment status (1 = open; 0 = past window; -1 = prior to window)...</td>
		</tr>
		<tr>
			<td colspan="2">er_enrollment_open</td>
<td><select name="er_enrollment_open">
<option value="1" selected>1</option>
<option value="0">0</option>
<option value="-1">-1</option>
</select></td>
		</tr>

		<tr><td colspan="3"><hr></td></tr>
		
		<tr>
			<td>cl_compensation_base</td>
			<td class="vartype">int</td>
			<td>$<input type="text" name="cl_compensation_base" value="5000" size="8" maxlength="7"></td>
		</tr>
		<tr>
			<td>cl_compensation_bonus</td>
			<td class="vartype">int</td>
			<td>$<input type="text" name="cl_compensation_bonus" value="2000" size="8" maxlength="7"></td>
		</tr>
		<tr>
			<td>cl_compensation_commission</td>
			<td class="vartype">int</td>
			<td>$<input type="text" name="cl_compensation_commission" value="3000" size="8" maxlength="7"></td>
		</tr>
		<tr>
			<td>cl_compensation_other</td>
			<td class="vartype">int</td>
			<td>$<input type="text" name="cl_compensation_other" value="4000" size="8" maxlength="7"></td>
		</tr>
		<tr>
			<td>cl_compensation_total</td>
			<td class="vartype">int</td>
			<td style="background-color:#66ffcc">$14000</td>
		</tr>
		
		

		<tr><td colspan="3" style="padding:4px;color:white;background-color:grey">EXISTING GROUP DATA</td></tr>

		<tr>
			<td colspan="2" class="vartype">set OLD compensation covered flags:</td>
<td><select name="er_old_compensation_set_menu">
<option value="base only">base only</option>
<option value="base & bonus" selected>base & bonus</option>
<option value="base & commission">base & commission</option>
<option value="base bonus & commission">base bonus & commission</option>
<option value="base & all other">base & all other</option>
<option value="base bonus & other selected">base bonus & other selected</option>
<option value="base commission & other selected">base commission & other selected</option>
<option value="base bonus commission & other selected">base bonus commission & other selected</option>
<option value="base & other selected">base & other selected</option>
</select></td>
		</tr>
		<tr>
			<td>er_old_compensation_covered_base</td>
			<td class="vartype">t/f</td>
			<td style="background-color:#66ffcc">1</td>
		</tr>
		<tr>
			<td>er_old_compensation_covered_bonus</td>
			<td class="vartype">t/f</td>
			<td style="background-color:#66ffcc">1</td>
		</tr>
		<tr>
			<td>er_old_compensation_covered_commission</td>
			<td class="vartype">t/f</td>
			<td style="background-color:#66ffcc">0</td>
		</tr>
		<tr>
			<td>er_old_compensation_covered_other</td>
			<td class="vartype">t/f</td>
			<td style="background-color:#66ffcc">0</td>
		</tr>
		<tr>
			<td>er_old_compensation_covered_selected</td>
			<td class="vartype">t/f</td>
			<td style="background-color:#66ffcc">0</td>
		</tr>

		<tr><td colspan="3"><hr></td></tr>
		
		<tr>
			<td colspan="2">er_old_disability_benefit</td>
<td><select name="er_old_disability_benefit">
<option value="20">20</option>
<option value="25">25</option>
<option value="30">30</option>
<option value="35">35</option>
<option value="40" selected>40</option>
<option value="45">45</option>
<option value="50">50</option>
<option value="55">55</option>
<option value="60">60</option>
<option value="65">65</option>
<option value="66">66</option>
<option value="70">70</option>
<option value="75">75</option>
<option value="80">80</option>
<option value="85">85</option>
</select>%</td>
		</tr>
		<tr>
			<td colspan="2">er_old_benefit_cap</td>
<td>$<select name="er_old_benefit_cap">
<option value="2000">2000</option>
<option value="2500">2500</option>
<option value="3000">3000</option>
<option value="3500">3500</option>
<option value="4000">4000</option>
<option value="4500">4500</option>
<option value="5000" selected>5000</option>
<option value="5500">5500</option>
<option value="6000">6000</option>
<option value="6500">6500</option>
<option value="7000">7000</option>
<option value="7500">7500</option>
<option value="8000">8000</option>
<option value="8500">8500</option>
<option value="9000">9000</option>
<option value="9500">9500</option>
<option value="10000">10000</option>
<option value="10500">10500</option>
<option value="11000">11000</option>
<option value="11500">11500</option>
<option value="12000">12000</option>
<option value="12500">12500</option>
<option value="13000">13000</option>
<option value="13500">13500</option>
<option value="14000">14000</option>
<option value="14500">14500</option>
<option value="15000">15000</option>
<option value="15500">15500</option>
<option value="16000">16000</option>
<option value="16500">16500</option>
<option value="17000">17000</option>
<option value="17500">17500</option>
<option value="18000">18000</option>
<option value="18500">18500</option>
<option value="19000">19000</option>
<option value="19500">19500</option>
<option value="20000">20000</option>
<option value="20500">20500</option>
<option value="21000">21000</option>
<option value="21500">21500</option>
<option value="22000">22000</option>
<option value="22500">22500</option>
<option value="23000">23000</option>
<option value="23500">23500</option>
<option value="24000">24000</option>
<option value="24500">24500</option>
<option value="25000">25000</option>
<option value="25500">25500</option>
<option value="26000">26000</option>
<option value="26500">26500</option>
<option value="27000">27000</option>
<option value="27500">27500</option>
<option value="28000">28000</option>
<option value="28500">28500</option>
<option value="29000">29000</option>
<option value="29500">29500</option>
<option value="30000">30000</option>
<option value="30500">30500</option>
<option value="31000">31000</option>
<option value="31500">31500</option>
<option value="32000">32000</option>
<option value="32500">32500</option>
<option value="33000">33000</option>
<option value="33500">33500</option>
<option value="34000">34000</option>
<option value="34500">34500</option>
<option value="35000">35000</option>
<option value="35500">35500</option>
<option value="36000">36000</option>
<option value="36500">36500</option>
<option value="37000">37000</option>
<option value="37500">37500</option>
<option value="38000">38000</option>
<option value="38500">38500</option>
<option value="39000">39000</option>
<option value="39500">39500</option>
<option value="40000">40000</option>
<option value="40500">40500</option>
<option value="41000">41000</option>
<option value="41500">41500</option>
<option value="42000">42000</option>
<option value="42500">42500</option>
<option value="43000">43000</option>
<option value="43500">43500</option>
<option value="44000">44000</option>
<option value="44500">44500</option>
<option value="45000">45000</option>
<option value="45500">45500</option>
<option value="46000">46000</option>
<option value="46500">46500</option>
<option value="47000">47000</option>
<option value="47500">47500</option>
<option value="48000">48000</option>
<option value="48500">48500</option>
<option value="49000">49000</option>
<option value="49500">49500</option>
<option value="50000">50000</option>
</select></td>
		</tr>
		<tr>
			<td colspan="2" class="vartype">set flag to omit VO reference to taxes</td>
<td><select name="er_old_benefit_omit_tax">
<option value="0" selected>0</option>
<option value="1">1</option>
</select></td>
		</tr>

		<tr><td colspan="3"><hr></td></tr>
		
		<tr>
			<td>cl_old_compensation_covered_total</td>
			<td class="vartype">int</td>
			<td>$<input type="text" name="cl_old_compensation_covered_total" value="7000" size="8" maxlength="7"></td>
		</tr>
		
		<tr>
			<td>cl_old_compensation_covered_at_risk</td>
			<td class="vartype">int</td>
			<td>$<input type="text" name="cl_old_compensation_covered_at_risk" value="7000" size="8" maxlength="7"></td>
		</tr>
		
		<tr>
			<td>cl_old_benefit</td>
			<td class="vartype">int</td>
			<td>$<input type="text" name="cl_old_benefit" value="2800" size="8" maxlength="7"></td>
		</tr>
		
		<tr>
			<td>cl_old_lost</td>
			<td class="vartype">int</td>
			<td>$<input type="text" name="cl_old_lost" value="11200" size="8" maxlength="7"></td>
		</tr>
		
		<tr>
			<td>cl_old_benefit_percent</td>
			<td class="vartype">int</td>
			<td><input type="text" name="cl_old_benefit_percent" value="20" size="8" maxlength="7">%</td>
		</tr>
		
		<tr>
			<td>cl_old_lost_percent</td>
			<td class="vartype">int</td>
			<td><input type="text" name="cl_old_lost_percent" value="80" size="7" maxlength="3">%</td>
		</tr>
		

		<tr><td colspan="3" style="padding:4px;color:white;background-color:grey">SUPPLEMENTAL DATA</td></tr>
		
		<tr>
			<td colspan="2">er_benefit_percentage</td>
<td><select name="er_benefit_percentage">
<option value="20">20</option>
<option value="25">25</option>
<option value="30">30</option>
<option value="35">35</option>
<option value="40">40</option>
<option value="45">45</option>
<option value="50" selected>50</option>
<option value="55">55</option>
<option value="60">60</option>
<option value="65">65</option>
<option value="66">66</option>
<option value="70">70</option>
<option value="75">75</option>
<option value="80">80</option>
<option value="85">85</option>
</select>%</td>
		</tr>
		<tr>
			<td colspan="2">er_benefit_cap</td>
<td>$<select name="er_benefit_cap">
<option value="2000">2000</option>
<option value="2500">2500</option>
<option value="3000">3000</option>
<option value="3500">3500</option>
<option value="4000">4000</option>
<option value="4500">4500</option>
<option value="5000">5000</option>
<option value="5500">5500</option>
<option value="6000">6000</option>
<option value="6500">6500</option>
<option value="7000">7000</option>
<option value="7500">7500</option>
<option value="8000">8000</option>
<option value="8500">8500</option>
<option value="9000">9000</option>
<option value="9500">9500</option>
<option value="10000" selected>10000</option>
<option value="10500">10500</option>
<option value="11000">11000</option>
<option value="11500">11500</option>
<option value="12000">12000</option>
<option value="12500">12500</option>
<option value="13000">13000</option>
<option value="13500">13500</option>
<option value="14000">14000</option>
<option value="14500">14500</option>
<option value="15000">15000</option>
<option value="15500">15500</option>
<option value="16000">16000</option>
<option value="16500">16500</option>
<option value="17000">17000</option>
<option value="17500">17500</option>
<option value="18000">18000</option>
<option value="18500">18500</option>
<option value="19000">19000</option>
<option value="19500">19500</option>
<option value="20000">20000</option>
<option value="20500">20500</option>
<option value="21000">21000</option>
<option value="21500">21500</option>
<option value="22000">22000</option>
<option value="22500">22500</option>
<option value="23000">23000</option>
<option value="23500">23500</option>
<option value="24000">24000</option>
<option value="24500">24500</option>
<option value="25000">25000</option>
<option value="25500">25500</option>
<option value="26000">26000</option>
<option value="26500">26500</option>
<option value="27000">27000</option>
<option value="27500">27500</option>
<option value="28000">28000</option>
<option value="28500">28500</option>
<option value="29000">29000</option>
<option value="29500">29500</option>
<option value="30000">30000</option>
<option value="30500">30500</option>
<option value="31000">31000</option>
<option value="31500">31500</option>
<option value="32000">32000</option>
<option value="32500">32500</option>
<option value="33000">33000</option>
<option value="33500">33500</option>
<option value="34000">34000</option>
<option value="34500">34500</option>
<option value="35000">35000</option>
<option value="35500">35500</option>
<option value="36000">36000</option>
<option value="36500">36500</option>
<option value="37000">37000</option>
<option value="37500">37500</option>
<option value="38000">38000</option>
<option value="38500">38500</option>
<option value="39000">39000</option>
<option value="39500">39500</option>
<option value="40000">40000</option>
<option value="40500">40500</option>
<option value="41000">41000</option>
<option value="41500">41500</option>
<option value="42000">42000</option>
<option value="42500">42500</option>
<option value="43000">43000</option>
<option value="43500">43500</option>
<option value="44000">44000</option>
<option value="44500">44500</option>
<option value="45000">45000</option>
<option value="45500">45500</option>
<option value="46000">46000</option>
<option value="46500">46500</option>
<option value="47000">47000</option>
<option value="47500">47500</option>
<option value="48000">48000</option>
<option value="48500">48500</option>
<option value="49000">49000</option>
<option value="49500">49500</option>
<option value="50000">50000</option>
</select></td>
		</tr>
		<tr>
			<td colspan="2">er_benefit_total_cap</td>
<td>$<select name="er_benefit_total_cap">
<option value="2000">2000</option>
<option value="2500">2500</option>
<option value="3000">3000</option>
<option value="3500">3500</option>
<option value="4000">4000</option>
<option value="4500">4500</option>
<option value="5000">5000</option>
<option value="5500">5500</option>
<option value="6000">6000</option>
<option value="6500">6500</option>
<option value="7000">7000</option>
<option value="7500">7500</option>
<option value="8000">8000</option>
<option value="8500">8500</option>
<option value="9000">9000</option>
<option value="9500">9500</option>
<option value="10000">10000</option>
<option value="10500">10500</option>
<option value="11000">11000</option>
<option value="11500">11500</option>
<option value="12000">12000</option>
<option value="12500">12500</option>
<option value="13000">13000</option>
<option value="13500">13500</option>
<option value="14000">14000</option>
<option value="14500">14500</option>
<option value="15000" selected>15000</option>
<option value="15500">15500</option>
<option value="16000">16000</option>
<option value="16500">16500</option>
<option value="17000">17000</option>
<option value="17500">17500</option>
<option value="18000">18000</option>
<option value="18500">18500</option>
<option value="19000">19000</option>
<option value="19500">19500</option>
<option value="20000">20000</option>
<option value="20500">20500</option>
<option value="21000">21000</option>
<option value="21500">21500</option>
<option value="22000">22000</option>
<option value="22500">22500</option>
<option value="23000">23000</option>
<option value="23500">23500</option>
<option value="24000">24000</option>
<option value="24500">24500</option>
<option value="25000">25000</option>
<option value="25500">25500</option>
<option value="26000">26000</option>
<option value="26500">26500</option>
<option value="27000">27000</option>
<option value="27500">27500</option>
<option value="28000">28000</option>
<option value="28500">28500</option>
<option value="29000">29000</option>
<option value="29500">29500</option>
<option value="30000">30000</option>
<option value="30500">30500</option>
<option value="31000">31000</option>
<option value="31500">31500</option>
<option value="32000">32000</option>
<option value="32500">32500</option>
<option value="33000">33000</option>
<option value="33500">33500</option>
<option value="34000">34000</option>
<option value="34500">34500</option>
<option value="35000">35000</option>
<option value="35500">35500</option>
<option value="36000">36000</option>
<option value="36500">36500</option>
<option value="37000">37000</option>
<option value="37500">37500</option>
<option value="38000">38000</option>
<option value="38500">38500</option>
<option value="39000">39000</option>
<option value="39500">39500</option>
<option value="40000">40000</option>
<option value="40500">40500</option>
<option value="41000">41000</option>
<option value="41500">41500</option>
<option value="42000">42000</option>
<option value="42500">42500</option>
<option value="43000">43000</option>
<option value="43500">43500</option>
<option value="44000">44000</option>
<option value="44500">44500</option>
<option value="45000">45000</option>
<option value="45500">45500</option>
<option value="46000">46000</option>
<option value="46500">46500</option>
<option value="47000">47000</option>
<option value="47500">47500</option>
<option value="48000">48000</option>
<option value="48500">48500</option>
<option value="49000">49000</option>
<option value="49500">49500</option>
<option value="50000">50000</option>
</select></td>
		</tr>
		
		<tr>
			<td colspan="3" class="vartype">Flag to trigger generic VO instead of data above...</td>
		</tr>
		<tr>
			<td colspan="2">er_other_cap</td>
<td><select name="er_other_cap">
<option value="0" selected>0</option>
<option value="1">1</option>
</select></td>
		</tr>

		<tr><td colspan="3"><hr></td></tr>
		
		<tr>
			<td colspan="2" class="vartype">set NEW compensation covered flags:</td>
<td><select name="er_compensation_set_menu">
<option value="base only">base only</option>
<option value="base & bonus">base & bonus</option>
<option value="base & commission">base & commission</option>
<option value="base bonus & commission">base bonus & commission</option>
<option value="base & all other" selected>base & all other</option>
<option value="base bonus & other selected">base bonus & other selected</option>
<option value="base commission & other selected">base commission & other selected</option>
<option value="base bonus commission & other selected">base bonus commission & other selected</option>
<option value="base & other selected">base & other selected</option>
</select></td>
		</tr>
		<tr>
			<td>er_compensation_covered_base</td>
			<td class="vartype">t/f</td>
			<td style="background-color:#66ffcc">1</td>
		</tr>
		<tr>
			<td>er_ompensation_covered_bonus</td>
			<td class="vartype">t/f</td>
			<td style="background-color:#66ffcc">1</td>
		</tr>
		<tr>
			<td>er_compensation_covered_commission</td>
			<td class="vartype">t/f</td>
			<td style="background-color:#66ffcc">1</td>
		</tr>
		<tr>
			<td>er_compensation_covered_other</td>
			<td class="vartype">t/f</td>
			<td style="background-color:#66ffcc">1</td>
		</tr>
		<tr>
			<td>er_compensation_covered_selected</td>
			<td class="vartype">t/f</td>
			<td style="background-color:#66ffcc">0</td>
		</tr>
		
		<tr><td colspan="3" style="padding:4px;color:white;background-color:grey">WRAP-UP DATA</td></tr>
		
		<tr>
			<td>cl_compensation_covered_additional_total</td>
			<td class="vartype">int</td>
			<td>$<input type="text" name="cl_compensation_covered_additional_total" value="7000" size="8" maxlength="7"></td>
		</tr>
		<tr>
			<td>cl_additional_benefit</td>
			<td class="vartype">int</td>
			<td>$<input type="text" name="cl_additional_benefit" value="7000" size="8" maxlength="7"></td>
		</tr>
		<tr>
			<td>cl_total_benefit</td>
			<td class="vartype">int</td>
			<td>$<input type="text" name="cl_total_benefit" value="9800" size="8" maxlength="7"></td>
		</tr>
		<tr>
			<td>cl_total_percentage</td>
			<td class="vartype">int</td>
			<td><input type="text" name="cl_total_percentage" value="70" size="7" maxlength="3">%</td>
		</tr>

		<tr><td colspan="3"><hr></td></tr>
		
		<tr>
			<td>cl_notobacco_premium</td>
			<td class="vartype">int</td>
			<td>$<input type="text" name="cl_notobacco_premium" value="400" size="8" maxlength="7"></td>
		</tr>
		<tr>
			<td>cl_tobacco_premium</td>
			<td class="vartype">int</td>
			<td>$<input type="text" name="cl_tobacco_premium" value="500" size="8" maxlength="7"></td>
		</tr>

		<tr><td colspan="3"><hr></td></tr>
		
		<tr>
			<td colspan="3"><input type="submit" value="post values"></td>
		</tr>
		</table>
	</form>
</div

<!-- FLASH DIV -->
<div id="flashdiv">
	<p>&nbsp;</p></p>This is one of our test forms for sending data to the Flash Movie.</p><p><a href="login.php5" title="Login to Benefit Coach">Proceed To The Login</a></p></div>

</body>
</html>
