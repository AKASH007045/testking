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
      
      onloadRsvpInfo(); 
      
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
});
/*
* End of Calendar PopUp JS
*/

jQuery(document).ready(function(){
  CalPopUp.addClasses();
  var $this = this;  
  jQuery('div.month-view table > tbody > tr > td .field-name-field-image').parents('a').click(function(e){
    e.preventDefault();    
    CalPopUp.createPopup(this);
  });   
  var url_text = window.location.href;
  
  if(url_text.search("event")){
    onloadRsvpInfo();
  }
});

function onloadRsvpInfo(){   
  jQuery('body').find('div.closePopup').on('click', function(){
    jQuery('body').find('#modal_bootstrap').trigger('hidden.bs.modal');
    jQuery('body').find('div.modal-backdrop').remove();
  });

  jQuery('body').find("a.more-info").on('click', function(){    
    jQuery("a.more-info").removeClass("active");  
    
    var cntxt = "";
    var class_val = jQuery(this).attr("class");
    jQuery(this).addClass("active");
    jQuery(this).html("LESS INFO");
    
    if(jQuery("div#"+class_val).length>0 && !jQuery("div#"+class_val).hasClass('openpop')){ 
      jQuery("div#more-info").hide();
      jQuery("div#"+class_val).addClass('openpop');
      
      jQuery("div#"+class_val).slideDown({
        duration: 400,
        easing:'easeOutCubic',
        complete:function(){
            jQuery("div#more-info").addClass('lessinfo');
   

        }
      });
    }
        
    if(jQuery("div#"+class_val).length>0 && jQuery("div#"+class_val).hasClass('lessinfo')){
               jQuery("a.more-info").html("MORE INFO");
      jQuery("div#"+class_val).slideUp({ 
          duration: 400,
          easing:'easeInCubic',
          complete:function(){
              jQuery("div#"+class_val).removeClass('lessinfo');
              jQuery("div#"+class_val).removeClass('openpop');
              jQuery("a.more-info").removeClass("active");
          }
      }); 
    }
  }); 
}
