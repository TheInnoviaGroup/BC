Ext.namespace('Ext.ux.TemplateSet');

Ext.ux.TemplateSet = function() {
    this.addEvents({
	    "preprocess" : true,
            "invaliddata" : true
            });
    Ext.ux.TemplateSet.superclass.constructor.call(this, {});
};

Ext.extend(Ext.ux.TemplateSet, Ext.util.Observable, {
        templates: []
        ,compiled: false
	,addTemplate: function(templateName, templateHTML) {
            this.templates[templateName] = new Ext.Template(templateHTML);
            if (this.compiled) {
                this.templates[templateName].compile();
            }
        }
        ,compile: function() {
            this.compiled = true;
	    console.log('This: %o', this);
            for (templateName in this.templates) {
		if (templateName != 'remove') {
		console.log('templateName: %o', templateName);
                this.templates[templateName].compile();
		}
            }
        }
        ,getConfig: function(r) {
            this.fireEvent('preprocess', r);
            if (r.templateName) {
                return r;
            } else {
                var resp = Ext.util.JSON.decode(r.responseText);
                if (resp.templateName) {
                    return resp;
                } else {
                    this.fireEvent('invaliddata', r);
                    return false;
                }
            }
        }
        ,append: function(e, r) {
            var resp = this.getConfig(r);
            if (resp) {
                this.templates[resp.templateName].append(e, resp.templateData);
            }
        }
        ,apply: function(e, r) {
            var resp = this.getConfig(r);
            if (resp) {
                this.templates[resp.templateName].apply(e, resp.templateData);
            }
        }
        ,insertAfter: function(e, r) {
            var resp = this.getConfig(r);
            if (resp) {
                this.templates[resp.templateName].insertAfter(e, resp.templateData);
            }
        }
        ,insertBefore: function(e, r) {
            var resp = this.getConfig(r);
            if (resp) {
                this.templates[resp.templateName].insertBefore(e, resp.templateData);
            }
        }
        ,insertFirst: function(e, r) {
            var resp = this.getConfig(r);
            if (resp) {
                this.templates[resp.templateName].insertAfter(e, resp.templateData);
            }
        }
        ,overwrite: function(e, r) {
            var resp = this.getConfig(r);
            if (resp) {
                this.templates[resp.templateName].overwrite(e, resp.templateData);
            }
        }

});