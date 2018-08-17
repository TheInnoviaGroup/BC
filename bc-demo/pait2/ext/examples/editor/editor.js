/*
 * Ext JS Library 1.0.1
 * Copyright(c) 2006-2007, Ext JS, LLC.
 * licensing@extjs.com
 * 
 * http://www.extjs.com/license
 */

Ext.onReady(function(){

    Ext.QuickTips.init();

    new Ext.Panel({
        title:'HTML Editor Example',
        cls:'x-panel-blue',
        frame:true,
        layout: new Ext.layout.FitLayout(),
        items : new Ext.form.HtmlEditor(),
        renderTo : 'ec',
        width:600,
        height:450
    });
});
