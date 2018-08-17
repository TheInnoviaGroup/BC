/*
 * Ext JS Library 1.0.1
 * Copyright(c) 2006-2007, Ext JS, LLC.
 * licensing@extjs.com
 * 
 * http://www.extjs.com/license
 */


xtrack.TicketGrid = function(ct){
    var xg = Ext.grid;

    // renderer functions
    function listRenderer(list){
        return function(v){
            return list[v].text;
        };
    }

    function summary(value){
        return String.format('<b><i>{0}</i></b>', value);
    }

    function summaryLong(value, p, record){
        return String.format('<b>{0}</b>{1}', value, record.data['excerpt']);
    }

    var bogus = 1000;

    var store = new Ext.data.Store({
        proxy: new Ext.data.HttpProxy({
            url: '/xbug/ticket-service.php'
        }),

        reader: new Ext.data.JsonReader({
            root: 'tickets',
            totalProperty: 'totalCount',
            id: 'bug_id'
        }, [
            {name: 'ticket', mapping: 'ticket_id', convert: function(){return ++bogus;}},
            {name: 'summary', convert: function(){return 'This is test ticket ' + bogus;}},
            {name: 'excerpt', mapping: 'description'},
            {name: 'owner'},
            {name: 'date', mapping: 'report_date', type: 'date', dateFormat: 'Y-m-d'},
            {name: 'status'},
            {name: 'priority'},
            {name: 'severity'}
        ]),

        // turn on remote sorting
        remoteSort: true
    });
    store.setDefaultSort('date', 'desc');


    var cm = new Ext.grid.ColumnModel([{
           id: 'ticket',
           header: "Ticket",
           dataIndex: 'ticket',
           width: 60
        },{
           id: 'summary',
           header: "Summary",
           dataIndex: 'summary',
           width: 300,
           renderer: summaryLong,
           css: 'white-space:normal;'
        },{
           header: 'Status',
           dataIndex: 'status',
           width: 75,
           renderer: listRenderer(xtrack.config.status)
        },{
           header: 'Priority',
           dataIndex: 'priority',
           width: 75,
           renderer: listRenderer(xtrack.config.priorities)
        },{
           header: 'Severity',
           dataIndex: 'severity',
           width: 75,
           renderer: listRenderer(xtrack.config.severities)
        },{
           header: "Reported",
           dataIndex: 'date',
           width: 80,
           renderer: Ext.util.Format.dateRenderer('M j, Y')
        }]);

    // by default columns are sortable
    cm.defaultSortable = true;

    var sm = new xg.RowSelectionModel({multiSelect:true});

    var grid = new xg.Grid(ct, {
        ds: store,
        cm: cm,
        selModel: sm,
        enableColLock:false,
        autoExpandColumn: 'summary',
        autoExpandMin: 200,
        trackMouseOver:false,
        ddGroup:'WatchDD'
    });

    grid.getDragDropText = function(){
        var s = sm.getSelections();
        if(s.length == 1){
            return String.format('<b>Drag Ticket #{0} to your Watch List<b>', s[0].data.ticket);
        }else{
            var tickets = [];
            for(var i = 0, len = s.length; i < len; i++){
                tickets.push('#' + s[i].data.ticket);
            }
            return String.format(
                    '<b>Drag the {0} selected tickets to your Watch List</b><br />{1}',
                    s.length, tickets.join(', '));
        }
    };

    this.getPanel = function(){
        return new Ext.GridPanel(grid);
    };

    this.getHeader = function(){
        return grid.getView().getHeaderPanel(true);
    };

    this.load = function(pid, filter){
        store.baseParams = {
            'pid' : pid,
            'filter' : filter ? filter : ''
        };

        store.load({params:{start:0, limit:20}});
    };

    this.render = function(){
        grid.render();

        var paging = new Ext.PagingToolbar(
            grid.getView().getFooterPanel(true),
            store, {
                pageSize: 20,
                displayInfo: true,
                displayMsg: 'Displaying tickets {0} - {1} of {2}',
                emptyMsg: "No tickets to display"
            }
        );
    };

    this.toggleDetails = function(pressed){
        cm.getColumnById('summary').renderer =
                pressed ? summaryLong : summary;
        grid.getView().refresh();
    };

    this.grid = grid;
};



Ext.onReady(function(){

    var windowIndex = 0;

    var activeWindow = null;
    var windows = new Ext.WindowGroup();
    //var winbar = new Ext.Toolbar('winbar');

    function createWin(title){
        var win = new Ext.Window({
            title:title,
            width:640,
            height:480,
            x:100,
            y:100,
            html : '<p>Something useful would be in here.</p>',
            iconCls: 'plugin',
            manager: windows,
            shim:false,
            animCollapse:false,
            maximized:true,
            tbar:true
        });
        win.render('desktop');

        if(title == 'Browse Tickets'){
            var g = new xtrack.TicketGrid(win.body);
            g.render();
            g.load(1000);
            win.on('resize', g.grid.autoSize, g.grid);        
            var tb = new Ext.Toolbar(win.tbar);
            tb.add({
                text: 'New Ticket',
                cls: 'x-btn-text-icon new-ticket',
                handler: function(){
                    xtrack.TicketPanel.layout = layout;
                    r.center.add(xtrack.TicketPanel.getInstance());
                    r.center.add(new Ext.ContentPanel({autoCreate:true, closable:true, title: "Get Premium Support"}))
                }
            },'-', {
                pressed: false,
                enableToggle:true,
                text: 'Preview Panel',
                cls: 'x-btn-text-icon preview',
                toggleHandler: function(btn, pressed){
                    r.preview[pressed ? 'show' : 'hide']();
                    enablePreview = pressed;
                }
            }, ' ',{
                pressed: true,
                enableToggle:true,
                text: 'Detailed View',
                cls: 'x-btn-text-icon details',
                toggleHandler: function(btn, pressed){
                    grid.toggleDetails(pressed);
                }
            });
        }

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
    var wbar = Ext.get('windows');
    
    function layoutDesktop(){
        desktop.setHeight(Ext.lib.Dom.getViewHeight()-taskbar.getHeight()-wbar.getHeight());
    }

    Ext.EventManager.onWindowResize(layoutDesktop);

    var qlaunch = new Ext.Toolbar('qlaunch');
    qlaunch.add({
        text:'XTrack Home',
        handler: function(){
            createWin('XTrack Home');
        }
    },{
        text:'Customer Accounts',
        clickEvent:'mousedown',
        menu : {
            items:[{
                text:'Manage Customers',
                handler: function(){
                    createWin('Manage Customers');
                }
            },{
                text:'Customer Reports',
                handler: function(){
                    createWin('Customer Reports');
                }
            }]
        }
    },new Ext.Toolbar.MenuButton({
        text:'Support Tickets',
        clickEvent:'mousedown',
        handler: function(){
            createWin('Browse Tickets');
        },
        menu : {
            items:[{
                text:'Browse Tickets',
                handler: function(){
                    createWin('Browse Tickets');
                }
            },{
                text:'Create Ticket',
                handler: function(){
                    createWin('Create Ticket');
                }
            }]
        }
    }),{
        text:'Preferences',
        handler: function(){
            createWin('Preferences');
        }
    });

    new Ext.Panel({
        frame:true,
        collapsible:true,
        title:"Extlet : Quotes",
        html: '<p>Ext\'s UpdateManager provides the speed and reliability that is needed to keep indycar.com Live Timing and Scoring data up-to-date when the cars are running 220+ mph around the track. <br/>-- Jon Whitcraft (indycar.com)</p>',
        renderTo:'dpanels',
        cls:'x-panel-blue'
    });

    var p = new Ext.Panel({
        id:'cal-widget',
        frame:true,
        collapsible:true,
        title:"Extlet : Calendar",
        renderTo:'dpanels',
        cls:'x-panel-blue',
        items: new Ext.DatePicker()
    });
    p.body.dom.firstChild.firstChild.style.width ='100%';

    layoutDesktop();


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