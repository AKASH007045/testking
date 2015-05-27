jQuery(document).ready(function(){
  jQuery('div.view-full-calendar div.view-content table tbody tr td div.inner, div.view-home-calender div.view-content table tbody tr td div.inner, div.view-calendar div.view-content table tbody tr td div.inner').cycle({ 
    fx: 'uncover',
    direction: 'down',  
    pause: 1,
    timeout: 2100,
    sync: 1    
  });  
  jQuery(document).ajaxComplete(function(){
    jQuery('div.view-full-calendar div.view-content table tbody tr td div.inner, div.view-home-calender div.view-content table tbody tr td div.inner, div.view-calendar div.view-content table tbody tr td div.inner').cycle({
      fx: 'uncover',
      direction: 'down',      
      pause: 1,
      timeout: 2100,
      sync: 1           
    });
  });
});