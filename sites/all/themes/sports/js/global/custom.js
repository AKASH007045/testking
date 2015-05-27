jQuery(window).load(function(){
	jQuery('.main-wrap .highlighted.slide').css('transition', 'right 1s');
	jQuery('.main-wrap .highlighted').css('transition', 'right 1s');
	
  //STH Event check the width of slider
	sliderWrapperWidth = jQuery('.event-list-slider').width();
	sliderWidth = jQuery('.event-list-slider ul').width(); 	 
	if(sliderWrapperWidth < sliderWidth){
		jQuery('.view-event-slider').addClass('slide');	
	}
});

jQuery(document).ready(function(){  
  //Add selectyze to pawp page, payment page
  jQuery('#custom-pawp-tmapi-request-form, #custom-invoice-payment-request')
  .find('.form-select')
  .Selectyze({
    theme : 'css3'
  });
  
  /* Sign in page */
  if(jQuery('body.page-user-login div#inline-messages').length>0){
    var str = jQuery('body.page-user-login div#inline-messages').clone().wrap('<div/>').parent().html();
    jQuery('body.page-user-login div#inline-messages').remove();
    jQuery('body.page-user-login form#user-login div.form-type-password').after(str);
    jQuery('body.page-user-login form#user-login').find('div#inline-messages').show();
  }
  /* Sign in form radio button */
  jQuery(".form#user-login .form-item-remember-me > span").show();
  add_custom_theme_radio_loginform();
  
  //Hide/Show Source button
  var interv="";
  function qtimer() {
    interv = setInterval(function(){
      var len = jQuery("body").find('a.cke_button__sourcedialog').length;
      var temp = 'show';
      if(len>0){          
        jQuery("*").on('keydown', null, 'alt+up', function () {
          if (temp == 'show') {
            jQuery("body").find('a.cke_button__sourcedialog').show();
            temp = 'hide';
          }
          else if (temp == 'hide') {
            jQuery("body").find('a.cke_button__sourcedialog').hide();
            temp = 'show';
          }
        });
        
        jQuery("body").on('keydown', null, 'alt+up', function () {
          if (temp == 'show') {
            jQuery("body").find('a.cke_button__sourcedialog').show();
            temp = 'hide';
          }
          else if (temp == 'hide') {
            jQuery("body").find('a.cke_button__sourcedialog').hide();
            temp = 'show';
          }
        });
        //jQuery("body").find('a.cke_button__sourcedialog').hide();
        clearInterval(interv);
      }
    }, 2000);
  }
  
  jQuery("div.tabs-wrap-primary-user ul.tabs-primary-live li.edit-link a, div.tabs-wrap-primary-user ul.tabs-primary-live li.edit a").click(function(){
    var len = jQuery("body").find('a.cke_button__sourcedialog').length;
    if(len<=0){
      qtimer();
    }
  });
  
  //Event Gallery View slider js
  if(jQuery('.event-gallary-thumb-slider .view-event-slider').length !=0) {
    jQuery('div.view-event-slider div.view-content > div.item-list')
    .addClass('viewport')
    .find('ul')
    .addClass('overview');
    
    jQuery('div.view-event-slider').tinycarousel();
  }
  jQuery(window).resize(function() {		
		jQuery('div.view-event-slider').tinycarousel();		
	});
  
  var nid = 0;
  jQuery('#gallery-slider a, .event-slider-box .tilehead a').click(function() {
    if(isNaN(jQuery(this).attr('id')) == false) {
      nid = jQuery(this).attr('id');
      var url = Drupal.settings.basePath + 'ajax/' + nid;
      jQuery.getJSON(url, function(dt) {
	console.log(dt);
        jQuery('.event-gallary-big-box').html(dt.data);
        //jQuery('.event-gallary-big-box .modal-header .poupatstud').html(dt.title);
       // jQuery('.sthevent-image #more-info .field-name-body .field-items .field-item').html(dt.body);        
       // jQuery('.event-gallary-thumb-slider .view-event-slider .view-content ul.overview li aside').each(function() {
         // if(jQuery(this).find('#gallery-slider a').attr('id') == nid) {
          //  jQuery(this).addClass('active');
         // }
        //  else {
        //    jQuery(this).removeClass('active');
         // }
       // });        
        if(jQuery('.sthevent-image #gallery').find('.carousel-inner .item').length > 1) {
          jQuery('.sthevent-image #gallery').find('ol.carousel-indicators').css('display', 'block');
          jQuery('.sthevent-image #gallery').find('a.carousel-control').css('display', 'inline');
        }
      });
    }
  });
  /**Countdown Timer Days to two digits if 0 is present on first digit**/
  if(jQuery('.nextgame-timer .countdownHolder .countDays').length > 0){
	  var days_position = jQuery('.nextgame-timer .countdownHolder .countDays .position.digit2 .digit');
	  var days_position_digit = days_position.html();
	  days_position_digit = Number(days_position_digit);
	  console.log(days_position_digit);
	  if(days_position_digit === 0){
			days_position.parent(".position").css("display","none");
		}
  }
  /**Countdown Timer Days to two digits if 0 is present on first digit**/
});


jQuery(document).ready(function(){
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
	
	/* Footer disable*/
   
    //First remove all the existing click handlers
    jQuery('.page-pawp-nojs-form .footer-copyright').find('a').off('click');
    //then disable the default click behavoiour on anchors
    jQuery('.page-pawp-nojs-form .footer-copyright').find('a').click(function(e){
        e.preventDefault();
        //
    });
    /* Footer disable*/ 
});