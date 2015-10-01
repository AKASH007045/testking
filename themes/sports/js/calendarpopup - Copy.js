/*
* Calendar PopUp JS
*/
var CalPopUp = {
  addClasses:function(){
    jQuery('div.game-away').each(function() {
      jQuery(this).closest('td').addClass('game-away');
        closestTd=jQuery(this).closest('td');
        if(!closestTd.hasClass('multi-day')){
          tempID=(closestTd.attr('id')).split('-');
        }else{
          tempID="";
        }
      rel="#"+tempID[0]+"-"+jQuery(this).closest('td').data('date')+'-date-box';
      jQuery(rel).addClass('game-away');
    });
    jQuery('div.game-home').each(function() {
      jQuery(this).closest('td').addClass('game-home');
        closestTd=jQuery(this).closest('td');
        if(!closestTd.hasClass('multi-day')){
          tempID=(closestTd.attr('id')).split('-');
        }else{
          tempID="";
        }
      rel="#"+tempID[0]+"-"+jQuery(this).closest('td').data('date')+'-date-box';
      jQuery(rel).addClass('game-home');
    });
    jQuery('div.sth-event').each(function() {
      jQuery(this).closest('td').addClass('sth-event');
        closestTd=jQuery(this).closest('td');
        if(!closestTd.hasClass('multi-day')){
          tempID=(closestTd.attr('id')).split('-');
        }else{
          tempID="";
        }
      rel="#"+tempID[0]+"-"+jQuery(this).closest('td').data('date')+'-date-box';
      jQuery(rel).addClass('sth-event cal-msg-games');
    });
  },
  createPopup:function($this){
    if($this){
      var href_value = jQuery($this).attr('href'); 
      var html_str = '<div id="modal_bootstrap"></div>';
      jQuery('body').append(html_str);
      jQuery('body').find('#modal_bootstrap').html("");
      jQuery('body').find('#modal_bootstrap').html(jQuery(href_value).html());
      
      jQuery('body').find('#modal_bootstrap').on('hidden.bs.modal', function (e) {
        jQuery('body').find('#modal_bootstrap').remove();
      });
    }
  },
  processIconClick:function(){
    //alert("processIconClick");    
  }
};

jQuery(document).ajaxComplete(function(){
  CalPopUp.addClasses();  
  
  jQuery('div.month-view table > tbody > tr > td .field-name-field-image').parents('a').click(function(e){
    e.preventDefault();    
    CalPopUp.createPopup(this);
  });
  
  //if(jQuery.browser.msie && jQuery.browser.version=="9.0"){
  //    jQuery('div.month-view table > tbody > tr > td .field-name-field-image').live('click', function(){
  //      var id = jQuery(this).attr('data-id');
  //      CalPopUp.createPopup(id);
  //    });
  //}
    
  //rsvpmoreinfo();
  onloadRsvpInfo(); 
});
/*
* End of Calendar PopUp JS
*/

jQuery(document).ready(function(){
  var $this = this;  
  jQuery('div.month-view table > tbody > tr > td .field-name-field-image').parents('a').click(function(e){
    e.preventDefault();    
    CalPopUp.createPopup(this);
  });
  onloadRsvpInfo();         
});
 

function onloadRsvpInfo(){
  jQuery("div#rsvp, div#more-info").hide();
  jQuery('a.upArrow').hide(); 
  jQuery('div#more-info').css('box-sizing','content-box');
  
  jQuery("a.more-info").click(function(){ 
    jQuery("a.more-info").removeClass("active");  
    if(jQuery(this).hasClass('active')){
      jQuery('a.upArrow').show();
    }
    var cntxt = "";
    var class_val = jQuery(this).attr("class");
    jQuery(this).addClass("active");
    
    if(jQuery("div#"+class_val).length>0 && !jQuery("div#"+class_val).hasClass('openpop')){ 
      jQuery("div#"+class_val).addClass('openpop');
      
      jQuery("div#"+class_val).slideDown({ 
        duration: 500,
        easing:'linear',
        complete:function(){
            jQuery('a.upArrow').show();
        }
      });
    }
  });
  jQuery('a.upArrow').click(function(){    
    var close_val = jQuery(this).parents('div').attr('id');    
    jQuery('a.upArrow').hide();      
    jQuery("div#"+close_val).slideUp({ 
        duration: 500,
        easing:'linear',
        complete:function(){
            jQuery("div#"+close_val).removeClass('openpop');
            jQuery("a.more-info").removeClass("active");
        }
    });    
  });
} 
  
function rsvpmoreinfo(){  
  /*jQuery("div#rsvp, div#more-info").hide();
  //jQuery('a.upArrow').hide(); 
  
  jQuery("a.rsvp, a.more-info").live('click', function(){ 
    jQuery("a.rsvp, a.more-info").removeClass("active");  
    if(jQuery(this).hasClass('active')){
      jQuery('a.upArrow').show();
    }
    
    var class_val = jQuery(this).attr("class");
    jQuery(this).addClass("active");
    
    if(jQuery("div#"+class_val).length>0 && !jQuery("div#"+class_val).hasClass('openpop')){
      if(!jQuery("div#"+class_val).hasClass('openpop')){ 
        jQuery("div#"+class_val).addClass('openpop');
      }
      
      var cntxt = "";
      if(class_val=="rsvp"){
        cntxt = "more-info";
      }
      if(class_val=="more-info"){
        cntxt = "rsvp";
      }
      if(jQuery("div#"+cntxt).length>0){
        if(jQuery("div#"+cntxt).hasClass('openpop')){ 
          jQuery("div#"+cntxt).removeClass('openpop').slideUp({ 
              duration: 300,
              easing:"easeInSine"
          }).find("a.upArrow").hide();       
        }      
      }
      jQuery("div#"+class_val).slideDown({ 
        duration: 300,
        easing:"easeInSine",
        complete:function(){
            jQuery('a.upArrow').show();
        }
      });
    }
  });  
  
  jQuery('a.upArrow').live('click', function(){    
    var close_val = jQuery(this).parents('div').attr('id');    
    jQuery('a.upArrow').hide();      
    jQuery("div#"+close_val).slideUp({ 
        duration: 300,
        easing:"easeInSine",
        complete:function(){
          jQuery("a.rsvp, a.more-info").removeClass("active");
          jQuery("div#"+close_val).removeClass('openpop')
        }
    });
    
  });*/
}