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