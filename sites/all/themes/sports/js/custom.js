var ttextCounter = false;
var ttextCounter1 = true;
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
jQuery(document).ready(function(){
  var browser = new cBrowser();
  
  /* Promo tiles disable*/
    jQuery('.page-goto-renew .promo-box').find('a.promo-link').click(function(e){
        e.preventDefault();
    });
    //First remove all the existing click handlers
    jQuery('.page-goto-renew .promo-box,.footer-copyright p.copyright').find('a').off('click');
    //then disable the default click behavoiour on anchors
    jQuery('.page-goto-renew .promo-box,.footer-copyright p.copyright').find('a').click(function(e){
        //alert("hello");
        e.preventDefault();
        //
    });
    /* Promo tiles disable*/
	
  //Video Message 
  /* if need to be show only for logged in user once
  if(Drupal.settings.custom_hooks.custom_name){
    var show_splash = jQuery.cookie('show_splash');
    if(!show_splash){ 
      jQuery("#videobutton").trigger('click');
      jQuery.cookie('show_splash', '1');
    }    
  }
  else{
    if(jQuery.cookie('show_splash')){
      jQuery.removeCookie('show_splash');
    }
  }*/
  
  if(!Drupal.settings.custom_hooks.custom_name){
    var show_splash = jQuery.cookie('show_splash');
    if(!show_splash){ 
      jQuery("#videobutton").trigger('click');
      jQuery.cookie('show_splash', '1');
    }    
  }
  jQuery('#myModal').on('hidden.bs.modal', function (e) {
    var attr_id = jQuery("video").attr("id");
    videojs(attr_id).ready(function(){
      var myPlayer = this;
      myPlayer.pause();  
    });
  });
  //Video Js fix for IE
  if(jQuery("video").length>0){
    var attr_id = jQuery("video").attr("id");
    var player_html = jQuery("#"+attr_id).clone().wrap('<div/>').parent().html();
    var pid = jQuery("#"+attr_id).parents('div').attr('id');
    var c=0;    
    videojs(attr_id).ready(function(){
      var myPlayer = this;
      
      if( browser.isIE ){  
        myPlayer.on('ended', function(){
          jQuery("#node-110 div.video-js div.vjs-control-bar div.vjs-play-control").trigger('click');
        });
      }
    });   
  }
  
  
  
  var tab_page = '';//getUrlParams();
    if (tab_page == 'yes') {
      //jQuery('.node-type-tab-page .view-tab-pages li:last-child').find('a').click();
    }
    else if(tab_page == 'edit'){
      //alert(tab_page);
    }
  var url = document.location.toString();
  if (url.match('#')) {
    // Clear active item
    jQuery('.carousel-inner div').removeClass('active');
    // Activate item number #hash
    var index = url.split('#')[1];
    jQuery('#node-'+index).parent().addClass('active');
    
  }
  
  var querystring = window.location.search;
  if(querystring == '?edit=true'){
    setTimeout(function(){
            jQuery('.region-content ul.tabs-primary-live li.edit-link').click();
    }, 2000);  
  }
  
//  if (url.match('?')) {
//    var index = url.split('?')[1];    
//    setTimeout(function(){
//        if(index == 'edit=true'){
//            jQuery('.region-content ul.tabs-primary-live li.edit-link').click();
//        }
//    }, 2000);
//  }
  
  
  

  jQuery('#views-bootstrap-carousel-1 .carousel-control').bind( 'click', function(e) {
    // Update location based on slide (index is 0-based)
    setTimeout(function(){
      //var item = jQuery('#views-bootstrap-carousel-1 .carousel-inner .item.active');
      //window.location.hash = "#"+parseInt(item.index() + 1);
      var a = jQuery('#views-bootstrap-carousel-1 .carousel-inner .active div').first();
      var id = a[0].id;
      var path = id.replace("node-", "");
      window.location.hash = "#"+path;
    }, 1000);
  })
  

  
  // view take over page- on-off status (only update content) 
  jQuery('.view-take-over-pages .views-field-status').click(function(){
      if(jQuery(this).hasClass('on')){
          jQuery(this).addClass('off');
          jQuery(this).text('0');
          jQuery(this).removeClass('on');
      }
      else if(jQuery(this).hasClass('off')){
          jQuery(this).addClass('on');
          jQuery(this).text('1');
          jQuery(this).removeClass('off');
      }
  })
  
	// view take over page, All URL-Alias Form update (hide)
	jQuery('.custom-form-tk-node').hide();
	//views-field-nid
	jQuery('#views-form-take-over-pages-page .views-field-nid').hide();
	//jQuery('#views-form-take-over-pages-page .views-field-view-node').hide();
    
// View take over page, on submit, node publish-unublish "save"
jQuery('#views-form-take-over-pages-page .btn-success').click(function() {
    if(jQuery(this).hasClass('custom-to-page-form') == false) {
        var nds =  new Array();
        jQuery('td.views-field-status.off').each(function(){
            nds.push(jQuery(this).closest('tr').attr('id'));
        });
            jQuery.ajax({
                type: "POST",
                url: Drupal.settings.basePath+"admin/post/take-over/ajax",
                data: {name: nds},
                success:function(data) {
                        jQuery('#views-form-take-over-pages-page').submit();
                    }                
                
            })
         return false;        
    }

});

  
  
    jQuery('.take-over-pages-form').hide();
    jQuery('.take-over-pages-form .vertical-tabs').hide();
    jQuery('.to-bg-img').hide();
    
    jQuery('.custom-to-page-nid').hide();

//    jQuery('td.to-page span').click(function(){
//            var data_id = jQuery(this).attr('id');
//            if(jQuery(this).hasClass('in-active')){
//                jQuery('.custom-form-tk-node-'+data_id).show();
//                jQuery(this).addClas.s('active');
//                jQuery(this).removeClass('in-active');
//            }                  
//            else if(jQuery(this).hasClass('active')){
//                jQuery('.custom-form-tk-node-'+data_id).hide();
//                jQuery(this).addClass('in-active');
//                jQuery(this).removeClass('active');
//            }     
//  })
  
    
    //start, Take over page-INLINE edit form
    jQuery('.take-over-pages-link-edit').click(function() {
        var jQuerythis = jQuery(this)
        jQuerythis.next('div.take-over-pages-form').show();
    });
    jQuery('.take-over-pages-form .popup-close').click(function() {
        // var r = confirm("Press a button!");
      alertifyreset ();
       alertify.confirm("You have unsaved changes. Do you want to discard the changes?", function (e) {
            if (e) {
               var wraper = jQuery(this).parents('div.take-over-pages-form');
            wraper.hide();
            } else {  return;}
        });
        //if (confirm("You have unsaved changes. Do you want to discard the changes?") == true) {
        //    var wraper = jQuery(this).parents('div.take-over-pages-form');
        //    wraper.hide();
        //} else {
        //    return;
        //}

    });
    // end, take over page inlineedit popup


    //start put BG-img in backgroup
    jQuery('.to-bg-img').find('img').hide();
    jQuery('.to-bg-img').parent().css('background-image', 'url(' + jQuery('.to-bg-img').find('img').attr('src') + ')');
    //end put BG-img in backgroup  
    
    jQuery('#block-custom-block-admin-home-link li.edit a').click(function(e) {
    e.preventDefault();
    if (ttextCounter == true) {
       alertifyreset ();
       alertify.confirm("You have unsaved changes. Do you want to discard the changes?", function (e) {
            if (e) {
              ttextCounter1 = false;
              location.reload(true);
            } else { ttextCounter1 = true; }
        });
        //if (confirm("You have unsaved changes. Do you want to discard the changes?") == true) {
        //    ttextCounter1 = false;
        //    location.reload();
        //}
         return;
     }
    else {
        jQuery('div.tile-image-form').css('visibility','hidden');

    }
    if (jQuery('.view-home .contextual-links li.quick-quickedit a').length >0 ) {
      jQuery('#block-custom-block-admin-home-link li.view a').removeClass('active');
      jQuery('#block-custom-block-admin-home-link li.edit a').addClass('active');
      jQuery('.view-home .contextual-links li.quick-quickedit a').click();
    }
     return false;
 });
        
    jQuery('.region-content ul.tabs-primary-live li.edit-link').click(function() {
       var curr_data_id = jQuery(this).attr('data-id');
    if (jQuery('#' + curr_data_id + ' .contextual-links li.quick-quickedit a').length >0 ) {
      jQuery('.main-wrap ul.tabs-primary-live li.node-view a').removeClass('active');
      jQuery('.main-wrap ul.tabs-primary-live li.edit-link a').addClass('active');
      jQuery('#' + curr_data_id + ' .contextual-links li.quick-quickedit a').click();
    }
     return false;
 });
    
    
    /* Promot tiles disable*/
    jQuery('.page-pawp-nojs-form .promo-box, .page-node-116 .promo-box').find('a.promo-link').click(function(e){
        e.preventDefault();
    });
    //First remove all the existing click handlers
    jQuery('.page-pawp-nojs-form .promo-box, .page-node-116 .promo-box').find('a').off('click');
    //then disable the default click behavoiour on anchors
    jQuery('.page-pawp-nojs-form .promo-box, .page-node-116 .promo-box').find('a').click(function(e){
        //alert("hello");
        e.preventDefault();
        //
    });
    /* Promot tiles disable*/
    
    
    /* Disable hover for iPad */
    if(navigator.userAgent.match(/iPad/i) == null) {
            jQuery('body').addClass('noTouch');
        }

    /* Event Gallery: Hide arrows for single slide*/
    jQuery('.carousel-inner').each(function() {
        if (jQuery(this).children('div').length === 1){
			jQuery(this).siblings('.carousel-control, .carousel-indicators').hide();	
		}else{
			jQuery(this).siblings('.carousel-control, .carousel-indicators').show();
		}
    });
  
  
  
  

  jQuery('#custom-pawp-tmapi-request-form, #custom-invoice-payment-request, #webform-client-form-116').find('.form-select').Selectyze({
    theme : 'css3'
  });
  
  if(browser.isIE && browser.isIE8){
    window.location.href = "/templateIe8Alert.html";
  }  
  
  /* Sign in page */
  if(jQuery('body.page-user-login div#inline-messages').length>0){
    var str = jQuery('body.page-user-login div#inline-messages').clone().wrap('<div/>').parent().html();
    jQuery('body.page-user-login div#inline-messages').remove();
    jQuery('body.page-user-login form#user-login div.form-type-password').after(str);
    jQuery('body.page-user-login form#user-login').find('div#inline-messages').show();
  }
  
  /*Navigation fixes*/
  function set_active(main_nav_link, sub_nav_text){
    jQuery("ul.menu > li > a").each(function(){
      //This condition will filter out My home game and events and Full schedule
      // from calendar page
      if(!jQuery(this).parents("ul.menu").hasClass("calendar-legend-right")){
        var attrstr = jQuery(this).attr('href');
        
        if(attrstr.indexOf(main_nav_link)>-1){ 
          jQuery(this).addClass('active-trail active').parent().addClass('active-trail active');
          var context = jQuery(this).parent();
          
          if(sub_nav_text){
            jQuery("ul.dropdown-menu > li > a", context).each(function(){
              var subattr = jQuery(this).text();
              subattr = subattr.toLowerCase(); 
              if(subattr==sub_nav_text){ 
                jQuery(this).addClass('active-trail active').parent().addClass('active-trail active');
              }          
            });
          }
        }
      }
    }); 
  }
  var host_url = window.location.href;
  var user_name = Drupal.settings.custom_hooks.custom_name; 
   
  if(host_url.indexOf("full-calendar") > -1){ 
    set_active('/my-calendar', 'calendar');
  }
  else if(host_url.indexOf("my-calendar") > -1){ 
    set_active('/my-calendar', 'calendar');
  }
  else if(host_url.indexOf("season-schedule") > -1){ 
    if(user_name){
      set_active('/my-calendar', 'calendar');
    }
    else{
      set_active('/calendar', 'calendar');
    }
    
  }
  else if(host_url.indexOf("event") > -1){ 
    if(user_name){
      set_active('/my-calendar', 'upcoming events');
    }
    else{
      set_active('/calendar', 'events');
    }
  }
  else{
    jQuery("ul.menu > li.active-trail").addClass('active');
  }
  //TODO
  //Replace with dynamic values 
  
  var clr = "#00479e";
  var bimg = "linear-gradient(to bottom, #00479e, #001938)";
  var bdr = "1px solid #002e66";
  var bxshd = "1px 1px 1px #1c6ccb inset";  
 //TODO
  //Replace with dynamic values
  //var clr = "#f00000";
  //var bimg = "linear-gradient(rgb(153, 8, 8) 0%, rgb(255, 107, 107) 1%, rgb(196, 13, 13) 3%, rgb(127, 1, 1) 100%)";
  //var bdr = "1px solid rgb(161, 18, 18)";
  //var bxshd = "rgb(240, 0, 0) 1px 1px 1px 0px inset";  

  if (navigator.userAgent.indexOf('Safari') != -1 && navigator.userAgent.indexOf('Chrome') == -1) {
    jQuery('div.row-header ul.menu > li > a').mouseover(function(){ 
      jQuery(this)
      .css('backgroundImage', bimg)
      .css('backgroundColor', clr)
      .css('border', bdr)
      .css('boxShadow', bxshd)
      .parents("li").addClass("active open");
    }).mouseout(function(){
      var isHovered = (jQuery(this).parents("li").find('ul.dropdown-menu:hover').length > 0) ? 1 : 0;
      if(!isHovered){
        if(!jQuery(this).parents("li").hasClass("active-trail")){
          jQuery(this)
          .css('backgroundImage', 'none')
          .css('backgroundColor', 'transparent')
          .css('border', '1px solid transparent')
          .css('boxShadow', 'none')
          .blur()
          .parents("li").removeClass("active open");
        }
      }
    });
  }
   
    jQuery('div.row-header ul.menu ul.dropdown-menu').prev("a").mouseover(function(){
      jQuery(this)
      .css('backgroundImage', bimg)
      .css('backgroundColor', clr)
      .css('border', bdr)
      .css('boxShadow', bxshd)
      .parents("li").addClass("active open");
      
      jQuery(this).parents("li").find('ul.dropdown-menu').show();
    }).mouseout(function(){
      var isHovered = (jQuery(this).parents("li").find('ul.dropdown-menu:hover').length > 0) ? 1 : 0;
      if(!isHovered){
        if(!jQuery(this).parents("li").hasClass("active-trail")){
          jQuery(this)
          .css('backgroundImage', 'none')
          .css('backgroundColor', 'transparent')
          .css('border', '1px solid transparent')
          .css('boxShadow', 'none')
      .blur()
          .parents("li").removeClass("active open");
        }
        
        jQuery(this).parents("li").find('ul.dropdown-menu').hide();
      }
    });  
    
    jQuery('div.row-header ul.menu ul.dropdown-menu').mouseover(function(){
      jQuery(this).parents("li").find('ul.dropdown-menu').show();
      jQuery(this).parents("li").find("a.dropdown-toggle")
      .css('backgroundImage', bimg)
      .css('backgroundColor', clr)
      .css('border', bdr)
      .css('boxShadow', bxshd)
      .parents("li").addClass("active open");
    }).mouseout(function(){
      if(!jQuery(this).parents("li").hasClass("active-trail")){
        jQuery(this).parents("li").find("a.dropdown-toggle")
        .css('backgroundImage', 'none')
        .css('backgroundColor', 'transparent')
        .css('border', '1px solid transparent')
        .css('boxShadow', 'none')
        .parents("li").removeClass("active open");
      }
      
      jQuery(this).parents("li").find('ul.dropdown-menu').hide();  
    });
  
  /*End of Navigation fixes*/
  /*End of Navigation fixes*/
  
  // Home page Promo
//  jQuery('.noTouch a.promo-link')
//    .mouseover(function() {
//    jQuery(this) .find('.first').hide();
//    jQuery(this) .find('.second').show();
//  })
//  .mouseout(function() {
//	jQuery(this) .find('.first').show();
//	jQuery(this) .find('.second').hide();
//  });
  jQuery("a.event-calender-box").click(function(){
	var cal_id = jQuery(this).attr('href');
    jQuery('#cal-event-16').modal('show');
  });

  /* LOG IN FORM RADIO BUTTON */
  jQuery(".form#user-login .form-item-remember-me > span").show();
  add_custom_theme_radio_loginform();
  function add_custom_theme_radio_loginform() {
	jQuery("form#user-login .form-item-remember-me input.form-checkbox").wrap('<span></span>');
	jQuery("form#user-login .form-item-remember-me span").click(function() {
	  jQuery(this).toggleClass("active");
	  if (jQuery(this).hasClass('active')){
		jQuery(this).find('input').attr("checked", false);
	  }
	  else {
		jQuery(this).find('input').attr("checked", true);
	  }
	});
  }
  jQuery('.parking-promo-ctool-btn').click();
  var user_name = '';//Drupal.settings.custom_hooks.custom_name;
  var windowWidth = jQuery(window).width()
  jQuery(".view-banner .carousel-inner img").width(windowWidth);
	    /* SEARCH BLOCK */
  jQuery( document ).ajaxComplete(function(event, request, settings) {
    ttextCounter1 = false;
 //   console.log(request);
  //  console.log(settings);
    //var myObj = JSON.parse(request.responseText);
    //var url = settings.url;
    //if (url == '/quickedit/entity/node/461' || url == '/quickedit/entity/node/732') {
    //  location.reload(true);
    //}
    if(jQuery('#modalContent').find('div.error').length>0) {
     // _gaq.push(['_trackEvent', 'Payment Popup', 'Validation Failed', user_name, 1, true]);
    }
    jQuery('#custom-pawp-tmapi-request-form').find('.form-select').Selectyze({
     theme : 'css3'
    });
    jQuery('#webform-client-form-116').find('.form-select').Selectyze({
     theme : 'css3'
    });
    // Calendar event click
   jQuery('div.month-view table > tbody > tr > td a.inline').click(function(){
    var title = jQuery(this).attr('ga-title');
     //_gaq.push(['_trackEvent', 'Calendar Popup', 'Game Event', title, 1, true]);
  });
   // Calendar STH event click
   jQuery('.sth-calendar-event a.inline').click(function(){
    var title = jQuery(this).attr('ga-title');
     //_gaq.push(['_trackEvent', 'Calendar Popup', 'STH Event', title, 1, true]);
  });
   });
  jQuery("#user-login-form .form-type-checkbox label").addClass("active");
  jQuery("#user-login-form .form-type-checkbox label").click(function(){
    if(jQuery(this).hasClass("active")){
      jQuery(this).removeClass("active");
    }
    else{
      jQuery(this).addClass("active");
     }
  });
  // add class on left menu for separtor
  jQuery(".left_menu .navbar-nav li a").addClass("pipe_mark");
  jQuery(".left_menu .navbar-nav li a").hover(function(){
		  jQuery(".left_menu .navbar-nav li a").removeClass("pipe_mark");
  });

  jQuery(".left_menu .navbar-nav li a").mouseout(function(){
		  jQuery(".left_menu .navbar-nav li a").addClass("pipe_mark");
  });
	
  // GA TRACKING
  // Social Feed Links
  jQuery("#block-custom-social-feed-custom-social-feed a.social").click(function() {
    var title = jQuery(this).attr('id');
    //_gaq.push(['_trackEvent', 'SocialMedia', 'Click', title, 1, true]);
  });
  //Login form forgot Password
  jQuery("#user-login-form a.forgot-pass").click(function() {
    var title = jQuery(this).attr('data-title');
    //_gaq.push(['_trackEvent', 'Forgot Password', 'Click', title, 1, true]);
  });
  jQuery(".region-footer a").click(function() {
    var title = jQuery(this).attr('data-title');
    //_gaq.push(['_trackEvent', 'Exit Links', 'Exit Site', title, 1, true]);
  });
  jQuery(".view-acct-reps .slider-wrap-detail a.rep-mail, #block-custom-block-customer-acct-rep-right a.rep-mail").click(function() {
	     var title = jQuery(this).attr('data-title');
    //_gaq.push(['_trackEvent', 'Service Coordinator', 'Email', title, 1, true]);
  });
  if (user_name) {
     //_gaq.push(['_setCustomVar', 5, 'archticsID', user_name, 2]);
  }
  jQuery(".page-invoice a.invoice-link").click(function() {
	    var title = jQuery(this).attr('data-title');
    if (user_name) {
	  //_gaq.push(['_trackEvent', 'Invoice', title, user_name, 1, true]);
    }
  });
  // User page View Tracking of the website as per client request
  jQuery(".navbar ul li a").click(function() {
    var title = jQuery(this).html();
    if (user_name) {
	  //_gaq.push(['_trackEvent', 'Page Views', title, user_name, 1, true]);
    }
  });
  //Game Day tabs
  jQuery(".page-game-day-info .view-tab-pages ul li a").click(function() {
    var title = jQuery(this).html();
    //_gaq.push(['_trackEvent', 'Game Day', 'Click', title, 1, true]);
     if (user_name) {
      //_gaq.push(['_trackEvent', 'Page Views', 'Game Day - ' + title, user_name, 1, true]);
    }
  });
  // Menus Tabs
  jQuery(".page-menus .view-tab-pages ul li a").click(function() {
    var title = jQuery(this).html();
    //_gaq.push(['_trackEvent', 'Menus', 'Click', title, 1, true]);
    if (user_name) {
      //_gaq.push(['_trackEvent', 'Page Views', 'Menus - ' + title, user_name, 1, true]);
    }
  });
  // Club Maps Tabs
  jQuery(".page-club-map .view-tab-pages ul li a").click(function() {
    var title = jQuery(this).html();
    //_gaq.push(['_trackEvent', 'Club Maps', 'Click', title, 1, true]);
    if (user_name) {
      //_gaq.push(['_trackEvent', 'Page Views', 'Club Maps - ' + title, user_name, 1, true]);
    }
  });
  // Stadium Maps
  jQuery(".page-stadium-maps .view-tab-pages ul li a").click(function() {
    var title = jQuery(this).html();
    //_gaq.push(['_trackEvent', 'Stadium Maps', 'Click', title, 1, true]);
    if (user_name) {
      //_gaq.push(['_trackEvent', 'Page Views', 'Stadium Maps - ' + title, user_name, 1, true]);
    }
  });
  
  // Calendar event click
   jQuery('div.month-view table > tbody > tr > td a.inline').click(function(){
    var title = jQuery(this).attr('ga-title');
     //_gaq.push(['_trackEvent', 'Calendar Popup', 'Game Event', title, 1, true]);
     if (user_name) {
       _gaq.push(['_trackEvent', 'Page Views', 'Game Event - ' + title, user_name, 1, true]);
     }
  });
   // Calendar event click
   jQuery('.3rdparty-event a').click(function(){
    var title = jQuery(this).attr('ga-title');
    // _gaq.push(['_trackEvent', 'Calendar', '3rd Party Event', title, 1, true]);
     if (user_name) {
       _gaq.push(['_trackEvent', 'Page Views', '3rd Party Event - ' + title, user_name, 1, true]);
     }
  });
   // Calendar STH event click
   jQuery('.sth-calendar-event a.inline').click(function(){
    var title = jQuery(this).attr('ga-title');
     //_gaq.push(['_trackEvent', 'Calendar Popup', 'STH Event', title, 1, true]);
  });
   
  // RSVP form click
   jQuery('#webform-client-form-116 a.btn').click(function(){
    var title = jQuery(this).html();
     //_gaq.push(['_trackEvent', 'RSVP', 'No Thanks', user_name, 1, true]);
  });
  if (jQuery("#webform-client-form-116 input.form-text, #webform-client-form-116 select.form-select").hasClass('error') && user_name) {
    // _gaq.push(['_trackEvent', 'RSVP', 'Fail', user_name, 1, true]);
  }
  if (jQuery("div#user-login").hasClass('rsvp-success-page') && user_name) {
     //_gaq.push(['_trackEvent', 'RSVP', 'Success', user_name, 1, true]);
  }
  if (jQuery('#webform-client-form-116 div.DivSelectyze').length>0) {
    jQuery("#webform-client-form-116 select.form-select").each(function(){
      if (jQuery(this).hasClass('error')) {
	jQuery(this).parent().addClass('select-error');
      }
    });
  }
 
 /* Pagination and slide effect */
  count_divs = 1;
  
  jQuery("div.event-gallary-big-box div.field-items > div.field-item:first-child, article.node-site-page div.field-items > div.field-item:first-child, #node-734 div.field-items > div.field-item:first-child, body.page-ticket-management div.field-items > div.field-item:first-child").addClass('active').show();
  pages = jQuery("div.event-gallary-big-box div.field-items > div.field-item, article.node-site-page div.field-items > div.field-item, #node-734 div.field-items > div.field-item, body.page-ticket-management div.field-items > div.field-item").length;
  
  if(jQuery("div.tab-pane.active").length>0){
    pages = jQuery("div.event-gallary-big-box div.field-items > div.field-item, article.node-site-page div.field-items > div.field-item, #node-734 div.field-items > div.field-item, body.page-ticket-management   div.field-name-field-page-body div.field-items > div.field-item", "div.tab-pane.active").length;
  }
  
  addPagination();
  if(jQuery("a.more-info").length>0){
    //code also added to calendar popup js code
    //for restoring the state when popup is closed
    jQuery("a.more-info").on('click', function(){
      if(!jQuery("div.event-gallary-big-box div.field-items > div.field-item").hasClass('active')){
        if(jQuery("div.event-gallary-big-box div.field-items > div.field-item").length>0){
          jQuery("div.event-gallary-big-box div.field-items > div.field-item").removeClass('active');
          jQuery("div.event-gallary-big-box div.field-items > div.field-item:first-child").addClass('active').show();
        }
        count_divs = 1;
        addPagination();
      }
    });
  }
  
  jQuery("#views-bootstrap-tab-1 > ul.nav-list > li > a").click(function(){
    //bring to initial state if tabbed navigation is clicked
    jQuery('div.next-previous').hide();
    jQuery("article.node-site-page").find('p.demo.demo1').remove();
    jQuery("article.node-site-page div.field-items > div.field-item").removeClass('active');
    
    var $id = jQuery(this).attr('href');
    context = $id;    
    jQuery($id+" article.node-site-page div.field-items > div.field-item:first-child, " + $id + " div.event-gallary-big-box div.field-items > div.field-item:first-child").addClass('active').show();
    pages = jQuery($id+" article.node-site-page div.field-name-field-page-body div.field-items > div.field-item," + $id + " div.event-gallary-big-box div.field-items > div.field-item").length;
    count_divs = 1;
    addPagination();
  });
  
  jQuery('div.next-previous div.next, div.next-previous div.previous').on('click', function(){
    if(jQuery(this).hasClass('next')){
      if(jQuery("div.event-gallary-big-box div.field-items > div.field-item.active, article.node-site-page div.field-items > div.field-item.active").next("div.field-item").length>0){
        jQuery("div.event-gallary-big-box div.field-items > div.field-item.active, article.node-site-page div.field-items > div.field-item.active").removeClass('active').next("div.field-item").addClass('active');
        count_divs++;
        jQuery(".pgnum").html(count_divs);
      }
    }
    else if(jQuery(this).hasClass('previous')){
      if(jQuery("div.event-gallary-big-box div.field-items > div.field-item.active, article.node-site-page div.field-items > div.field-item.active").prev("div.field-item").length>0){
        jQuery("div.event-gallary-big-box div.field-items > div.field-item.active, article.node-site-page div.field-items > div.field-item.active").removeClass('active').prev("div.field-item").addClass('active');
        var pgnum = jQuery("div.event-gallary-big-box div.field-items > div.field-item.active, article.node-site-page div.field-items > div.field-item.active").prev("div.field-item").eq();
        count_divs--;
        jQuery(".pgnum").html(count_divs);
      }
    }       
    
    if(count_divs==pages){ 
      jQuery('div.next-previous div.next').hide();
      jQuery('div.next-previous div.previous').show();
    }
    
    if(count_divs==1 && pages>1){ 
      jQuery('div.next-previous div.previous').hide();
      jQuery('div.next-previous div.next').show();
    }
    
    if(count_divs>1 && count_divs<pages){ 
      jQuery('div.next-previous div.next').show();
      jQuery('div.next-previous div.previous').show();
    }
  });
  // ipad fix - added blank click bindings
  jQuery('div.next-previous div.next, div.next-previous div.previous').click(function(){ 
  });
  
  function addPagination(pages1){     
    if (pages>1) { 
      jQuery('div.next-previous, div.next-previous div').show();
      jQuery("div.event-gallary-big-box div.field-items, article.node-site-page div.field-items").append('<p class="demo demo1">Page <span class="pgnum"></span> of <span class="maxpg"></span></p>'); 
      jQuery("div.event-gallary-big-box div.field-items  > div.field-item:first-child, article.node-site-page div.field-items > div.field-item:first-child").addClass('active').show();
      jQuery("div.event-gallary-big-box div.field-items, article.node-site-page div.field-items").find(".maxpg").html(pages); 
      jQuery("div.event-gallary-big-box div.field-items, article.node-site-page div.field-items").find(".pgnum").html(count_divs);
      jQuery('div.next-previous div.previous').hide();
      
      /*jQuery('div.next-previous div.next').live('click', function(){ 
        if(jQuery("article.move-np div.field-items > div.field-item.active").next("div.field-item").length>0){
          jQuery("article.move-np div.field-items > div.field-item.active").removeClass('active').next("div.field-item").addClass('active');
          count_divs++;
          jQuery(".pgnum").html(count_divs);
        }
      });
      
      jQuery('div.next-previous div.previous').live('click', function(){
        if(jQuery("article.move-np div.field-items > div.field-item.active").prev("div.field-item").length>0){
          jQuery("article.move-np div.field-items > div.field-item.active").removeClass('active').prev("div.field-item").addClass('active');
          var pgnum = jQuery("article.move-np div.field-items > div.field-item.active").prev("div.field-item").eq();
          count_divs--;
          jQuery(".pgnum").html(count_divs);
        }
      });*/
    }
  }
  /* End of Pagination and slide effect */
  
  // Calendar page
  
    jQuery('div.game-away').each(function() {
    jQuery(this).closest('td').addClass('game-away');
      closestTd=jQuery(this).closest('td')
      if(!closestTd.hasClass('multi-day')){
        tempID=(closestTd.attr('id')).split('-')
      }else{
        tempID=""
      }
      rel="#"+tempID[0]+"-"+jQuery(this).closest('td').data('date')+'-date-box';
      jQuery(rel).addClass('game-away');
    });
    jQuery('div.game-home').each(function() {
      jQuery(this).closest('td').addClass('game-home');
        closestTd=jQuery(this).closest('td')
        if(!closestTd.hasClass('multi-day')){
          tempID=(closestTd.attr('id')).split('-')
        }else{
          tempID=""
        }
      rel="#"+tempID[0]+"-"+jQuery(this).closest('td').data('date')+'-date-box';
      jQuery(rel).addClass('game-home');
    });
    jQuery('div.sth-event').each(function() {
      jQuery(this).closest('td').addClass('sth-event');
        closestTd=jQuery(this).closest('td')
        if(!closestTd.hasClass('multi-day')){
          tempID=(closestTd.attr('id')).split('-')
        }else{
          tempID=""
        }
      rel="#"+tempID[0]+"-"+jQuery(this).closest('td').data('date')+'-date-box';
      jQuery(rel).addClass('sth-event cal-msg-games');
    });
  });
  


jQuery(document).ready(function(){
    var form_id = '';
   
	jQuery('.promo-link-edit').click(function() {
      //custom_confirm(jQuery(this), 'testaction');
     //   return;
     if (ttextCounter == true || jQuery('.quickedit-changed').length) {
        //if (confirm("You have unsaved changes. Do you want to discard the changes?") == true) {
        //    ttextCounter1 = false;
        //    location.reload(true);
        //}
        alertifyreset () ;
        alertify.confirm("You have unsaved changes. Do you want to discard the changes?", function (e) {
            if (e) {
               ttextCounter1 = false;
               location.reload(true);
            } else {}
        });
        return;
     }
     if (jQuery('.quickedit-toolgroup .action-cancel').length) {
         jQuery('.quickedit-toolgroup .action-cancel').click();
         return;
     }
    var jQuerythis = jQuery(this)
		jQuery('div.tile-image-form').css('visibility','hidden');
		jQuerythis.next('div.tile-image-form').css('visibility','visible');
    
	});
	jQuery('.tile-image-form .popup-close').click(function() {
   // var r = confirm("Press a button!");
   if (ttextCounter == true) {
    alertifyreset () ;
        alertify.confirm("You have unsaved changes. Do you want to discard the changes?", function (e) {
            if (e) {
              var wraper = jQuery(this).parents('div.tile-image-form');
              wraper.css('visibility','hidden');
              ttextCounter1 = false;
              location.reload(true);
            } else {}
        });
        //if (confirm("You have unsaved changes. Do you want to discard the changes?") == true) {
        //    var wraper = jQuery(this).parents('div.tile-image-form');
        //    wraper.css('visibility','hidden');
        //    ttextCounter1 = false;
        //    location.reload();
        //} 
         return;
   }else {
      var wraper = jQuery(this).parents('div.tile-image-form');
      wraper.css('visibility','hidden');
   }
    
		
	});
	
	jQuery('.main-wrap .highlighted .home-tgl').click(function(){
		right: "0"
});

 jQuery("#block-custom-block-promo-tile div.home-page-tile").each(function() {
	var $id = '';
	$id = jQuery(this).attr('id');
	
	jQuery("#" + $id + " form.node-form .field-name-field-type select.form-select").each(function(){
    var $pid = jQuery(this).parents('div.home-page-tile').attr('id');
	    hidepromoSelect(this, $pid);
      jQuery(this).on('change', function(){
         var $pid = jQuery(this).parents('div.home-page-tile').attr('id')
         hidepromoSelect(this, $id);
      });
	});

	
    });
    //});
//Slide Toggle right
jQuery('.home-tgl').click(function() {	
	jQuery(this).toggleClass('click').parents('.highlighted, .stp-editor-links').toggleClass('slide');
	jQuery(this).toggleClass('click').parents('.page-tabs').toggleClass('slide');
	jQuery(this).find('i').toggleClass('fa-angle-left fa-angle-right');
	
	
	
});

jQuery('.home-tgl').append('<span class="bg"/>');




/*if(jQuery('.bottom-promo-tile .tile-image-form form .form-type-advanced-link').hasClass('has-error')){
	//alert('yes');
	jQuery('.form-type-advanced-link.has-error').parents('div.tile-image-form').css('visibility','visible');
}
*/



});

function hidepromoSelect($this, $id){
	if(jQuery($this).val()=='regular'){
	    jQuery('#' + $id).find('div.field-name-field-countdown-title').hide();
	    jQuery('#' + $id).find('div.field-name-field-date').hide();
	    //jQuery('#' + $id).find('div.field-name-field-link').show();
	}
	else if(jQuery($this).val() == 'countdown'){
	    //jQuery('#' + $id).find('div.field-name-field-link').hide();
	    jQuery('#' + $id).find('div.field-name-field-countdown-title').show();
	    jQuery('#' + $id).find('div.field-name-field-date').show();
	}
}


/*set cookies for editor pannel*/
var cookies;
function setCookie(){
	jQuery.cookie('stp-editorp-toggle', '1');
}

function deleteCookie(){
	jQuery.cookie('stp-editorp-toggle', null);
}

function getCookie(){
	cookies = jQuery.cookie("stp-editorp-toggle");
}
jQuery(document).ready(function(e) {
	getCookie();
	if (cookies == "1"){
		jQuery('.highlighted').addClass('slide');
		jQuery('.home-tgl').find('i').addClass('fa-angle-left').removeClass('fa-angle-right');
	}
	jQuery('.home-tgl').click(function(){   
		if(jQuery('.highlighted').hasClass('slide')){
			setCookie();
		}
		else{
			deleteCookie();   
		}
	});
});

/*Mobile Navigation*/
jQuery(document).ready(function() {
	jQuery('button.navbar-toggle').click(function(){
		jQuery('body').toggleClass('slide-nav');
		//jQuery('.container-fluid').toggleClass('slide-container');
	});
  jQuery('.asaf-area-wrapper').bind('html-change', function() {
        console.log('html-change-pre triggered', this, arguments);
    });
  jQuery('.tile-image-form').each(function() {
    var fsisi = jQuery(this).find('form').attr('id');
    tiles_form_action(fsisi);
   // console.log(fsisi);
  });
  jQuery( document ).ajaxComplete(function(event, request, settings) {
    var url = settings.url;
    if (url.substr(0,28) == '/file/ajax/field_image/und/0') {
        var ares = url.split('/');
        check_submit(null, jQuery( jQuery('input[name="form_build_id"][value="' +ares[6] + '"]')));
    }
 });
});


function tiles_form_action(fid) {
    jQuery('#' + fid).find('[name=op]').attr("disabled", true);
    var textCounter = false;
    jQuery('#' + fid).find('input:text').keyup(check_submit);
    jQuery('#' + fid).find('input:text, input:checkbox').change(check_submit);
    jQuery('#' + fid + ' select').change(check_submit);
     jQuery('#' + fid + ' .imageeditor-widget-item').click(check_submit);
}

 

function check_submit(e, fid1) {
    ttextCounter = true;
    if ( typeof fid1 !== "undefined") {
        var fid=fid1.parents('.home-page-tile').attr('id');//code
    }
    else {
        var fid=jQuery(this).parents('.node-form').attr('id');
    }
    var textCounter = false;
    var textCounter1 = false;
    var $this =  jQuery('#' + fid).find('[name=title]');
    if ($this && $this.val().length > 0) {
        textCounter = true;
    }
    else {
        textCounter = false;
    }
      // console.log($this.val());
    var $this =  jQuery('#' + fid).find('[name=field_image\\[und\\]\\[0\\]\\[fid\\]]');
    if ($this && $this.val().length > 0 && $this.val() >0) {
        textCounter1 = true;
    }
    else {
        textCounter1 = false;
    }
  //   console.log($this.val());
    var ttt = true;
    if (textCounter && textCounter1) {
        ttt = false;
    }
    jQuery('#' + fid).find('[name=op]').attr("disabled", ttt);
}

jQuery(document).ready(function(){
  jQuery('.views-bootstrap-tab-plugin-style li a').click(function(e){
       var tabid = jQuery(this).attr('href');
			 var iframe = jQuery(tabid).find('iframe');
		///	 console.log(iframe);
			 if (iframe) {
				iframe.attr('src', iframe.attr('src'));
			 }
  
    });
  
});
var shouldsubmit = false; // Represent the button should ask for confirmation or not.
window.onbeforeunload = confirmUnloading;



// Funcion called before unloading of the page.
function confirmUnloading() {
    if ((ttextCounter == true || jQuery('.quickedit-changed').length) && ttextCounter1) {
      return "You have unsaved changes. Do you want to discard the changes?";
    }
}

function alertifyreset () {
    alertify.set({
      labels : {
        ok     : "Yes",
        cancel : "No"
      },
      delay : 5000,
      buttonReverse : true,
      buttonFocus   : "ok"
    });
}

jQuery(window ).load(function(){
jQuery('.main-wrap .highlighted.slide').css('transition', 'right 1s');
jQuery('.main-wrap .highlighted').css('transition', 'right 1s');	
});
