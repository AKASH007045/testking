
jQuery(document).ready(function() {

  var scrollTop = '';
  var newHeight = '100';
  jQuery(window).bind('scroll', function() {
    scrollTop = jQuery(window).scrollTop();
    newHeight = scrollTop + 100;
  });
	
  jQuery('#edit-invoice-template').after(jQuery('<input type="button" value="preview" class="mail-preview">'));
  jQuery('#custom-invoice-admin-invoice-form').append(jQuery('<div class="preview-node">preview mode</div>'));
  jQuery(".mail-preview").click(function(e) {
    e.stopPropagation();

    var selected_node = jQuery("#edit-invoice-template").find(":selected").val();

    preview_node(selected_node);
    if (jQuery(window).width() < 767) {
      jQuery(this).after(jQuery(".preview-node"));
      jQuery('.preview-node').show().addClass('popup-mobile').css('top', 0);
      jQuery('html, body').animate({
        scrollTop: jQuery('.preview-node').offset().top
      }, 500);
    } else {
      jQuery('.preview-node').removeClass('popup-mobile').css('top', newHeight).toggle();
    };
  });

  jQuery('html').click(function() {
    jQuery('.preview-node').hide();
  });

  jQuery('.popup-btn-close').click(function() {
    jQuery('.preview-node').hide();
  });

  jQuery('.preview-node').click(function() {
    e.stopPropagation();
  });
  jQuery('.mail-preview').click(function(e) {
//    jQuery(".admin-preview-node").children().removeAttr( 'style' );
//       jQuery(".admin-preview-node").children("*").css('all', " initial !important");
//    jQuery("link[href*='style.css']").remove(); .("style").detach()
    e.stopPropagation();
  });  
});

function preview_node($this) {
  if ($this) {
    console.log($this);
//   jQuery('.form-item-invoice-template').append(jQuery('<div class="preview-node">preview mode</div>'));
    jQuery.ajax({
      url: "/admin/manage/mail_preview/" + $this,
      context: document.body,
      dataType: "html",
      beforeSend: function() {
        jQuery(".preview-node").html("Waiting for response...");
      },
      success: function(result) {
        console.log(result);
        jQuery(".preview-node").html(result);
      }
    });

  }

}

// Payment Invoice Preview Popup

jQuery(document).ready(function() {

  var scrollTop = '';
  var newHeight = '100';
  jQuery(window).bind('scroll', function() {
    scrollTop = jQuery(window).scrollTop();
    newHeight = scrollTop + 100;
  });

  jQuery('#edit-payment-template').after(jQuery('<input type="button" value="preview" class="payment-mail-preview">'));
  jQuery(".payment-mail-preview").click(function(e) {
    e.stopPropagation();
    var selected_node = jQuery("#edit-payment-template").find(":selected").val();
    preview_node(selected_node);
    if (jQuery(window).width() < 767) {
      jQuery(this).after(jQuery(".preview-node"));
      jQuery('.preview-node').show().addClass('popup-mobile').css('top', 0);
      jQuery('html, body').animate({
        scrollTop: jQuery('.preview-node').offset().top
      }, 500);
    } else {
      jQuery('.preview-node').removeClass('popup-mobile').css('top', newHeight).toggle();
    };
  });
   jQuery('.payment-mail-preview').click(function(e) {    
    e.stopPropagation();
    
    jQuery('div.admin-preview-node *').removeAttr('style');

    
    jQuery("style link[rel=stylesheet][href~='style.css']").detach();
//      jQuery('link[rel=stylesheet][href*="style.css"]').detach();
//      jQuery('link[rel=stylesheet][href~="style.css"]').detach();
//    jQuery("link[href*='style.css']").prop("disabled", true);
//     jQuery("link[href*='style.css']").remove();
  });
});