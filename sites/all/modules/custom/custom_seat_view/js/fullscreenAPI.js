(function() {
    var that;
    var
        fullScreenApi = {
            supportsFullScreen: false,
            isFullScreen: function() { return false; },
            requestFullScreen: function() {},
            cancelFullScreen: function() {},
            fullScreenEventName: '',
            prefix: ''
        },
        browserPrefixes = 'webkit moz o ms khtml'.split(' ');
    that = this;
    // check for native support
    if (typeof document.cancelFullScreen != 'undefined') {
        fullScreenApi.supportsFullScreen = true;
    } else {
        // check for fullscreen support by vendor prefix
        for (var i = 0, il = browserPrefixes.length; i < il; i++ ) {
            fullScreenApi.prefix = browserPrefixes[i];
            if (typeof document[fullScreenApi.prefix + 'CancelFullScreen' ] != 'undefined' ) {
                fullScreenApi.supportsFullScreen = true;
                break;
            }
        }
    }
    // update methods to do something useful
    if (fullScreenApi.supportsFullScreen) {
        //fullScreenApi.fullScreenEventName = fullScreenApi.prefix + 'fullscreenchange';
        document.addEventListener("fullscreenchange",function () {
            that.fullscreenHandler((document.fullscreen)? true : false);
        }, false);
        document.addEventListener("mozfullscreenchange",function () {
            that.fullscreenHandler((document.mozFullScreen)? true : false);
        }, false);
        document.addEventListener("webkitfullscreenchange",function () {
            that.fullscreenHandler((document.webkitIsFullScreen)? true : false);
        }, false);
        fullScreenApi.isFullScreen = function() {
            switch (this.prefix) {
                case '':
                    return document.fullScreen;
                case 'webkit':
                    return document.webkitIsFullScreen;
                case 'moz':
                    return document.mozFullScreen;
                default:
                    return document[this.prefix + 'FullScreen'];
            }
        }
        fullScreenApi.requestFullScreen = function(el) {
            return (this.prefix === '') ? el.requestFullScreen() : el[this.prefix + 'RequestFullScreen']();
        }
        fullScreenApi.cancelFullScreen = function(el) {
            return (this.prefix === '') ? document.cancelFullScreen() : document[this.prefix + 'CancelFullScreen']();
        }
    }
    fullscreenHandler = function(_flag){
        if(!_flag){
           // eventDispatcher.trigger(Constants.EVENT_APP_STATE,Constants.APP_STATE_NORMALSCREEN);
        }
    }
    // jQuery plugin
    if (typeof jQuery != 'undefined') {
        jQuery.fn.requestFullScreen = function() {

            return this.each(function() {
                if (fullScreenApi.supportsFullScreen) {
                    fullScreenApi.requestFullScreen(this);
                }
            });
        };
    }
    // export api
    window.fullScreenApi = fullScreenApi;
})();