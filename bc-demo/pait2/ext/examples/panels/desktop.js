/*
 * Ext JS Library 1.0.1
 * Copyright(c) 2006-2007, Ext JS, LLC.
 * licensing@extjs.com
 * 
 * http://www.extjs.com/license
 */

Ext.onReady(function(){

    var windowIndex = 0;

    var activeWindow = null;
    var windows = new Ext.WindowGroup();
    //var winbar = new Ext.Toolbar('winbar');
    
    function createWin(animTarget){
        var win = new Ext.Window({
            title:'Generic Window '+(++windowIndex),
            width:800,
            height:600,
            html : '<p>Something useful would be in here.</p>',
            iconCls: 'plugin',
            manager: windows,
            shim:false,
            animCollapse:false
        });
        win.render('desktop');
        win.taskItem =  new Ext.Button('winbar', {
            cls: 'x-btn-text-icon',
            text: win.title,
            handler : function(){
                if(win.minimized || win.hidden){
                    win.show();
                }else if(win == activeWindow){
                    minimizeWin(win);
                }else{
                    win.toFront();
                }
            },
            //handleMouseEvents:false,
            clickEvent:'mousedown'
        });
        layoutDesktop();
        win.on('activate', markActive);
        win.on('beforeshow', markActive);
        win.on('deactivate', markInactive);
        win.on('minimize', minimizeWin);
        win.on('close', removeWin);
        markActive(win);
        win.show(win.taskItem.el);
    }

    function minimizeWin(win){
        win.minimized = true;
        win.hide(win.taskItem.el);
    }

    function markActive(win){
        if(activeWindow && activeWindow != win){
            markInactive(activeWindow);
        }
        activeWindow = win;
        Ext.fly(win.taskItem.el).addClass('active-win');
        win.minimized = false;
    }

    function markInactive(win){
        if(win == activeWindow){
            activeWindow == null;
            Ext.fly(win.taskItem.el).removeClass('active-win');
        }
    }

    function removeWin(win){
        win.taskItem.destroy();
        layoutDesktop();
    }

    var desktop = Ext.get('desktop');
    var taskbar = Ext.get('taskbar');

    function layoutDesktop(){
        desktop.setHeight(Ext.lib.Dom.getViewHeight()-taskbar.getHeight());
    }

    Ext.EventManager.onWindowResize(layoutDesktop);

    var qlaunch = new Ext.Toolbar('qlaunch');
    qlaunch.add({
        text:'Create Window',
        handler: function(){
            createWin();
        }
    });




/*

    return;
    var windowIndex = 0;

    new Ext.Button('buttons', {
        text: 'Create Window',
        handler : function(){
            var win = new Ext.Window({
                title:'Window '+(++windowIndex),
                //autoScroll:true,
                width:550,
                height:300,
                //html : '<div style="width:100%;height:100%;overflow:auto;"></div>'
                contentEl: Ext.getDom('some-content').cloneNode(true),
                iconCls: 'plugin'
            });

            win.render('desktop');

            win.show(this.el);
        }
    });


    var p = new Ext.Panel({
        frame:true,
        width:400,
        collapsible:true,
        title:"Static Framed Panel",
        html: Ext.getDom('some-content').innerHTML // add some existing markup
    });
    //p.render(document.body);


    var p2 = new Ext.Window({
                title:'Framed Floating Panel w/ Ajax, Scrolling, Toolbar',
                autoScroll:true,
                width:550,
                height:300,
                tbar:true,
                bbar:true,
                items:[p, p.cloneConfig(), p.cloneConfig()]
            });

            p2.render('desktop');



            // stick toolbar in the top bar
            var tb = new Ext.Toolbar(p2.tbar, [{
                text:'Load Content',
                handler : function(){
                    p2.load({url:'../tabs/ajax1.htm'});
                }
            }]);


            // stick toolbar in the top bar
            var tb2 = new Ext.Toolbar(p2.bbar, [{
                text:'Do Something',
                handler : function(){
                    Ext.Msg.alert('Yeah', 'The buttons works!');
                }
            }]);

            p2[p2.isVisible() ? 'hide' : 'show'](this.el);

    // let it know about the new toolbar
    p2.syncSize();*/



});