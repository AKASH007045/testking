function ucwords(str){
  if(str){
    str = str.toLowerCase().replace(/\b[a-z]/g, function(letter) {
      return letter.toUpperCase();
    });
  }
}

var page_name = "";
jQuery(document).ready(function(){
  var user_name = Drupal.settings.custom_hooks.custom_name;
  var plan_name = Drupal.settings.custom_hooks.plan_name;
  
  if (user_name) {
     _gaq.push(['_setCustomVar', 1, 'Archtics_ID', 'blazers.'+user_name, 2]);
  }
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
  
  jQuery(".page-invoice a.invoice-link").click(function() {
    var title = jQuery(this).attr('data-title');
    if (user_name) {
    _gaq.push(['_trackEvent', 'Invoice', title, user_name, 1, true]);
    }
  });
  //Promo tiles click
  //Facebook, Twitter, Instagram feed icon click
  jQuery("#facebook, #twitter, #instagram").click(function(){
    var title = jQuery(this).attr('id'); 
    if (user_name) {
      _gaq.push(['_trackEvent', 'Social Feed ' + title + 'Icon Click', title, user_name, 1, true]);
    }
    else{
      _gaq.push(['_trackEvent', 'Social Feed ' + title + 'Icon Click', title, 'Anonymous', 1, true]);
    }
  });
  
  jQuery("aside.promo-box a").click(function(){
    var href_value = jQuery(this).attr('href');
    if(href_value=='http://blazers.io2/experience'){
      var title = 'Experience';
      if (user_name) {
        _gaq.push(['_trackEvent', 'Experience Click', title, user_name, 1, true]);
      }
      else{
        _gaq.push(['_trackEvent', 'Experience Click', title, 'Anonymous', 1, true]);
      }
    }
    
    if(href_value=='http://blazers.io2/goto-account-manager'){
      var title = 'Account Manager';
      if (user_name) {
        _gaq.push(['_trackEvent', 'Account Manager click', title, user_name, 1, true]);
      }
      else{
        _gaq.push(['_trackEvent', 'Mobile app Click', title, 'Anonymous', 1, true]);
      }
    }    
  });
  
  //Footer iomedia link click
  jQuery("p.copyright a").click(function(){
    var href_value = jQuery(this).attr('href');
    var title = jQuery(this).text(); 
    if(href_value=='http://www.io-media.com/'){
      if (user_name) {
       _gaq.push(['_trackEvent', 'IOMedia website Click', title, user_name, 1, true]);
      }
      else{
        _gaq.push(['_trackEvent', 'IOMedia website Click', title, 'Anonymous', 1, true]);
      }
    }
  }); 
  //Contact us account rep email id click
  jQuery("body.page-node-4 div.namedetil a").click(function(){
    var href_value = jQuery(this).attr('href');
    var title = jQuery(this).text(); 
    if(href_value.indexOf("mailto") > -1){
      if (user_name) {
       _gaq.push(['_trackEvent', 'Email ID Click', title, user_name, 1, true]);
      }
      else{
        _gaq.push(['_trackEvent', 'Email ID Click', title, 'Anonymous', 1, true]);
      }
    }
  });
  //My Home Games And Events && Full Schedule
  jQuery("div.calendar-legend ul.menu > li > a").click(function(){
    var href_value = jQuery(this).attr('href');
    var title = jQuery(this).text(); 
    if(href_value=='/my-calendar'){
      if (user_name) {
        _gaq.push(['_trackEvent', 'Tab Click', title, user_name, 1, true]);
      }
      else{
        _gaq.push(['_trackEvent', 'Tab Click', title, 'Anonymous', 1, true]);
      }
    }
    
    if(href_value.indexOf("full-calendar") > -1){
      if (user_name) {
       _gaq.push(['_trackEvent', 'Tab Click', title, user_name, 1, true]);
      }
      else{
        _gaq.push(['_trackEvent', 'Tab Click', title, 'Anonymous', 1, true]);
      }
    }
  });
  
  //Calendar events click
  jQuery('div.month-view table > tbody > tr > td .field-name-field-image').click(function(){
    var class_value = jQuery(this).parents("div.open-game-popup").attr('class');
    if(class_value.indexOf("game-away") > -1){
      if (user_name) {
       _gaq.push(['_trackEvent', 'Select Game', 'Away Game', user_name, 1, true]);
      }
      else{
        _gaq.push(['_trackEvent', 'Select Game', 'Away Game', 'Anonymous', 1, true]);
      }
    }
    
    if(class_value.indexOf("game-home") > -1){
      if (user_name) {
       _gaq.push(['_trackEvent', 'Select Game', 'Home Game', user_name, 1, true]);
      }
      else{
        _gaq.push(['_trackEvent', 'Select Game', 'Home Game', 'Anonymous', 1, true]);
      }
    }
    
    if(class_value.indexOf("sthevent") > -1){
      if (user_name) {
       _gaq.push(['_trackEvent', 'Select Event', 'Event', user_name, 1, true]);
      }
      else{
        _gaq.push(['_trackEvent', 'Select Event', 'Event', 'Anonymous', 1, true]);
      }
    }
  });
  //More Info
  jQuery("div.game-popup a.more-info").click(function(){
    if (user_name) {
      _gaq.push(['_trackEvent', 'Click More Info', 'Event Info', user_name, 1, true]);
    }
    else{
      _gaq.push(['_trackEvent', 'Click More Info', 'Event Info', 'Anonymous', 1, true]);
    }
  });
  //Manage Tickets
  jQuery("div.game-popup div.mgTicket a").click(function(){
    if (user_name) {
      _gaq.push(['_trackEvent', 'Click Manage Tickets', 'Manage Tickets', user_name, 1, true]);
    }
    else{
      _gaq.push(['_trackEvent', 'Click Manage Tickets', 'Manage Tickets', 'Anonymous', 1, true]);
    }
  });
  
  jQuery("body.page-node-136 a").click(function(){
    var href_value = jQuery(this).attr('href');
    var title = jQuery(this).text(); 
    if(href_value.indexOf("mailto") > -1){
      if (user_name) {
        _gaq.push(['_trackEvent', 'General Information Email Click', title, user_name, 1, true]);
      }
      else{
        _gaq.push(['_trackEvent', 'General Information Email Click', title, 'Anonymous', 1, true]);
      }
    }
  });
  
  jQuery("article#node-260 a").click(function(){
    if(!jQuery(this).is("#facebook, #twitter, #instagram")) { 
      var href_value = jQuery(this).attr('href');
      var title = jQuery(this).text();
      
      if (user_name) {
        _gaq.push(['_trackEvent', title + ' click', title, user_name, 1, true]);
      }
      else{
        _gaq.push(['_trackEvent', title + ' click', title, 'Anonymous', 1, true]);
      }
    }
  });
  
  jQuery("ul.menu.nav ul.dropdown-menu > li > a").click(function(){
    var href_value = jQuery(this).attr('href');
    var title = jQuery(this).text();
    if(href_value=="/goto-account-manager"){
      if (user_name) {
        _gaq.push(['_trackEvent', 'My Account Manager Click', title, user_name, 1, true]);
      }
      else{
        _gaq.push(['_trackEvent', 'My Account Manager Click', title, 'Anonymous', 1, true]);
      }
    }
  });
  
  jQuery("body.page-ticket-management ul.nav.nav-list > li > a").click(function(){
    var href_value = jQuery(this).attr('href');
    var title = jQuery(this).text();
    
    if (user_name) {
      _gaq.push(['_trackEvent', title + ' Click', title, user_name, 1, true]);
    }
    else{
      _gaq.push(['_trackEvent', title + ' Click', title, 'Anonymous', 1, true]);
    }
  });
  
  jQuery("body.page-ticket-management a").click(function(){ 
    var href_value = jQuery(this).attr('href');    
    if(href_value=="http://TrailBlazers.com/AccountManager" || href_value=='http://www.trailblazers.com/accountmanager'){
      var title = jQuery("body.page-ticket-management ul.nav.nav-list > li.active > a").text(); 
      if (user_name) {
        _gaq.push(['_trackEvent', title + ' Account Manager Click', title, user_name, 1, true]);
      }
      else{
        _gaq.push(['_trackEvent', title + ' Account Manager Click', title, 'Anonymous', 1, true]);
      }
    }
  });
  
  //page load tracking
  var url_text = window.location.href;
  var host = window.location.host;
  var prot = document.location.protocol;
  url_text = url_text.replace(prot+'//'+host+'/', '');
  
  if(url_text=='' || url_text=='home'){
    if (user_name) {
      page_name = 'Account Holder Home';
    }
    else{
      page_name = getPageName('home'); 
    }
  }
  else if(url_text=="benefits"){
    page_name = getPageName('benefits');
  }
  else if(url_text=="calendar"){
    page_name = getPageName('calendar');
  }
  else if(url_text=="contact-us"){
    if (!user_name) {
      page_name = getPageName('contact-us');
    }
    else{
      page_name = 'Account Holder Contact Us';
    }
  }
  else if(url_text.indexOf("login") > -1){
    page_name = getPageName('user');
  }
  else if(url_text.indexOf("my-benefits") > -1){    
    if(url_text.indexOf("promotions") > -1){
      page_name = getPageName('promotions');
    }
    else if(url_text.indexOf("rewards") > -1){
      page_name = getPageName('rewards');
    }
    else if(url_text.indexOf("news") > -1){
      page_name = getPageName('news');
    }
    else if(url_text.indexOf("rofr") > -1){
      page_name = getPageName('rofr');
    }
    else{
      page_name = getPageName('my-benefits');
    }
  }
  else if(url_text.indexOf("my-calendar") > -1){    
    if(url_text.indexOf("sthevent") > -1){
      page_name = getPageName('sthevent');
    }
    else {
      page_name = getPageName('my-calendar');
    }
  }
  else if(url_text.indexOf("full-calendar") > -1){
    page_name = getPageName('full-calendar');
  }
  else if(url_text.indexOf("quick-links") > -1){    
    page_name = getPageName('quick-links');
  }
  else if(url_text.indexOf("links") > -1){
    page_name = getPageName('links');
  }
  else if(url_text.indexOf("ticket-management") > -1){
    page_name = getPageName('ticket-management');
  }
  else if(url_text.indexOf("goto-account-manager") > -1){
    page_name = getPageName('goto-account-manager');
  }
  
  _gaq.push(["_set", "title", page_name]);
  _gaq.push(["_trackPageview"]); 
});

function getPageName(pgname){
  if(pgname){
    var pages = new Array()
    //Non Logged In Pages
    pages["home"] = "Anonymous Home";
    pages["benefits"] = "Anonymous Benefits";
    pages["calendar"] = 'Anonymous Calendar';
    pages["contact-us"] = 'Anonymous Contact Us';
    pages["user"] = 'Login';
    
    //Logged In pages
    pages["my-benefits"] = 'Account Holder Benefits';
    pages["promotions"] = 'Account Holder Benefits Prmotions';
    pages["rewards"] = 'Account Holder Benefits Rewards';
    pages["news"] = 'Account Holder Benefits News';
    pages["rofr"] = 'Account Holder Benefits rofr';
    pages["my-calendar"] = 'Account Holder Calendar';
    pages["sthevent"] = 'Account Holder Calendar sthevent';
    pages["full-calendar"] = 'Account Holder Calendar full calendar';
    //'Account Holder Calendar contact us';
    pages["quick-links"] = 'Account Holder Quick Link';
    pages["links"] = 'Account Holder link';
    pages["ticket-management"] = 'Account Holder ticket management';
    pages["goto-account-manager"] = 'Account Holder account manager';
    return pages[pgname];
  }
}
