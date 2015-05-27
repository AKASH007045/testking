jQuery(document).ready(function(){  
  if((navigator.userAgent.match(/iPad/i))) {
    jQuery('head').append('<meta name="viewport" content="width=device-width, initial-scale=1.0">');
  }
  /* Disable hover for iPad */
  if(navigator.userAgent.match(/iPad/i) == null) {
    jQuery('body').addClass('noTouch');
  }
});