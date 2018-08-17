// vim: ts=2:sw=2:nu:fdc=4:nospell
/**
	* Ext.ux.UploadForm Widget Example Application
	*
	* @author  Ing. Jozef Sakalos
	* @version $Id: uploadform.js,v 1.1 2007/09/20 11:54:22 bryan Exp $
	*
	*/

// set blank image to local file
Ext.BLANK_IMAGE_URL = 'ext/resources/images/default/s.gif';

// run this function when document becomes ready
Ext.onReady(function() {

	var iconPath = 'icons/';

	Ext.QuickTips.init();

	var upform = new Ext.ux.UploadForm('form-ct-in', {
		autoCreate: true
		, iconPath: iconPath
		, url: '/lifeblog/formposter.php'
		, method: 'post'
		, maxFileSize: 10485700000
		, pgCfg: {
			uploadIdName: 'UPLOAD_IDENTIFIER'
			, uploadIdValue: 'auto'
			, progressBar: true
			, progressTarget: 'under'
			, interval: 1000
			, maxPgErrors: 10
			, options: {
				url: '/lifeblog/formposter.php'
				, method: 'post'
//				, callback: pgCallback
			}
		}
		, baseParams: {
			cmd:'upload'
			, path: 'root'
		}
	});

	

}) // end of onReady


// end of file
