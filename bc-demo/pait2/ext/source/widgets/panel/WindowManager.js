/*
 * Ext JS Library 1.0.1
 * Copyright(c) 2006-2007, Ext JS, LLC.
 * licensing@extjs.com
 * 
 * http://www.extjs.com/license
 */

Ext.WindowGroup = function(){
    var list = {};
    var accessList = [];
    var front = null;

    // private
    var sortWindows = function(d1, d2){
        return (!d1._lastAccess || d1._lastAccess < d2._lastAccess) ? -1 : 1;
    };

    // private
    var orderWindows = function(){
        accessList.sort(sortWindows);
        var seed = Ext.DialogManager.zseed;
        for(var i = 0, len = accessList.length; i < len; i++){
            var win = accessList[i];
            if(win && !win.hidden){
                win.setZIndex(seed + (i*10));
            }
        }
        activateLast();
    };

    var setActiveWin = function(win){
        if(win != front){
            if(front){
                front.setActive(false);
            }
            front = win;
            if(win){
                win.setActive(true);
            }
        }
    };

    var activateLast = function(){
        for(var i = accessList.length-1; i >=0; --i) {
            if(!accessList[i].hidden){
                setActiveWin(accessList[i]);
                return;
            }
        }
        // none to activate
        setActiveWin(null);
    };

    return {
        /**
         * The starting z-index for windows (defaults to 9000)
         * @type Number The z-index value
         */
        zseed : 9000,

        // private
        register : function(win){
            list[win.id] = win;
            accessList.push(win);
            win.on('hide', activateLast);
        },

        // private
        unregister : function(win){
            delete list[win.id];
            win.un('hide', activateLast);
            accessList.remove(win);
        },

        /**
         * Gets a registered Window by id
         * @param {String/Object} id The id of the Window or a Window
         * @return {Ext.BasicDialog} this
         */
        get : function(id){
            return typeof id == "object" ? id : list[id];
        },

        /**
         * Brings the specified Window to the front
         * @param {String/Object} win The id of the Window or a Window
         * @return {Ext.BasicDialog} this
         */
        bringToFront : function(win){
            win = this.get(win);
            if(win != front){
                win._lastAccess = new Date().getTime();
                orderWindows();
            }
            return win;
        },

        /**
         * Sends the specified Window to the back
         * @param {String/Object} win The id of the Window or a Window
         * @return {Ext.BasicDialog} this
         */
        sendToBack : function(win){
            win = this.get(win);
            win._lastAccess = -(new Date().getTime());
            orderWindows();
            return win;
        },

        /**
         * Hides all Windows
         */
        hideAll : function(){
            for(var id in list){
                if(list[id] && typeof list[id] != "function" && list[id].isVisible()){
                    list[id].hide();
                }
            }
        },



        getBy : function(fn, scope){
            var r = [];
            for(var i = accessList.length-1; i >=0; --i) {
                var win = accessList[i];
                if(fn.call(scope||win, win) !== false){
                    r.push(win);
                }
            }
            return r;
        }
    };
};


/**
 * @class Ext.WindowManager
 * @extends Ext.WindowGroup
 * The default global group of windows.
 * @singleton
 */
Ext.WindowMgr = new Ext.WindowGroup();