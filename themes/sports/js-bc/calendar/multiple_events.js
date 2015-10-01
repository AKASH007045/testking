jQuery(document).ready(function(){
  separateSthevents();
  jQuery('div.view-full-calendar div.view-content table tbody tr td div.inner, div.view-home-calender div.view-content table tbody tr td div.inner, div.view-calendar div.view-content table tbody tr td div.inner').cycle({ 
    fx: 'uncover',
    direction: 'down',  
    pause: 1,
    timeout: 2100,
    sync: 1    
  });  
  jQuery(document).ajaxComplete(function(){
      separateSthevents();
    jQuery('div.view-full-calendar div.view-content table tbody tr td div.inner, div.view-home-calender div.view-content table tbody tr td div.inner, div.view-calendar div.view-content table tbody tr td div.inner').cycle({
      fx: 'uncover',
      direction: 'down',      
      pause: 1,
      timeout: 2100,
      sync: 1           
    });
  });
});
function separateSthevents(){ 
  if(jQuery('body').find('div.sth-calendar-event img.sth-event-star-icon').length>0){
    jQuery('body').find('div.sth-calendar-event img.sth-event-star-icon').each(function(){
      var cd = jQuery(this).parents('div.item').clone();
      
      if(cd){        
        jQuery(this).parents('td.single-day').prepend(cd);
        jQuery(this).parents('div.item').remove();
      }
    });
	jQuery("body").find('div.month-view table > tbody > tr > td div.field-name-field-image').parents('a').on('click', function(e){
		e.preventDefault();
		CalPopUp.createPopup(this);
	});
  }
}