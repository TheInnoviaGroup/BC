
/*
	debugruler()
	written by Chris Heilmann for alistapart.
	enables a rollover of rows for each table with the classname "debugtable"

	modified by Bryan Johnson to remember the tr's old class.
*/

function debugruler()
{
	if (document.getElementById && document.createTextNode)
	{
		var tables=document.getElementsByTagName('table');
		for (var i=0;i<tables.length;i++)
		{
			if(tables[i].className=='debugtable')
			{
				var trs=tables[i].getElementsByTagName('tr');
				for(var j=0;j<trs.length;j++)
				{
					if(trs[j].parentNode.nodeName=='TBODY')
					{
					  trs[j].onmouseover=function(){this.oldClass=this.className;this.className='ruled';return false}
						trs[j].onmouseout=function(){this.className=this.oldClass;return false}
					}
				}
			}
		}
	}
}

window.onload=function(){debugruler();}