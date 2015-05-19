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
      
      /*carousel fix*/
      jQuery('body').find('#modal_bootstrap').find('.carousel').carousel();
      jQuery('body').find('#modal_bootstrap').find('.carousel a.right').on('click', function(){
        jQuery('body').find('#modal_bootstrap').find('.carousel').carousel('next');
      });
      jQuery('body').find('#modal_bootstrap').find('.carousel a.left').on('click', function(){
        jQuery('body').find('#modal_bootstrap').find('.carousel').carousel('prev');
      });
      jQuery('body').find('#modal_bootstrap').find('.carousel ol.carousel-indicators li').on('click', function(){
        var index = jQuery(this).index();
        jQuery('body').find('#modal_bootstrap').find('.carousel').carousel(index);
      });
      /*End of carousel fix*/
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
  if (jQuery('body').hasClass('node-type-event')) {
    onloadRsvpInfo();
  }
  jQuery('div.month-view table > tbody > tr > td .field-name-field-image')
  .parents('a')
  .click(function(e){
    e.preventDefault();    
    CalPopUp.createPopup(this);
  });
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
  var tab_group = new Array(); //"a.more-info, a.rsvp";
  var body_group = new Array(); //"div#more-info, div#rsvp";
  
  //At the time of closing the body 
  //it active class along with tab classes
  //so adding the data attribute resolves 
  //the issue 
  jQuery("div.event-gallary-big-box div.pptic-evt > div > a, div.seatview-dd > div > a").each(function(){
    //if more info is already visible then hide it
    if(jQuery(this).hasClass('active')){
      jQuery(this).removeClass('active');
      var ini_class = jQuery(this).attr('class');
      jQuery("div#"+ini_class).slideUp({
        duration: 4,
        easing:'easeOutCubic',
        complete:function(){
          jQuery("div#"+ini_class).removeClass('form-tab-info');
          jQuery("a."+ini_class).removeClass('active');
        }
      });
    }
    
    
    var a_class = jQuery(this).attr('class');    
    jQuery(this).attr('data', a_class);
    tab_group.push("a."+a_class);
    body_group.push("div#"+a_class);
  });
  tab_group = tab_group.join(", ");
  body_group = body_group.join(", ");  

  jQuery('body').find(tab_group).on('click', function(){
    var pick_tab_class = jQuery(this).attr('data');
    if(jQuery("div#"+pick_tab_class).length>0){
      //open body for current tab
      if(!jQuery("div#"+pick_tab_class).hasClass('form-tab-info')){ 
        jQuery(body_group).removeClass('form-tab-info');
        jQuery(tab_group).removeClass('active'); 
        jQuery(body_group).slideUp({
          duration: 400,
          easing:'easeOutCubic',
          complete:function(){}
        });        
        //Show the body
        jQuery("div#"+pick_tab_class).addClass('form-tab-info');
        jQuery("a."+pick_tab_class).addClass('active');
        jQuery("div#"+pick_tab_class).slideDown({
          duration: 400,
          easing:'easeInCubic',
          complete:function(){}
        });
      }    
      else{
        jQuery("div#"+pick_tab_class).slideUp({
          duration: 400,
          easing:'easeOutCubic',
          complete:function(){
            jQuery("div#"+pick_tab_class).removeClass('form-tab-info');
            jQuery("a."+pick_tab_class).removeClass('active');
          }
        }); 
      }
    }
  });
}

function onloadSeatInfo(){   
  jQuery('body').find('div.closePopup').on('click', function(){
    jQuery('body').find('#modal_bootstrap').trigger('hidden.bs.modal');
    jQuery('body').find('div.modal-backdrop').remove();
  });
  var class_group = "a.more-info, a.rsvp";
  var id_group = "div#more-info, div#rsvp";
  jQuery('body').find(class_group).on('click', function(){    
    jQuery(class_group).removeClass("active");  
    
    var cntxt = "";
    var class_val = jQuery(this).attr("class");
    jQuery(this).addClass("active");
    jQuery(this).html("LESS INFO");
    
    if(jQuery("div#"+class_val).length>0 && !jQuery("div#"+class_val).hasClass('openpop')){ 
      jQuery(id_group).hide();
      jQuery("div#"+class_val).addClass('openpop');
      
      jQuery("div#"+class_val).slideDown({
        duration: 400,
        easing:'easeOutCubic',
        complete:function(){
            jQuery("div#"+class_val).addClass('lessinfo');
   

        }
      });
    }
    //alert(class_val);    
    if(jQuery("div#"+class_val).length>0 && jQuery("div#"+class_val).hasClass('lessinfo')){
      var link_text = "";
      if(class_val=="rsvp"){
        link_text = "RSVP";
      }
      else{
        link_text = "MORE INFO";
      }
      jQuery("a."+class_val).html(link_text);
      jQuery(".vr-seat-view-wrapper a."+class_val).html("SEAT INFO");
      jQuery("div#"+class_val).slideUp({
          duration: 400,
          easing:'easeInCubic',
          complete:function(){
              jQuery("div#"+class_val).removeClass('lessinfo');
              jQuery("div#"+class_val).removeClass('openpop');
              jQuery("a."+class_val).removeClass("active");
          }
      }); 
    }
  }); 
}