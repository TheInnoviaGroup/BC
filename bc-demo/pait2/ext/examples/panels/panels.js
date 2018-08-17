/*
 * Ext JS Library 1.0.1
 * Copyright(c) 2006-2007, Ext JS, LLC.
 * licensing@extjs.com
 * 
 * http://www.extjs.com/license
 */

Ext.onReady(function(){


    var p = new Ext.Viewport({
        //width:800,
        //height:500,
        //elements:'body',
        layout: new Ext.layout.BorderLayout(),

        items:[
            new Ext.Panel({
                region:'west',
                title:'Text Tab 1',
                html:'<p>Some Content</p><p>Some Content</p><p>Some Content</p><p>Some Content</p><p>Some Content</p><p>Some Content</p><p>Some Content</p><p>Some Content</p><p>Some Content</p><p>Some Content</p><p>Some Content</p><p>Some Content</p><p>Some Content</p><p>Some Content</p><p>Some Content</p><p>Some Content</p><p>Some Content</p><p>Some Content</p><p>Some Content</p><p>Some Content</p><p>Some Content</p><p>Some Content</p><p>Some Content</p><p>Some Content</p><p>Some Content</p><p>Some Content</p>',
                autoScroll:true,
                width:200,
                collapsible:true,
                split:true
            }),
            new Ext.Panel({
                region:'east',
                title:'Text Tab 1',
                html:'<p>Some Content</p><p>Some Content</p><p>Some Content</p><p>Some Content</p><p>Some Content</p><p>Some Content</p><p>Some Content</p><p>Some Content</p><p>Some Content</p><p>Some Content</p><p>Some Content</p><p>Some Content</p><p>Some Content</p><p>Some Content</p><p>Some Content</p><p>Some Content</p><p>Some Content</p><p>Some Content</p><p>Some Content</p><p>Some Content</p><p>Some Content</p><p>Some Content</p><p>Some Content</p><p>Some Content</p><p>Some Content</p><p>Some Content</p>',
                autoScroll:true,
                width:200,
                collapsible:true,
                split:true
            }),
            new Ext.Panel({
                region:'south',
                title:'Text Tab 1',
                html:'<p>Some Content</p><p>Some Content</p><p>Some Content</p><p>Some Content</p><p>Some Content</p><p>Some Content</p><p>Some Content</p><p>Some Content</p><p>Some Content</p><p>Some Content</p><p>Some Content</p><p>Some Content</p><p>Some Content</p><p>Some Content</p><p>Some Content</p><p>Some Content</p><p>Some Content</p><p>Some Content</p><p>Some Content</p><p>Some Content</p><p>Some Content</p><p>Some Content</p><p>Some Content</p><p>Some Content</p><p>Some Content</p><p>Some Content</p>',
                autoScroll:true,
                height:100,
                collapsible:true,
                split:true
            }),
            new Ext.Panel({
                id:'north-test',
                region:'north',
                title:'Text Tab 1',
                html:'<p>Some Content</p><p>Some Content</p><p>Some Content</p><p>Some Content</p><p>Some Content</p><p>Some Content</p><p>Some Content</p><p>Some Content</p><p>Some Content</p><p>Some Content</p><p>Some Content</p><p>Some Content</p><p>Some Content</p><p>Some Content</p><p>Some Content</p><p>Some Content</p><p>Some Content</p><p>Some Content</p><p>Some Content</p><p>Some Content</p><p>Some Content</p><p>Some Content</p><p>Some Content</p><p>Some Content</p><p>Some Content</p><p>Some Content</p>',
                autoScroll:true,
                height:100,
                collapsible:true,
                split:true
            }),
            new Ext.TabPanel2({
                region:'center',
                id:'tabs',
                activeItem:0,
                resizeTabs:true,
                tabWidth:100,
                tabPosition:'bottom',
                items:[
                    new Ext.Panel({
                        title:'Text Tab 1',
                        elements:'body',
                        autoScroll:true
                    }),
                    new Ext.Panel({
                        title:'Text Tab 2',
                        html:'<p>Some Content</p>',
                        elements:'body'
                    }),
                    new Ext.Panel({
                        title:'Text Tab 3',
                        html:'<p>Some Content</p>',
                        elements:'body',
                        closable:true
                    }),
                    new Ext.Panel({
                        title:'Text Tab 4',
                        html:'<p>Some Content</p>',
                        elements:'body'
                    })
                ]
            })
        ]/*,

        renderTo: document.body*/ // inline render
    });

    return;
    var p = new Ext.Panel({
        frame:true,
        width:400,
        collapsible:true,
        title:"Static Framed Panel",
        contentEl:'some-content', // add some existing markup
        renderTo: document.body // inline render
    });


    var p4 = new Ext.Window({
        width:600,
        height:400,
        layout: new Ext.layout.FitLayout(),
        collapsible:true,
        title:"Nested Tabs Panel",
        buttons : [{
            text:'Submit'
        }],
        tbar : [{
            text: 'Dyanmic Toolbar'
        }],
        items: new Ext.TabPanel2({
                id:'tabs',
                activeItem:0,
                resizeTabs:true,
                tabWidth:100,
                items:[
                    new Ext.Panel({
                        title:'Text Tab 1',
                        elements:'body',
                        autoScroll:true
                    }),
                    new Ext.Panel({
                        title:'Text Tab 2',
                        html:'<p>Some Content</p>',
                        elements:'body'
                    }),
                    new Ext.Panel({
                        title:'Text Tab 3',
                        html:'<p>Some Content</p>',
                        elements:'body',
                        closable:true
                    }),
                    new Ext.Panel({
                        title:'Text Tab 4',
                        html:'<p>Some Content</p>',
                        elements:'body'
                    })
                ]
            }),

        renderTo: document.body // inline render
    });
    p4.show();

    setTimeout(function(){
    var tabs = Ext.getCmp('tabs');
    tabs.add(new Ext.Panel({
        title:'Text Tab 2',
        html:'<p>Dynamic added tab content</p>',
        elements:'body'
    }));
    }, 2000);

    var p2 = new Ext.TabPanel2({
        width:400,
        renderTo: document.body,
        tabTextWidth:100,
        items:[
            new Ext.Panel({
                title:'Text Tab 1',
                html:'<p>Some Content</p>',
                elements:'body',
                autoHeight:true
            }),
            new Ext.Panel({
                title:'Text Tab 2',
                html:'<p>Some Content</p>',
                elements:'body',
                autoHeight:true
            }),
            new Ext.Panel({
                title:'Text Tab 3',
                html:'<p>Some Content</p>',
                elements:'body',
                autoHeight:true,
                closable:true
            }),
            new Ext.Panel({
                title:'Text Tab 4',
                html:'<p>Some Content</p>',
                elements:'body',
                autoHeight:true
            })
        ]
    });
    p2.setActiveTab(0);

    var p3 = new Ext.TabPanel2({
        width:400,
        renderTo: document.body,
        tabPosition:'bottom',
        items:[
            new Ext.Panel({
                title:'Text Tab 1',
                html:'<p>Some Content</p>',
                elements:'body',
                autoHeight:true
            }),
            new Ext.Panel({
                title:'Text Tab 2',
                html:'<p>Some Content</p>',
                elements:'body',
                autoHeight:true
            }),
            new Ext.Panel({
                title:'Text Tab 3',
                html:'<p>Some Content</p>',
                elements:'body',
                autoHeight:true
            }),
            new Ext.Panel({
                title:'Text Tab 4',
                html:'<p>Some Content</p>',
                elements:'body',
                autoHeight:true
            })
        ]
    });
    p3.setActiveTab(0);
});