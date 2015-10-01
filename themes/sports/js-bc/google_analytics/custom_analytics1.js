jQuery(document).ready(function(){
  
  var page_name = "";
  var user_name = Drupal.settings.custom_hooks.custom_name;
  var mycalendar = Drupal.settings.custom_hooks.my_calendar;
  var fullcalendar = Drupal.settings.custom_hooks.full_calendar;
  var calendar = Drupal.settings.custom_hooks.calendar;
  
  //Social Icons Click
  jQuery("#facebook, #twitter, #instagram").click(function(){
    var title = jQuery(this).attr('id'); 
    if (user_name) { 
      ga('send', 'event', 'Promotion Tiles', 'icon', title);
    }
    else{
      ga('send', 'event', 'Promotion Tiles', 'icon', title);
    }
  });
  
  //Track external links
  jQuery('a[href^="//"],a[href^="http"]')
  .not('[href*="' + window.location.hostname + '"]')
  .click(function(){
    var href_url = jQuery(this).attr('href');
    ga('send', 'event', 'click', 'external Link', href_url);
  });
    
  //Promo tiles click
  jQuery("aside.promo-box a").click(function(){
    var href_value = jQuery(this).attr('href');
    var parentsDiv = jQuery(this).parents('aside.promo-box'); 
    var title = jQuery("div.tilehead span", parentsDiv).text();
    
    if (user_name) {
      ga('send', 'event', 'Promotion Tiles', 'icon', title);
    }
    else{
      ga('send', 'event', 'Promotion Tiles', 'icon', title);
    }  
  });
  
  //Footer Iomedia Link Click
  jQuery("footer a[href*='io-media']").click(function(){
    ga('send', 'event', 'Iomedia', 'Iomedia Website', 'external link');
  });
  
  
  //Contact Us account rep slider Next click
  jQuery('div#block-views-acct-reps-block a.carousel-control.right').click(function(){
    var acctrepname = "";
    var len = jQuery(this)
    .parent()
    .find('div.carousel-inner div.item.active')
    .next()
    .find('h4.usename')
    .length;
    
    if(len==0){
      acctrepname = jQuery(this)
      .parent()
      .find('div.carousel-inner div.item:first-child')
      .find('h4.usename')
      .text();
    }
    else{
      acctrepname = jQuery(this)
      .parent()
      .find('div.carousel-inner div.item.active')
      .next()
      .find('h4.usename')
      .text();
    }
    ga('send', 'event', 'contact us', 'next', acctrepname);
  });
  
  //Contact Us account rep slider Previous click
  jQuery('div#block-views-acct-reps-block a.carousel-control.left').click(function(){
    var acctrepname = "";
    var len = jQuery(this)
    .parent()
    .find('div.carousel-inner div.item.active')
    .prev()
    .find('h4.usename')
    .length;
    
    if(len==0){
      acctrepname = jQuery(this)
      .parent()
      .find('div.carousel-inner div.item:last-child')
      .find('h4.usename')
      .text();
    }
    else{
      acctrepname = jQuery(this)
      .parent()
      .find('div.carousel-inner div.item.active')
      .prev()
      .find('h4.usename')
      .text();
    }
    
    ga('send', 'event', 'contact us', 'previous', acctrepname);
  });
  
  //Contact Us hours and information below account rep slider
  //email click
  jQuery("div.infohourbg div.hour-imf a.mail").click(function(){
    var mailtxt = jQuery(this).attr('href');
    mailtxt = mailtxt.replace("mailto:","");
    ga('send', 'event', 'contact us', 'email id', 'email id | '+mailtxt);
  });
  
  //calendar popups - As Page laod
  calendar_popups(user_name); 
  jQuery(document).ajaxComplete(function(event, xhr, settings){ 
    calendar_popups(user_name);
  });
  
  //STH Events
  if(jQuery("div.view-gallery div.carousel-inner > div.item").length>0){
    var src_value = "";
    
    jQuery("div.view-gallery a.carousel-control.right").on('click', function(){
      if(jQuery("div.view-gallery div.carousel-inner > div.item.active").next().length>0){
        src_value = jQuery("div.view-gallery div.carousel-inner > div.item.active").next().find("img").attr('src');        
      }
      else{
        src_value = jQuery("div.view-gallery div.carousel-inner > div.item:first-child").find("img").attr('src');
      }
      process_image_url(src_value, 'next');
    });
    
    jQuery("div.view-gallery a.carousel-control.left").on('click', function(){
      if(jQuery("div.view-gallery div.carousel-inner > div.item.active").prev().length>0){
        src_value = jQuery("div.view-gallery div.carousel-inner > div.item.active").prev().find("img").attr('src');
      }
      else{
        src_value = jQuery("div.view-gallery div.carousel-inner > div.item:last-child").find("img").attr('src');
      }
      process_image_url(src_value, 'prev');
    });
    
    jQuery("div.view-gallery ol.carousel-indicators > li").on('click', function(){
      var slide_number = jQuery(this).attr('data-slide-to');
      src_value = jQuery("div.view-gallery div.carousel-inner > div.item").eq(slide_number).find("img").attr('src');
      process_image_url(src_value, 'dot');
    });
  }
  //List View Event Tracking
  if(jQuery('body').hasClass('page-season-schedule')){ 
  	var url4 = window.location.pathname;
  	jQuery('body').find('div.panel').on('click', function(){
  		var that = jQuery(this);
  		var lstviewpath = "";
  		var lstviewtitle = "";
  		if(jQuery(this).find('div.panel-collapse.in').length<=0){
  			lstviewpath = url4 + "/" + (that.attr('data-str')).split(' ').join('-'); 
        //alert(url4 + " " + that.attr('data-str'));
  			lstviewtitle = that.attr('data-str');
  			if (user_name) {
		      ga('send', 'pageview', {
		         'dimension1': user_name,
		         'page': lstviewpath,
		         'title': lstviewtitle
		      });
			 }
			 else{
		      ga('send', 'pageview', {
		        'dimension1': 'Anonymous',
		        'page': lstviewpath,
		        'title': lstviewtitle
		      });
			 }
  		}
  	}); 
  }
  
  //Benefits tab - Menus Tabs - As Page Load
  jQuery("div.view-tab-pages ul.nav-tabs > li > a").click(function() {
    var url3 = window.location.pathname;
    var title = jQuery(this).text(); 
    var tabpage = jQuery(this).attr('href');     
    var subnavtext = "";
    var tabpath = "";
    
    if(jQuery("header ul.menu > li.active > ul.dropdown-menu > li.active > a.active.active-trail").length>0){
      subnavtext = jQuery("header ul.menu > li.active > ul.dropdown-menu > li.active > a.active.active-trail").text();
    }
    
    if(subnavtext==""){
      tabpath = url3 + '/' + title;
    }
    else{
      tabpath = url3 + '/' + subnavtext + '/' + title;
    }

    if (user_name) {
       ga_title = "Logged In | " + user_name + " | " + title;
       ga('send', 'pageview', {
          'page': tabpath,
          'title': title
       });
    }
    else{
      ga_title = "Anonymous | " + title;
      ga('send', 'pageview', {
        'page': tabpath,
        'title': title
     });
    }
  });
  
  //page load tracking
  var title = document.title;
  var drupal_site_name = Drupal.settings.custom_hooks.sitename;
  title = title.replace(" | "+drupal_site_name, "");
  var url = window.location.protocol + '//' + window.location.hostname + window.location.pathname + window.location.search + window.location.hash;
  var url2 = window.location.pathname + window.location.search + window.location.hash;
  var ga_path = window.location.pathname + window.location.hash;
  var ga_title = "";
  
  if(jQuery('body').hasClass('page-my-calendar') || jQuery('body').hasClass('page-full-calendar') || jQuery('body').hasClass('page-calendar')){
    var my = jQuery("div.view-header div.date-nav div.date-heading h3").text(); 
    var str = url.split("/"); 
    if(str[str.length-1]=='my-calendar' || str[str.length-1]=='full-calendar' || str[str.length-1]=='calendar'){
      str = str[str.length-1];
    }
    else if(str[str.length-2]=='my-calendar' || str[str.length-2]=='full-calendar' || str[str.length-2]=='calendar'){
      str = str[str.length-2];
    }
    
    if (user_name) {
      ga('send', 'pageview', {
         'dimension1': user_name,
         'page': str + "/" + my,
         'title': 'Load | ' + my
      });
	 }
	 else{
      ga('send', 'pageview', {
        'dimension1': 'Anonymous',
        'page': str + "/" + my,
        'title': 'Load | ' + my
      });
	 }
  }
  else if(jQuery('body').hasClass('node-type-tab-page')){ 
    var tab_title = jQuery("div.view-tab-pages ul.nav-tabs > li.active > a").text();
    var tabpage = jQuery("div.view-tab-pages ul.nav-tabs > li.active > a").attr('href');
    var subnavtext = "";
    if(jQuery("header ul.menu > li.active > ul.dropdown-menu > li.active > a.active.active-trail").length>0){
      subnavtext = jQuery("header ul.menu > li.active > ul.dropdown-menu > li.active > a.active.active-trail").text();
    }
    var tabpath = "";
    if(subnavtext==""){
      tabpath = url2 + '/' + tab_title;
    }
    else{
      tabpath = url2 + '/' + subnavtext + '/' + tab_title;
    }
    
    if (user_name) {
      ga_title = "Logged In | " + tab_title;
      ga('send', 'pageview', {
        'dimension1': user_name,
        'page': tabpath,
        'title': ga_title
      });
    }
    else{
      ga_title = "Anonymous | " + tab_title;
      ga('send', 'pageview', {
        'dimension1': 'Anonymous',
        'page': tabpath,
        'title': ga_title
      });
    }	
  }
  else{  
	  //Archtics_ID custom variable tracking and page load tracking  
	  if (user_name){    
      ga_title = "Logged In | " + title;
      ga('send', 'pageview', {
        'dimension1': user_name,
        'page': ga_path,
        'title': ga_title
      });
	  }
	  else{
      ga_title = "Anonymous | " + title;
      ga('send', 'pageview', {
        'dimension1': 'Anonymous',
        'page': ga_path,
        'title': ga_title
      });
	  }
  }
});