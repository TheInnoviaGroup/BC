<html 
    xmlns="http://www.w3.org/1999/xhtml" 
    xml:lang="en" 
    lang="en">
<head>
 <title>Ext.ux.UploadDialog user extension.</title>
 <meta http-equiv='Content-Type' content='text/html; charset=utf-8' />  
 <style type='text/css'>
  @import url('ext/resources/css/ext-all.css');
  @import url('ext/resources/css/xtheme-aero.css');
  @import url('ext/ProgressBar/Ext.ux.ProgressBar.css');
  @import url('ext/UploadDialog/css/Ext.ux.UploadDialog.css');
  @import url('upload-dialog.css');  
</style>
<style type='text/css'>
    #loading-mask 
    {
      width:100%;
      height:100%;
      background:#c3daf9;
      position:absolute;
      z-index:20000;
      left:0px;
      top:0px;
    }
    
    #loading 
    {
      position:absolute;
      left:42%;
      top:40%;
      border:1px solid #6593cf;
      padding:2px;
      background:#c3daf9;
      width:200px;
      text-align:center;
      z-index:20001;
    }
    
    #loading .loading-indicator 
    {
      border:1px solid #a3bad9;
      background: white;
        background-position: 15px center;
      color:#003366;
      font:bold 13px tahoma,arial,helvetica;
      padding:10px;
      margin:0;
    }
</style>
</head>

<body>
 <div id="loading-mask" style="width:100%;height:100%;background:#c3daf9;position:absolute;z-index:20000;left:0;top:0;">&#160;</div>

 <div id="loading" style="z-index: 20001; position: absolute">
  <div class="loading-indicator">
   <img src="ext/resources/images/default/grid/loading.gif" style="width:16px; height:16px; vertical-align: middle" alt="Loading indicator" />
   &#160;Loading
  </div>
 </div>
 <!-- ########################################################################################## -->
 
 <div id='demo-panel'>
  <h3>Demo panel</h3>

  <div id='file-list'></div>
  <div id='show-dialog-btn'>
  </div>
 </div>
 
 <div id="help-panel">
  <h1>Ext.ux.UploadDialog (for ExtJS 1.1) demo page.</h1>
  <h2>Usage example.</h2>
  <p>

   This is the code taken from upload-dialog.js.
  </p>
  <pre>dialog = new Ext.ux.UploadDialog.Dialog(null, {
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
  url: '/lifeblog/formposter.php',
  reset_on_hide: false,
  allow_close_on_upload: true
});
...
dialog.show('show-button');</pre>
  <h2>Configuration.</h2>
    <p>
     Most configuration options are inherited from BasicDialog (see ExtJs docs).
     The added ones are:
    </p>
    <ul>
     <li><b>url</b> - the url where to post uploaded files.</li>

     <li><b>base_params</b> - additional post params (default to {}).</li>
     <li><b>permitted_extensions</b> - array of file extensions which are permitted to upload (default to []).</li>
     <li><b>reset_on_hide</b> - whether to reset upload queue on dialog hide or not (default true).</li>
     <li><b>allow_close_on_upload</b> - whether to allow hide/close dialog during upload process (default false).</li>

     <li><b>upload_autostart</b> - whether to start upload automaticaly when file added or not (default false).</li> 
    </ul>
    <h2>Events.</h2>
    <ul>
     <li>
      <b>filetest</b> - fires before file is added into the queue, parameters:<br/>

      <i>dialog</i> - reference to dialog<br/>
      <i>filename</i> - file name<br/>
      If handler returns false then file will not be queued.
     </li>
     
   <li>
    <b>fileadd</b> - fires when file is added into the queue, parameters:<br/>

    <i>dialog</i> - reference to dialog<br/>
      <i>filename</i> - file name
   </li>
   
   <li>
    <b>fileremove</b> - fires when file is removed from the queue, parameters:<br/>

    <i>dialog</i> - reference to dialog<br/>
      <i>filename</i> - file name    
   </li>
   
   <li>
    <b>resetqueue</b> - fires when upload queue is resetted, parameters:<br/>

    <i>dialog</i> - reference to dialog<br/>
   </li>
   
   <li>
    <b>uploadsuccess</b> - fires when file is successfuly uploaded, parameters:<br/>
    <i>dialog</i> - reference to dialog<br/>

    <i>filename</i> - uploaded file name<br/>
    <i>data</i> - js-object builded from json-data returned from upload handler response.     
   </li>
   
   <li>
    <b>uploaderror</b> - fires when file upload error occured, parameters:<br/>

    <i>dialog</i> - reference to dialog<br/>
    <i>filename</i> - uploaded file name<br/>
    <i>data</i> - js-object builded from json-data returned from upload handler response.
   </li>
   
   <li>

    <b>uploadfailed</b> - fires when file upload failed, parameters:<br/>
    <i>dialog</i> - reference to dialog<br/>
    <i>filename</i> - failed file name
   </li>
   
   <li>

    <b>uploadstart</b> - fires when upload process starts, parameters:<br/>
    <i>dialog</i> - reference to dialog    
   </li>
   
   <li>
    <b>uploadstop</b> - fires when upload process stops, parameters:<br/>

    <i>dialog</i> - reference to dialog
   </li>
     
   <li>
    <b>uploadcomplete</b> - fires when upload process complete (no files to upload left), parameters:<br/>
    <i>dialog</i> - reference to dialog
   </li>

    </ul>
    
    <h2>Public methods</h2>
    <p>
     Better go see the source.
    </p>
    
    <h2>I18n.</h2>
    <p>
     The class is ready for i18n, override the Ext.ux.UploadDialog.Dialog.prototype.i18n object with
     your language strings, or just pass i18n object in config. 
    </p>

    
    <h2>Server side handler.</h2>
    <p>
     The files in the queue are posted one at a time, the file field name is <i>'file'</i>. The handler
     should return json encoded object with following properties:
    </p>
    <pre>{
  success: true|false, // required
  error: 'Error or success message' // optional, also can be named 'message'
}</pre>
    
    <h2>Download.</h2>
    
    <p>

     <a href="/ext.ux/Ext.ux.UploadDialog.zip" title="Download the Ext.ux.UploadDialog extension.">
      Ext.ux.UploadDialog.zip
     </a>
    </p>
    
    <h2>Licence.</h2>
    
    <p>
     No warranties, use it on your own risk, respectoware :D 
     (if you like it and feels it's useful for you go to 
     <a href="http://extjs.com/forum/showthread.php?t=11111" title="Go to ExtJS forum.">
      ExtJS forum
     </a>

     find any of my posts (username MaximGB) and add to my reputation :)))))
    </p>
    
    <h2>Author.</h2>
    <p>
     Maxim Bazhenov (aka MaximGB)
    </p>
    
    <hr/>
    <h2>Change history</h2>
    <ul>

     <li>
      <b>13.08.2007</b><br/>
      - initial release.
     </li>
     <li>
      <b>14.08.2007</b><br/>
      - stopUpload() and abort action are now synchronous.<br/>
      - uploadstopping event removed.<br/>

      - allow_close_on_upload config parameter and respective getter/setter added.<br/>
      - upload_autostart config parameter and respective getter/setter added.<br/>
      - now user can add files during upload process.
     </li>
     <li>
      <b>16.08.2007</b><br/>
      - Fixed: scrollbars shown in dialog body when aero theme applyed.<br/>

      - Dialogs progress bar style changed from 100% to auto.<br/>
      - Added ru.utf-8 locale.
     </li>
     <li>
      <b>17.08.2007</b><br/>
      - Fixed division by zero bug in Ext.ux.ProgressBar component which UploadDialog depends on.<br/>
      - Fixed wrong complete count bug. Processing file was counted as completed.<br/>

      - Fixed upload/abort button state mishandling when upload_autostart option is enabled.
     </li>
    </ul>
    
 </div>
 
 <!-- ########################################################################################## -->
 <script type='text/javascript' src='ext/adapter/ext/ext-base.js'></script>
 <script type='text/javascript' src='ext/ext-all.js'></script>
 <script type='text/javascript' src='ext/ProgressBar/Ext.ux.ProgressBar.js'></script>

 <script type='text/javascript' src='ext/UploadDialog/Ext.ux.UploadDialog.js'></script>
 <script type='text/javascript' src='upload-dialog.js'></script>
</body>