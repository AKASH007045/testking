var ttextCounter = false;
var ttextCounter1 = true;
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

jQuery(document).ready(function(){
  jQuery('.page-admin-manage-pages-add-pdf a#overlay-close').click(function() {
    window.confirm('Are you sure?');
	alertifyreset () ;
        alertify.confirm("You have unsaved changes. Do you want discard changes?", function (e) {
            if (e) {
              var wraper = jQuery(this).parents('div.tile-image-form');
              wraper.css('visibility','hidden');
              ttextCounter1 = false;
              location.reload(true);
            } else {
                e.preventDefault();
            }
        });
    });
  // view take over page- on-off status (only update content) 
//  jQuery('.view-take-over-pages .views-field-status').click(function(){
//      if(jQuery(this).hasClass('on')){
//          jQuery(this).addClass('off');
//          jQuery(this).text('0');
//          jQuery(this).removeClass('on');
//      }
//      else if(jQuery(this).hasClass('off')){
//          jQuery(this).addClass('on');
//          jQuery(this).text('1');
//          jQuery(this).removeClass('off');
//      }
//  })
  
  jQuery('#views-form-take-over-pages-page .views-field-nid').hide();

// View take over page, on submit, node publish-unublish "save"
//jQuery('#views-form-take-over-pages-page .form-submit').click(function() {
//        var nds =  new Array();
//        jQuery('td.views-field-status.off').each(function(){
//            nds.push(jQuery(this).closest('tr').attr('id'));
//        });
//            jQuery.ajax({
//                type: "POST",
//                url: Drupal.settings.basePath+"admin/post/take-over/ajax",
//                data: {name: nds},
//                success:function(data) {
//                        jQuery('#views-form-take-over-pages-page').submit();
//                    }                
//                
//            })
//         return false;        
//    
//
//});

});
