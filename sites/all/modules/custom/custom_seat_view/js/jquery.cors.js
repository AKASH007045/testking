/**

 */
/*globals XDomainRequest, flensed, XMLHttpRequest, window, jQuery */
(function($){
    if(!window.console){
        window.console = {log: function(){}, error: function(){}, warn: function(){}};
    }
    var supportedMethods = /^(?:post|get)$/i;
    $.support.cors = true;
    // Check to see if the browser supports CORS natively
    if('withCredentials' in new XMLHttpRequest()){
        // It does, do nothing
        return;
    }

    // Attach a prefilter to handle switching transports
    $.ajaxPrefilter(function(options, originalOptions, jqXHR){
        // Both flXHR and XDomainRequest only support async requests
        if(options.crossDomain && options.async && supportedMethods.test(options.type)){
            if(window.XDomainRequest){
                return "__xdomain__";
            }else{
                return "__flxhr__";
            }
        }
    });
    // Setup CORS transports
    if(window.XDomainRequest){

        $.ajaxTransport('__xdomain__', function(options, originalOptions, jqXHR){
            var xdr;
            options.dataTypes.shift();
            // Return the transport object
            return {
                send: function(headers, completeCallback){

                    xdr = new XDomainRequest();
                    xdr.open(options.type, options.url);

                    function completeHandler(status, statusText, responses, responseHeaders){
                        // Clean up the xdr object
                        xdr.onload = xdr.onerror = xdr.ontimeout = function(){};
                        xdr = undefined;
                        // Invoke the callback
                        completeCallback(status, statusText, responses, responseHeaders);
                    }

                    // Request completed handler
                    xdr.onload = function(){
                        completeHandler(200, "OK", {text: xdr.responseText}, "Content-Type: " + xdr.contentType);
                    };

                    // Request error handler
                    xdr.onerror = function(){
                        completeHandler(-1, "Error", {text: xdr.responseText}, "Content-Type: " + xdr.contentType);
                    };

                    // Empty function for on progress event, prevents issue with I9 sometimes failing if no onprogress handler was established
                    xdr.onprogress = function(){};

                    // Timeout handler
                    xdr.ontimeout = function(){
                        completeHandler(0, "Timeout");
                    };

                    if(options.timeout){
                        xdr.timeout = options.timeout;
                    }

                    // Send the request
                    xdr.send((options.hasContent && options.data) || null);
                },

                abort: function(){
                    if(xdr){
                        xdr.onerror = function(){};
                        xdr.abort();
                    }
                }
            };
        });
    }else{
        // Make sure the flXHR js is loaded
        var paths = $('script[src*="jquery.cors.js"]').attr('src').split('/');
        paths.pop();
        // Put a hold on the ready state of the document
        $.holdReady(true);
        window.flensed = {base_path: paths.join('/') + '/'};
        paths.push('flXHR.js');
        $.getScript(paths.join('/'))
            .done(function(script, textStatus, jqXHR){
                $.ajaxTransport('__flxhr__', function(options, originalOptions, jqXHR){
                    var xhr,
                        defaultOptions = {
                            instancePooling: true,
                            autoUpdatePlayer: true
                        };
                    options.dataTypes.shift();
                    return {
                        send: function(headers, complete){
                            var header;
                            xhr = new flensed.flXHR( options || defaultOptions);
                            xhr.open(options.type, options.url, options.async, options.username, options.password);
                            xhr.onreadystatechange = function(status, error){
                                if(error || xhr.readyState === 4){
                                    if(error){
                                        complete(status, error);
                                    }else{
                                        var responses = {},
                                            responseXML = xhr.responseXML;
                                        if ( responseXML && responseXML.documentElement ) {
                                            responses.xml = responseXML;
                                        }
                                        responses.text = xhr.responseText;
                                        complete(xhr.status, xhr.statusText, responses, xhr.getAllResponseHeaders());
                                    }
                                    xhr.onreadystatechange = xhr.onerror = $.noop;
                                    xhr = undefined;
                                }
                            };
                            xhr.onerror = function(error, statusText) {
                                complete(-1, statusText);
                            };
                            for(header in headers){
                                xhr.setRequestHeader(header, headers[header]);
                            }
                            xhr.send((options.hasContent && options.data) || null);
                        },

                        abort: function(){
                            if(xhr){
                                xhr.onerror = $.noop;
                                xhr.abort();
                            }
                        }
                    };
                });
                // Release the hold on the ready state
                $.holdReady(false);
            })
            .fail(function(jqXHR, settings, exception){
                console.log('failed to load script', jqXHR, settings, exception);
                $.holdReady(false);
            });
    }
}(jQuery));