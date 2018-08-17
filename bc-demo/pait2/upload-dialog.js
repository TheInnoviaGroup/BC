var UploadDialogController = function()
{
  var dialog = null;  
  var button = null;
  var file_list_tpl = new Ext.Template(
    "<div class='file-list-entry'>File {name} successfully uploaded{extraText}.</div>"
  );
  file_list_tpl.compile();

  

  function getDialog()
  {
    if (!dialog) {
      dialog = new Ext.ux.UploadDialog.Dialog(null, {
        autoCreate: true,
        closable: true,
        collapsible: false,
        draggable: true,
        minWidth: 400,      
        minHeight: 200,
        width: 400,
        height: 350,
        proxyDrag: true,
        resizable: true,
        constraintoviewport: true,
        title: 'File upload queue.',
        url: 'uploadReceiver.php5',
        reset_on_hide: true,
        allow_close_on_upload: true,
        upload_autostart: false
      });
      
      dialog.on('uploadsuccess', onUploadSuccess);
    }
    return dialog;
  }
  
  
  
  function onUploadSuccess(dialog, filename, resp_data)
  {
    console.log('resp_data: %o', resp_data);
    var parts = filename.split(/\/|\\/);
    if (parts.length == 1) {
      filename = parts[0];
    }
    else {
      filename = parts.pop();
    }
    if (filename != resp_data.filename) {
      file_list_tpl.append('file-list', {name: filename,extraText: ' <b>and renamed to '+resp_data.filename+'</b>'});
    } else {
    file_list_tpl.append('file-list', {name: filename,extraText: ''});
    }
    var sheetList = Ext.get('sheetListDiv');
    sheetList.load(
                   {
        url: 'index.php5?mode=Workspace&action=sheetList',
        scripts:true,
        text: "Refreshing..."
   });
                   
  }

return {  showDialog: function (button)
  {
    getDialog().show(button.getEl());
  }
  }
}();


