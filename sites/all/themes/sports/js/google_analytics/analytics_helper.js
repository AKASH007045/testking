function ucwords(str){
  if(str){
    str = str.toLowerCase().replace(/\b[a-z]/g, function(letter) {
      return letter.toUpperCase();
    });
  }
}

function get_month_year(hosturl, nxtprvclkurl, state, lgstr){
  var month_arr = ['January', 'February', 'March', 'April', 'May', 'June', 'July', 'August', 'September', 'October', 'November', 'December'];  
  
  var mycalendar = Drupal.settings.custom_hooks.my_calendar;
  var fullcalendar = Drupal.settings.custom_hooks.full_calendar;
  var calendar = Drupal.settings.custom_hooks.calendar;
  
  if(hosturl && state){
    var str_urltext = "";
    var str = hosturl.split("/"); 
    if(str[str.length-1]==mycalendar || str[str.length-1]==fullcalendar || str[str.length-1]==calendar){
      str_urltext = str[str.length-1];
    }
    else if(str[str.length-2]==mycalendar || str[str.length-2]==fullcalendar || str[str.length-2]==calendar){
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
      /**********************   For ticket STPDEV-704 *************************/
      jQuery(".js_spin_loader").show();
      /********************** End For ticket STPDEV-704 ***********************/
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
      get_month_year(hostUrl, heading, "next", "Logged In | "+userid);
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
    var classVal = ""; 
    if(jQuery(this).parents('div.open-game-popup').hasClass('nhl')){
    	classVal = 'NYR';
    }
    else if(jQuery(this).parents('div.open-game-popup').hasClass('nba')){
    	classVal = 'NYK';
    }
    if(elem.hasClass('game-home')){ 
      pathstring = "home game (" + classVal + ") | ";
    }
    else if(elem.hasClass('game-away')){
      pathstring = "away game (" + classVal + ") | ";
    }
    else if(elem.hasClass('sth-calendar-event')){
      pathstring = "event | ";
    }
    //alert("Anonymous | " + pathstring + a_title);
    if(pathstring!="" && a_title){
      if (userid) {
         ga('send', 'pageview', {
         	'page': window.location.pathname + window.location.hash,
         	'title': "Logged In | " + userid + " " + pathstring + a_title
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