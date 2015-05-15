function ucwords(str){
  if(str){
    str = str.toLowerCase().replace(/\b[a-z]/g, function(letter) {
      return letter.toUpperCase();
    });
  }
}

function get_month_year(hosturl, nxtprvclkurl, state, lgstr){
  var month_arr = ['January', 'February', 'March', 'April', 'May', 'June', 'July', 'August', 'September', 'October', 'November', 'December'];  
  if(hosturl && state){
    var str_urltext = "";
    var str = hosturl.split("/"); 
    if(str[str.length-1]=='my-calendar' || str[str.length-1]=='full-calendar' || str[str.length-1]=='calendar'){
      str_urltext = str[str.length-1];
    }
    else if(str[str.length-2]=='my-calendar' || str[str.length-2]=='full-calendar' || str[str.length-2]=='calendar'){
      str_urltext = str[str.length-2];
    }
    
    var mpurl = nxtprvclkurl.split("/");
    if(mpurl[mpurl.length-1]){
      mpurl = mpurl[mpurl.length-1];
      mpurl = mpurl.split('-');      
      var n = {
        "year": mpurl[0],
        "month": month_arr[mpurl[1]-1]
      };      
      var month_year = str_urltext + '/' + n.month + ' ' + n.year;
      /*alert(month_year);
      alert(lgstr + ' | ' + str_urltext + ' | ' + state + " | " + n.month + ' ' + n.year);*/
	    if(lgstr!=""){
		    ga('send', 'pageview', {
				  'page': month_year,
			    'title': lgstr + ' | ' + str_urltext + ' | ' + state + " | " + n.month + ' ' + n.year
		    });
	    }
    }
  }
}

function calendar_popups(userid){ 
  //Calendar Next Click Tracking
  var hostUrl = window.location.href;
  jQuery("div.view-header div.date-nav ul.pager li.date-next a").on('click', function(){
    var heading = jQuery(this).attr('href'); 
    if(userid){
      get_month_year(hostUrl, heading, "next", "Logged In");
    }
    else{
      get_month_year(hostUrl, heading, "next", "Anonymous");
    }
  });
  //Calendar Previous Click Tracking
  jQuery("div.view-header div.date-nav ul.pager li.date-prev a").on('click', function(){
    var heading = jQuery(this).attr('href');
    if(userid){
      get_month_year(hostUrl, heading, "prev", "Logged In");
    }
    else{
      get_month_year(hostUrl, heading, "prev", "Anonymous");
    }
  });
  //calendar popups
  jQuery('div.month-view table > tbody > tr > td div.field-name-field-image').on('click', function(){
    var elem = jQuery(this).parents('div.open-game-popup'); 
    var a_title = elem.find('a.inline').attr('ga-title');
    var pathstring = "";
    /*var classVal = ""; 
    if(jQuery(this).parents('div.open-game-popup').hasClass('nhl')){
    	classVal = 'NYR';
    }
    else if(jQuery(this).parents('div.open-game-popup').hasClass('nba')){
    	classVal = 'NYK';
    }*/
    if(elem.hasClass('game-home')){ 
      pathstring = "home game | ";
    }
    else if(elem.hasClass('game-away')){
      pathstring = "away game | ";
    }
    else if(elem.hasClass('sth-calendar-event')){
      pathstring = "event | ";
    }
    //alert("Anonymous | " + pathstring + a_title);
    if(pathstring!="" && a_title){
      if (userid) {
         ga('send', 'pageview', {
         	'page': window.location.pathname + window.location.hash,
         	'title': "Logged In | " + userid + " | " + pathstring + a_title
         });
      }
      else{
         ga('send', 'pageview', {
         	'page': window.location.pathname + window.location.hash,
         	'title': "Anonymous | " + pathstring + a_title
         });
      }
    }
  });
}

function process_image_url(url, state){
  if(url && state){
    var url_arr = url.split('?');
    var url_string = url_arr[0].split("/");
    var img_name = url_string[url_string.length-1];
    var title = "sth image | ";
    title = title + img_name;
    ga('send', 'event', 'benefits', state, title);
  }
}

var page_name = "";
jQuery(document).ready(function(){
  var user_name = Drupal.settings.custom_hooks.custom_name;
  jQuery("#facebook, #twitter, #instagram").click(function(){
    var title = jQuery(this).attr('id'); 
    if (user_name) { 
      ga('send', 'event', 'Promotion Tiles', 'icon', title);
    }
    else{
      ga('send', 'event', 'Promotion Tiles', 'icon', title);
    }
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
  			lstviewpath = url4 + "/" + (that.attr('data-str')).split(' ').join('-'); //alert(url4 + " " + that.attr('data-str'));
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
  title = title.replace(" | Sacramento Kings", "");
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
