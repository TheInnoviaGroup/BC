<table class="sortable" id="searchresults"><thead><caption>Search results for: <?=$for;?></caption>
<tr><th>Name</th><th>Company</th><th>City</th><th>State</th></tr>
</thead>
<tbody>
<? foreach ($results as $row): ?>
<tr class="hoverable">
<td><a href="<?=$self;?>?mode=Person&action=showPerson&personID=<?=$row['personID'];?>"><?=($row['cn'])?$row['cn']:$row['gn'].' '.$row['sn'];?></a></td>
<td><a href="<?=$self;?>?mode=Company&action=showCompany&companyID=<?=$row['companyID'];?>"><?=$row['companyName'];?></a></td>
<td><?=$row['city'];?></td>
<td><?=$row['state'];?></td>
</tr>
<? endforeach; ?>
</tbody>
</table>