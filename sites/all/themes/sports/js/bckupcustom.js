jQuery(document).ready(function(){
  jQuery('#custom-pawp-tmapi-request-form, #webform-client-form-116, #custom-invoice-payment-request').find('.form-select').Selectyze({
     theme : 'css3'
    });
  /* VideoPlayer CSS Here*/
  //jQuery("#videojs-110-field-video-video").find('.vjs-big-play-button').click(function(){
  //  jQuery('#videojs-110-field-video-video .vjs-poster').css('background-image', 'none').show();
  //});
    
    
//    jQuery(function(){
//	videojs.plugin('showPosterAtEnd', function(){
//		this.on('ended',function() {
//            alert("hi");
//			console.log( this );
//			this.posterImage.show();
//			this.bigPlayButton.show();
//			this.currentTime(0.0);
//			this.pause();
//            
//		});
//	});			
//});
//    
  function validateExpDate(vexd, format){
    if(format=="mmyy"){
      var mm = vexd.substring(0, 2);
      var yy = vexd.substring(2, 4);
      dateObj = new Date();
      var month = dateObj.getUTCMonth();
      month = month + 1;
      var day = dateObj.getUTCDate();
      var year = dateObj.getUTCFullYear();
      year = year.toString();
      year = year.substring(2, 4);
      console.log(day + " " + month + " " + year);
      if(yy<year){ return false; }
      if( (mm<=0) && (mm>12) ){ return false; }
      return true;
    }  
    return false;    
 }  
  
  /* Sign in page */
  if(jQuery('body.page-user-login div#inline-messages').length>0){
    var str = jQuery('body.page-user-login div#inline-messages').clone().wrap('<div/>').parent().html();
    jQuery('body.page-user-login div#inline-messages').remove();
    jQuery('body.page-user-login form#user-login div.form-type-password').after(str);
    jQuery('body.page-user-login form#user-login').find('div#inline-messages').show();
  }
  
  /*Navigation fixes*/
  var host_url = window.location.href;
  var strcp = host_url.split('/');
  if(strcp[strcp.length-1]=="full-calendar"){
    jQuery("ul.menu > li").eq(1).addClass('active-trail active').find('a', this).addClass('active-trail active');
  }
  else{
    jQuery("ul.menu > li.active-trail").addClass('active');
  }
  
  var clr = "#00479e";
  var bimg = "linear-gradient(to bottom, #00479e, #001938)";
  var bdr = "1px solid #002e66";
  var bxshd = "1px 1px 1px #1c6ccb inset";
  
  jQuery('div.row-header ul.menu > li > a').mouseover(function(){
    jQuery(this).css('backgroundImage', bimg).css('backgroundColor', clr).css('border', bdr).css('boxShadow', bxshd);
  }).mouseout(function(){
    if(!jQuery(this).parents("li").hasClass("active-trail")){
      jQuery(this).css('backgroundImage', 'none').css('backgroundColor', 'transparent').css('border', '1px solid transparent').css('boxShadow', 'none');
    }
  });
  
  jQuery('div.row-header ul.menu > li').mouseover(function() {
    if(jQuery(this).hasClass("active-trail")){
      return;
    }
  
    if(jQuery("div.row-header ul.menu > li.active-trail").length>0){
      jQuery("div.row-header ul.menu > li").each(function(){
        if(!jQuery(this).hasClass('active-trail')){
          jQuery(this).removeClass("active").removeClass('open');
          jQuery("> a", this).css('backgroundImage', 'none').css('backgroundColor', 'transparent').css('border', '1px solid transparent').css('boxShadow', 'none');
        }
      });      
    }
    
    jQuery(this).addClass('active');
    if(jQuery(this).hasClass('leaf')){
      jQuery("> a", this).css('backgroundImage', bimg).css('backgroundColor', clr).css('border', bdr).css('boxShadow', bxshd);
    }
    else{
      jQuery("a.dropdown-toggle", this).css('backgroundImage', bimg).css('backgroundColor', clr).css('border', bdr).css('boxShadow', bxshd);
    }    
    jQuery('div.row-header ul.menu ul.dropdown-menu', this).show();
    
  }).mouseout(function() {
    jQuery("div.row-header ul.menu > li").removeClass("active").removeClass('open');
    jQuery("div.row-header ul.menu > li.active-trail").addClass('active');
    jQuery('div.row-header ul.menu ul.dropdown-menu', this).hide();
  });
  
  jQuery('div.row-header ul.menu ul.dropdown-menu').mouseover(function(){
    if(jQuery("div.row-header ul.menu > li.active-trail").length>0){
      jQuery("div.row-header ul.menu > li").each(function(){
        if(!jQuery(this).hasClass('active-trail')){
          jQuery("> a", this).css('backgroundImage', 'none').css('backgroundColor', 'transparent').css('border', '1px solid transparent').css('boxShadow', 'none');
        }
      });      
    }
    
    if(!jQuery(this).parents("li").hasClass("active-trail")){
      jQuery(this).parents("li").find("a.dropdown-toggle").css('backgroundImage', bimg).css('backgroundColor', clr).css('border', bdr).css('boxShadow', bxshd);
    }
  }).mouseout(function(){
      if(!jQuery(this).parents("li").hasClass("active-trail")){
        jQuery(this).parents("li").find("a.dropdown-toggle").css('backgroundImage', 'none').css('backgroundColor', 'transparent').css('border', '1px solid transparent').css('boxShadow', 'none');
      }
      
      if(jQuery("div.row-header ul.menu > li.active-trail").length>0){
        jQuery("div.row-header ul.menu > li").each(function(){
          if(!jQuery(this).hasClass('active-trail')){
            jQuery("> a", this).css('backgroundImage', 'none').css('backgroundColor', 'transparent').css('border', '1px solid transparent').css('boxShadow', 'none');
          }
        });      
      }
  });
  /*End of Navigation fixes*/
  
  // Home page Promo
  jQuery('a.promo-link')
    .mouseover(function() {
    jQuery(this) .find('.first').hide();
    jQuery(this) .find('.second').show();
  })
  .mouseout(function() {
	jQuery(this) .find('.first').show();
	jQuery(this) .find('.second').hide();
  });
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
  var user_name = Drupal. settings.custom_hooks.custom_name;
  var windowWidth = jQuery(window).width()
  //var windowWidth = jQuery(window).height()
  //alert(windowWidth);
  jQuery(".view-banner .carousel-inner img").width(windowWidth);
	    /* SEARCH BLOCK */
  
	    /* LOG IN ACTIVE */
		jQuery("#om-menu-secondary-menu #user-login-form .form-item input.form-text").click(function(){
		  jQuery("#om-menu-secondary-menu li.leaf-login").addClass("log-in-active");
		});
		if (jQuery("#om-menu-secondary-menu #user-login-form .form-item input").hasClass('error')) {
		  jQuery("#om-menu-secondary-menu li.leaf-login").addClass("log-in-active");
		  _gaq.push(['_trackEvent', 'Login Form', 'Submit', 'Fail', 1, true]);
		}
	  /* LOG IN ACTIVE */
  
	  /* link-login */
	    jQuery("#om-menu-secondary-menu .link-login").click(function(){
		if(jQuery("#om-menu-secondary-menu li.leaf-login").css("display") == "block"){
		  jQuery("#om-menu-secondary-menu li.leaf-login").removeClass("log-in-active");
		  //jQuery("#om-menu-secondary-menu .om-maximenu-content").hide();
		}
		  
		});
		jQuery( document ).ajaxComplete(function() {
    if(jQuery('#modalContent').find('div.error').length>0) {
      _gaq.push(['_trackEvent', 'Payment Popup', 'Validation Failed', user_name, 1, true]);
    }
    jQuery('#custom-pawp-tmapi-request-form').find('.form-select').Selectyze({
     theme : 'css3'
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
  // TOP Social Links
  jQuery(".top-social-links a.sc-links").click(function() {
	     var title = jQuery(this).attr('data-title');
    _gaq.push(['_trackEvent', 'SocialMedia', 'Click', title, 1, true]);
  });
  //Login form forgot Password
  jQuery("#user-login-form a.forgot-pass").click(function() {
	     var title = jQuery(this).attr('data-title');
    _gaq.push(['_trackEvent', 'Forgot Password', 'Click', title, 1, true]);
  });
  jQuery(".region-footer a").click(function() {
	     var title = jQuery(this).attr('data-title');
    _gaq.push(['_trackEvent', 'Exit Links', 'Exit Site', title, 1, true]);
  });
  jQuery(".view-acct-reps .slider-wrap-detail a.rep-mail, #block-custom-block-customer-acct-rep-right a.rep-mail").click(function() {
	     var title = jQuery(this).attr('data-title');
    _gaq.push(['_trackEvent', 'Service Coordinator', 'Email', title, 1, true]);
  });
  if (user_name) {
     _gaq.push(['_setCustomVar', 1, 'Account_ID', user_name, 2]);
  }
  jQuery(".page-invoice a.invoice-link").click(function() {
	    var title = jQuery(this).attr('data-title');
    if (user_name) {
	  _gaq.push(['_trackEvent', 'Invoice', title, user_name, 1, true]);
    }
  });
  
  /* Pagination and slide effect */
  var count_divs = 1;
  jQuery('div.next-previous').hide();
  jQuery("article.move-np.node-benefits div.field-items > div.field-item:first-child").addClass('active').show();
  var pages = jQuery("article.move-np.node-benefits div.field-items > div.field-item").length;
  if (pages>1) {
    jQuery('div.next-previous').show();
    jQuery("article.move-np div.field-items").append('<p class="demo demo1">Page <span class="pgnum"></span> of <span class="maxpg"></span></p>');
    jQuery("article.move-np div.field-items > div.field-item:first-child").addClass('active').show();
    
    jQuery("article.move-np div.field-items").find(".maxpg").html(pages); 
    jQuery("article.move-np div.field-items").find(".pgnum").html(count_divs);
    jQuery('div.next-previous div.previous').hide();
    // ipad fix - added blank click bindings
    jQuery('div.next-previous div.next, div.next-previous div.previous').click(function(){ });
    jQuery('div.next-previous div.next').live('click', function(){ 
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
    });

    jQuery('div.next-previous div.next, div.next-previous div.previous').live('click', function(){   
      if(count_divs==pages){
        jQuery('div.next-previous div.next').hide();
      }
      
      if(count_divs==1){
        jQuery('div.next-previous div.previous').hide();
      }
      
      if(count_divs>1 && count_divs<pages){
        jQuery('div.next-previous div.next').show();
        jQuery('div.next-previous div.previous').show();
      }
    });
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
  
  
  
	/*
	repositionPopUps();
	
	jQuery('.open-game-popup').on('click', function(e){					
		jQuery('.game-popup').hide();
		
		var popUp = jQuery(this).next();
		console.log(popUp);
		popUp.show();
		/*var popUpOffset= popUp.offset();
		console.log(popUpOffset);
		h=jQuery('.calendar-calendar').height();
		
		calOffset = jQuery('.calendar-calendar').offset();
		
		console.log('cal offset', calOffset);
		
		console.log('current left and top '+ parseInt(popUp.css('left')) +', '+ parseInt(popUp.css('top'), 10) )
		
		var currentTop = parseInt(popUp.css('top'), 10);
		var currentLeft = parseInt(popUp.css('left'), 10);
		
		pt= -(popUpOffset.top - currentTop - calOffset.top);
		pl= -(popUpOffset.left - currentLeft - calOffset.left);
		console.log('reducing pos by:t, l: '+ pt + ', '+pl);
		
		popUp.css({'left': pl ,top: pt});*/
		
		//console.log(pt+":"+pl);
		/*
		jQuery(this).closest('td').addClass('popup-active');
		jQuery('.bg-cal-popup').css({'opacity':'.8', height: h}).show();
		jQuery('.cal-jump-month.m-active').trigger('click');
		* /
	});
	
	jQuery('.game-popup .close-button').bind('click', function(e){
	/*	e.preventDefault();
		jQuery(this).parents('.game-popup').hide();
		jQuery('.bg-cal-popup').hide();
		jQuery('.month-view').find('td').removeClass('popup-active');
		jQuery('.dropdown').hide();
		jQuery('a.cal-dd').removeClass('active-calndr');
		* /
	});
});

function repositionPopUps(){
	var eventHolders = jQuery('.open-game-popup');
	for(var i=0; i< eventHolders.length ; i++){
		
		var popUp = jQuery(eventHolders[i]).next();
		console.log(popUp);		
		var popUpOffset= popUp.offset();
		console.log(popUpOffset);
		h=jQuery('.calendar-calendar').height();
		
		calOffset = jQuery('.calendar-calendar').offset();
		
		console.log('cal offset', calOffset);
		
		console.log('current left and top '+ parseInt(popUp.css('left')) +', '+ parseInt(popUp.css('top'), 10) )
		
		var currentTop = parseInt(popUp.css('top'), 10);
		var currentLeft = parseInt(popUp.css('left'), 10);
		
		pt= -(popUpOffset.top - currentTop - calOffset.top);
		pl= -(popUpOffset.left - currentLeft - calOffset.left);
		console.log('reducing pos by:t, l: '+ pt + ', '+pl);
		
		popUp.css({'left': pl ,top: pt});
	}
}

jQuery(document).ajaxComplete(function(event,request, settings) {
   console.log('sdfsddsf');
   alert('hi');
   repositionPopUps();
 });
*/
