function isiDevice(){
  var isiPad = navigator.userAgent.match(/iPad/i) != null;
  var isiPhone = navigator.platform.indexOf("iPhone") != -1;
  var isiPod = navigator.platform.indexOf("iPod") != -1;
  if(isiPad || isiPhone || isiPod){
    return true;
  }
  else{
    return false;
  }
}

function getUrlParams(){
  var page = location.search.match(new RegExp('t' + "=(.*?)($|\&)", "i"));
  var mp_page = '';
  if(page) { mp_page = page[1] || []; } else { mp_page = false; }
  return mp_page;
}

function add_custom_theme_radio_loginform() {
  jQuery("form#user-login .form-item-remember-me input.form-checkbox").wrap('<span></span>');
  jQuery("form#user-login .form-item-remember-me span").click(function(event) {
    event.preventDefault();
    jQuery(this).toggleClass("active"); 
    if (jQuery(this).hasClass('active')){
      jQuery(this).find('input').attr("checked", false);
    }
    else {
      jQuery(this).find('input').attr("checked", true);
    }
  });
}
/******************************** For Spin JS Global Setting *****************************/
/*************************  For ticket STPDEV-704,STPDEV-582,STPDEV-934,STPDEV-936  ******/
var opts = {
                        lines: 12,
                        length: 6,
                        width: 2.5,
                        radius: 5,
                        color: '#fff',
                        speed: 0.6,
                        trail: 60,
                        shadow: false,
                        hwaccel: false,                       
                        zIndex: 2e9,
                        top: 'auto',
                        left: 'auto'}; 
jQuery.fn.spin = function(opts) 
{
    this.each(function() 
    {    
        var $this = jQuery(this),
        spinner = $this.data('spinner');
        if(spinner) spinner.stop();
        if(opts !== false) 
        {
            opts = jQuery.extend({color: $this.css('color')}, opts);
            spinner = new Spinner(opts).spin(this);
            $this.data('spinner', spinner);
        }
    });
    return this;
};
jQuery(function() 
{ 
    jQuery(".vjs-loading-spinner").html('<div class="video_ldr video_ldr_color" style="position: absolute;z-index: 99999;left: 50%;top: 50%;"></div>');
    jQuery(".video_ldr").spin(opts);//For Video Loader
    
    jQuery('#gallery-slider a, .event-slider-box a').click(function() 
    {
        jQuery(".carousel-inner .field-content").append('<div class="js_spin_loader"></div>');
        jQuery(".js_spin_loader").spin(opts);// For STH Page loader
    });
    jQuery('.btn-pay').on('click',function(e) 
    {
            jQuery(".btn-pay").before('<div class="paymnt_loader" style="margin-top:16px;padding-right:18px;float:left;"></div>');
            opts['color']= '#000';
            jQuery(".paymnt_loader").spin(opts);
            loaderHide = true;
    });// For Payment Button click loader 
    jQuery('a,input').on('click',function(e) 
    {
       if(userRole !='editor')
       {
            return true;
       }
        var blockedTile = new Array("NULL", "#", "javascript:void(0);","admin","node");
        var hrefs = '';
        var sbmts = '';
        var dataTarget = '';
        var dataSlide = '';
        var loaderHide = false;
        var dataTilt = '';
        var target = '';
        hrefs = jQuery(this).attr('href');
        target = jQuery(this).attr('target');
        sbmts = jQuery(this).attr('type');
        dataTarget = jQuery(this).attr('data-target');
        dataSlide = jQuery(this).attr('data-slide');
        dataTilt = jQuery(this).attr('data-tilt');
  
        if(jQuery(this).hasClass("bg-page-link-edit") || jQuery(this).parent('div').hasClass("event-link-edit") || jQuery(this).parent('li').hasClass("admin-menu-toolbar-category") || jQuery(this).parent('li').hasClass("edit"))
        { 
            loaderHide = true;
        }else
        {
            jQuery.each(blockedTile, function(index,value) 
            {
                if(hrefs.toLowerCase().indexOf(value) >= 0)
                {
                    loaderHide = true; 
                }
            });
        }
        if((target !=='_blank' &&  dataTarget !=='#' && dataSlide != 'next' && dataSlide != 'prev' && loaderHide == false && typeof dataTilt ==='undefined') || sbmts === 'submit')
        {
            jQuery("body").append('<div class="js_spin_loader" style="position:fixed"></div>');
            jQuery(".js_spin_loader").spin(opts);
        }else if(hrefs.toLowerCase().indexOf("#") >= 0)
        {        
            jQuery(".js_spin_loader").hide();
        }
    });
    jQuery(document).ajaxSuccess(function( event, xhr, settings )
    {
        jQuery(".js_spin_loader").hide();
    });// Resetting loader Globally for Ajax Success
    jQuery(".js_spin_loader").hide();
});
jQuery(document).bind('drupalOverlayBeforeLoad', function(event) 
{
    jQuery("body").append('<div class="js_spin_loader" style="position:fixed"></div>');
    jQuery(".js_spin_loader").spin(opts); 
    
    if(jQuery('.overlay-active').contents().length < 1)
    {
        var refreshId  = setInterval(function () 
        {
               if(jQuery('.overlay-active').contents().length > 0)
               {
                    jQuery(".js_spin_loader").remove();
                    clearInterval(refreshId);
               }
        }, 1000);
    }
    jQuery('.overlay-active').load(function()
    {
        jQuery(".js_spin_loader").remove();       
    });
});
/*************************  End For ticket STPDEV-704,STPDEV-582,STPDEV-934,STPDEV-936  ******/
/******************************** End For Spin JS Global Setting *****************************/


/***************                  ***********************/